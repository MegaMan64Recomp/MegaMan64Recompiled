#ifndef TAGGING_H
#define TAGGING_H

extern u32 gCurrGfxTag;

#define ACTOR_TAG_ID 0x1
#define STATIC_ACTOR_TAG_ID 0x2
#define BOSS_TAG_ID 0x3
#define MEGAMAN_TAG_ID 0x4
#define DROPS_TAG_ID 0x5
#define PLAYER_PROJECT_TAG_ID 0x6
#define PROJECT_TAG_ID 0x7
#define EFFECT_TAG_ID 0x9
#define TERRAIN_TAG_ID 0xA
#define TERRAIN_XLU_TAG_ID 0xB
#define SHADOW_TAG_ID 0xC
#define SPRITE_TAG_ID 0xD

#define GET_TAG_ID(tag) (tag >> 28)
#define ADDRESS_TAG(ptr) ((u32) (ptr) & 0x0FFFFFFF)
#define COORD_TAG(x, z) ((((u16)(x) & 0xFFF) << 12) | ((u16)(z) & 0xFFF))

#define TAG_ACTOR(actor) ((u32) (ACTOR_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_STATIC_ACTOR(actor) ((u32) (STATIC_ACTOR_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_BOSS_ACTOR(actor) ((u32) (BOSS_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_MEGAMAN(actor) ((u32) (MEGAMAN_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_DROPS(actor) ((u32) (DROPS_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_PLAYER_PROJECTILE(actor) ((u32) (PLAYER_PROJECT_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_PROJECTILE(actor) ((u32) (PROJECT_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_EFFECT(actor) ((u32) (EFFECT_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_TERRAIN(x, z) ((u32)(TERRAIN_TAG_ID << 28 | COORD_TAG(x, z)))
#define TAG_TERRAIN_XLU(ptr) ((u32) (TERRAIN_XLU_TAG_ID << 28 | ADDRESS_TAG(ptr)))
#define TAG_SHADOW(actor) ((u32) (SHADOW_TAG_ID << 28 | (ADDRESS_TAG(actor))))
#define TAG_SPRITE(actor) ((u32) (SPRITE_TAG_ID << 28 | (ADDRESS_TAG(actor))))

#endif //TAGGING_H
