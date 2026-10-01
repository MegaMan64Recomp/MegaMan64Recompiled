#ifndef MOUSE_CAMERA_H
#define MOUSE_CAMERA_H

#include "patch_helpers.h"

typedef enum {
    MOUSE_CAMERA_OFF = 0,
    MOUSE_CAMERA_THIRD_PERSON = 1,
    MOUSE_CAMERA_FIRST_PERSON = 2,
} MouseCameraMode;

DECLARE_FUNC(s32, recomp_get_mouse_camera_mode);
// Mouse movement since the previous call, scaled by the mouse sensitivity.
DECLARE_FUNC(void, recomp_get_mouse_camera_deltas, float* x, float* y);
// Horizontal field of view in degrees at 4:3.
DECLARE_FUNC(s32, recomp_get_mouse_camera_fov);
// Whether over the shoulder aiming (right mouse button in third person) is held.
DECLARE_FUNC(s32, recomp_get_mouse_camera_aim);
// Mouse wheel notches scrolled since the previous call, positive when scrolling up.
DECLARE_FUNC(s32, recomp_get_mouse_camera_wheel);

#ifdef MIPS
bool mouse_camera_adjust_projection(Mtx* projection, u16* perspNorm);
void mouse_camera_draw_crosshair(void);
#endif

#endif //MOUSE_CAMERA_H
