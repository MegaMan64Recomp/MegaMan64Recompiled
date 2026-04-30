#include "patches.h"
#include "common_structs.h"
#include "tag_terrain.h"
#include "tag_helper.h"
#include "patches.h"

// extern u32 gCurrGfxTag;
extern bool skip_terrain_interpolation;

void recomp_tag_terrain(u16 x, u16 z) {
    gCurrGfxTag = TAG_TERRAIN(x,z);
}

void recomp_tag_terrain_transparent(u32 id) {
    gCurrGfxTag = TAG_TERRAIN_XLU(id);
}

extern s32 D_800CEA24_A9E24;
extern void* func_800869D0_61DD0;
extern void* func_800281C8_35C8(s32);
extern void func_800283A8_37A8(s8, s8, void*, void*);
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} UnkStructTile;


//render_fill_tile
RECOMP_PATCH void func_800868C0_61CC0(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    UnkStructTile* temp_v0;

    //TODO Remove when implementing draw distance patches, as it will be redundant
    //@recomp hardcode the tile dimensions to prevent graphical issues
    arg0 = 0;
    arg2 = 0;
    arg1 = 0x7F;
    arg3 = 0x7F;

    if (D_800CEA24_A9E24 != 0) {
        temp_v0 = func_800281C8_35C8(8);
        if (temp_v0 != NULL) {
            var_v0 = arg0;
            if (var_v0 < 0) {
                var_v0 += 3;
            }
            temp_v0->unk0 = (s16) (var_v0 >> 2);

            var_v0_2 = arg2;
            if (var_v0_2 < 0) {
                var_v0_2 += 3;
            }
            temp_v0->unk4 = (s16) (var_v0_2 >> 2);

            var_v0_3 = arg1 + 4;
            if (var_v0_3 < 0) {
                var_v0_3 = arg1 + 7;
            }
            temp_v0->unk2 = (s16) (var_v0_3 >> 2);

            var_v0_4 = arg3 + 4;
            if (var_v0_4 < 0) {
                var_v0_4 = arg3 + 7;
            }
            temp_v0->unk6 = (s16) (var_v0_4 >> 2);

            //@recomp set map tile tag
            gCurrGfxTag = TAG_TERRAIN(0xFFFF, 0xFFFF);

            func_800283A8_37A8(0, 4, &func_800869D0_61DD0, temp_v0);
        }
    }
}

extern Tile D_801B5088_190488[1710];
extern Unk_801BC55C D_801BC55C_19795C;
//gesellshaft_clouds_loop
RECOMP_PATCH void func_80239A20_ovl25(Arg_80239A20_ovl25* arg0) {
    u16 var_v0;
    s32 iter;
    s16* zOffset;
    s16* unk2Ptr;
    s32 unkConst = 0x200;


    if (arg0->unk8 == 0) {
        if (D_801BC55C_19795C.unk2 == 0) {
            D_801BC55C_19795C.unk2 = -unkConst;

            zOffset = &D_801B5088_190488->v.z;
            iter = 0xC;

            loop_1:
            if (*(void**)((char*)D_801B5088_190488 + iter) != NULL) {
                *zOffset -= 0x200;
                zOffset += 0x6;
                iter += 0xC;
                goto loop_1;
            }

        }
        arg0->unk8 = (u8) (arg0->unk8 + 1);
        return;
    }


    zOffset = &D_801B5088_190488->v.z;
    iter = 0xC;
    unk2Ptr = &D_801BC55C_19795C.unk2;
    *unk2Ptr = *unk2Ptr + 0x40;

    loop_7:
        if (*(void**)((char*)D_801B5088_190488 + iter) != NULL) {
            if (*unk2Ptr == unkConst) {
                var_v0 = *zOffset - 0x3C0;
            } else {
                var_v0 = *zOffset + 0x40;
            }

            *zOffset = var_v0;
            zOffset += 0x6;
            iter += 0xC;
            goto loop_7;
        }

    if (D_801BC55C_19795C.unk2 == 0x200) {
        D_801BC55C_19795C.unk2 = -0x200;

        //@recomp skip interpolation when resetting the position of the cloud terrain
        skip_terrain_interpolation = TRUE;
    }
}
