#include "tag_terrain.h"
#include "tag_helper.h"
#include "patches.h"

// extern u32 gCurrGfxTag;

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