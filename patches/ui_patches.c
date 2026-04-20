#include "patches.h"
#include "graphics.h"
#include "ui_patches.h"
#include "gfx_patches.h"
#include "common_structs.h"

static U32ValueHashmapHandle recompUIMap;
static _Bool recompUIMapInitialized = 0;
static u32 gCurrentUIElement;

extern void recomp_set_ui_rect_alignment(u32);
extern u32 recomp_get_ui_alignment(void *);
extern void recomp_erase_ui_key(void *);
extern void func_8002FD30_B130(s32, void *);

typedef enum {
    OTHER,
    HEALTH_BAR,
    ENERGY_BAR,
    BOSS_HEALTH_BAR,
    ROLL_HEALTH_BAR,
    MINIMAP,
} UIAlignTypes;

extern void *D_8017B600_156A00;
extern void *D_8019A7A8_175BA8;
extern s32 D_8020497C_1DFD7C;
extern Gfx *D_801A90F0_1844F0;
//Boss Health Bar
RECOMP_PATCH void func_80049DC4_251C4(uiElemData *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    UiElementGeneric *elem;
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_v1;
    s16 temp_v1_2;
    s8 temp_a3_2;
    s8 temp_t0;
    u32 temp_t7;
    u8 temp_a3;
    u8 temp_t1;

    temp_t7 = arg3 & 0xFFFF;
    if (temp_t7 != 0) {
        temp_t1 = arg0->texture.s;
        elem = D_8019A7A8_175BA8;
        temp_a3 = arg0->texture.t;
        D_8019A7A8_175BA8 = elem + 0x28;
        elem->ui_Header_Bitfield = 0x09000000;
        elem->rgba.c.a = 0x2C;
        elem->rgba.c.g = (s8)(arg4 >> 8);
        elem->rgba.c.r = (s8) arg4;
        elem->rgba.c.b = (s8)(arg4 >> 0x10);
        temp_a0 = arg1 + arg0->llxOffset;
        elem->texture = arg0->texture;
        temp_a2 = temp_a0 + (arg3 & 0xFF);
        temp_t0 = temp_t1 + arg0->xySpriteSize.xSpriteSize;
        elem->textureYUnk = (s8) temp_a3;
        temp_a3_2 = temp_a3 + arg0->xySpriteSize.ySpriteSize;
        elem->ulxOffset = temp_a0;
        elem->llxOffset = temp_a0;
        elem->textureXUnk = temp_t0;
        elem->textureX = (s8) temp_t1;
        elem->textureY = temp_a3_2;
        elem->width = temp_t0;
        elem->height = temp_a3_2;
        temp_v1 = arg2 + arg0->llyOffset;
        elem->ulyOffset = temp_v1;
        elem->uryOffset = temp_v1;
        temp_v1_2 = temp_v1 + (temp_t7 >> 8);
        elem->urxOffset = temp_a2;
        elem->llyOffset = temp_v1_2;
        elem->lrxOffset = temp_a2;
        elem->lryOffset = temp_v1_2;
        elem->texturePallete = arg0->textureBank;

        //@recomp
        if (recompUIMapInitialized && gCurrentUIElement != 0) {
            collection_key_t key = (collection_key_t) elem;
            recomputil_u32_value_hashmap_insert(recompUIMap, key, gCurrentUIElement);
        }

        func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, elem);
    }
}

// insertUIStackGeneral
RECOMP_PATCH void func_80049CCC_250CC(uiElemData *arg0_elemData, s32 arg1_xOffset, s32 arg2_yOffset,
                                      s32 arg3_spriteSize, s16 alignmentFlag) {
    s16 xOffsetFinal;
    s16 spriteSizeX;
    s16 spriteSizeY;
    s16 yOffset;
    s16 yOffsetFinal;
    s16 xOffset;
    s16 extraXOffset;
    u16 textureBank;
    u32 temp_v0;
    u32 extraYOffset;
    u8 spriteXPos;
    u8 spriteWidth;
    u8 spriteYPos;
    u8 spriteHeight;
    UiElementGeneric *element_generic;

    element_generic = D_8019A7A8_175BA8;
    D_8019A7A8_175BA8 = element_generic + sizeof(element_generic);

    spriteSizeX = arg0_elemData->xySpriteSize.xSpriteSize;
    spriteSizeY = arg0_elemData->xySpriteSize.ySpriteSize;
    temp_v0 = arg3_spriteSize & 0xFFFF;

    if (temp_v0 != 0) {
        extraXOffset = arg3_spriteSize & 0xFF;
        extraYOffset = temp_v0 >> 8;
    } else {
        extraXOffset = spriteSizeX;
        extraYOffset = (u32) spriteSizeY;
    }

    element_generic->ui_Header_Bitfield = 0x09000000;
    element_generic->rgba = arg0_elemData->rgba;
    element_generic->texture = arg0_elemData->texture;
    textureBank = arg0_elemData->textureBank;
    spriteXPos = (u8) arg0_elemData->texture.s;
    spriteYPos = (u8) arg0_elemData->texture.t;
    spriteWidth = spriteXPos + spriteSizeX;
    spriteHeight = spriteYPos + spriteSizeY;
    element_generic->textureYUnk = spriteYPos;
    element_generic->textureXUnk = spriteWidth;
    element_generic->textureX = spriteXPos;
    element_generic->textureY = spriteHeight;
    element_generic->width = spriteWidth;
    element_generic->height = spriteHeight;
    xOffset = arg1_xOffset + arg0_elemData->llxOffset;
    yOffset = arg2_yOffset + arg0_elemData->llyOffset;
    xOffsetFinal = xOffset + extraXOffset;
    yOffsetFinal = yOffset + extraYOffset;
    element_generic->urxOffset = xOffsetFinal;
    element_generic->lrxOffset = xOffsetFinal;
    element_generic->ulxOffset = xOffset;
    element_generic->llxOffset = xOffset;
    element_generic->ulyOffset = yOffset;
    element_generic->uryOffset = yOffset;
    element_generic->llyOffset = yOffsetFinal;
    element_generic->lryOffset = yOffsetFinal;
    element_generic->texturePallete = textureBank;
    func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, element_generic);

    //@recomp Pass along the alignment for this UI element
    if (recompUIMapInitialized && gCurrentUIElement != 0) {
        collection_key_t key = (collection_key_t) element_generic;
        recomputil_u32_value_hashmap_insert(recompUIMap, key, gCurrentUIElement);
        // recomp_printf("Passing along Alignment %X for elem %X \n", gCurrentUIElement, key);
    }

    D_8017B600_156A00 = element_generic;
}

//draw_ui_texture
extern void func_8002CD50_8150(UiElementGeneric *);
extern void func_8002D08C_848C(UiElementGeneric *, s32, s32, s32, s32);
extern void func_8002D370_8770(UiElementGeneric *, s32, u32, s32, u16, s32);
RECOMP_PATCH void func_8002E16C_956C(UiElementGeneric *elemData) {
    s16 sVar1;
    s32 varTemp;
    u16 uVar2;
    u32 uVar3;
    u32 yPos;
    u32 xPos;
    UiElementGeneric uiElement;

    //@recomp push scissor for widescreen
    gEXPushScissor(D_801A90F0_1844F0++);
    gEXSetScissor(D_801A90F0_1844F0++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, SCREEN_HEIGHT);

    uiElement = *elemData;
    if ((uiElement.ulxOffset < 0) && (uiElement.urxOffset < 0)) {
        varTemp = -1;
    } else {
        varTemp = 0;
        if (uiElement.ulyOffset < 0) {
            varTemp = (uiElement.llyOffset << 0x10) >> 0x1f;
        }
    }
    if (varTemp == 0) {
        varTemp = uiElement.width - uiElement.texture.s;
        if (varTemp < 0) {
            varTemp = -varTemp;
        }
        sVar1 = (s16) varTemp;
        xPos = sVar1;
        varTemp = uiElement.height - uiElement.texture.t;
        if (varTemp < 0) {
            varTemp = -varTemp;
        }
        uVar2 = (u16) varTemp;
        if (xPos != uiElement.urxOffset - uiElement.ulxOffset) {
            sVar1 = uiElement.urxOffset - uiElement.ulxOffset;
        }
        yPos = (u32)(s16)
        uVar2;
        if (yPos != uiElement.llyOffset - uiElement.ulyOffset) {
            uVar2 = uiElement.llyOffset - uiElement.ulyOffset;
        }

        if (xPos != 0 && yPos != 0) {
            if (sVar1 != 0 && uVar2 != 0) {\
                //@recomp Receive the alignment for this UI element
                u32 alignment = recomp_get_ui_alignment(elemData);
                recomp_set_ui_rect_alignment(alignment);

                uVar3 = (uiElement.texturePallete >> 7) & 3;
                varTemp = uVar3;
                func_8002CD50_8150(&uiElement);
                func_8002D08C_848C(&uiElement, varTemp * 0xf0 + 0x10, uVar3, (uiElement.texturePallete >> 5) & 3, 0);
                func_8002D370_8770(&uiElement, xPos, yPos, sVar1, uVar2, varTemp);

                //@recomp reset alignment
                gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
            }
        }
    }
    //@recomp remove ui key and pop scissor
    recomp_erase_ui_key(elemData);
    gEXPopScissor(D_801A90F0_1844F0++);
    return;
}

RECOMP_EXPORT void recomp_set_ui_rect_alignment(u32 alignmentFlag) {
    switch (alignmentFlag) {
        case HEALTH_BAR:
        case MINIMAP:
        case ROLL_HEALTH_BAR:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
            break;
        case ENERGY_BAR:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                            -(SCREEN_WIDTH) * 4, 0);
            break;
        case BOSS_HEALTH_BAR:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                            -(SCREEN_WIDTH) * 4, 0);
            break;
        case OTHER:
        default:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
            break;
    }

    return;
}


void recomp_insert_ui_elem_alignment() {
    if (recompUIMapInitialized) {
        collection_key_t key = (u32) D_8019A7A8_175BA8;
        recomputil_u32_value_hashmap_insert(recompUIMap, key, MINIMAP);
        // recomp_printf("Tagged Minimap Elem=%X\n", key);
    }
}

u32 recomp_get_ui_alignment(void *ptr) {
    u32 value = 0;
    collection_key_t key;
    if (recompUIMapInitialized) {
        key = (collection_key_t) ptr;
        recomputil_u32_value_hashmap_get(recompUIMap, key, &value);
    }
    return value;
}

void recomp_erase_ui_key(void *ptr) {
    collection_key_t key;
    if (recompUIMapInitialized) {
        key = (collection_key_t) ptr;
        recomputil_u32_value_hashmap_erase(recompUIMap, key);
    }
}

void recomp_init_ui_map() {
    if (!recompUIMapInitialized) {
        recompUIMap = recomputil_create_u32_value_hashmap();
        recompUIMapInitialized = 1;
    }
}


extern s32 D_80206B30_1E1F30;
extern s16 D_801BC67C_197A7C;
extern s16 D_801BC67E_197A7E;
extern u8 D_801BC5B0_1979B0;
extern s32 D_800AD0CC_884CC;
extern s32 D_800AD0D0_884D0;
extern s32 D_800AD0D4_884D4;
extern s32 D_800AD0D8_884D8;
extern s32 D_800AD0DC_884DC;
extern s32 D_800AD0E0_884E0;
extern s8 D_801BC66E_197A6E;
extern s8 D_801BC692_197A92;
extern s8 D_801BC6B6_197AB6;
extern s8 D_801BC6DA_197ADA;
extern s8 D_801BC6FE_197AFE;
extern s8 D_801BC722_197B22;
extern s16 D_801BC674_197A74;
extern s16 D_801BC676_197A76;
extern s16 D_801BC698_197A98;
extern s16 D_801BC69A_197A9A;
extern s16 D_801BC6BC_197ABC;
extern s16 D_801BC6BE_197ABE;
extern s16 D_801BC6E0_197AE0;
extern s16 D_801BC6E2_197AE2;
extern s16 D_801BC704_197B04;
extern s16 D_801BC706_197B06;
extern s16 D_801BC728_197B28;
extern s16 D_801BC72A_197B2A;
extern s8 D_80204B21_1DFF21;

extern HealthBarData D_801BC660_197A60;
extern HealthBarData D_801BC684_197A84;
extern BossHealthBarData D_801BC6A8_197AA8[3];
extern HealthBarData D_801BC714_197B14;

extern void func_80049204_24604(void *);
extern void func_800490F4_244F4(void *);
extern void func_80049330_24730(void *);
extern void func_800492F8_246F8(void *);
extern void func_800496C0_24AC0(void *);
extern void func_80049948_24D48(void *);
extern void func_80049B38_24F38(void *);

//render_ui_bars
RECOMP_PATCH void func_80048C7C_2407C(void) {
    s32 temp;
    s32 temp2;
    s32 iter;
    HealthBarData *ptr;
    BossHealthBarData *bossPtr;
    BossHealthBarData *bossPtr2;
    s32 bossIndex;

    func_80049204_24604(&D_801BC684_197A84);

    if (D_801BC67C_197A7C > 0) {
        D_801BC67C_197A7C--;
    }

    if (D_801BC67E_197A7E > 0) {
        D_801BC67E_197A7E--;
    }

    D_80206B30_1E1F30 = 0;

    if (D_800AD0CC_884CC != D_801BC5B0_1979B0) {
        if (D_800AD0CC_884CC != 0) {
            D_800AD0D0_884D0 = 3;
        } else {
            D_800AD0D0_884D0 = 0;
        }
    }

    D_800AD0CC_884CC = D_801BC5B0_1979B0;

    switch (D_800AD0D0_884D0) {
        default:
            D_800AD0D0_884D0 = 1;
            D_800AD0D4_884D4 = 0xA;
            D_800AD0E0_884E0 = 1;
            D_801BC66E_197A6E = 1;
            D_801BC692_197A92 = 1;
            D_801BC6B6_197AB6 = 1;
            D_801BC6DA_197ADA = 1;
            D_801BC6FE_197AFE = 1;
            D_801BC722_197B22 = 1;
            D_801BC676_197A76 = 0x88;
            D_801BC698_197A98 = 0x110;
            D_801BC69A_197A9A = 0xBE;
            D_801BC6BC_197ABC = 0x120;
            D_801BC6E0_197AE0 = 0x120;
            D_801BC704_197B04 = 0x120;
            D_801BC72A_197B2A = 0x18;
            D_800AD0D8_884D8 = 8;
            D_800AD0DC_884DC = 0x60;
            D_801BC674_197A74 = 0x10;
            D_801BC6BE_197ABE = 0x10;
            D_801BC6E2_197AE2 = 0x10;
            D_801BC706_197B06 = 0x10;
            D_801BC728_197B28 = 0;
            break;
        case 1:
            temp = D_800AD0D4_884D4--;
            if (temp == 0) {
                D_800AD0D0_884D0 = 2;
                break;
            }
            temp2 = (0xB - temp) << 3;
            D_801BC674_197A74 = 0x10 - temp2;
            D_801BC698_197A98 = temp2 + 0x110;
            D_801BC6BC_197ABC = temp2 + 0x120;
            D_801BC6E0_197AE0 = temp2 + 0x120;
            D_801BC704_197B04 = temp2 + 0x120;
            D_801BC728_197B28 = -temp2;
            D_800AD0D8_884D8 = 8 - temp2;
            break;
        case 2:
            break;

        case 3:
            D_800AD0D4_884D4 = 0xA;
            D_800AD0D0_884D0 = 4;
            D_801BC66E_197A6E = 1;
            D_801BC692_197A92 = 1;
            D_801BC6B6_197AB6 = 1;
            D_801BC6DA_197ADA = 1;
            D_801BC6FE_197AFE = 1;
            D_801BC722_197B22 = 1;
            D_801BC674_197A74 = -0x40;
            D_801BC676_197A76 = 0x88;
            D_801BC698_197A98 = 0x160;
            D_801BC69A_197A9A = 0xBE;
            D_801BC6BC_197ABC = 0x170;
            D_801BC6E0_197AE0 = 0x170;
            D_801BC704_197B04 = 0x170;
            D_801BC6BE_197ABE = 0x10;
            D_801BC6E2_197AE2 = 0x10;
            D_801BC706_197B06 = 0x10;
            D_801BC728_197B28 = -0x50;
            D_801BC72A_197B2A = 0x18;
            D_800AD0D8_884D8 = -0x48;
            break;
        case 4:
            temp = D_800AD0D4_884D4--;
            if (temp == 0) {
                D_800AD0D0_884D0 = 5;
                break;
            }
            temp2 = (temp - 1) << 3;
            D_801BC674_197A74 = 0x10 - temp2;
            D_801BC698_197A98 = temp2 + 0x110;
            D_801BC6BC_197ABC = temp2 + 0x120;
            D_801BC6E0_197AE0 = temp2 + 0x120;
            D_801BC704_197B04 = temp2 + 0x120;
            D_801BC728_197B28 = -temp2;
            D_800AD0D8_884D8 = 8 - temp2;
            break;
        case 5:
            D_801BC66E_197A6E = 0;
            D_801BC692_197A92 = 0;
            D_801BC6B6_197AB6 = 0;
            D_801BC6DA_197ADA = 0;
            D_801BC6FE_197AFE = 0;
            D_801BC722_197B22 = 0;
            D_800AD0E0_884E0 = 0;
            break;
    }

    //@recomp init UI hashmap
    recomp_init_ui_map();

    if (D_801BC660_197A60.status & 1) {
        func_800490F4_244F4(&D_801BC660_197A60);
    }

    //Healthbar
    if (D_801BC660_197A60.status & 2) {
        gCurrentUIElement = HEALTH_BAR;
        func_80049330_24730(&D_801BC660_197A60);
        gCurrentUIElement = 0;
    }

    //Draw Energy Bar
    if (D_80204B21_1DFF21 != 0) {
        if (D_801BC660_197A60.energyBarStatus & 2) {
            gCurrentUIElement = ENERGY_BAR;
            func_800496C0_24AC0(&D_801BC684_197A84);
            gCurrentUIElement = 0;
        }
    }

    //Boss Healthbar
    iter = 0;
    bossPtr = D_801BC6A8_197AA8;
    while (iter < 3) {
        if (((&D_801BC6A8_197AA8[0].status)[iter] & 1) != 0) {
            gCurrentUIElement = BOSS_HEALTH_BAR;
            func_800492F8_246F8(bossPtr);
            gCurrentUIElement = 0;
        }
        if (D_801BC6A8_197AA8[iter].status & 2) {
            gCurrentUIElement = BOSS_HEALTH_BAR;
            func_80049948_24D48(bossPtr);
            gCurrentUIElement = 0;
        }
        bossPtr += 0x1;
        iter++;
    }

    if (D_801BC714_197B14.status & 2) {
        gCurrentUIElement = ROLL_HEALTH_BAR;
        func_80049B38_24F38(&D_801BC714_197B14);
        gCurrentUIElement = 0;
    }
}

extern s8 D_801BC3E8_1977E8;
void func_8002ED90_A190(UIElemQuad *);
//draw_colored_vert_rect
RECOMP_PATCH void func_8002FA3C_AE3C(UIElemMonoQuad *arg0) {
    UIElemQuad uiQuad;
    UIElemQuad *uiQuadPtr = &uiQuad;

    memcpy(uiQuadPtr, arg0, 8);

    uiQuad.verts[0].rgba.c.r = arg0->rgba32.c.r;
    uiQuad.verts[0].rgba.c.g = arg0->rgba32.c.g;
    uiQuad.verts[0].rgba.c.b = arg0->rgba32.c.b;

    uiQuad.verts[1].rgba.c.r = arg0->rgba32.c.r;
    uiQuad.verts[1].rgba.c.g = arg0->rgba32.c.g;
    uiQuad.verts[1].rgba.c.b = arg0->rgba32.c.b;

    uiQuad.verts[2].rgba.c.r = arg0->rgba32.c.r;
    uiQuad.verts[2].rgba.c.g = arg0->rgba32.c.g;
    uiQuad.verts[2].rgba.c.b = arg0->rgba32.c.b;

    uiQuad.verts[3].rgba.c.r = arg0->rgba32.c.r;
    uiQuad.verts[3].rgba.c.g = arg0->rgba32.c.g;
    uiQuad.verts[3].rgba.c.b = arg0->rgba32.c.b;

    uiQuad.verts[0].v.x = arg0->verts[0].x;
    uiQuad.verts[0].v.y = arg0->verts[0].y;

    uiQuad.verts[1].v.x = arg0->verts[1].x;
    uiQuad.verts[1].v.y = arg0->verts[1].y;

    uiQuad.verts[2].v.x = arg0->verts[2].x;
    uiQuad.verts[2].v.y = arg0->verts[2].y;

    uiQuad.verts[3].v.x = arg0->verts[3].x;
    uiQuad.verts[3].v.y = arg0->verts[3].y;

    D_800A8848_83C48 = 1;

    //@recomp search for the minimap bg rect alignment
    u32 alignmentFlag = recomp_get_ui_alignment(arg0);

    if (alignmentFlag == MINIMAP && D_801BC3E8_1977E8 != 7) {
        // recomp_printf("Minimap BG Ptr=%X\n",arg0);
        recomp_set_ui_rect_alignment(alignmentFlag);
        recomp_erase_ui_key(arg0);
        gEXPushScissor(D_801A90F0_1844F0++);
        gEXSetScissor(D_801A90F0_1844F0++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                      SCREEN_HEIGHT);
    }

    func_8002ED90_A190(uiQuadPtr);

    //@recomp reset alignment
    gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    gEXPopScissor(D_801A90F0_1844F0++);
}

extern Gfx D_800A8810_83C10[];
// extern u8 D_800A8848_83C48;
extern GfxContext D_802047F0_1DFBF0;
extern u16 D_800A8850_83C50;
extern s32 D_80193C30_16F030;
extern Mtx D_801D8844_1B3C44;
extern void *D_801D8828_1B3C28; //unk
extern Mtx D_802048B0_1DFCB0; //Ident Mtx
extern void func_8009CCA0_780A0(Mtx *); //guMtxIdentF
extern void func_8009CD00_78100(float mf[4][4], Mtx *); //guMtxF2L
RECOMP_PATCH void func_8002ED90_A190(UIElemQuad *quad) {
    Mtx *mtx;
    s32 type;
    s16 ulx, uly, lrx, lry;
    u32 red32, green32, blue32, alpha;
    s32 uVar18;
    //@recomp
    Vtx *verts;

    type = 0;
    if ((D_800A8848_83C48 == 1) || (D_800A8848_83C48 == 3) ||
        (quad->verts[0].rgba.c.r == quad->verts[1].rgba.c.r &&
         quad->verts[0].rgba.c.r == quad->verts[2].rgba.c.r &&
         quad->verts[0].rgba.c.r == quad->verts[3].rgba.c.r &&
         quad->verts[0].rgba.c.g == quad->verts[1].rgba.c.g &&
         quad->verts[0].rgba.c.g == quad->verts[2].rgba.c.g &&
         quad->verts[0].rgba.c.g == quad->verts[3].rgba.c.g &&
         quad->verts[0].rgba.c.b == quad->verts[1].rgba.c.b &&
         quad->verts[0].rgba.c.b == quad->verts[2].rgba.c.b &&
         quad->verts[0].rgba.c.b == quad->verts[3].rgba.c.b)) {
        if (quad->verts[0].v.x == quad->verts[2].v.x &&
            quad->verts[1].v.x == quad->verts[3].v.x &&
            quad->verts[0].v.y == quad->verts[1].v.y &&
            quad->verts[2].v.y == quad->verts[3].v.y) {
            type = 1;
        } else if (quad->verts[0].v.x == quad->verts[1].v.x &&
                   quad->verts[2].v.x == quad->verts[3].v.x &&
                   quad->verts[0].v.y == quad->verts[2].v.y &&
                   quad->verts[1].v.y == quad->verts[3].v.y) {
            type = 1;
        }
    }

    if (D_80193C30_16F030 != 6) {
        D_80193C30_16F030 = 6;
        gSPDisplayList(D_801A90F0_1844F0++, D_800A8810_83C10);
        D_801D8844_1B3C44.m[0][0] = 0x04000001;
        gSPMatrix(D_801A90F0_1844F0++, (Mtx *) ((u8 *) &D_801D8844_1B3C44 - 0x1C),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    } else {
        gDPPipeSync(D_801A90F0_1844F0++);
    }

    gDPSetPrimColor(D_801A90F0_1844F0++, 0, 0, 0, 0, 0, 0);

    if ((D_800A8848_83C48 == 3) &&
        (quad->verts[0].v.x == (quad->verts[3].v.x - 0x140)) &&
        (quad->verts[0].v.y == (quad->verts[3].v.y - 0xf0))) {
        gDPSetRenderMode(D_801A90F0_1844F0++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    } else {
        gDPSetRenderMode(D_801A90F0_1844F0++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    }

    gDPSetCombineLERP(D_801A90F0_1844F0++,
                      1, 0, 0, PRIMITIVE,
                      1, 0, 0, PRIMITIVE,
                      1, 0, 0, PRIMITIVE,
                      1, 0, 0, PRIMITIVE);
    {
        s16 x0 = quad->verts[0].v.x;
        s16 y0 = quad->verts[0].v.y;
        s16 x1 = quad->verts[1].v.x;
        s16 y1 = quad->verts[1].v.y;

        if (x0 < x1) {
            ulx = x0;
            lrx = x1;
        } else {
            ulx = x1;
            lrx = x0;
        }

        if (y0 < y1) {
            uly = y0;
            lry = y1;
        } else {
            uly = y1;
            lry = y0;
        }

        gDPScisFillRectangle(D_801A90F0_1844F0++, ulx - 1, uly - 1, lrx + 2, lry + 2);
    } {
        s16 x2 = quad->verts[2].v.x;
        s16 y2 = quad->verts[2].v.y;
        s16 x3 = quad->verts[3].v.x;
        s16 y3 = quad->verts[3].v.y;

        if (x2 < x3) {
            ulx = x2;
            lrx = x3;
        } else {
            ulx = x3;
            lrx = x2;
        }

        if (y2 < y3) {
            uly = y2;
            lry = y3;
        } else {
            uly = y3;
            lry = y2;
        }

        gDPScisFillRectangle(D_801A90F0_1844F0++, ulx - 1, uly - 1, lrx + 2, lry + 2);
    } {
        s16 x0 = quad->verts[0].v.x;
        s16 y0 = quad->verts[0].v.y;
        s16 x2 = quad->verts[2].v.x;
        s16 y2 = quad->verts[2].v.y;

        if (x0 < x2) {
            ulx = x0;
            lrx = x2;
        } else {
            ulx = x2;
            lrx = x0;
        }

        if (y0 < y2) {
            uly = y0;
            lry = y2;
        } else {
            uly = y2;
            lry = y0;
        }

        gDPScisFillRectangle(D_801A90F0_1844F0++, ulx - 1, uly - 1, lrx + 2, lry + 2);
    } {
        s16 x1 = quad->verts[1].v.x;
        s16 y1 = quad->verts[1].v.y;
        s16 x3 = quad->verts[3].v.x;
        s16 y3 = quad->verts[3].v.y;

        if (x1 < x3) {
            ulx = x1;
            lrx = x3;
        } else {
            ulx = x3;
            lrx = x1;
        }

        if (y1 < y3) {
            uly = y1;
            lry = y3;
        } else {
            uly = y3;
            lry = y1;
        }

        gDPScisFillRectangle(D_801A90F0_1844F0++, ulx - 1, uly - 1, lrx + 2, lry + 2);
    }

    gDPPipeSync(D_801A90F0_1844F0++);

    if (quad->verts[0].rgba.c.a & 2) {
        red32 = quad->verts[0].rgba.c.r;
        green32 = quad->verts[0].rgba.c.g;
        blue32 = quad->verts[0].rgba.c.b;
        uVar18 = (D_800A8850_83C50 >> 5) & 3;

        switch (uVar18) {
            case 1:
                alpha = 0xbf;
                quad->verts[0].rgba.c.r = red32 + 0x3f;
                quad->verts[0].rgba.c.g += 0x3f;
                quad->verts[0].rgba.c.b += 0x3f;
                quad->verts[1].rgba.c.r += 0x3f;
                quad->verts[1].rgba.c.g += 0x3f;
                quad->verts[1].rgba.c.b += 0x3f;
                quad->verts[2].rgba.c.r += 0x3f;
                quad->verts[2].rgba.c.g += 0x3f;
                quad->verts[2].rgba.c.b += 0x3f;
                quad->verts[3].rgba.c.r += 0x3f;
                quad->verts[3].rgba.c.g += 0x3f;
                quad->verts[3].rgba.c.b += 0x3f;
                break;
            case 2:
                if (D_800A8848_83C48 == 3 && ((green32 != (red32 & 0xff)) | (blue32 != (red32 & 0xff)))) {
                    alpha = 0x60;
                    break;
                }
                alpha = (blue32 + green32 + red32) / 3 + 0x20;
                if (alpha >= 0x100) {
                    alpha = 0xff;
                }
                blue32 = 0;
                green32 = 0;
                red32 = 0;
                break;
            case 0:
            default:
                alpha = 0x80;
                break;
        }

        if (D_800A8848_83C48 == 3 &&
            ((((D_800A8850_83C50 >> 5) & 3) != 2) & ((red32 & 0xff) == (green32 & 0xff))) &&
            (green32 & 0xff) == blue32) {
            red32 = red32 & 0xff;
            green32 = green32 & 0xff;
            alpha = red32 + green32;
            alpha += green32;
            alpha /= 3;
            blue32 = 0xff;
            green32 = 0xff;
            red32 = 0xff;
        }
        gDPSetPrimColor(D_801A90F0_1844F0++, 0, 0, red32, green32, blue32, alpha);
    } else {
        gDPSetPrimColor(D_801A90F0_1844F0++, 0, 0,
                        quad->verts[0].rgba.c.r,
                        quad->verts[0].rgba.c.g,
                        quad->verts[0].rgba.c.b,
                        0xFF);
        gDPSetRenderMode(D_801A90F0_1844F0++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }

    switch ((u8) type) {
        case 0:
        case 2:
#if 1
            //@recomp rewrite of perspective UI elements to be ortho
            //This fixes the textbox borders and minimap cursor from stretching and helps with alignment
            mtx = (Mtx*) func_800281C8_35C8(0x40);
            verts = (Vtx*) func_800281C8_35C8(0x40);
            if (mtx != NULL && verts != NULL) {
                GfxContext *gfxContext = &D_802047F0_1DFBF0;
                // gSPViewport(D_801A90F0_1844F0++, &gfxContext->viewport); //required if using gEXSetViewportAlign

                bzero_recomp(mtx, 0x40);
                bzero_recomp(verts, 0x40);
                guOrtho(mtx, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);

                gDPSetCombineLERP(D_801A90F0_1844F0++,
                    1, 0, 0, SHADE,
                    1, 0, 0, PRIMITIVE,
                    1, 0, 0, SHADE,
                    1, 0, 0, PRIMITIVE);

                gSPMatrix(D_801A90F0_1844F0++, mtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPMatrix(D_801A90F0_1844F0++, &gfxContext->identMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

                for (int i = 0; i < 4; i++) {
                    ColoredVert *vert = &quad->verts[i];
                    verts[i].v.ob[0] = vert->v.x;
                    verts[i].v.ob[1] = vert->v.y;
                    verts[i].v.ob[2] = 0;
                    verts[i].v.flag = 0;
                    verts[i].v.tc[0] = 0;
                    verts[i].v.tc[1] = 0;
                    verts[i].v.cn[0] = vert->rgba.c.r;
                    verts[i].v.cn[1] = vert->rgba.c.g;
                    verts[i].v.cn[2] = vert->rgba.c.b;
                    verts[i].v.cn[3] = vert->rgba.c.a;
                }

                gSPVertex(D_801A90F0_1844F0++, verts, 4, 0);

                if (D_800A8848_83C48 == 2) {
                    gSP1Triangle(D_801A90F0_1844F0++, 0, 2, 1, 0);
                } else {
                    gSP2Triangles(D_801A90F0_1844F0++, 0, 2, 1, 0, 2, 3, 1, 0);
                }
            }
            break;

#else
            mtx = (Mtx *) func_800281C8_35C8(0x40);
            if (mtx != NULL) {
                gDPSetCombineLERP(D_801A90F0_1844F0++,
                    1, 0, 0, SHADE,
                    1, 0, 0, PRIMITIVE,
                    1, 0, 0, SHADE,
                    1, 0, 0, PRIMITIVE);
                bzero(mtx, 0x40);

                gSPVertex(D_801A90F0_1844F0++, mtx, 4, 0);

                gSPModifyVertex(D_801A90F0_1844F0++, 0, G_MWO_POINT_XYSCREEN,
                    (quad->verts[0].v.x << 18) | ((quad->verts[0].v.y << 2) & 0xFFFF));
                gSPModifyVertex(D_801A90F0_1844F0++, 1, G_MWO_POINT_XYSCREEN,
                    (quad->verts[1].v.x << 18) | ((quad->verts[1].v.y << 2) & 0xFFFF));
                gSPModifyVertex(D_801A90F0_1844F0++, 2, G_MWO_POINT_XYSCREEN,
                    (quad->verts[2].v.x << 18) | ((quad->verts[2].v.y << 2) & 0xFFFF));
                gSPModifyVertex(D_801A90F0_1844F0++, 3, G_MWO_POINT_XYSCREEN,
                    (quad->verts[3].v.x << 18) | ((quad->verts[3].v.y << 2) & 0xFFFF));

                gSPModifyVertex(D_801A90F0_1844F0++, 0, G_MWO_POINT_RGBA,
                    (quad->verts[0].rgba.c.r << 24) | (quad->verts[0].rgba.c.g << 16) |
                    (quad->verts[0].rgba.c.b << 8) | 0xFF);
                gSPModifyVertex(D_801A90F0_1844F0++, 1, G_MWO_POINT_RGBA,
                    (quad->verts[1].rgba.c.r << 24) | (quad->verts[1].rgba.c.g << 16) |
                    (quad->verts[1].rgba.c.b << 8) | 0xFF);
                gSPModifyVertex(D_801A90F0_1844F0++, 2, G_MWO_POINT_RGBA,
                    (quad->verts[2].rgba.c.r << 24) | (quad->verts[2].rgba.c.g << 16) |
                    (quad->verts[2].rgba.c.b << 8) | 0xFF);
                gSPModifyVertex(D_801A90F0_1844F0++, 3, G_MWO_POINT_RGBA,
                    (quad->verts[3].rgba.c.r << 24) | (quad->verts[3].rgba.c.g << 16) |
                    (quad->verts[3].rgba.c.b << 8) | 0xFF);

                if (D_800A8848_83C48 == 2) {
                    gSP1Triangle(D_801A90F0_1844F0++, 0, 2, 1, 0);
                } else {
                    gSP2Triangles(D_801A90F0_1844F0++, 0, 2, 1, 0, 2, 3, 1, 0);
                }
            }
            break;
#endif

        case 1:
            gDPScisFillRectangle(D_801A90F0_1844F0++, quad->verts[0].v.x, quad->verts[0].v.y, quad->verts[3].v.x + 1,
                                 quad->verts[3].v.y + 1);
            break;
    }

    D_800A8848_83C48 = 0;
}

//TODO Move these somewhere or add to decomp project
void guOrthoF(float m[4][4], float left, float right, float bottom, float top, float near, float far, float scale) {
    int row;
    int col;
    guMtxIdentF(m);
    m[0][0] = 2 / (right - left);
    m[1][1] = 2 / (top - bottom);
    m[2][2] = -2 / (far - near);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(far + near) / (far - near);
    m[3][3] = 1;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            m[row][col] *= scale;
        }
    }
}

void guOrtho(Mtx *m, float left, float right, float bottom, float top, float near, float far, float scale) {
    float sp28[4][4];
    guOrthoF(sp28, left, right, bottom, top, near, far, scale);
    guMtxF2L(sp28, m);
}

void guMtxIdentF(float mf[4][4]) {
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            if (r == c) {
                mf[r][c] = 1.0f;
            } else {
                mf[r][c] = 0.0f;
            }
        }
    }
}

void guMtxF2L(float mf[4][4], Mtx *m) {
    int r, c;
    s32 tmp1;
    s32 tmp2;
    s32 *m1 = &m->m[0][0];
    s32 *m2 = &m->m[2][0];
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 2; c++) {
            tmp1 = mf[r][2 * c] * 65536.0f;
            tmp2 = mf[r][2 * c + 1] * 65536.0f;
            *m1++ = (tmp1 & 0xffff0000) | ((tmp2 >> 0x10) & 0xffff);
            *m2++ = ((tmp1 << 0x10) & 0xffff0000) | (tmp2 & 0xffff);
        }
    }
}

void func_8003055C_B95C(void *, u8, u8, s32, u8);
s32 func_80032498_D898(s32);
s32 func_800324B4_D8B4(s32);
void func_8004AB78_25F78(Vec2s *, Vec2s *, s32, s32, s32);
extern u8 D_800ACAB2_87EB2;
extern u8 D_800ACAB3_87EB3;
extern s16 D_800ACAB8_87EB8;
extern s16 D_800ACABA_87EBA;
extern u32 D_800AD12C_8852C;
extern s32 D_80204978_1DFD78;
extern s32 D_8020497C_1DFD7C;
extern s16 D_802049C4_1DFDC4;
extern s16 D_802049C8_1DFDC8;
extern s16 D_80204A06_1DFE06;
//draw_map_content
RECOMP_PATCH void func_8004B008_26408(Vec2s *arg0, Vec2s *arg1, Vec2s *arg2, MinimapBackground *minimapBackground) {
    u32 sp2C;
    u32 temp_fp;
    UIElemUnk *unkElem;
    UIElemQuad *quad;
    UIElemQuad *next;
    u32 *ptr;
    s32 playerYaw;
    s32 xPos;
    s32 yPos;
    s32 temp_s4;
    s32 var_s0;
    s32 var_s1;
    u32 temp_a1;

    s32 temp_v0;
    s32 temp_a0;
    s32 temp_2;

    if ((minimapBackground->xOffset + minimapBackground->width) >= 0) {
        unkElem = D_8019A7A8_175BA8;
        temp_v0 = D_80204978_1DFD78 * 12;
        temp_fp = *(&D_800ACAB2_87EB2 + temp_v0);
        sp2C = *(&D_800ACAB3_87EB3 + temp_v0);
        D_8019A7A8_175BA8 += sizeof(UIElemUnk);
        func_8003055C_B95C(unkElem, 1, 1, 0x140, 0);
        func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, unkElem);

        quad = (UIElemQuad *) D_8019A7A8_175BA8;
        D_8019A7A8_175BA8 += sizeof(UIElemQuad);
        ptr = &quad->header.header;
        *ptr = 0x8000000;

        quad->verts[0].rgba.rgba32 = 0xFFFF0038;
        quad->verts[1].rgba.rgba32 = 0xFFFF0000;
        quad->verts[2].rgba.rgba32 = 0xFF000000;
        quad->verts[3].rgba.rgba32 = 0xFF000000;

        temp_a0 = D_80204978_1DFD78 * 6;
        xPos = arg0->x + (((D_802049C4_1DFDC4 - *(&D_800ACAB8_87EB8 + (temp_a0))) >> 9) - arg2->x) * D_800AD12C_8852C +
               (D_800AD12C_8852C >> 1);
        yPos = arg0->y - (((D_802049C8_1DFDC8 - *(&D_800ACABA_87EBA + (temp_a0))) >> 9) - arg2->y) * D_800AD12C_8852C -
               (D_800AD12C_8852C >> 1);

        //@recomp adjust the map arrow position if rendering the minimap
        if (minimapBackground->width == (D_800AD12C_8852C * 9)) {
            float original_aspect = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
            float cur_aspect = recomp_get_target_hud_aspect_ratio(original_aspect);
            s32 new_width = (s32)(SCREEN_HEIGHT * cur_aspect);
            xPos -= (new_width - SCREEN_WIDTH) / 2;
        }

        playerYaw = -D_80204A06_1DFE06;

        //@recomp commenting addition for first 2 verts, to correct arrow shape
        // temp_s4 = playerYaw + 0x40;
        temp_s4 = playerYaw;
        quad->verts[0].v.x = ((func_800324B4_D8B4(temp_s4) * 6) + 0x800) >> 12;
        quad->verts[0].v.x += xPos;
        quad->verts[0].v.y = ((func_80032498_D898(temp_s4) * 6) + 0x800) >> 12;
        quad->verts[0].v.y += yPos;

        // temp_s4 = playerYaw + 0x40;
        temp_s4 = playerYaw;
        quad->verts[1].v.x = ((func_800324B4_D8B4(temp_s4) * 6) + 0x800) >> 12;
        quad->verts[1].v.x += xPos;
        quad->verts[1].v.y = ((func_80032498_D898(temp_s4) * 6) + 0x800) >> 12;
        quad->verts[1].v.y += yPos;

        temp_s4 = playerYaw + 0x600;
        quad->verts[2].v.x = ((func_800324B4_D8B4(temp_s4) * 4) + 0x800) >> 12;
        quad->verts[2].v.x += xPos;
        quad->verts[2].v.y = ((func_80032498_D898(temp_s4) * 4) + 0x800) >> 12;
        quad->verts[2].v.y += yPos;

        playerYaw -= 0x600;
        quad->verts[3].v.x = ((func_800324B4_D8B4(playerYaw) * 4) + 0x800) >> 12;
        quad->verts[3].v.x += xPos;
        quad->verts[3].v.y = ((func_80032498_D898(playerYaw) * 4) + 0x800) >> 12;
        quad->verts[3].v.y += yPos;

        func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, quad);

        if (minimapBackground->width == (D_800AD12C_8852C * 9)) {
            for (var_s1 = 1; var_s1 < arg1->y - 1; var_s1++) {
                if ((arg2->y + var_s1) < sp2C) {
                    for (var_s0 = 1; var_s0 < arg1->x - 1; var_s0++) {
                        func_8004AB78_25F78(arg0, arg2, var_s0, var_s1, temp_fp);
                    }
                }
            }
        } else {
            for (var_s1 = 0; var_s1 < arg1->y; var_s1++) {
                if ((arg2->y + var_s1) < sp2C) {
                    for (var_s0 = 0; var_s0 < arg1->x; var_s0++) {
                        func_8004AB78_25F78(arg0, arg2, var_s0, var_s1, temp_fp);
                    }
                }
            }
        }
    }
}

extern void func_8002FD5C_B15C(s32, void *, void *);
extern void func_8003055C_B95C(void *, u8, u8, s32, u8);
extern u16 D_801D8CF4_1B40F4;
//pause_menu_bg
RECOMP_PATCH void func_800572DC_326DC(void) {
    UIElemSquare *orig;
    UIElemSquare *elem;
    UIElemSquare *next;
    u32 *ptr;
    s32 iterWidth, iterHeight;
    s32 xOffset, yOffset;
    s32 diagonal;

    orig = D_8019A7A8_175BA8;
    elem = orig;
    func_8003055C_B95C(orig, 0, 0, 0xD, 0);

    ptr = &elem->header;
    *ptr = (((u32)(elem + 1)) << 8) | 2;
    next = elem + 1;
    elem = next;

    iterHeight = 0;
    yOffset = -0x40;
    do {
        iterWidth = 0;
        xOffset = -0x40;
        do {
            next = elem + 1;
            ptr = &elem->header;
            *ptr = (((u32) next & 0xFFFFFF) << 8) | 4;
            elem->rgba.c.a = 0x64;
            elem->rgba.c.b = 0x80;
            elem->rgba.c.g = 0x80;
            elem->rgba.c.r = 0x80;
            diagonal = D_801D8CF4_1B40F4 % 64;
            elem->ulx = xOffset + diagonal;
            elem->uly = yOffset + diagonal;
            elem->s = 0xC0;
            elem->t = 0x88;
            elem->unkE = 0x3E77;
            elem->h = 64;
            elem->w = 64;
            elem = next;

            xOffset += 0x40;
        } while (++iterWidth < 7);
        yOffset += 0x40;
    } while (++iterHeight < 5);

    func_8002FD5C_B15C(D_8020497C_1DFD7C + 0xA8, orig, elem - 1);
    D_8019A7A8_175BA8 = elem;
}