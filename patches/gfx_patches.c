#include "patches.h"
#include "common_structs.h"
#include "tag_helper.h"
#include "gfx_patches.h"

bool skip_all_interpolation = FALSE;

void recomp_check_camera_jump();

void yield_self_1ms(void);
extern volatile u32 D_801FFBB0_1DAFB0;

//nuGfxTaskAllEndWait
RECOMP_PATCH void func_80092EB0_6E2B0() {
    while (D_801FFBB0_1DAFB0) {
        yield_self_1ms();
    }
}

extern s32 D_800A6CA8_820A8;
extern GfxTaskNode *D_800D6FAC_B23AC;
extern GfxTaskNode *D_800D6FA8_B23A8; //gTaskPoolHead
extern GfxContext D_802047F0_1DFBF0;
//gfx_add_render_task
RECOMP_PATCH void func_800283A8_37A8(s8 bufferIndex, s8 poolIndex, void *func, void *funcData) {
    GfxTaskNode *taskNode;
    GfxTaskNode *headNode;
    GfxContext *gfxContext;
    GfxContext *gfxContext2;

    gfxContext = &D_802047F0_1DFBF0;

    if (gfxContext->bufferEnabled != 0) {
        taskNode = NULL;

        if (D_800A6CA8_820A8 < 2) {
            headNode = D_800D6FA8_B23A8;
            taskNode = (GfxTaskNode *) ((char *) headNode + 0x18);

            if ((u32) D_800D6FAC_B23AC < (u32) taskNode) {
                taskNode = NULL;
            } else {
                D_800D6FA8_B23A8 = taskNode;
                taskNode = headNode;
            }
        }

        if (taskNode != NULL) {
            gfxContext2 = (GfxContext *) ((char *) gfxContext + (poolIndex << 2));

            taskNode->next = NULL;
            taskNode->prev = NULL;
            taskNode->func = func;
            taskNode->poolIndex = poolIndex;
            taskNode->arg = funcData;
            taskNode->unk12 = 0;

            //@recomp read and embed transform id into padding
            if (funcData != 0) {
                taskNode->unk14 = gCurrGfxTag;
                gCurrGfxTag = 0;
            }

            if (gfxContext2->taskPoolEnd[0] != NULL) {
                taskNode->prev = gfxContext2->taskPoolEnd[0];
                gfxContext2->taskPoolEnd[0]->next = taskNode;
            } else {
                gfxContext2->taskPoolStart[0] = taskNode;
            }
            gfxContext2->taskPoolEnd[0] = taskNode;
        }
    }
}

void func_80025D14_1114(); /* extern */
void func_80092E10_6E210(void *); /* extern */
void func_80093090_6E490(); /* extern */
void func_8009CC40_78040(void *); /* extern */
extern s16 D_800A6C9A_8209A;
extern s16 D_800A6C9C_8209C;
extern s32 D_800A6CA0_820A0;
extern s32 D_800A6CA4_820A4;
extern s32 D_800A6CA8_820A8;
extern GfxTaskNode *D_800D6FA0_B23A0;
extern GfxTaskNode *D_800D6FA4_B23A4;
extern GfxTaskNode *D_800D6FA8_B23A8;
extern GfxBuffers D_800D6FB0_B23B0;
extern Gfx *D_800D6FB4_B23B4;
extern void *D_800D6FB8_B23B8;
extern Gfx D_800D6FC0_B23C0[15360];
extern Gfx D_800F4FC0_D03C0[15360];
extern TaskNode D_80112FC0_EE3C0[2901];
extern TaskNode D_80123FC0_FF3C0[2901];
extern void *D_80134FC0_1103C0;
extern GfxContext *D_8019A6A4_175AA4;
extern Gfx *D_801A90F0_1844F0;
extern void *D_801D8828_1B3C28; //unk
extern GfxContext D_802047F0_1DFBF0;
extern s8 D_8020489B_1DFC9B;
extern s8 D_8020559C_1E099C;


GfxTaskNode extendedArena1[2901 * 16];
GfxTaskNode extendedArena2[2901 * 16];
Gfx extendedDisplayList1[15360 * 16];
Gfx extendedDisplayList2[15360 * 16];

//init_display_lists
RECOMP_PATCH void func_80027550_2950(void *arg0) {
    GfxContext *gfxContext2;
    GfxContext *gfxContext;
    GfxContext *var_v1;
    s32 poolIndex;
    s32 index2;
    s32 var_s1;
    s32 index;
    s32 poolSize;
    s32 displayListSize;

    poolSize = 0x11000 * 16;
    displayListSize = 0x1E000 * 16;

    D_800A6C9A_8209A = 0x140; //Scissor X
    D_800A6C9C_8209C = 0xF0; //Scissor Y
    D_800A6CA4_820A4 = -1;

    D_800D6FA0_B23A0 = extendedArena1; //Assign Arena 1 Ptr [2900]
    D_800D6FAC_B23AC = extendedArena2 + poolSize; //Assign Arena 1 Tail
    D_800D6FA8_B23A8 = extendedArena1; //Assign Arena 1 Head


    D_800D6FA4_B23A4 = extendedArena2; //Assign Arena 2 Ptr [2900]
    D_800D6FB0_B23B0.dls[0] = extendedDisplayList1; //gDisplayList1 Ptr

    D_8019A6A4_175AA4 = 0;
    D_8020559C_1E099C = 0;
    D_800A6CA0_820A0 = 0;

    D_800D6FB0_B23B0.dls[1] = extendedDisplayList2; //gDisplayList2 Ptr
    D_801A90F0_1844F0 = extendedDisplayList1 + displayListSize; //Gfx head
    D_800D6FB8_B23B8 = extendedDisplayList1; //Display List End
    D_80134FC0_1103C0 = arg0; //Gfx Thread Func (func_80026A30)

    D_800A6CA8_820A8 = 0; //Additional Scanlines

    func_8009CC40_78040(&D_801D8828_1B3C28);

    var_s1 = 0;
    gfxContext = &D_802047F0_1DFBF0;
    index = 0;
    do {
        bzero(gfxContext, sizeof(GfxContext));
        (&D_8020489B_1DFC9B)[var_s1] = index;
        gfxContext++;
        index += 1;
        var_s1 += sizeof(GfxContext);
    } while (index <= 0);
    bzero((void *) 0x803B5000, 0x4B000);
    func_80025D14_1114();
    func_80092E10_6E210(D_80134FC0_1103C0);
    func_80093090_6E490();
    index2 = 0;
    gfxContext2 = &D_802047F0_1DFBF0;
    do {
        poolIndex = 0;
        var_v1 = gfxContext2;
        do {
            var_v1->taskPoolStart[poolIndex] = 0;
            var_v1->taskPoolEnd[poolIndex] = 0;
            poolIndex += 1;
        } while (poolIndex < 16);
        index2 += 1;
        gfxContext2++;
    } while (index2 <= 0);
}

//Main Draw
RECOMP_PATCH void func_800276EC_2AEC(s32 arg0) {
    s32 sp14;
    s32 uly;
    s32 ulx;
    s32 lrx;
    s32 lry;
    GfxTaskNode *temp_v0_5;
    Gfx *gfx;
    GfxContext *gfxContext;
    GfxContext *gfxContext2;
    Vp *viewport;
    Mtx *identMtx;
    Mtx *mtxprojectionMtx;
    s32 width;
    u32 zbuffer;
    s32 unkMtx;
    s32 var_a0;
    s32 var_a2;
    s32 var_a3;
    s32 poolNum;
    s32 gfxContextIndex;
    u32 var_a2_2;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 temp_v0;
    s32 colorDither;
    GfxTaskNode *gfxTaskNode;


    sp14 = -1;
    D_801A53D8_1807D8 = 0;
    var_a2 = 0;
    if (arg0 < 2) {
        D_801BC17C_19757C += 1;
        gfxContextIndex = 0;
        func_8002CAD0_7ED0();
        gfxContext = &D_802047F0_1DFBF0;
        gfx = D_800D6FB0_B23B0.dls[D_8020559C_1E099C];
        D_800D6FB8_B23B8 = gfx + 15360 * 10;
        D_801A90F0_1844F0 = gfx;
        D_80193C30_16F030 = -1;
        D_801A90F0_1844F0 = gfx;
        D_801A90F0_1844F0 = gfx + 1;
        gSPDisplayList(gfx++, &D_80224E80);

        //@recomp Enable RT64 enhancements
        gEXEnable(D_801A90F0_1844F0++);
        gEXSetRDRAMExtended(D_801A90F0_1844F0++, TRUE);
        gEXSetRefreshRate(D_801A90F0_1844F0++, 30);
        gEXForceTrueBilerp(D_801A90F0_1844F0++, G_EX_BILERP_ONLY);
        // recomp_printf("-----Begin Frame----- \n");
        //@recomp check if the camera has jumped this frame
        recomp_check_camera_jump();
        do {
            temp_v0 = gfxContext->fadeoutTimer;
            if (temp_v0 != 0) {
                temp_a0_2 = gfxContext->rgba.c.a;
                temp_a0_3 = temp_a0_2 + (((u8) gfxContext->primAlpha2 - temp_a0_2) / temp_v0);
                gfxContext->rgba.c.a = temp_a0_3;
                gfxContext->fadeoutTimer--;
                if (!(gfxContext->fadeoutTimer & 0xFF) && (gfxContext->rgba.c.a >= 0xF1) && !(
                        gfxContext->rgba.rgba32 & ~0xFF)) {
                    func_80089EA0_652A0();
                }
            }
            gfxContext++;
            gfxContextIndex++;
        } while (gfxContextIndex <= 0);
        gfxContextIndex = 0;
        do {
            gfxContext = &D_802047F0_1DFBF0;
            D_8019A6A4_175AA4 = gfxContext;
            if (gfxContext->bufferEnabled != 0) {
                uly = gfxContext->uly;
                ulx = gfxContext->ulx;
                lrx = gfxContext->lrx + 1;
                lry = gfxContext->lry + 1;
                if (gfxContextIndex == 0) {
                    uly = 0;
                    ulx = 0;
                    lrx = D_800A6C9A_8209A;
                    lry = D_800A6C9C_8209C;
                }
                gDPSetScissor(D_801A90F0_1844F0++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
                if ((gfxContext->hiResEnabled != 0) && (D_800A6CA0_820A0 == 0)) {
                    zbuffer = D_801ADC84_189084;
                    width = D_800A6C9A_8209A - 1;

                    gDPSetDepthImage(D_801A90F0_1844F0++, OS_K0_TO_PHYSICAL(zbuffer));
                    gDPSetCycleType(D_801A90F0_1844F0++, G_CYC_FILL);
                    gDPSetColorImage(D_801A90F0_1844F0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, width + 1,
                                     OS_K0_TO_PHYSICAL(zbuffer));
                    gDPSetFillColor(D_801A90F0_1844F0++, 0xFFFCFFFC);
                    gDPFillRectangle(D_801A90F0_1844F0++, 0, 0, width, D_800A6C9C_8209C - 1);
                    gDPPipeSync(D_801A90F0_1844F0++);
                }
                if (gfxContext->unk_A8 != 0) {
                    gDPSetColorImage(D_801A90F0_1844F0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800A6C9A_8209A,
                                     osVirtualToPhysical(D_80210980_1EBD80));
                    gDPSetFillColor(D_801A90F0_1844F0++,
                                    FILL_COLOR(gfxContext->bgPrimRed, gfxContext->bgPrimGreen, gfxContext->bgPrimBlue));
                    gDPFillRectangle(D_801A90F0_1844F0++, 0, 0, D_800A6C9A_8209A - 1, D_800A6C9C_8209C - 1);
                    gDPPipeSync(D_801A90F0_1844F0++);
                }
                viewport = func_800281C8_35C8(0x10);
                mtxprojectionMtx = func_800281C8_35C8(0x40);
                identMtx = func_800281C8_35C8(0x40);
                if (((viewport != NULL) & (mtxprojectionMtx != NULL)) && (identMtx != NULL)) {
                    memcpy(viewport, &gfxContext->viewport, 0x10);
                    memcpy(mtxprojectionMtx, &gfxContext->projectionMtx, 0x40);
                    memcpy(identMtx, &gfxContext->identMtx, 0x40);
                    memcpy(&D_801AF370_18A770, &gfxContext->projectionMtx, 0x40);

                    gSPViewport(D_801A90F0_1844F0++, viewport);
                    gSPPerspNormalize(D_801A90F0_1844F0++, gfxContext->perspNorm);
                    gSPMatrix(D_801A90F0_1844F0++, mtxprojectionMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                    gSPMatrix(D_801A90F0_1844F0++, identMtx, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);

                    gSPFogPosition(D_801A90F0_1844F0++, gfxContext->fogStart, gfxContext->fogEnd);
                    gDPSetFogColor(D_801A90F0_1844F0++, gfxContext->fogRed, gfxContext->fogGreen, gfxContext->fogBlue,
                                   gfxContext->fogAlpha);

                    poolNum = 0;
                    var_a3 = 4;
                    var_a2_2 = 0x1D800 * 16; //@recomp check using increased DL size
                    do {
                        gfxTaskNode = gfxContext->taskPoolStart[poolNum];
                        unkMtx = *(s32 *) ((u8 *) &gfxContext->taskPoolStart[poolNum] + 0x144);
                        if (unkMtx != D_801A53D8_1807D8) {
                            if (gfxTaskNode != NULL) {
                                D_801A53D8_1807D8 = unkMtx;
                                switch (unkMtx) {
                                    case 0:
                                        gSPLoadUcode(D_801A90F0_1844F0++, 0xA4770, 0xD1450);
                                        break;
                                    case 1:
                                        gSPLoadUcode(D_801A90F0_1844F0++, 0xA5B00, 0xD1870);
                                        break;
                                }
                                gSPMatrix(D_801A90F0_1844F0++, mtxprojectionMtx,
                                          G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                                gSPMatrix(D_801A90F0_1844F0++, identMtx, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
                            }
                        }

                        if (gfxTaskNode != NULL) {
                            do {
                                colorDither = G_CD_BAYER;
                                D_800D6FBC_B23BC =
                                        (u32) D_801A90F0_1844F0 - (u32) D_800D6FB0_B23B0.dls[D_8020559C_1E099C];
                                if (var_a2_2 >= D_800D6FBC_B23BC) {
                                    if ((sp14 == var_a3) && (D_80193C30_16F030 != sp14)) {
                                        gDPSetColorDither(D_801A90F0_1844F0++, colorDither);
                                    }
                                    sp14 = D_80193C30_16F030;
                                    //@recomp Basic Tagging, read ID from the padding
                                    if (gfxTaskNode->unk14 != 0) {
                                        recomp_interp(gfxTaskNode->unk14);
                                        gfxTaskNode->func(gfxTaskNode->arg);
                                        gEXPopMatrixGroup(D_801A90F0_1844F0++, G_MTX_MODELVIEW);
                                    } else {
                                        gfxTaskNode->func(gfxTaskNode->arg);
                                    }
                                    gfxTaskNode = gfxTaskNode->next;
                                }
                            } while (gfxTaskNode != NULL);
                        }

                        poolNum += 1;
                    } while (poolNum < 16);
                    if (gfxContext->rgba.c.a != 0) {
                        gSPDisplayList(D_801A90F0_1844F0++, &D_80224F68);
                        gDPSetPrimColor(D_801A90F0_1844F0++, 0, 0, gfxContext->rgba.c.r, gfxContext->rgba.c.g,
                                        gfxContext->rgba.c.b, gfxContext->rgba.c.a);
                        gSPTextureRectangle(D_801A90F0_1844F0++, ulx * 4, uly * 4, lrx * 4, lry * 4, 0, 0, 0, 1 << 10,
                                            1 << 10);
                        gDPPipeSync(D_801A90F0_1844F0++);
                        gDPSetAlphaDither(D_801A90F0_1844F0++, G_CD_BAYER);
                    }
                }
            }

            gfxContextIndex += 1;
        } while (gfxContextIndex <= 0);
        D_8019A6A4_175AA4 = 0;
        gDPFullSync(D_801A90F0_1844F0++);
        gSPEndDisplayList(D_801A90F0_1844F0++);
        D_800D6FBC_B23BC = (u32) D_801A90F0_1844F0 - (u32) D_800D6FB0_B23B0.dls[(u8) D_8020559C_1E099C];
        func_80092C6C_6E06C(D_800D6FB0_B23B0.dls[(u8) D_8020559C_1E099C], D_800D6FBC_B23BC, 0, 0x40001U);
        D_8020559C_1E099C = 1 - (u8) D_8020559C_1E099C;
    }

    //Reset Arena Pointers
    var_a2 = 0;
    gfxContext2 = &D_802047F0_1DFBF0;
    do {
        var_a0 = 0;
        do {
            gfxContext2->taskPoolStart[var_a0] = NULL;
            gfxContext2->taskPoolEnd[var_a0] = NULL;
            var_a0++;
        } while (var_a0 < 16);
        gfxContext2++;
        var_a2++;
    } while (var_a2 <= 0);

    temp_v0_5 = (&D_800D6FA0_B23A0)[D_8020559C_1E099C];
    D_800D6FA8_B23A8 = temp_v0_5;
    D_800D6FAC_B23AC = (void *) temp_v0_5 + 0x11000 * 16;

    //@recomp reset the interpolation flag after rendering frame
    set_all_interpolation_skipped(FALSE);
    // recomp_printf("-----End Frame----- \n");
}

void recomp_interp(u32 id) {
    if (all_interpolation_skipped()) {
        gEXMatrixGroupSkipAll(D_801A90F0_1844F0++, G_EX_ID_IGNORE, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else if (id != 0){
        gEXMatrixGroupDecomposedNormal(D_801A90F0_1844F0++, id, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        gEXMatrixGroupSkipAll(D_801A90F0_1844F0++, G_EX_ID_IGNORE, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    }
}

extern s32 D_801BC654_197A54; //Current Cutscene Pointer
s32 sPreviousCutscene = NULL;
void recomp_check_camera_jump() {
    if (D_801BC654_197A54 != sPreviousCutscene) {
        set_all_interpolation_skipped(TRUE);
    }
    sPreviousCutscene = D_801BC654_197A54;
}

void set_all_interpolation_skipped(bool skipped) {
    skip_all_interpolation = skipped;
}

bool all_interpolation_skipped() {
    return skip_all_interpolation;
}

void rect_aspect_stretch() {
    gEXSetRectAspect(D_801A90F0_1844F0++, G_EX_ASPECT_STRETCH);
}

void rect_aspect_auto() {
    gEXSetRectAspect(D_801A90F0_1844F0++, G_EX_ASPECT_AUTO);
}
