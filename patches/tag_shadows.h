#ifndef TAGGING_SHADOWS_H
#define TAGGING_SHADOWS_H

extern s32 D_801A9064_184464;
extern void D_801BC0A8_1974A8;
extern s32 D_801BC0B0_1974B0;
extern s16 D_801D4760_1AFB60;
extern s16 D_801D4762_1AFB62;
extern s16 D_801D4764_1AFB64;
extern s16 D_801D4766_1AFB66;
extern s16 D_801D4768_1AFB68;
extern s16 D_801D476A_1AFB6A;
extern s16 D_801D476C_1AFB6C;
extern s16 D_801D476E_1AFB6E;
extern s16 D_801D4770_1AFB70;
extern s32 D_801D4774_1AFB74;
extern s32 D_801D4778_1AFB78;
extern s32 D_801D477C_1AFB7C;
extern u16 D_801D87F0_1B3BF0;
extern s32 D_801D886C_1B3C6C;
extern s32 D_801DEB14_1B9F14;
extern s16 D_802046A0_1DFAA0;
extern s16 D_80206B58_1E1F58;
extern u8 D_80210B61_1EBF61;
extern s32 D_8021D95C_1F8D5C;

extern s32 D_801A9064_184464;
extern u16 D_802049AC_1DFDAC;
extern s16 D_801FEEC0_1DA2C0;
extern s16 D_801FEEC2_1DA2C2;
extern s16 D_801FEEC4_1DA2C4;
extern s16 D_801FEEC6_1DA2C6;
extern s16 D_801FEEC8_1DA2C8;
extern s16 D_801FEECA_1DA2CA;
extern s8 D_801FEECC_1DA2CC;
extern s8 D_801FEECD_1DA2CD;
extern s8 D_801FEECE_1DA2CE;
extern s8 D_801FEECF_1DA2CF;
// extern NPCActor D_801FFBE0_1DAFE0[0];

void func_8003A2EC_156EC(void *);
s32 func_8003E934_19D34(void *);
void func_800896D8_64AD8();
void* func_800281C8_35C8(s32);
void func_800283A8_37A8(s8, s8, void *, void *);
void func_8002C5A4_79A4(Vp *, Mtx *);
void func_80031E5C_D25C(void *);
void func_80032068_D468(void *, void *, void *);
s16 func_8003E964_19D64(void *, s32, s32, s32);

typedef struct {
    s16 xOffset;
    s16 xUnk1;
    s16 xUnk2;
    s16 yOffset;
    s16 yUnk1;
    s16 yUnk2;
    s16 zOffset;
    s16 zUnk1;
    s16 zUnk2;
    s16 padding;
    s32 x, y, z;
} UnkVecStruct;

typedef struct {
    Mtx mtx;
    s16 x, y, z;
    s16 rot;
    s16 size;
    s16 opacity;
    s32 index;
} ShadowBuff;

#endif //TAGGING_SHADOWS_H
