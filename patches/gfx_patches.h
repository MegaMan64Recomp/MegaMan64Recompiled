#ifndef GFX_PATCHES_H
#define GFX_PATCHES_H

#include "common_structs.h"

#define GFX_TASK_POOL_SIZE 0x11000
#define GFX_DL_SIZE 0x1E000

#define FILL_COLOR(r, g, b) \
(((GPACK_RGBA5551(r, g, b, 0) | 1) << 16) | GPACK_RGBA5551(r, g, b, 0) | 1)

void* func_800281C8_35C8(s32);

extern void func_8003AA3C_15E3C(void*, s32);
extern void func_8003AB60_15F60(void*, s32);
extern void func_8003AFB0_163B0(void*, s32);
extern void func_8003B27C_1667C(void*, s32);
extern void func_8003B298_16698(void*, s32);
extern u32 D_800D6FBC_B23BC;
extern s32 D_80193C30_16F030;
extern s32 D_801A53D8_1807D8;
extern s32 D_801ADC84_189084;
extern s32 D_801BC17C_19757C;
extern void *D_80210980_1EBD80;
extern Mtx D_801AF370_18A770;
extern Gfx D_80224E80;
extern Gfx D_80224F68;

extern void func_8002CAD0_7ED0();
extern void func_80089EA0_652A0();
extern void func_80092C6C_6E06C(Gfx *, u32, s32, u32);

extern s64 D_A4770[];
extern s64 D_A5B00[];
extern s64 D_D1450[];
extern s64 D_D1870[];

typedef struct GfxBuffers{
    Gfx* dls[2];
} GfxBuffers;

typedef struct GfxContext{
    /* 0x00 */ GfxTaskNode* taskPoolStart[16];
    /* 0x40 */ GfxTaskNode* taskPoolEnd[16];
    /* 0x80 */ s16 unk_80;
    /* 0x82 */ s16 unk_82;
    /* 0x84 */ s16 unk_84;
    /* 0x86 */ s16 unk_86;
    /* 0x88 */ s16 unk_88;
    /* 0x8A */ s16 unk_8A;
    /* 0x8C */ s16 uly;
    /* 0x8E */ s16 ulx;
    /* 0x90 */ s16 lrx;
    /* 0x92 */ s16 lry;
    /* 0x94 */ s16 fogStart;
    /* 0x96 */ s16 fogEnd;
    /* 0x98 */ s8 fogRed;
    /* 0x99 */ s8 fogGreen;
    /* 0x9A */ s8 fogBlue;
    /* 0x9B */ s8 fogAlpha;
    /* 0x9C */ RGBA32 rgba;
    /* 0xA0 */ s8 primAlpha2;
    /* 0xA1 */ s8 fadeoutTimer;
    /* 0xA2 */ s8 unk_A2;
    /* 0xA3 */ s8 unk_A3;
    /* 0xA4 */ u8 bgPrimRed;
    /* 0xA5 */ u8 bgPrimGreen;
    /* 0xA6 */ u8 bgPrimBlue;
    /* 0xA7 */ s8 bgPrimAlpha;
    /* 0xA8 */ s8 unk_A8;
    /* 0xA9 */ s8 hiResEnabled;
    /* 0xAA */ s8 bufferEnabled;
    /* 0xAB */ s8 bufferNum;
    /* 0xAC */ s8 unk_AC;
    /* 0xAD */ s8 unk_AD;
    /* 0xAE */ s8 unk_AE;
    /* 0xAF */ s8 unk_AF;
    /* 0xB0 */ Vp_t viewport;
    /* 0xC0 */ Mtx identMtx;
    /* 0x100 */ Mtx projectionMtx ;
    /* 0x140 */ u16 perspNorm;
    /* 0x142 */ s16 unk_142;
    /* 0x144 */ s32 unk_144[16];
} GfxContext; // size = 0x188;

#endif //GFX_PATCHES_H
