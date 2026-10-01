#include "patches.h"
#include "mouse_camera.h"
#include "tag_helper.h"

// Mouse camera mod: first/third person mouse look.
//
// The game is a port of Mega Man Legends and uses PSX GTE style math: 12-bit angles (0x1000 = 360 degrees),
// 3x3 s16 rotation matrices in 4.12 fixed point and a world space where -Y is up. Every gameplay camera
// ends up in func_80032FC0 (camera_set_orbit), which builds the view matrix from a target point, a distance,
// a yaw and a pitch. The patch below is a straight C port of that function with a hook for the main gameplay
// camera, which lets the mouse turn Mega Man and tilt the camera.

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
} PsxMatrix; // size = 0x20

typedef struct {
    s32 x, y, z;
    s32 pad;
} Vec3i; // size = 0x10

typedef struct {
    s16 x, y, z;
    s16 pad;
} SVec3; // size = 0x8

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 blendAmount; // Non-zero while blending between two camera setups.
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 enabled;
    /* 0x05 */ u8 mode; // 0 follow, 4/5 locked behind Mega Man, others track actors.
    /* 0x06 */ u8 scripted; // Non-zero while a cutscene drives the camera.
    /* 0x07 */ u8 shakeY;
    /* 0x08 */ u8 unk08[4];
    /* 0x0C */ s32 distance;
    /* 0x10 */ s32 yaw;
    /* 0x14 */ s32 pitch;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 yawOffset;
    /* 0x20 */ s32 pitchOffset;
    /* 0x24 */ u8 unk24[0x24];
    /* 0x48 */ Vec3i target;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 targetHeight; // Target is placed this far above Mega Man's feet.
} GameCamera;

extern GameCamera D_80204348_1DF748;
extern Vec3i D_80162830_13DC30;
extern s32 D_80162840_13DC40;
extern s32 D_80162844_13DC44;
extern s32 D_80162848_13DC48;
extern SVec3 D_80204400_1DF800;
extern PsxMatrix D_80210990_1EBD90[2]; // [0] rotation, [1] its transpose.
extern u8 D_80210B61_1EBF61; // Coordinate precision shift.
extern PsxMatrix D_801D4760_1AFB60; // Current view matrix.
extern Vec3i D_802043E0_1DF7E0; // Camera position.
extern Vec3i D_802043F0_1DF7F0; // Previous camera position.
extern s16 D_80195E90_171290[3];
extern s32 D_80206B58_1E1F58[3];

extern u8 D_802049B0_1DFDB0[]; // Mega Man.
#define PLAYER_FLAGS(p) (*(u8*)((p) + 0x00))
#define PLAYER_ACTIVE(p) (*(u8*)((p) + 0x06))
#define PLAYER_POS_X(p) (*(s16*)((p) + 0x14))
#define PLAYER_POS_Y(p) (*(s16*)((p) + 0x16))
#define PLAYER_POS_Z(p) (*(s16*)((p) + 0x18))
#define PLAYER_YAW(p) (*(s16*)((p) + 0x56))
#define PLAYER_INPUT_FLAGS(p) (*(u8*)((p) + 0xBD)) // 0x40 input cleared, 0x20 input driven by a script.

void func_80031600_CA00(SVec3* angles, PsxMatrix* out); // RotMatrix
PsxMatrix* func_80031E10_D210(PsxMatrix* in, PsxMatrix* out); // TransposeMatrix
s32 func_800324B4_D8B4(s32 angle); // rsin
s32 func_80032498_D898(s32 angle); // rcos
void func_80030AA0_BEA0(PsxMatrix* m, Vec3i* in, Vec3i* out); // ApplyMatrixLV
s32 func_80032424_D824(s32 value); // Integer square root.
s32 func_80032664_DA64(s32 x, s32 z); // ratan2: 12-bit angle of the direction (x, z), matching the camera's yaw.
void func_800306F0_BAF0(PsxMatrix* a, PsxMatrix* b, PsxMatrix* out); // CompMatrix
void func_800308E8_BCE8(PsxMatrix* m, SVec3* in, Vec3i* out); // ApplyMatrix
void func_800283A8_37A8(s8 bufferIndex, s8 poolIndex, void* func, void* funcData); // gfx_add_render_task
void func_8008128C_5C68C(void* arg);
void func_80084870_5FC70(s32 bufferIndex, s32 poolIndex, s32 part, PsxMatrix* mtx, s32 arg4); // draw Mega Man part
void func_8008498C_5FD8C(s32 bufferIndex, s32 poolIndex, s32 face, s32 mouth, PsxMatrix* mtx); // draw Mega Man face

extern u8 D_800AC9BC_87DBC[]; // Part counts per render group, 5 groups per body variant.
extern u8 D_800AC9CC_87DCC[]; // Part (bone) indices, in draw order.
extern u8 D_80210B62_1EBF62;

#define PLAYER_BONE_MATRICES(p) ((PsxMatrix*)((p) + 0x1C8))
#define PLAYER_BONE_POSITIONS(p) ((SVec3*)((p) + 0x3A8))
#define PLAYER_FACE(p) (*(s8*)((p) + 0x168))
#define PLAYER_MOUTH(p) (*(s8*)((p) + 0x169))
#define MEGAMAN_HEAD_BONE 1
#define MEGAMAN_HELMET_PART 15
// Every part except the head, which the first person camera sits inside of.
#define FIRST_PERSON_PART_MASK (0x7FFF & ~(1 << MEGAMAN_HEAD_BONE))
#define PLAYER_COMBAT_STATE(p) (*(u8*)((p) + 0x171)) // Zero in towns, where Mega Man kicks instead of shooting.

// Angle units applied per (sensitivity scaled) mouse pixel.
#define MOUSE_CAMERA_YAW_SCALE 12.0f
#define MOUSE_CAMERA_PITCH_SCALE 8.0f

#define THIRD_PERSON_PITCH_OFFSET_MIN -0x200
#define THIRD_PERSON_PITCH_OFFSET_MAX 0x300
#define THIRD_PERSON_PITCH_MIN -0x180
#define THIRD_PERSON_PITCH_MAX 0x3C0

#define FIRST_PERSON_PITCH_MAX 0x380
// Mega Man is ~130 units tall and his head bone sits ~116 units above his feet.
#define FIRST_PERSON_EYE_HEIGHT 122
// Moves the first person camera slightly ahead of the head so it doesn't clip into the face.
#define FIRST_PERSON_FORWARD_OFFSET 6
// The game's near plane is ~190 units, which would clip Mega Man's own arms in first person, and his back when the
// third person camera aims over his shoulder or zooms in.
#define FIRST_PERSON_NEAR_PLANE 8.0f
#define THIRD_PERSON_NEAR_PLANE 16.0f

// Only the gameplay camera sets these. Other cameras (the sky, cutscenes) also go through camera_set_orbit, often later
// in the same frame, so they must not clear them. The main draw counts sMouseCameraFrames down instead, which turns
// the mouse camera's effects off once the gameplay camera stops updating.
static bool sFirstPersonActive = FALSE;
static s32 sMouseCameraFrames = 0;
// Set when the projection is adjusted for a first person frame where Mega Man is armed.
static bool sCrosshairVisible = FALSE;
// View matrix used to draw Mega Man's body in first person: at his eyes, facing where he faces, but ignoring the
// camera pitch. His own animations (shooting, special weapons, kicks) then play in view like a shooter's viewmodel
// instead of swinging around as the player looks up and down.
static PsxMatrix sViewmodelView;

static float sFirstPersonPitch = 0.0f;
static float sThirdPersonPitchOffset = 0.0f;
static float sYawRemainder = 0.0f;

// Third person: the mouse orbits the camera around Mega Man, and his movement becomes relative to the camera.
static s32 sOrbitYaw = 0;
static s32 sLastMode = MOUSE_CAMERA_OFF;
static bool sFreeCameraActive = FALSE; // Set by the camera for the player input hook.
static float sZoom = 1.0f;
#define ZOOM_MIN 0.4f
#define ZOOM_MAX 2.0f
#define ZOOM_STEP 0.1f
// Fastest Mega Man turns towards the direction pressed, per frame.
#define FREE_CAMERA_TURN_RATE 0x140

// Over the shoulder aiming (right mouse button in third person).
static bool sAiming = FALSE;
static float sAimPitch = 0.0f;
#define AIM_DISTANCE 175
#define AIM_HEIGHT 118
#define AIM_SIDE 48

// Hitmarker, frames left to show it.
static s32 sHitmarkerFrames = 0;
#define HITMARKER_FRAMES 10

// Aim assist: inside the cone, mouse turning slows down and the aim is pulled gently towards the enemy.
#define AIM_ASSIST_CONE 0x50 // ~7 degrees.
#define AIM_ASSIST_RANGE 3000
#define AIM_ASSIST_FRICTION 0.55f
#define AIM_ASSIST_PULL 0.08f
#define AIM_ASSIST_TARGET_HEIGHT 40 // Aim this far above an enemy's origin.

static s32 clamp_s32(s32 value, s32 min, s32 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static float clamp_f32(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

// Builds a view matrix for a camera sitting at pos, the same way camera_set_orbit does with a distance of zero.
static void build_view_at(Vec3i* pos, s32 yaw, s32 pitch, PsxMatrix* out) {
    SVec3 angles;
    PsxMatrix rot;
    Vec3i offset;
    s32 shift = D_80210B61_1EBF61;

    angles.x = -pitch & 0xFFF;
    angles.y = yaw & 0xFFF;
    angles.z = 0;
    func_80031600_CA00(&angles, &rot);
    func_80031E10_D210(&rot, out);

    offset.x = (s32)((u32)-pos->x << shift);
    offset.y = (s32)((u32)-pos->y << shift);
    offset.z = (s32)((u32)-pos->z << shift);
    func_80030AA0_BEA0(out, &offset, (Vec3i*)out->t);
}

// Whether the gameplay camera is following Mega Man, even if the game has taken control of him (talking to someone).
static bool mouse_camera_following_player(void) {
    GameCamera* cam = &D_80204348_1DF748;
    u8* player = D_802049B0_1DFDB0;

    if (!cam->enabled || cam->scripted) {
        return FALSE;
    }
    if (cam->mode != 0 && cam->mode != 4 && cam->mode != 5) {
        return FALSE;
    }
    return (PLAYER_FLAGS(player) & PLAYER_ACTIVE(player) & 1) != 0;
}

static bool mouse_camera_player_controllable(void) {
    GameCamera* cam = &D_80204348_1DF748;
    u8* player = D_802049B0_1DFDB0;

    if (!cam->enabled || cam->scripted) {
        return FALSE;
    }
    // Leave actor tracking cameras (lock-on, bosses...) to the game.
    if (cam->mode != 0 && cam->mode != 4 && cam->mode != 5) {
        return FALSE;
    }
    if (!(PLAYER_FLAGS(player) & PLAYER_ACTIVE(player) & 1)) {
        return FALSE;
    }
    if (PLAYER_INPUT_FLAGS(player) & 0x60) {
        return FALSE;
    }
    return TRUE;
}

// Mega Man's shots, 16 slots of 0x118 bytes. Byte 0 bit 0 marks a live shot, the position is at 0x1C as 16.16 fixed
// point with s16 copies at 0x14 and 0x90. Shots only fly horizontally in the original game.
extern u8 D_802059A0_1E0DA0[];
#define PLAYER_SHOT_COUNT 16
#define PLAYER_SHOT_SIZE 0x118

static bool sShotTracked[PLAYER_SHOT_COUNT];
static s32 sShotSlope[PLAYER_SHOT_COUNT]; // Vertical travel per unit of horizontal travel, 4.12 fixed point.
static s32 sShotLastX[PLAYER_SHOT_COUNT];
static s32 sShotLastZ[PLAYER_SHOT_COUNT];

// Makes shots follow the first person aim vertically: each frame, a shot rises or drops by its horizontal travel
// times the slope of the camera pitch it was fired at. Measuring the travel keeps this independent of shot speed.
static void aim_player_shots(bool aiming, s32 pitch) {
    s32 slope = 0;
    s32 i;

    if (aiming) {
        s32 cosPitch = func_80032498_D898(pitch);
        if (cosPitch > 0) {
            slope = (func_800324B4_D8B4(pitch) << 12) / cosPitch;
        }
    }

    for (i = 0; i < PLAYER_SHOT_COUNT; i++) {
        u8* shot = &D_802059A0_1E0DA0[i * PLAYER_SHOT_SIZE];
        s32* pos = (s32*)(shot + 0x1C);
        s32 dx, dz, travel;

        if (!(shot[0] & 1)) {
            sShotTracked[i] = FALSE;
            continue;
        }
        if (!sShotTracked[i]) {
            sShotTracked[i] = TRUE;
            sShotSlope[i] = slope;
            sShotLastX[i] = pos[0];
            sShotLastZ[i] = pos[2];
            continue;
        }

        // Work in 1/256 units so the squares fit in 32 bits.
        dx = (pos[0] - sShotLastX[i]) >> 8;
        dz = (pos[2] - sShotLastZ[i]) >> 8;
        sShotLastX[i] = pos[0];
        sShotLastZ[i] = pos[2];
        if (sShotSlope[i] == 0 || dx > 0x7FFF || dx < -0x7FFF || dz > 0x7FFF || dz < -0x7FFF) {
            continue;
        }

        travel = func_80032424_D824(dx * dx + dz * dz);
        // Positive pitch looks down and +Y points down, so the slope applies as is.
        pos[1] += ((travel * sShotSlope[i]) >> 12) << 8;
        *(s16*)(shot + 0x16) = pos[1] >> 16;
        *(s16*)(shot + 0x92) = pos[1] >> 16;
    }
}

// Called by the us.rev1.toml hook where a flying buster shot ends on an enemy.
void mouse_camera_register_hit(void) {
    sHitmarkerFrames = HITMARKER_FRAMES;
}

// Enemies: 18 actors of 0x3C8 bytes and 4 large (multi part) actors of 0x5AC bytes. Like Mega Man and his shots,
// byte 0 & byte 6 & 1 marks an active one and the position is an s16 vector at 0x14.
extern u8 D_801FFBE0_1DAFE0[];
extern u8 D_801BA0C0_1954C0[];

static s32 wrap_angle(s32 angle) {
    angle &= 0xFFF;
    return angle >= 0x800 ? angle - 0x1000 : angle;
}

// Finds the active enemy closest to the aim within the assist cone, as yaw and pitch offsets from the aim.
static bool find_aim_assist_target(s32 eyeX, s32 eyeY, s32 eyeZ, s32 yaw, s32 pitch, s32* yawDiff, s32* pitchDiff) {
    static u8* const tables[] = { D_801FFBE0_1DAFE0, D_801BA0C0_1954C0 };
    static const s32 counts[] = { 18, 4 };
    static const s32 sizes[] = { 0x3C8, 0x5AC };
    s32 best = 0x7FFFFFFF;
    s32 t, i;

    for (t = 0; t < 2; t++) {
        for (i = 0; i < counts[t]; i++) {
            u8* actor = tables[t] + i * sizes[t];
            s32 dx, dy, dz, horizontal, yd, pd, score;

            if (!(actor[0] & actor[6] & 1)) {
                continue;
            }
            dx = *(s16*)(actor + 0x14) - eyeX;
            dy = *(s16*)(actor + 0x16) - AIM_ASSIST_TARGET_HEIGHT - eyeY;
            dz = *(s16*)(actor + 0x18) - eyeZ;
            if (dx > AIM_ASSIST_RANGE || dx < -AIM_ASSIST_RANGE || dz > AIM_ASSIST_RANGE || dz < -AIM_ASSIST_RANGE) {
                continue;
            }
            horizontal = func_80032424_D824(dx * dx + dz * dz);
            if (horizontal < 24 || horizontal > AIM_ASSIST_RANGE) {
                continue;
            }

            yd = wrap_angle(func_80032664_DA64(dx, dz) - yaw);
            pd = wrap_angle(func_80032664_DA64(dy, horizontal) - pitch);
            if (yd > AIM_ASSIST_CONE || yd < -AIM_ASSIST_CONE || pd > AIM_ASSIST_CONE * 2 || pd < -AIM_ASSIST_CONE * 2) {
                continue;
            }
            score = (yd < 0 ? -yd : yd) + (pd < 0 ? -pd : pd);
            if (score < best) {
                best = score;
                *yawDiff = yd;
                *pitchDiff = pd;
            }
        }
    }
    return best != 0x7FFFFFFF;
}

// Applies aim assist to this frame's mouse input, already turned into yaw and pitch changes (angle units).
static void apply_aim_assist(s32 eyeX, s32 eyeY, s32 eyeZ, s32 yaw, s32 pitch, float* yawChange, float* pitchChange) {
    s32 yawDiff, pitchDiff;

    if (!find_aim_assist_target(eyeX, eyeY, eyeZ, yaw, pitch, &yawDiff, &pitchDiff)) {
        return;
    }
    *yawChange = *yawChange * AIM_ASSIST_FRICTION + yawDiff * AIM_ASSIST_PULL;
    *pitchChange = *pitchChange * AIM_ASSIST_FRICTION + pitchDiff * AIM_ASSIST_PULL;
}

// Keeps the fractional part of yaw changes so slow mouse movements still add up.
static s32 take_yaw_change(float change) {
    s32 whole;
    sYawRemainder += change;
    whole = (s32)sYawRemainder;
    sYawRemainder -= (float)whole;
    return whole;
}

// Called for the main gameplay camera only. Can rewrite the parameters passed to the camera builder.
static void mouse_camera_update(Vec3i* target, s32* distance, s32* yaw, s32* pitch) {
    GameCamera* cam = &D_80204348_1DF748;
    u8* player = D_802049B0_1DFDB0;
    s32 mode = recomp_get_mouse_camera_mode();
    bool armed = PLAYER_COMBAT_STATE(player) != 0;
    float dx = 0.0f;
    float dy = 0.0f;
    float yawChange;
    float pitchChange;
    s32 wheel;

    sFirstPersonActive = FALSE;
    sFreeCameraActive = FALSE;
    sAiming = FALSE;
    sMouseCameraFrames = 0;

    // Always consume the input so none of it is applied late once Mega Man can move again.
    recomp_get_mouse_camera_deltas(&dx, &dy);
    wheel = recomp_get_mouse_camera_wheel();

    if (mode == MOUSE_CAMERA_OFF || !mouse_camera_player_controllable()) {
        aim_player_shots(FALSE, 0);
        sLastMode = MOUSE_CAMERA_OFF;
        return;
    }

    sMouseCameraFrames = 2;
    yawChange = dx * MOUSE_CAMERA_YAW_SCALE;
    pitchChange = dy * MOUSE_CAMERA_PITCH_SCALE;

    if (mode == MOUSE_CAMERA_FIRST_PERSON) {
        s32 eyeX = PLAYER_POS_X(player);
        s32 eyeY = PLAYER_POS_Y(player) - FIRST_PERSON_EYE_HEIGHT;
        s32 eyeZ = PLAYER_POS_Z(player);

        if (armed) {
            apply_aim_assist(eyeX, eyeY, eyeZ, PLAYER_YAW(player) + 0x800, (s32)sFirstPersonPitch, &yawChange, &pitchChange);
        }

        // Mega Man uses tank controls, so turning him turns the camera as well.
        PLAYER_YAW(player) = (PLAYER_YAW(player) + take_yaw_change(yawChange)) & 0xFFF;
        // Stay locked behind Mega Man instead of the game's lazy follow so the mouse feels direct. The game's own
        // yaw is kept in sync so its follow logic doesn't swing the camera around when the mouse camera is turned off.
        cam->yaw = PLAYER_YAW(player) + 0x800;
        *yaw = cam->yaw + cam->yawOffset;

        sFirstPersonPitch = clamp_f32(sFirstPersonPitch + pitchChange, -FIRST_PERSON_PITCH_MAX, FIRST_PERSON_PITCH_MAX);
        target->x = eyeX;
        target->y = eyeY;
        target->z = eyeZ;
        // A negative distance puts the camera in front of the target, looking the same way.
        *distance = -FIRST_PERSON_FORWARD_OFFSET;
        *pitch = (s32)sFirstPersonPitch;
        sFirstPersonActive = TRUE;
        aim_player_shots(TRUE, *pitch);

        {
            Vec3i eyePos;
            eyePos.x = eyeX;
            eyePos.y = eyeY;
            eyePos.z = eyeZ;
            build_view_at(&eyePos, cam->yaw, 0, &sViewmodelView);
        }
    } else {
        // Start orbiting from behind Mega Man when entering third person.
        if (sLastMode != MOUSE_CAMERA_THIRD_PERSON) {
            sOrbitYaw = (PLAYER_YAW(player) + 0x800) & 0xFFF;
            sAimPitch = 0.0f;
        }
        sFreeCameraActive = TRUE;
        sAiming = recomp_get_mouse_camera_aim() && armed;

        if (sAiming) {
            s32 sinYaw, cosYaw;

            apply_aim_assist(D_802043E0_1DF7E0.x, D_802043E0_1DF7E0.y, D_802043E0_1DF7E0.z, sOrbitYaw, (s32)sAimPitch,
                             &yawChange, &pitchChange);
            sOrbitYaw = (sOrbitYaw + take_yaw_change(yawChange)) & 0xFFF;
            sAimPitch = clamp_f32(sAimPitch + pitchChange, -FIRST_PERSON_PITCH_MAX, FIRST_PERSON_PITCH_MAX);

            // Close behind the right shoulder, looking where Mega Man aims. The camera's right is (cos, 0, -sin).
            sinYaw = func_800324B4_D8B4(sOrbitYaw);
            cosYaw = func_80032498_D898(sOrbitYaw);
            target->x = PLAYER_POS_X(player) + ((AIM_SIDE * cosYaw) >> 12);
            target->y = PLAYER_POS_Y(player) - AIM_HEIGHT;
            target->z = PLAYER_POS_Z(player) - ((AIM_SIDE * sinYaw) >> 12);
            *distance = AIM_DISTANCE;
            *pitch = (s32)sAimPitch;
            aim_player_shots(TRUE, *pitch);
        } else {
            sOrbitYaw = (sOrbitYaw + take_yaw_change(yawChange)) & 0xFFF;
            sThirdPersonPitchOffset = clamp_f32(sThirdPersonPitchOffset + pitchChange,
                                                THIRD_PERSON_PITCH_OFFSET_MIN, THIRD_PERSON_PITCH_OFFSET_MAX);
            *pitch = clamp_s32(*pitch + (s32)sThirdPersonPitchOffset, THIRD_PERSON_PITCH_MIN, THIRD_PERSON_PITCH_MAX);
            // Scrolling up zooms in.
            sZoom = clamp_f32(sZoom - wheel * ZOOM_STEP, ZOOM_MIN, ZOOM_MAX);
            *distance = (s32)(*distance * sZoom);
            aim_player_shots(FALSE, 0);
        }

        cam->yaw = sOrbitYaw;
        *yaw = sOrbitYaw + cam->yawOffset;
    }

    sLastMode = mode;
}

// Called by the us.rev1.toml hook in Mega Man's update, right after it copies the controller into his own pad
// (0x11C held, 0x11E pressed, PSX style bits: 0x10 up, 0x20 right, 0x40 down, 0x80 left, 0x400/0x800 L1/R1 strafe).
// With the free third person camera, the direction pressed is relative to the camera: Mega Man turns towards it
// and walks forward with his own tank controls, keeping the game's collision and animations.
void mouse_camera_player_input(void) {
    u8* player = D_802049B0_1DFDB0;
    u16* held = (u16*)(player + 0x11C);
    u16* pressed = (u16*)(player + 0x11E);
    s32 side, forward, desired, diff;

    if (!sFreeCameraActive || sMouseCameraFrames <= 0 || (PLAYER_INPUT_FLAGS(player) & 0x60)) {
        return;
    }

    if (sAiming) {
        // Face where the camera aims and strafe with left and right, like a shooter.
        PLAYER_YAW(player) = (sOrbitYaw - 0x800) & 0xFFF;
        if (*held & 0x80) {
            *held |= 0x400;
        }
        if (*held & 0x20) {
            *held |= 0x800;
        }
        if (*pressed & 0x80) {
            *pressed |= 0x400;
        }
        if (*pressed & 0x20) {
            *pressed |= 0x800;
        }
        *held &= ~0xA0;
        *pressed &= ~0xA0;
        return;
    }

    // Leave the game's own strafing and lock-on alone.
    if (*held & 0xE00) {
        return;
    }

    forward = ((*held & 0x10) ? 1 : 0) - ((*held & 0x40) ? 1 : 0);
    side = ((*held & 0x20) ? 1 : 0) - ((*held & 0x80) ? 1 : 0);
    if (forward == 0 && side == 0) {
        return;
    }

    // Mega Man faces half a turn away from the camera's yaw when it's behind him.
    desired = sOrbitYaw - 0x800 + func_80032664_DA64(side, forward);
    diff = clamp_s32(wrap_angle(desired - PLAYER_YAW(player)), -FREE_CAMERA_TURN_RATE, FREE_CAMERA_TURN_RATE);
    PLAYER_YAW(player) = (PLAYER_YAW(player) + diff) & 0xFFF;

    *held = (*held & ~0xF0) | 0x10;
    if (*pressed & 0xF0) {
        *pressed = (*pressed & ~0xF0) | 0x10;
    }
}

// camera_set_orbit
RECOMP_PATCH void func_80032FC0_E3C0(Vec3i* target, s32 distance, s32 yaw, s32 pitch) {
    PsxMatrix* rot = &D_80210990_1EBD90[0];
    PsxMatrix* view = &D_80210990_1EBD90[1];
    Vec3i localTarget;
    Vec3i offset;
    Vec3i viewOffset;
    s32 shift;
    s32 scaledDistance;

    //@recomp Let the mouse camera drive the main gameplay camera, which always orbits the camera's own target.
    if (target == &D_80204348_1DF748.target) {
        localTarget = *target;
        target = &localTarget;
        mouse_camera_update(target, &distance, &yaw, &pitch);
    }

    memcpy(&D_80162830_13DC30, target, sizeof(Vec3i));
    D_80162840_13DC40 = distance;
    D_80162844_13DC44 = yaw;
    D_80162848_13DC48 = pitch;

    D_80204400_1DF800.x = -pitch & 0xFFF;
    D_80204400_1DF800.y = yaw & 0xFFF;
    D_80204400_1DF800.z = 0;
    func_80031600_CA00(&D_80204400_1DF800, rot);
    func_80031E10_D210(rot, view);

    // Shifts go through u32 like the original sllv, since shifting negative values left is undefined in C.
    shift = D_80210B61_1EBF61;
    scaledDistance = (s32)((u32)distance << shift);

    offset.x = (s32)((u32)-target->x << shift) +
               ((((scaledDistance * func_800324B4_D8B4(yaw)) >> 12) * func_80032498_D898(pitch)) >> 12);
    offset.y = (s32)((u32)-target->y << shift) + ((scaledDistance * func_800324B4_D8B4(pitch)) >> 12);
    offset.z = (s32)((u32)-target->z << shift) +
               ((((scaledDistance * func_80032498_D898(yaw)) >> 12) * func_80032498_D898(pitch)) >> 12);
    func_80030AA0_BEA0(view, &offset, (Vec3i*)view->t);
    memcpy(&D_801D4760_1AFB60, view, sizeof(PsxMatrix));

    offset.x = ((((distance * func_800324B4_D8B4(yaw)) >> 12) * func_80032498_D898(pitch)) >> 12) - target->x;
    offset.y = ((distance * func_800324B4_D8B4(pitch)) >> 12) - target->y;
    offset.z = ((((distance * func_80032498_D898(yaw)) >> 12) * func_80032498_D898(pitch)) >> 12) - target->z;

    memcpy(&D_802043F0_1DF7F0, &D_802043E0_1DF7E0, sizeof(Vec3i));

    D_80195E90_171290[0] = -offset.x;
    D_802043E0_1DF7E0.x = (s16)-offset.x;
    D_80195E90_171290[1] = -offset.y;
    D_802043E0_1DF7E0.y = (s16)-offset.y;
    D_80195E90_171290[2] = -offset.z;
    D_802043E0_1DF7E0.z = (s16)-offset.z;

    func_80030AA0_BEA0(view, &offset, &viewOffset);
    D_80206B58_1E1F58[0] = viewOffset.x;
    D_80206B58_1E1F58[1] = viewOffset.y;
    D_80206B58_1E1F58[2] = viewOffset.z;
}

// Tag for the Mega Man part being queued. Each part gets its own so RT64 interpolates it against the same part on the
// previous frame (they used to share one), and the viewmodel uses different ones so it never blends with the body.
#define MEGAMAN_PART_GFX_TAG(part) (0x4D454700 | (part))
#define MEGAMAN_VIEWMODEL_GFX_TAG(part) (0x4D455600 | (part))
static u32 sMegaManPartTag = MEGAMAN_PART_GFX_TAG(0);

// Called by the us.rev1.toml hook right before a Mega Man part is queued.
void set_megaman_part_gfx_tag(void) {
    gCurrGfxTag = sMegaManPartTag;
}

// Queues one of Mega Man's parts, placed by its bone and seen through the given view matrix.
static void draw_megaman_part(u8* player, PsxMatrix* view, s32 bone, s32 part, u32 tag) {
    PsxMatrix* partMtx = &D_80210990_1EBD90[0];
    Vec3i bonePos;

    func_800306F0_BAF0(view, &PLAYER_BONE_MATRICES(player)[bone], partMtx);
    func_800308E8_BCE8(view, &PLAYER_BONE_POSITIONS(player)[bone], &bonePos);
    partMtx->t[0] = view->t[0] + bonePos.x;
    partMtx->t[1] = view->t[1] + bonePos.y;
    partMtx->t[2] = view->t[2] + bonePos.z;
    sMegaManPartTag = tag;
    func_80084870_5FC70(0, 6, part, partMtx, D_80210B62_1EBF62);
}

// draw_megaman
RECOMP_PATCH void func_80039FE0_153E0(u8* player) {
    PsxMatrix* view = &D_801D4760_1AFB60;
    s32 variant;
    u8* groupCounts;
    s32 group;
    s32 partIndex = 0;
    s32 i;
    //@recomp In first person the camera sits inside the head, so the head, helmet and face are hidden and the rest of
    // the body is drawn through the viewmodel camera.
    bool firstPerson = sFirstPersonActive && sMouseCameraFrames > 0;

    if (PLAYER_COMBAT_STATE(player) == 0) {
        variant = 0;
    } else if (*(s8*)(player + 0x170) != 0) {
        variant = 1;
    } else {
        variant = 2;
    }

    func_800283A8_37A8(0, 6, func_8008128C_5C68C, NULL);

    groupCounts = &D_800AC9BC_87DBC[variant * 5];
    for (group = 0; group < 5; group++) {
        for (i = 0; i < groupCounts[group]; i++) {
            s32 part = D_800AC9CC_87DCC[partIndex++];
            if (!firstPerson) {
                draw_megaman_part(player, view, part, part, MEGAMAN_PART_GFX_TAG(part));
            } else if (FIRST_PERSON_PART_MASK & (1 << part)) {
                draw_megaman_part(player, &sViewmodelView, part, part, MEGAMAN_VIEWMODEL_GFX_TAG(part));
            }
        }
    }

    if (firstPerson) {
        return;
    }

    // The helmet and face are both placed by the head bone, and the face reuses the helmet's matrix.
    draw_megaman_part(player, view, MEGAMAN_HEAD_BONE, MEGAMAN_HELMET_PART, MEGAMAN_PART_GFX_TAG(MEGAMAN_HELMET_PART));
    func_8008498C_5FD8C(0, 6, PLAYER_FACE(player), PLAYER_MOUTH(player), &D_80210990_1EBD90[0]);
}

void guMtxF2L(float mf[4][4], Mtx* m);
float sinf(float x);
float cosf(float x);

static void mtx_l2f(Mtx* m, float mf[4][4]) {
    s32* m1 = &m->m[0][0];
    s32* m2 = &m->m[2][0];
    s32 r, c;

    for (r = 0; r < 4; r++) {
        for (c = 0; c < 2; c++) {
            s32 hi = *m1++;
            s32 lo = *m2++;
            mf[r][2 * c] = (float)(s32)((hi & 0xFFFF0000) | ((lo >> 16) & 0xFFFF)) / 65536.0f;
            mf[r][2 * c + 1] = (float)(s32)((hi << 16) | (lo & 0xFFFF)) / 65536.0f;
        }
    }
}

// The game's own perspective builder (func_8002C238) keeps the PSX conventions: Y points down, so m[1][1] is negative,
// and Z points forward, so m[2][3] is +scale. It also leaves the near and far planes it used in these.
extern f32 D_80210B18_1EBF18; // Near plane.
extern f32 D_8021D928_1F8D28; // Far plane.

// Rebuilds the game's perspective matrix with the first person field of view, and a close near plane in first
// person so Mega Man's arms aren't clipped (the game uses a near plane of ~190 units). The scene's aspect ratio and far
// plane are kept since each area sets up its own projection. Returns FALSE if nothing changed.
bool mouse_camera_adjust_projection(Mtx* projection, u16* perspNorm) {
    float mf[4][4];
    float scale;
    float aspect;
    float near = D_80210B18_1EBF18;
    float far = D_8021D928_1F8D28;
    float halfFov;
    float xScale;
    s32 r, c;

    sCrosshairVisible = FALSE;
    if (sMouseCameraFrames <= 0) {
        return FALSE;
    }
    sMouseCameraFrames--;
    sCrosshairVisible = (sFirstPersonActive || sAiming) && PLAYER_COMBAT_STATE(D_802049B0_1DFDB0) != 0;

    mtx_l2f(projection, mf);
    scale = mf[2][3];
    // Only touch matrices that look like the game's perspective.
    if (scale <= 0.0f || mf[3][3] != 0.0f || mf[0][0] <= 0.0f || mf[1][1] >= 0.0f || near <= 0.0f || far <= near) {
        return FALSE;
    }

    aspect = -mf[1][1] / mf[0][0];
    if (sFirstPersonActive) {
        near = FIRST_PERSON_NEAR_PLANE;
    } else if (sFreeCameraActive) {
        near = THIRD_PERSON_NEAR_PLANE;
    }

    // The field of view is horizontal, measured at the game's native 4:3, and only replaces the game's in first person.
    if (sFirstPersonActive) {
        halfFov = (float)recomp_get_mouse_camera_fov() * (3.14159265f / 360.0f);
        xScale = cosf(halfFov) / sinf(halfFov) * (4.0f / 3.0f) / aspect;
    } else {
        xScale = mf[0][0] / scale;
    }

    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            mf[r][c] = 0.0f;
        }
    }
    mf[0][0] = xScale * scale;
    mf[1][1] = -xScale * aspect * scale;
    mf[2][2] = -(near + far) / (near - far) * scale;
    mf[2][3] = scale;
    mf[3][2] = 2.0f * near * far / (near - far) * scale;
    guMtxF2L(mf, projection);

    if (near + far <= 2.0f) {
        *perspNorm = 65535;
    } else {
        *perspNorm = (u16)((2.0f * 65536.0f) / (near + far));
        if (*perspNorm == 0) {
            *perspNorm = 1;
        }
    }
    return TRUE;
}

extern Gfx* D_801A90F0_1844F0; // Display list head.

static void draw_rect(s32 ulx, s32 uly, s32 lrx, s32 lry, u8 r, u8 g, u8 b, u8 a) {
    gDPSetPrimColor(D_801A90F0_1844F0++, 0, 0, r, g, b, a);
    gDPFillRectangle(D_801A90F0_1844F0++, ulx, uly, lrx, lry);
}

// Draws a small cross at the center of the screen, with a dark outline so it reads on bright and dark backgrounds.
void mouse_camera_draw_crosshair(void) {
    const s32 cx = SCREEN_WIDTH / 2;
    const s32 cy = SCREEN_HEIGHT / 2;
    const s32 gap = 2;
    const s32 length = 5;

    if (sHitmarkerFrames > 0) {
        sHitmarkerFrames--;
    }
    if (!sCrosshairVisible) {
        return;
    }

    gDPPipeSync(D_801A90F0_1844F0++);
    gDPSetCycleType(D_801A90F0_1844F0++, G_CYC_1CYCLE);
    gDPSetRenderMode(D_801A90F0_1844F0++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineMode(D_801A90F0_1844F0++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gEXSetRectAspect(D_801A90F0_1844F0++, G_EX_ASPECT_AUTO);

    // Outline.
    draw_rect(cx - gap - length - 1, cy - 1, cx - gap + 1, cy + 2, 0, 0, 0, 160);
    draw_rect(cx + gap, cy - 1, cx + gap + length + 2, cy + 2, 0, 0, 0, 160);
    draw_rect(cx - 1, cy - gap - length - 1, cx + 2, cy - gap + 1, 0, 0, 0, 160);
    draw_rect(cx - 1, cy + gap, cx + 2, cy + gap + length + 2, 0, 0, 0, 160);
    // Cross.
    draw_rect(cx - gap - length, cy, cx - gap, cy + 1, 255, 255, 255, 230);
    draw_rect(cx + gap + 1, cy, cx + gap + length + 1, cy + 1, 255, 255, 255, 230);
    draw_rect(cx, cy - gap - length, cx + 1, cy - gap, 255, 255, 255, 230);
    draw_rect(cx, cy + gap + 1, cx + 1, cy + gap + length + 1, 255, 255, 255, 230);

    // Hitmarker: a red X between the arms of the cross, fading out.
    if (sHitmarkerFrames > 0) {
        u8 alpha = (u8)(255 * sHitmarkerFrames / HITMARKER_FRAMES);
        s32 d;
        for (d = 3; d <= 7; d++) {
            draw_rect(cx - d, cy - d, cx - d + 2, cy - d + 2, 255, 50, 40, alpha);
            draw_rect(cx + d - 1, cy - d, cx + d + 1, cy - d + 2, 255, 50, 40, alpha);
            draw_rect(cx - d, cy + d - 1, cx - d + 2, cy + d + 1, 255, 50, 40, alpha);
            draw_rect(cx + d - 1, cy + d - 1, cx + d + 1, cy + d + 1, 255, 50, 40, alpha);
        }
    }

    gDPPipeSync(D_801A90F0_1844F0++);
}
