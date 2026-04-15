#include "patches.h"
#include "tag_actors.h"
#include "tag_helper.h"
#include "common_structs.h"

s32 gCurrentActorTag;

//process_actors
RECOMP_PATCH void func_8003A7CC_15BCC(void) {
    MultiPartActor *multiActorPtr;
    NPCActor *actorPtr;
    s32 multipartActorCount;
    s32 actorCount;

    if (D_802049AC_1DFDAC == 0) {
        multipartActorCount = 4;
        multiActorPtr = D_801BB770_196B70;

        do {
            multiActorPtr--;
            multipartActorCount--;
            if (multiActorPtr->renderFlag & multiActorPtr->unkFlag & 2) {
                //@recomp Tag multipart actors
                gCurrGfxTag = TAG_MULTIPART_ACTOR(multiActorPtr);
                gCurrentActorTag = TAG_MULTIPART_ACTOR(multiActorPtr);
                // recomp_printf("Multipart Actor Tag=%X\n",gCurrGfxTag);
                func_8003A2EC_156EC(multiActorPtr);
                gCurrentActorTag = 0;
            }
        } while (multipartActorCount != 0);

        actorCount = 18;
        actorPtr = D_80203FF0_1DF3F0;
        do {
            actorPtr--;
            actorCount--;
            if (actorPtr->renderFlag & actorPtr->unkFlag & 2) {
                //@recomp Tag NPC actors
                gCurrGfxTag = TAG_ACTOR(actorPtr);
                gCurrentActorTag = TAG_ACTOR(actorPtr);
                // recomp_printf("Actor Tag=%X\n",gCurrGfxTag);
                func_8003A2EC_156EC(actorPtr);
                gCurrentActorTag = 0;
            }
        } while (actorCount != 0);

        if (D_801BC3BC_1977BC != 0) {
            bzero(&D_801BC3BC_1977BC - 0x7, 0x20);
        }
    }
}

//process_static_actors
RECOMP_PATCH void func_8003A9C4_15DC4(void) {
    StaticActor *actor;
    ActorFunc *funcs;
    s32 index;
    u32 iter;

    iter = 0;
    funcs = &D_800AC980_87D80;
    do {
        actor = &D_80206B70_1E1F70[iter];
        index = actor->funcIndex;
        if ((index & actor->actorState & 2) != 0) {
            //@recomp Tag embedding before calling func
            gCurrGfxTag = TAG_STATIC_ACTOR(actor);
            // recomp_printf("Static Actor Tag=%X\n",gCurrGfxTag);
            funcs[(index & 0x1C) >> 2](&actor->actorModelPtr, index);
            gCurrGfxTag = 0;
        }
        iter++;
    } while (iter < 32);
}

//process_projectiles
RECOMP_PATCH void func_8003A888_15C88(void) {
    ProjectileActor *playerProjectile;
    ProjectileActor *enemyProjectile;
    ActorFunc *funcs;
    s32 temp_a1;
    s32 temp_a1_2;
    u32 indexPlayer;
    u32 indexEnemy;

    //Player Projectiles
    indexPlayer = 0;
    funcs = &D_800AC980_87D80;
    do {
        playerProjectile = &D_802059A0_1E0DA0[indexPlayer];
        temp_a1 = playerProjectile->funcIndex;
        if (temp_a1 & playerProjectile->unkFlag & 2) {
            //@recomp Tag NPC actors
            gCurrGfxTag = TAG_PLAYER_PROJECTILE(playerProjectile);
            // recomp_printf("Projectile Tag=%X\n",gCurrGfxTag);
            funcs[(temp_a1 & 0x1C) >> 2](&playerProjectile->unkPtr, temp_a1);
        }
        indexPlayer++;
    } while (indexPlayer < 16);

    //Enemy Projectiles
    indexEnemy = 0;
    funcs = &D_800AC980_87D80;
    do {
        enemyProjectile = &D_8020DF00_1E9300[indexEnemy];
        temp_a1_2 = enemyProjectile->funcIndex;
        if (temp_a1_2 & enemyProjectile->unkFlag & 2) {
            //@recomp Tag NPC actors
            gCurrGfxTag = TAG_PROJECTILE(enemyProjectile);
            // recomp_printf("Enemy Projectile Tag=%X\n",gCurrGfxTag);
            funcs[(temp_a1_2 & 0x1C) >> 2](&enemyProjectile->unkPtr, temp_a1_2);
        }
        indexEnemy++;
    } while (indexEnemy < 32);
}

//process_effect_actors
RECOMP_PATCH void func_8003A94C_15D4C(void) {
    EffectActor *actor;
    s32 funcIndex;
    u32 index;
    ActorFunc *funcs;

    index = 0;
    funcs = &D_800AC980_87D80;

    do {
        actor = &D_801ADD70_189170[index];
        funcIndex = actor->funcIndex;
        if (funcIndex & actor->unkFlag & 2) {
            //@recomp Tag effect actors
            gCurrGfxTag = TAG_EFFECT(actor);
            // recomp_printf("Effect Tag=%X\n", gCurrGfxTag);
            funcs[((funcIndex & 0x1C) >> 2)](&actor->unkPtr, funcIndex);
        }
        index++;
    } while (index < 64);
}

//update_effect_actors
RECOMP_PATCH void func_8004C270_27670(void) {
    EffectActor *actor;
    u8 index;

    actor = D_801ADD70_189170;
    D_80206B48_1E1F48 = actor;
    while (actor < &D_801ADD70_189170[64]) {
        if (D_80206B48_1E1F48->funcIndex & D_80206B48_1E1F48->unkFlag & 1) {
            index = D_80206B48_1E1F48->unk3;
            if (index < 0x80U) {
                gCurrGfxTag = TAG_EFFECT(D_80206B48_1E1F48);
                // recomp_printf("Update Effect Tag=%X\n", gCurrGfxTag);
                D_801AF428_18A828[index](D_80206B48_1E1F48);
            }
        }
        actor = &D_80206B48_1E1F48[1];
        D_80206B48_1E1F48 = actor;
    }
}

//process_sprites?
RECOMP_PATCH void func_8003B994_16D94(void) {
    s32 iter;
    s32 value;
    UnkSpriteStruct* temp_s0;
    UnkSpriteStruct** ptr;

    iter = 0;

    if (D_801ADC80_189080 > iter) {
        value = 0x02FFFFFF;
        ptr = &D_80210480_1EB880;
        do {
            temp_s0 = *ptr;
            if ((temp_s0->unkUnion.unk4Bytes[0] >> 5) & 1) {
                gCurrGfxTag = (s32) temp_s0;
                // recomp_printf("Sprite Tag = %X \n", gCurrGfxTag);
                func_800859B0_60DB0(temp_s0);
                temp_s0->unkUnion.unk4 &= value;
            } else {
                func_80086110_61510(temp_s0);
                temp_s0->unkUnion.unk4 &= value;
                temp_s0->unk18 &= value;
            }
            iter += 1;
            ptr += 1;
        } while (iter < D_801ADC80_189080);
    }
    D_801ADC80_189080 = 0;
}

//actor_bones?
RECOMP_PATCH void func_800817BC_5CBBC(s8 arg0, s8 arg1, s16 boneIndex, UnkStruct3* arg3, UnkStruct2* arg4, u32 arg5, u8 arg6, u8 arg7) {
    ActorBuffer* actorBuffer;
    u8 temp_v1;

    actorBuffer = func_800281C8_35C8(0x68);
    if (actorBuffer != NULL) {
        actorBuffer->unk0 = boneIndex;
        func_8002C5A4_79A4(arg3, (Mtx*)actorBuffer->unk18);
        actorBuffer->unkC = arg4->unkF;
        actorBuffer->unkD = 1;
        actorBuffer->unk2 = 0xFF;

        actorBuffer->unkE = arg4->unkD;

        temp_v1 = arg4->unkE | ((arg5 & 0x8000) >> 15);
        actorBuffer->unk64 = arg5 & 0x7FFF;
        actorBuffer->unkF = temp_v1;
        actorBuffer->unk10 = arg4->unkC;
        temp_v1 = actorBuffer->unkF;
        actorBuffer->unk14 = arg4;
        actorBuffer->unk60 = 0;
        actorBuffer->unk5C = 0;
        actorBuffer->unk66 = arg6;
        actorBuffer->unk67 = arg7;
        actorBuffer->unk58 = 0;

        //@recomp actor bone tagging
        if (gCurrentActorTag != 0) {
            gCurrGfxTag = gCurrentActorTag + boneIndex;
        }

        if (temp_v1 != 0) {
            func_80028220_3620(arg0, arg1, &func_800805F8_5B9F8, actorBuffer, (s32) arg3->unk1E);
        } else {
            func_800283A8_37A8(arg0, arg1, &func_800805F8_5B9F8, actorBuffer);
        }
    }
    return;
}
