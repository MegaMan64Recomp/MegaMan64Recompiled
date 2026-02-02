#include "patches.h"
#include "gfx_patches.h"

extern s32 D_800A6CA8_820A8;
extern ArenaNode* D_800D6FAC_B23AC;
extern ArenaNode* D_800D6FA8_B23A8; //Current Arena Head
extern GfxContext D_802047F0_1DFBF0[1];

extern s32 D_GFX_TAG;

//Alloc Arena
RECOMP_PATCH void func_800283A8_37A8(s8 bufferIndex, s8 arenaIndex, void* func, void* funcData) {
    ArenaNode* arenaNode;
    ArenaNode* headNode;
    ArenaFuncNode* arenaBody;
    GfxContext* gfxContext;
    GfxContext* gfxContext2;

    gfxContext = &D_802047F0_1DFBF0[bufferIndex];

    if (gfxContext->bufferEnabled != 0) {
        arenaNode = NULL;

        // Check if Additional Scanlines < 2
        if (D_800A6CA8_820A8 < 2) {
            headNode = D_800D6FA8_B23A8;
            arenaNode = (ArenaNode*)((char*)headNode + 0x18);

            if ((u32)D_800D6FAC_B23AC < (u32)arenaNode) {
                arenaNode = NULL;
            } else {
                D_800D6FA8_B23A8 = arenaNode;
                arenaNode = headNode;
            }
        }

        if (arenaNode != NULL) {
            // Calculate gfxContext pointer: arenaIndex * 4 + temp_t1
            gfxContext2 = (GfxContext*)((char*)gfxContext + (arenaIndex << 2));

            arenaBody = (ArenaFuncNode*)&arenaNode->body;
            arenaNode->next = NULL;
            arenaNode->prev = NULL;
            arenaBody->func = func;
            arenaBody->arenaIndex = arenaIndex;
            arenaBody->arg = funcData;
            arenaBody->unk12 = 0;
            //@recomp read and embed transform id into padding
            arenaBody->unk14 = D_GFX_TAG;
            D_GFX_TAG = 0;
            // recomp_printf("Embbeding Flag %X\n", arenaBody->unk14);

            if (gfxContext2->arenaNext[0] != NULL) {
                arenaNode->prev = gfxContext2->arenaNext[0];
                gfxContext2->arenaNext[0]->next = arenaNode;
            } else {
                gfxContext2->arenaStart[0] = arenaNode;
            }
            gfxContext2->arenaNext[0] = arenaNode;
        }
    }
}

extern void func_8003AA3C_15E3C(void*, s32);
extern void func_8003AB60_15F60(void*, s32);
extern void func_8003AFB0_163B0(void*, s32);
extern void func_8003B27C_1667C(void*, s32);
extern void func_8003B298_16698(void*, s32);

typedef void (*ActorFunc)(void* ptr, s32 arg1);
ActorFunc ActorFuncs[] = {
    func_8003AA3C_15E3C,
    func_8003AB60_15F60,
    func_8003AA3C_15E3C,
    func_8003AB60_15F60,
    func_8003AFB0_163B0,
    func_8003B27C_1667C,
    func_8003B298_16698,
};

extern ActorFunc D_800AC980_87D80;
extern StaticActor D_80206B70_1E1F70 [32];

//Render Static Actor Array
RECOMP_PATCH void func_8003A9C4_15DC4(void) {
    StaticActor* actor;
    ActorFunc* funcs;
    s32 index;
    u32 iter;

    iter = 0;
    funcs = &D_800AC980_87D80;
    do {
        actor = &D_80206B70_1E1F70[iter];
        index = actor->funcIndex;
        if ((index & actor->actorState & 2) != 0) {
            //@recomp Tag embedding before calling func
            D_GFX_TAG = (s32) actor;

            funcs[(index & 0x1C) >> 2](&actor->actorModelPtr, index);

        }

        iter++;
    } while (iter < 32);
}

//---------
void func_80025D14_1114();                             /* extern */
void func_80092E10_6E210(void*);                         /* extern */
void func_80093090_6E490();                            /* extern */
void func_8009CC40_78040(void*, s32, s32);                    /* extern */
extern s16 D_800A6C9A_8209A;
extern s16 D_800A6C9C_8209C;
extern s32 D_800A6CA0_820A0;
extern s32 D_800A6CA4_820A4;
extern s32 D_800A6CA8_820A8;
extern ArenaNode* D_800D6FA0_B23A0;
extern ArenaNode* D_800D6FA4_B23A4;
extern ArenaNode* D_800D6FA8_B23A8;
extern Gfx* D_800D6FB0_B23B0;
extern Gfx* D_800D6FB4_B23B4;
extern void* D_800D6FB8_B23B8;
extern Gfx D_800D6FC0_B23C0 [15360];
extern Gfx D_800F4FC0_D03C0 [15360];
extern ArenaNode D_80112FC0_EE3C0 [2901];
extern ArenaNode D_80123FC0_FF3C0 [2901];
extern void* D_80134FC0_1103C0;
extern s32 D_8019A6A4_175AA4;
extern Gfx* D_801A90F0_1844F0;
extern void* D_801D8828_1B3C28; //unk
extern GfxContext D_802047F0_1DFBF0 [1];
extern s8 D_8020489B_1DFC9B;
extern s8 D_8020559C_1E099C;

typedef struct GfxBuffers{
    Gfx dl1 [15360];
    Gfx dl2 [15360];
} GfxBuffers;

ArenaNode extendedArena1 [2901 * 10];
ArenaNode extendedArena2 [2901 * 10];
Gfx extendedDisplayList1 [15360 * 10];
Gfx extendedDisplayList2 [15360 * 10];

//InitDisplayLists
RECOMP_PATCH void func_80027550_2950(void* arg0) {
    GfxContext* gfxContext2 ;
    GfxContext* gfxContext;
    GfxContext* var_v1;
    s32 arenaIndex;
    s32 index2;
    s32 var_s1;
    s32 index;
    s32 arenaSize;
    s32 displayListSize;

    arenaSize = 0x11000 * 10;
    displayListSize = 0x1E000 * 10;

    D_800A6C9A_8209A = 0x140; //Scissor X
    D_800A6C9C_8209C = 0xF0; //Scissor Y
    D_800A6CA4_820A4 = -1; //Unk

    D_800D6FA0_B23A0 = extendedArena1; //Assign Arena 1 Ptr [2900]
    D_800D6FAC_B23AC = extendedArena2 + arenaSize; //Assign Arena 1 Tail
    D_800D6FA8_B23A8 = extendedArena1; //Assign Arena 1 Head


    D_800D6FA4_B23A4 = extendedArena2;  //Assign Arena 2 Ptr [2900]
    D_800D6FB0_B23B0 = extendedDisplayList1;  //gDisplayList1 Ptr

    D_8019A6A4_175AA4 = 0; //Unk
    D_8020559C_1E099C = 0; //FrameBufferIndex?
    D_800A6CA0_820A0 = 0; //HiRes Enabled bool?

    D_800D6FB4_B23B4 = extendedDisplayList2; //gDisplayList2 Ptr
    D_801A90F0_1844F0 = extendedDisplayList1 + displayListSize; //Gfx head
    D_800D6FB8_B23B8 = extendedDisplayList1; //Display List End
    D_80134FC0_1103C0 = arg0; //Gfx Thread Func (func_80026A30)



    D_800A6CA8_820A8 = 0; //Additional Scanlines

    func_8009CC40_78040(&D_801D8828_1B3C28, arenaSize, displayListSize); //Pointless?

    var_s1 = 0; //GfxContext Index
    gfxContext = D_802047F0_1DFBF0; //GfxContext
    index = 0; //Arena Index
    do {
        bzero(gfxContext, sizeof(GfxContext));
        (&D_8020489B_1DFC9B)[var_s1] = index;
        gfxContext++;
        index += 1;
        var_s1 += sizeof(GfxContext); //Unused?
    } while (index <= 0);
    bzero((void* )0x803B5000, 0x4B000);
    func_80025D14_1114();
    func_80092E10_6E210(D_80134FC0_1103C0);
    func_80093090_6E490();
    index2 = 0;
    gfxContext2 = &D_802047F0_1DFBF0[0];
    do {
        arenaIndex = 0;
        var_v1 = gfxContext2;
        do {
            var_v1->arenaStart[arenaIndex] = 0;
            var_v1->arenaNext[arenaIndex] = 0;
            arenaIndex += 1;
        } while (arenaIndex < 16);
        index2 += 1;
        gfxContext2++;
    } while (index2 <= 0);
}

//--------------

extern MultiPartActor D_801BB770_196B70 [4];
extern s32 D_801BC3BC_1977BC;
extern NPCActor D_80203FF0_1DF3F0 [18];
extern u16 D_802049AC_1DFDAC;
void func_8003A2EC_156EC(void*);

//RenderActors
RECOMP_PATCH void func_8003A7CC_15BCC(void) {
    MultiPartActor* multiActorPtr;
    NPCActor* actorPtr;
    s32 multipartActorCount;
    s32 actorCount;
    void* temp_a0;

    if (D_802049AC_1DFDAC == 0) {
        multipartActorCount = 4;
        multiActorPtr = D_801BB770_196B70;

        do {
            multiActorPtr--;
            multipartActorCount--;
            if (multiActorPtr->renderFlag & multiActorPtr->unkFlag & 2) {
                //@recomp Tag multipart actors
                D_GFX_TAG = (s32) multiActorPtr;

                func_8003A2EC_156EC(multiActorPtr);
            }

        } while (multipartActorCount != 0);

        actorCount = 18;
        actorPtr = D_80203FF0_1DF3F0;
        do {
            actorPtr--;
            actorCount--;
            if (actorPtr->renderFlag & actorPtr->unkFlag & 2) {
                //@recomp Tag NPC actors
                D_GFX_TAG = (s32) actorPtr;

                func_8003A2EC_156EC(actorPtr);
            }

        } while (actorCount != 0);

        if (D_801BC3BC_1977BC != 0) {
            bzero(&D_801BC3BC_1977BC - 0x7, 0x20);
        }
    }
}


//----
s32 func_8003E934_19D34(void*);                     /* extern */
extern s32 D_801A9064_184464;
extern u16 D_802049AC_1DFDAC;
extern s16 D_801FEEC0_1DA2C0;
extern s16 D_801FEEC2_1DA2C2;
extern s16 D_801FEEC4_1DA2C4;
extern s16 D_801FEEC6_1DA2C6;
extern s16 D_801FEEC8_1DA2C8;
extern s16 D_801FEECA_1DA2CA;
extern s8 D_801FEECC_1DA2CC;
extern s8 D_801FEECD_1DA2CD;
extern s8 D_801FEECE_1DA2CE;
void func_8003A2EC_156EC(void*);

//push_actor_shadow
RECOMP_PATCH void func_8003B564_16964(NPCActor* arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v2;
    s32 var_a1;
    s32 offset2;
    s32 offset;
    s32 actorNum;
    s32 temp_s6;

    actorNum = D_801A9064_184464;
    temp_s6 = -3;
    var_a1 = arg0->unk16;
    if (actorNum < 0x40U) {
        offset = actorNum << 4;
        *((u16*)((u8*)&D_801FEEC0_1DA2C0 + offset)) = arg0->unk14;
        if (arg0->unk76 != 0) {
            var_a1 = (s16) arg0->unk16;
        } else {
            var_a1 = func_8003E934_19D34(arg0);
            temp_v2 = arg0->unk16;
            if (var_a1 < temp_v2) {
                var_a1 = temp_v2;
            }

        }

        *((u16*)((u8*)&D_801FEEC2_1DA2C2 + offset)) = var_a1;
        *((u16*)((u8*)&D_801FEEC4_1DA2C4 + offset)) = arg0->unk18;
        offset2 = actorNum << 4;
        *((u16*)((u8*)&D_801FEEC6_1DA2C6 + offset2)) = (arg0->unk56 & 0xFFF) | ((arg2 >> 6) << 0xC);
        *((u16*)((u8*)&D_801FEEC8_1DA2C8 + offset2)) = arg1 | (arg3 << 0xE);
        *((u16*)((u8*)&D_801FEECA_1DA2CA + offset2)) = var_a1 - arg0->unk16;
        *((u8*)&D_801FEECC_1DA2CC + offset) = 0;
        *((u8*)&D_801FEECD_1DA2CD + offset) = temp_s6;
        *((u8*)&D_801FEECE_1DA2CE + offset) = 0;
        //Recomp Embed actor number into shadow struct
        // *((u8*)&D_801FEECF_1DA2CF + offset) = actorNum;

        D_801A9064_184464 += 1;
    }
}

