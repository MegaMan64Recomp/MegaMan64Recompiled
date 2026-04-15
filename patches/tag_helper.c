#include "patches.h"

u32 gCurrGfxTag;

void set_curr_gfx_tag(u32 tag) {
    gCurrGfxTag = tag;
}

u32 get_curr_gfx_tag() {
    return gCurrGfxTag;
}

void clear_curr_gfx_tag() {
    gCurrGfxTag = 0;
}