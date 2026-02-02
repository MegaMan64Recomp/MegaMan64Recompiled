#include "patches.h"
#include "graphics.h"

// #define HEALTH_BAR ((u16)0x00A1)
// #define ENERGY_BAR ((u16)0x00A2)
// #define BOSS_HEALTH_BAR ((u16)0x00A3)
// #define MINIMAP ((u16)0x00A4)

static U32ValueHashmapHandle recompUIMap;
static _Bool recompUIMapInitialized = FALSE;

typedef enum {
    OTHER,
    HEALTH_BAR,
    ENERGY_BAR,
    BOSS_HEALTH_BAR,
    MINIMAP,
} UIAlignTypes;

typedef struct {
    s8 red;
    s8 green;
    s8 blue;
    u8 command;
} rgba;

/*typedef struct {
    rgba rgba;
    u16 llxOffset;
    u16 llyOffset;
    u16 xyOffset;
    u16 textureDetailsUnks;
    u8 unk_10;
    u8 unk_11;
    u16 textureBankUnk;
} uiElementGeneric;*/

typedef struct {
    u32 *ui_stack;
    void *data;
} uiStack;

//---------------
typedef struct {
    u32 addr: 24;
    u8 flag;
} bitfield_address_flag;

typedef struct  {
    s8 textureXOffset;
    s8 textureYOffset;
} textureOffsets;

typedef struct  {
    textureOffsets textureOffsets;
    s8 pallete1;
    s8 pallete2;
} textureDetails;

typedef struct  {
    s8 xUpper;
    s8 xLower;
} xSpriteSize;

typedef struct  {
    s8 yUpper;
    s8 yLower;
} ySpriteSize;

typedef struct  {
    s16 xSpriteSize;
    s16 ySpriteSize;
} xySpriteSize;

typedef struct {
    u8 spriteSheetXOffset;
    u8 spriteSheetYOffset;
    u16 pallete;
} spriteDetails;

typedef struct  {
    rgba rgba;
    s16 llxOffset;
    s16 llyOffset;
    xySpriteSize xySpriteSize;
    spriteDetails spriteDetails;
    u16 unk_10;
    // u8 uk_11;
    u16 textureBank;
} uiElemData;

typedef struct  {
    u32 ui_Header_Bitfield;
    rgba rgba;
    s16 ulxOffset;
    s16 ulyOffset;
    spriteDetails spriteDetails;
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

void Set_UI_Rect_Alignment(u16, UiElementGeneric*);
extern void func_8002FD30_B130(s32, void *, u16, s16);
extern void *D_8017B600_156A00;
extern void *D_8019A7A8_175BA8;
extern s32 D_8020497C_1DFD7C;
extern Gfx *D_801A90F0_1844F0;

//Boss Health Bar
RECOMP_PATCH void func_80049DC4_251C4(uiElemData* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    UiElementGeneric* elem;
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
        temp_t1 = arg0->spriteDetails.spriteSheetXOffset;
        elem = D_8019A7A8_175BA8;
        temp_a3 = arg0->spriteDetails.spriteSheetYOffset;
        D_8019A7A8_175BA8 = elem + 0x28;
        elem->ui_Header_Bitfield = 0x09000000;
        elem->rgba.command = 0x2C;
        elem->rgba.green = (s8) (arg4 >> 8);
        elem->rgba.red =(s8) arg4;
        elem->rgba.blue = (s8) (arg4 >> 0x10);
        temp_a0 = arg1 + arg0->llxOffset;
        elem->spriteDetails = arg0->spriteDetails;
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
        // elem->unk_1e = arg0->unk_10;
        if (!recompUIMapInitialized) {
            recompUIMap = recomputil_create_u32_value_hashmap();
            recompUIMapInitialized = true;
            recomp_printf("Initialized recompUIMap\n");
        }
        collection_key_t key = (collection_key_t) elem;
        u32 value = (u32) arg0->unk_10;
        recomp_printf("Insert Key= %X\n", key);
        recomp_printf("Insert value= %X\n", value);
        recomputil_u32_value_hashmap_insert(recompUIMap, key, arg0->unk_10);

        func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, elem, temp_a2, temp_a3_2);
    }
}

// insertUIStackGeneral
RECOMP_PATCH void func_80049CCC_250CC(uiElemData *arg0_elemData, s32 arg1_xOffset, s32 arg2_yOffset, s32 arg3_spriteSize , s16 alignmentFlag) {
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

    Update_UI_Alignments();

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
    element_generic->spriteDetails = arg0_elemData->spriteDetails;
    textureBank = arg0_elemData->textureBank;
    spriteXPos = (u8) arg0_elemData->spriteDetails.spriteSheetXOffset;
    spriteYPos = (u8) arg0_elemData->spriteDetails.spriteSheetYOffset;
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
    func_8002FD30_B130(D_8020497C_1DFD7C + 0x80, element_generic, textureBank, extraXOffset);

    //@recomp Utilize padded field in uiElement to pass along alignment
    // element_generic->unk_1e = arg0_elemData->unk_10;
    // u32 x, y;
    // recomp_get_window_resolution(&x, &y);
    // recomp_printf("Window Resolution Width=%X, Height=%X\n", x, y);
    // recomp_printf("Set UI Element PTR= %X\n", element_generic);
    if (!recompUIMapInitialized) {
        recompUIMap = recomputil_create_u32_value_hashmap();
        recompUIMapInitialized = true;
        recomp_printf("Initialized recompUIMap\n");
    }
    collection_key_t key = (collection_key_t) element_generic;
    u32 value = (u32) arg0_elemData->unk_10;
    // recomp_printf("Insert Key= %X\n", key);
    // recomp_printf("Insert value= %X\n", value);
    recomputil_u32_value_hashmap_insert(recompUIMap, key, arg0_elemData->unk_10);

    D_8017B600_156A00 = element_generic;
}

//DrawUITexture
extern void func_8002CD50_8150(UiElementGeneric*);
extern void func_8002D08C_848C(UiElementGeneric*, s32, s32, s32, s32);
extern void func_8002D370_8770(UiElementGeneric*, s32, u32, s32, u16, s32);
RECOMP_PATCH void func_8002E16C_956C(UiElementGeneric *elemData) {
    s16 sVar1;
    s32 varTemp;
    u16 uVar2;
    u32 uVar3;
    u32 yPos;
    u32 xPos;
    UiElementGeneric uiElement;

    uiElement = *elemData;
    if ((uiElement.ulxOffset < 0) && (uiElement.urxOffset < 0)) {
        varTemp = -1;
    }
    else {
        varTemp = 0;
        if (uiElement.ulyOffset < 0) {
            varTemp = (uiElement.llyOffset << 0x10) >> 0x1f;
        }
    }
    if (varTemp == 0) {
        varTemp = uiElement.width - uiElement.spriteDetails.spriteSheetXOffset;
        if (varTemp < 0) {
            varTemp = -varTemp;
        }
        sVar1 = (s16)varTemp;
        xPos = sVar1;
        varTemp = uiElement.height - uiElement.spriteDetails.spriteSheetYOffset;
        if (varTemp < 0) {
            varTemp = -varTemp;
        }
        uVar2 = (u16)varTemp;
        if (xPos != uiElement.urxOffset - uiElement.ulxOffset) {
            sVar1 = uiElement.urxOffset - uiElement.ulxOffset;
        }
        yPos = (u32)(s16)uVar2;
        if (yPos != uiElement.llyOffset - uiElement.ulyOffset) {
            uVar2 = uiElement.llyOffset - uiElement.ulyOffset;
        }

        if (xPos != 0 && yPos != 0) {
            if (sVar1 != 0 && uVar2 != 0) {
                //@recomp Utilize padded field in uiElement to retrieve alignment
                // recomp_printf("Embedded Flag = %X\n", uiElement.unk_1e);
                // Set_UI_Rect_Alignment(uiElement.unk_1e, &uiElement);

                if (recompUIMapInitialized) {
                    collection_key_t key = (collection_key_t) elemData;
                    u32 value;
                    // recomp_printf("Get UI Element PTR= %X\n", elemData);
                    recomputil_u32_value_hashmap_get(recompUIMap, key, &value);
                    // recomputil_u32_value_hashmap_erase(recompUIMap, key);
                    recomputil_u32_value_hashmap_insert(recompUIMap, key, 0);
                    // recomp_printf("Get Key= %X\n", key);
                    // recomp_printf("Get value= %X\n", value);
                    Set_UI_Rect_Alignment(value, &uiElement);
                }


                uVar3 = (uiElement.texturePallete >> 7) & 3;
                varTemp = uVar3;
                func_8002CD50_8150(&uiElement);
                func_8002D08C_848C(&uiElement,varTemp * 0xf0 + 0x10,uVar3, (uiElement.texturePallete >> 5) & 3,0);
                func_8002D370_8770(&uiElement,xPos,yPos,sVar1,uVar2,varTemp);

                //@recomp
                gEXSetRectAlign(D_801A90F0_1844F0++,G_EX_ORIGIN_NONE,G_EX_ORIGIN_NONE,0,0,0,0);
            }
        }
    }
    return;
}

RECOMP_EXPORT void Set_UI_Rect_Alignment(u16 alignmentFlag, UiElementGeneric* elem) {

    switch (alignmentFlag) {
        case HEALTH_BAR:
        case MINIMAP:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
            // recomp_printf("HP\n");
            break;
        case ENERGY_BAR:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4, 0);
            // recomp_printf("EN\n");
            break;
        case BOSS_HEALTH_BAR:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4, 0);
            // recomp_printf("BOSS HP\n");
            break;
        default:
            gEXSetRectAlign(D_801A90F0_1844F0++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
            break;
    }

    return;
}

extern uiElemData D_800ACC90_88090[]; // HP Bar
extern uiElemData D_800ACF24_88324[]; // EN Bar
extern uiElemData D_800ACF9C_8839C[]; // Boss Bar
RECOMP_EXPORT void Update_UI_Alignments() {
    //Set HP Bar
    for (int i = 0; i < 33; i++) {
        D_800ACC90_88090[i].unk_10 = HEALTH_BAR;
    }

    //Set EN Bar
    for (int i = 0; i < 7; i++) {
        D_800ACF24_88324[i].unk_10 = ENERGY_BAR;
    }

    //Set Boss Bar
     for (int i = 0; i < 10; i++) {
         D_800ACF9C_8839C[i].unk_10 = BOSS_HEALTH_BAR;
     }
    return;
}

//Sky Patch
extern void func_8002CB20_7F20;
extern void func_8002CC58_8058;
extern void func_8002E8FC_9CFC;
extern void func_8002ED90_A190;
extern void func_8002FA3C_AE3C;
extern void func_8002FB2C_AF2C;
extern void func_8002FC1C_B01C;
extern void func_800306B0_BAB0;
extern void func_800861E0_615E0;
extern void func_800283A8_37A8(s8, s8, void*, void*);

//Graph UI Functions
// RECOMP_PATCH void func_80030354_B754(UiElementGeneric* arg0, s8 arg1, s8 arg2) {
//     void* var_a2;
//     s32 temp_v1;
//     UiElementGeneric* var_s0;
//     u32 temp_a0;
//     u32* temp_a3;
//
//     temp_a0 = arg0->ui_Header_Bitfield;
//     recomp_printf("temp_a0 = %x\n", temp_a0);
//     if ((temp_a0 & 0xFFFFFF00) != 0xffffff00) {
//         var_s0 = (UiElementGeneric*)((temp_a0 >> 8)+ 0x80000000);
//         recomp_printf("var_s0=%X\n", var_s0);
//         loop_2:
//                 temp_v1 = var_s0->rgba.command;
//                 recomp_printf("temp_v1=%X\n", temp_v1);
//         switch (temp_v1) {                          /* irregular */
//             case 0xE0:
//                 var_a2 = &func_800306B0_BAB0;
//                 block_28:
//                             func_800283A8_37A8(arg1, arg2, var_a2, (void* ) var_s0);
//                 break;
//             case 0xD0:
//                 //Draw Sky
//                 var_a2 = &func_8002E8FC_9CFC;
//                 goto block_28;
//             case 0xC0:
//                 var_a2 = &func_800861E0_615E0;
//                 goto block_28;
//             case 0x28:
//                 var_a2 = &func_8002FA3C_AE3C;
//                 goto block_28;
//             case 0x2C:
//                 var_a2 = &func_8002E16C_956C;
//                 goto block_28;
//             case 0x30:
//                 var_a2 = &func_8002FB2C_AF2C;
//                 goto block_28;
//             case 0x38:
//                 var_a2 = &func_8002ED90_A190;
//                 goto block_28;
//             case 0x60:
//                 var_a2 = &func_8002FC1C_B01C;
//                 goto block_28;
//             case 0x64:
//                 var_a2 = &func_8002CB20_7F20;
//                 goto block_28;
//             case 0x74:
//                 var_a2 = &func_8002CC58_8058;
//                 goto block_28;
//         }
//         temp_a0 = var_s0->ui_Header_Bitfield;
//         if ((temp_a0 & 0xFFFFFF00) != 0xffffff00) {
//             var_s0 = (UiElementGeneric*)((temp_a0 >> 8)+ 0x80000000);
//             goto loop_2;
//         } else {
//             return;
//         }
//     }
// }
//
