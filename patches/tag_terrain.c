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