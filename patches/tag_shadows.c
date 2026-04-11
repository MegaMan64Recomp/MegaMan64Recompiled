#include "patches.h"
#include "common_structs.h"
#include "tag_helper.h"
#include "gfx_patches.h"
#include "tag_shadows.h"
#include "tag_actors.h"

//@recomp used to keep track of which actor created the shadow
u32 sShadowTags[64];

//process_shadows
RECOMP_PATCH void func_800893E4_647E4(void) {
    UnkVecStruct unkVec;
    s32 temp_x;
    s32 temp_y;
    s32 temp_z;
    s32 shadowArraySize;
    s32 x;
    s32 y;
    s32 z;
    u32 offset;
    s32 index;
    s16 *unkVecPtr;
    ShadowBuff *shadowBuff;
    s32 *temp_s5;

    shadowArraySize = D_801A9064_184464;
    index = 0;
    if (shadowArraySize > 0) {
        offset = 0;
        unkVecPtr = &D_801D4760_1AFB60;
        temp_s5 = &D_8021D95C_1F8D5C;
        do {
            shadowBuff = (ShadowBuff *) func_800281C8_35C8(0x50);
            if (shadowBuff != NULL) {
                bzero(shadowBuff, 0x50);
                memcpy(&unkVec, unkVecPtr, 0x20);
                memcpy(&shadowBuff->x, ((u8 *) &D_801FEEC0_1DA2C0 + offset), 8);
                shadowBuff->size = *((u16 *) ((u8 *) &D_801FEEC8_1DA2C8 + offset));
                shadowBuff->opacity = *((u16 *) ((u8 *) &D_801FEECA_1DA2CA + offset));
                func_80031E5C_D25C(unkVecPtr);
                memcpy(&D_802046A0_1DFAA0, &D_80206B58_1E1F58, 0x10);
                memcpy(temp_s5, &shadowBuff->x, 8);
                func_80032068_D468(temp_s5, &D_801BC0A8_1974A8, &D_801D886C_1B3C6C);

                x = D_801BC0B0_1974B0;
                if (x < 0) {
                    x += 3;
                }

                if (D_801D87F0_1B3BF0 < (x >> 2)) {
                } else {
                    temp_x = shadowBuff->x << D_80210B61_1EBF61;
                    temp_y = shadowBuff->y << D_80210B61_1EBF61;
                    temp_z = shadowBuff->z << D_80210B61_1EBF61;

                    x = (D_801D4760_1AFB60 * temp_x)
                        + (D_801D4762_1AFB62 * temp_y)
                        + (D_801D4764_1AFB64 * temp_z);

                    if (x < 0) {
                        x += 0xFFF;
                    }

                    y = (D_801D4766_1AFB66 * temp_x)
                        + (D_801D4768_1AFB68 * temp_y)
                        + (D_801D476A_1AFB6A * temp_z);

                    if (y < 0) {
                        y += 0xFFF;
                    }

                    z = (D_801D476C_1AFB6C * temp_x)
                        + (D_801D476E_1AFB6E * temp_y)
                        + (D_801D4770_1AFB70 * temp_z);

                    if (z < 0) {
                        z += 0xFFF;
                    }

                    unkVec.x = (x >> 0xC)
                                 + D_801D4774_1AFB74
                                 + *((s8 *) ((u8 *) &D_801FEECC_1DA2CC + offset));

                    unkVec.y = (y >> 0xC)
                                 + D_801D4778_1AFB78
                                 + *((s8 *) ((u8 *) &D_801FEECD_1DA2CD + offset));

                    unkVec.z = (z >> 0xC)
                                 + D_801D477C_1AFB7C
                                 + *((s8 *) ((u8 *) &D_801FEECE_1DA2CE + offset));

                    shadowBuff->index = index;
                    func_8002C5A4_79A4(&unkVec, shadowBuff);

                    //@recomp tag shadows
                    u32 shadowArrayIndex = offset/0x10;
                    gCurrGfxTag = TAG_SHADOW(sShadowTags[shadowArrayIndex]);
                    
                    func_800283A8_37A8(0, 6, &func_800896D8_64AD8, shadowBuff);
                }
            }
            index++;
            offset += 0x10;
        } while (index < shadowArraySize);
    }
    D_801DEB14_1B9F14 = D_801A9064_184464;
    D_801A9064_184464 = 0;
}


//push_actor_shadow
RECOMP_PATCH void func_8003B564_16964(NPCActor *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v2;
    s32 var_a1;
    s32 offset2;
    s32 offset;
    s32 shadowArraySize;
    s32 temp_s6;

    shadowArraySize = D_801A9064_184464;
    temp_s6 = -3;
    var_a1 = arg0->unk16;

    if (shadowArraySize < 64) {
        offset = shadowArraySize << 4;
        *((u16 *) ((u8 *) &D_801FEEC0_1DA2C0 + offset)) = arg0->unk14;
        if (arg0->unk76 != 0) {
            var_a1 = (s16) arg0->unk16;
        } else {
            var_a1 = func_8003E934_19D34(arg0);
            temp_v2 = arg0->unk16;
            if (var_a1 < temp_v2) {
                var_a1 = temp_v2;
            }
        }

        *((u16 *) ((u8 *) &D_801FEEC2_1DA2C2 + offset)) = var_a1;
        *((u16 *) ((u8 *) &D_801FEEC4_1DA2C4 + offset)) = arg0->unk18;
        offset2 = shadowArraySize << 4;
        *((u16 *) ((u8 *) &D_801FEEC6_1DA2C6 + offset2)) = (arg0->unk56 & 0xFFF) | ((arg2 >> 6) << 0xC);
        *((u16 *) ((u8 *) &D_801FEEC8_1DA2C8 + offset2)) = arg1 | (arg3 << 0xE);
        *((u16 *) ((u8 *) &D_801FEECA_1DA2CA + offset2)) = var_a1 - arg0->unk16;
        *((u8 *) &D_801FEECC_1DA2CC + offset) = 0;
        *((u8 *) &D_801FEECD_1DA2CD + offset) = temp_s6;
        *((u8 *) &D_801FEECE_1DA2CE + offset) = 0;

        //@recomp Embed actor number into shadow struct
        sShadowTags[shadowArraySize] = (u32) arg0;

        D_801A9064_184464 += 1;
    }
}

// push_player_shadow
RECOMP_PATCH void func_8003B2B4_166B4(PlayerState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u16 var_a2;
    s32 offset2;
    s32 offset;
    u32 shadowArraySize;

    shadowArraySize = D_801A9064_184464;
    if (shadowArraySize < 0x40U) {
        offset = shadowArraySize * 0x10;
        *((u16 *) ((u8 *) &D_801FEEC0_1DA2C0 + offset)) = arg0->x;
        if (arg0->unk76 != 0) {
            var_a2 = arg0->z;
        } else {
            var_a2 = func_8003E934_19D34(arg0);
        }
        *((u16 *) ((u8 *) &D_801FEEC2_1DA2C2 + offset)) = var_a2;
        *((u16 *) ((u8 *) &D_801FEEC4_1DA2C4 + offset)) = arg0->y;
        offset2 = shadowArraySize * 0x10;
        *((u16 *) ((u8 *) &D_801FEEC6_1DA2C6 + offset2)) = ((arg0->yaw - arg0->unk116) & 0xFFF) | ((arg2 >> 6) << 0xC);
        *((u16 *) ((u8 *) &D_801FEEC8_1DA2C8 + offset2)) = arg1 | (arg3 << 0xE);
        *((u16 *) ((u8 *) &D_801FEECA_1DA2CA + offset2)) = var_a2 - arg0->z;
        *((u8 *) &D_801FEECC_1DA2CC + offset2) = 0;
        *((u8 *) &D_801FEECD_1DA2CD + offset2) = -3;
        *((u8 *) &D_801FEECE_1DA2CE + offset2) = 0;

        //@recomp Embed actor number into shadow struct
        sShadowTags[shadowArraySize] = (u32) arg0;

        D_801A9064_184464 += 1;
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} UnkActor;

//push_boss_shadows
RECOMP_PATCH void func_8003B6B8_16AB8(UnkActor *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 temp_v0;
    s32 offset;
    s32 temp_v1;
    s16 temp_a0;
    if (D_801A9064_184464 < 64) {
        offset = D_801A9064_184464 * 0x10;
        *((u16 *) ((u8 *) &D_801FEEC0_1DA2C0 + offset)) = arg0->unk0;
        temp_v0 = func_8003E964_19D64(arg0, 0, 0, 0xFFFF);
        *((u16 *) ((u8 *) &D_801FEEC2_1DA2C2 + offset)) = temp_v0;

        *((u16 *) ((u8 *) &D_801FEEC4_1DA2C4 + offset)) = arg0->unk4;
        *((u16 *) ((u8 *) &D_801FEEC6_1DA2C6 + offset)) = (arg3 & 0xFFF) | ((arg2 >> 6) << 0xC);
        *((u16 *) ((u8 *) &D_801FEEC8_1DA2C8 + offset)) = arg1 | (arg0->unk6 << 0xE);
        temp_a0 = arg0->unk2;
        *((u8 *) &D_801FEECC_1DA2CC + offset) = 0;
        *((s8 *) &D_801FEECD_1DA2CD + offset) = -3;
        *((u8 *) &D_801FEECE_1DA2CE + offset) = 0;
        temp_v1 = D_801A9064_184464 + 1;
        *((u16 *) ((u8 *) &D_801FEECA_1DA2CA + offset)) = temp_v0 - temp_a0;
        D_801A9064_184464 = temp_v1;

        //@recomp Embed actor number into shadow struct
        u32 shadowArraySize = D_801A9064_184464;
        sShadowTags[shadowArraySize] = (u32) arg0;
    }
}
