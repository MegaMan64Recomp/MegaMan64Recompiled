#ifndef TAG_ACTORS_H
#define TAG_ACTORS_H

#include "common_structs.h"

typedef struct {
    s32 unk0;
    union {
        s32 unk4;
        u8 unk4Bytes[4];
    } unkUnion;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} UnkSpriteStruct; // size = 0x28

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char pad3[0x9];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ char pad11[0x3];
    /* 0x14 */ void* unk14;
    /* 0x18 */ u8 unk18[0x40];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
} ActorBuffer;

typedef struct {
    /* 0x00 */ char pad0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
} UnkStruct2;

typedef struct {
    /* 0x00 */ char pad0[0x1E];
    /* 0x1E */ u16 unk1E;
} UnkStruct3;

typedef void (*ActorFunc)(void *ptr, s32 arg1);
extern void (**D_801AF428_18A828)(EffectActor *);

extern ActorFunc D_800AC980_87D80;
extern StaticActor D_80206B70_1E1F70[32];
extern NPCActor D_80203FF0_1DF3F0[18];
extern MultiPartActor D_801BB770_196B70[4];
extern EffectActor D_801ADD70_189170[64];
extern ProjectileActor D_802059A0_1E0DA0[16];
extern ProjectileActor D_8020DF00_1E9300[32];
extern EffectActor *D_80206B48_1E1F48;
extern s32 D_801BC3BC_1977BC;
extern u16 D_801ADC80_189080;
extern u16 D_802049AC_1DFDAC;
extern UnkSpriteStruct* D_80210480_1EB880[];
extern void* D_801FF364_1DA764;
extern void func_800805F8_5B9F8;

void func_8003A2EC_156EC(void *);
void func_800859B0_60DB0(void *);
void func_80086110_61510(void *);
void func_80028220_3620(s8, s8, void*, void*, s32);
void func_800283A8_37A8(s8, s8, void*, void*);
void func_8002C5A4_79A4(Vp*, Mtx*);
void* func_800281C8_35C8(s32);

#endif //TAG_ACTORS_H
