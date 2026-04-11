#ifndef UI_PATCHES_H
#define UI_PATCHES_H

#include "common_structs.h"

typedef struct {
    RGBA32 rgba;
    Vec2s v;
} ColoredVert;

typedef struct {
    u8 s, t;
    u16 pallete;
} Texture;

typedef struct {
    s8 xUpper;
    s8 xLower;
} xSpriteSize;

typedef struct {
    s8 yUpper;
    s8 yLower;
} ySpriteSize;

typedef struct {
    s16 xSpriteSize;
    s16 ySpriteSize;
} xySpriteSize;

typedef union {
    struct {
        u32 flag : 8;
        u32 prev : 24;
    } bitfield;
    u32 header;
} UIStackHeader;

typedef struct {
    UIStackHeader header;
    RGBA32 rgba;
    void* data;
} UiStackElement;

typedef struct {
    RGBA32 rgba;
    s16 llxOffset;
    s16 llyOffset;
    xySpriteSize xySpriteSize;
    Texture texture;
    u16 unk_10;
    u16 textureBank;
} uiElemData;

typedef struct  {
    u32 ui_Header_Bitfield;
    RGBA32 rgba;
    s16 ulxOffset;
    s16 ulyOffset;
    Texture texture;
    s16 urxOffset;
    s16 uryOffset;
    u8 textureXUnk;
    u8 textureYUnk;
    u16 texturePallete;
    s16 llxOffset;
    s16 llyOffset;
    u8 textureX;
    u8 textureY;
    u16 unk_1e;
    s16 lrxOffset;
    s16 lryOffset;
    u8 width; /* Unused? */
    u8 height; /* Unused? */
    u16 unk_26; /* Padding? */
} UiElementGeneric;

typedef struct {
    UIStackHeader header;
    ColoredVert verts[4];
} UIElemQuad;

typedef struct {
    UIStackHeader header;
    RGBA32 rgbFlag;
    u32 unk8;
} UIElemUnk;

typedef struct {
    UIStackHeader header;
    RGBA32 rgba32;
    Vec2s verts[4];
} UIElemMonoQuad;

typedef struct {
    u32 header;
    RGBA32 rgba;
    s16 ulx, uly;
    u8 s, t;
    u16 unkE;
    s16 w, h;
} UIElemSquare;

typedef struct {
    s16 xOffset, yOffset;
    s16 width, height;
} MinimapBackground;

typedef struct {
    u8 status;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 combatFrame;
    u8 unk9;
    u16 injuredAnimation;
    u8 combatFlag;
    u8 injured;
    u8 positionType;
    u8 unkf;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    s16 xOffset;
    s16 yOffset;
    s32 unk18;
    s16 unk1c;
    s16 unk1e;
    s32 unk20;
    u8 energyBarStatus;
} HealthBarData; // size = 0x24

typedef struct {
    u8 status;
    u8 unks[35];
} BossHealthBarData;

typedef struct {
    u8 status;
} UnkHealthBarData;

extern s8 D_800A8848_83C48;

#endif //UI_PATCHES_H
