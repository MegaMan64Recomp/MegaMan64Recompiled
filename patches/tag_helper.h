#ifndef TAGGING_H
#define TAGGING_H

#include "ultra64.h"

extern u32 gCurrGfxTag;

#define ADDRESS_TAG(ptr) ((u32) (ptr) & 0x0FFFFFFF)
#define COORD_TAG(x, z) ((((u16)(x) & 0xFFF) << 12) | ((u16)(z) & 0xFFF))

#define TAG_ACTOR(actor) ((u32) (0x10000000 | (ADDRESS_TAG(actor))))
#define TAG_STATIC_ACTOR(actor) ((u32) (0x20000000 | (ADDRESS_TAG(actor))))
#define TAG_MULTIPART_ACTOR(actor) ((u32) (0x30000000 | (ADDRESS_TAG(actor))))
#define TAG_MEGAMAN(actor) ((u32) (0x40000000 | (ADDRESS_TAG(actor))))
#define TAG_DROPS(actor) ((u32) (0x50000000 | (ADDRESS_TAG(actor))))
#define TAG_PLAYER_PROJECTILE(actor) ((u32) (0x60000000 | (ADDRESS_TAG(actor))))
#define TAG_PROJECTILE(actor) ((u32) (0x70000000 | (ADDRESS_TAG(actor))))
#define TAG_EFFECT(actor) ((u32) (0x90000000 | (ADDRESS_TAG(actor))))
#define TAG_TERRAIN(x, z) ((u32)(0xA0000000 | COORD_TAG(x, z)))
#define TAG_TERRAIN_XLU(ptr) ((u32)(0xB0000000 | ADDRESS_TAG(ptr)))
#define TAG_SHADOW(actor) ((u32) (0xC0000000 | (ADDRESS_TAG(actor))))

#endif //TAGGING_H
