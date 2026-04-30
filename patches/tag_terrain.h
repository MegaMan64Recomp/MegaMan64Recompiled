#ifndef TAG_TERRAIN_H
#define TAG_TERRAIN_H

typedef struct {
    void* ptr;
    Vec3s v;
    s16 unk;
} Tile;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u8 unk8;
} Arg_80239A20_ovl25;

typedef struct {
    s16 unk0;
    s16 unk2;
} Unk_801BC55C;

#endif //TAG_TERRAIN_H
