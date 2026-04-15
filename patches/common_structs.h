#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

typedef struct TaskNode {
    struct TaskNode* next;
    struct TaskNode* prev;
    void* body;
} TaskNode;

typedef void (*GfxCallback)(void* arg);

typedef struct GfxTaskNode {
    struct GfxTaskNode* next;
    struct GfxTaskNode* prev;
    GfxCallback func;
    void* arg;
    s8 poolIndex;
    s8 unk11;
    s16 unk12;
    s32 unk14;
} GfxTaskNode;

typedef union {
    struct {
        u8 r, g, b, a;
    } c;
    u32 rgba32;
} RGBA32;

typedef struct {
    s16 x;
    s16 y;
} Vec2s;

typedef struct {
    /* 0x000 */ s8 tempFiller1[8];
    /* 0x008 */ s32 animation;
    /* 0x00C */ s8 tempFiller2[8];
    /* 0x014 */ s16 x, z, y;
    /* 0x01A */ s8 tempFiller3[60];
    /* 0x056 */ s16 yaw;
    /* 0x058 */ s8 tempFiller4[22];
    /* 0x06E */ s16 hp;
    /* 0x070 */ s8 tempFiller5[6];
    /* 0x076 */ s8 unk76;
    /* 0x077 */ s8 tempFiller6[57];
    /* 0x0B0 */ s16 hpUpgrades;
    /* 0x0B2 */ s8 tempFiller7[100];
    /* 0x116 */ s16 unk116;
    /* 0x118 */ s8 tempFiller8[89];
    /* 0x171 */ u8 combatState;
    /* 0x172 */ s8 tempFiller9[734];
    /* 0x450 */ u16 equipedBusterParts;
    /* 0x452 */ s8 tempFiller10[257];
    /* 0x553 */ s16 energy;
    /* 0x555 */ s8 tempFiller11[1683];
} PlayerState; // size = 0xBE8;

typedef struct {
    s8 renderFlag;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unkFlag;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    s8 bufferIndex;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s16 unk74;
    s8 unk76;
    s8 unk77;
    s32 unks[212];
} NPCActor; // size = 0x3C8;

typedef struct StaticActor {
    s8 funcIndex;
    s8 arrayNum;
    s8 actorType;
    s8 unk3;
    u16 unk4;
    u8 actorState;
    s32 unk7;
    s32 unkB;
    s32 unk10;
    void* actorModelPtr;
    s8 unks[68];
} StaticActor; // size = 0x5C;

typedef struct {
    s8 renderFlag;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unkFlag;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    s8 bufferIndex;
    s32 unks [360];
} MultiPartActor; // size = 0x5AC;

typedef struct {
    s8 funcIndex;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unkFlag;
    u8 unk8;
    s32 unks1 [32];
    s32 unkPtr;
    s32 unk[35];
} ProjectileActor;

typedef struct {
    s8 funcIndex;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unkFlag;
    u8 unk8;
    s32 unks1 [3];
    s32 unkPtr;
    s32 unks2 [16];
} EffectActor; //size = 0x58;


#endif //COMMON_STRUCTS_H
