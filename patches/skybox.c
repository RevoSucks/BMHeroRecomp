#include "patches.h"

#include <ultra64.h>

// skybox.c

f32 __sinf(f32);

extern f64 D_80105580;
extern f64 D_80105588;
extern f64 D_80105590;
extern f64 D_80105598;
extern f64 D_801055A0;
extern f32 D_80177A14;
extern s16 D_80177A1A;
extern s16 D_80177A1E;
extern f32 D_80177A24;

extern Gfx* gSkyboxImageBase;
extern Gfx* gSkyboxImageOffset; // gSkyboxImageOffset = 0x801775B4;
extern Gfx* gMasterDisplayList;

extern f32 gSkyboxXCoord;
extern f32 gSkyboxYCoord;

extern f32 D_801055A8;
extern f32 D_801055AC;
extern f32 D_801055B0;

extern s16 D_801778F0;
extern f32 D_80177A58;

extern f32 D_801055B4;
extern u16 gActiveContButton;

union U16_LE {
    u8 asU8[4];
    u16 asU16[2];
    u32 asU32;
};

// the contents of which the skybox file decompresses to
struct SkyboxFile {
    char filler0[0x4];
    /* 0x04 */ s8 defaultTile; // the default index into the image to start rendering the skybox at when it starts loading.
    /* 0x08 */ union U16_LE imgWidth;
    /* 0x0C */ union U16_LE imgHeight;
    u32 image[1];
};

#define SKYBOX_FILE_ID 0x18
#define SKYBOX_FILE ((struct SkyboxFile *)gFileArray[SKYBOX_FILE_ID].ptr)

/**
 * Load in the image height/width values from the loaded skybox file.
 */
RECOMP_PATCH void Skybox_GetWidthHeight(s32* ptrWidth, s32* ptrHeight) {
    // the 16-bit values are for some reason stored as little endian in the skybox bins, so we must manually read them in
    // as a big endian value.
    *ptrWidth = (SKYBOX_FILE->imgWidth.asU8[1] << 8) + SKYBOX_FILE->imgWidth.asU8[0];
    *ptrHeight = (SKYBOX_FILE->imgHeight.asU8[1] << 8) + SKYBOX_FILE->imgHeight.asU8[0];
}

/**
 * Draw the "standard" type skybox which will move around in a 32-pixel buffer determined by the current view/camera.
 */
RECOMP_PATCH void Skybox_DrawStandard(f32 x, f32 y, s32 img_width, UNUSED s32 img_height) {
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 ulx;
    s32 uly;
    s32 width;
    s32 height;
    s32 i; // sp64

    gSPClearGeometryMode(gMasterDisplayList++, G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH);
    gSPSetGeometryMode(gMasterDisplayList++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetTexturePersp(gMasterDisplayList++, G_TP_NONE);
    gDPSetRenderMode(gMasterDisplayList++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
    gDPSetCombineMode(gMasterDisplayList++, G_CC_DECALRGB, G_CC_DECALRGB);
    gDPSetColorDither(gMasterDisplayList++, G_CD_BAYER);
    gDPSetTextureFilter(gMasterDisplayList++, G_TF_BILERP);
    gDPSetTextureLUT(gMasterDisplayList++, G_TT_RGBA16);
    gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSkyboxImageBase);
    gDPTileSync(gMasterDisplayList++);
    gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gMasterDisplayList++);
    gDPLoadTLUTCmd(gMasterDisplayList++, G_TX_LOADTILE, 15);
    gDPPipeSync(gMasterDisplayList++);

    sp80 = (s32) y / 4;
    uly = (s32)(y * 4.0f) & 0xF;
    if (uly != 0) {
        sp80 += 1;
        uly = 0x10 - uly;
    }
    sp78 = 0x1E;
    height = 0x1E0;
    sp84 = (s32) x / 8;
    sp84 *= 2;
    ulx = (s32)(x * 4.0f) & 0x1F;
    if (ulx != 0) {
        sp84 += 2;
        ulx = 0x20 - ulx;
    }
    sp7C = 0x50;
    width = 0x500;

    for(i = 0; i < 2; i++) {
        gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, img_width >> 1, gSkyboxImageOffset);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, (((sp84 + sp7C - sp84 + 1) >> 1) + 7) >> 3, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPLoadSync(gMasterDisplayList++);
        gDPLoadTile(gMasterDisplayList++, G_TX_LOADTILE, (sp84 * 2), (sp80 * 4), ((sp84 + sp7C) * 2), ((sp80 + sp78) * 4));
        gDPPipeSync(gMasterDisplayList++);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_4b, ((s32) (((s32) (((sp84 + sp7C) - sp84) + 1) >> 1) + 7) >> 3), 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTileSize(gMasterDisplayList++, G_TX_RENDERTILE, (sp84 * 4), (sp80 * 4), ((sp84 + sp7C) * 4), ((sp80 + sp78) * 4));
        gDPSetTileSize(gMasterDisplayList++, G_TX_RENDERTILE, 0, 0, (sp7C * 4), (sp78 * 4));
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_STRETCH);
        gSPTextureRectangle(gMasterDisplayList++, ulx, uly, (ulx + width), (uly + height), G_TX_RENDERTILE, 0, 0, 0x0100, 0x0100);
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_AUTO);
        sp80 += sp78;
        uly += height;
    }
}

/**
 * Draw the "wave" type skybox which, while fixed to a 0,0 origin, will do a 'wave' distortion effect.
 */
RECOMP_PATCH void Skybox_DrawWave(f32 x, f32 y, s32 img_width, UNUSED s32 img_height) {
    s32 spAC;
    s32 spA8;
    s32 spA4;
    s32 spA0;
    s32 ulx;
    s32 uly;
    s32 width;
    s32 height;
    s32 i;
    s32 j;
    f32 sp84;
    s32 sp80;

    gSPClearGeometryMode(gMasterDisplayList++, G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH);
    gSPSetGeometryMode(gMasterDisplayList++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetTexturePersp(gMasterDisplayList++, G_TP_NONE);
    gDPSetRenderMode(gMasterDisplayList++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
    gDPSetCombineMode(gMasterDisplayList++, G_CC_DECALRGB, G_CC_DECALRGB);
    gDPSetColorDither(gMasterDisplayList++, G_CD_BAYER);
    gDPSetTextureFilter(gMasterDisplayList++, G_TF_BILERP);
    gDPSetTextureLUT(gMasterDisplayList++, G_TT_RGBA16);
    gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, gSkyboxImageBase);
    gDPTileSync(gMasterDisplayList++);
    gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gMasterDisplayList++);
    gDPLoadTLUTCmd(gMasterDisplayList++, G_TX_LOADTILE, 15);
    gDPPipeSync(gMasterDisplayList++);

    spA8 = (s32) y / 4;
    uly = (s32) (y * 4.0f) & 0xF;
    if (uly != 0) {
        spA8 += 1;
        uly = 0x10 - uly;
    }
    spA0 = 0x1E;
    height = 4;
    spAC = (s32) x / 8;
    spAC *= 2;
    ulx = (s32) (x * 4.0f) & 0x1F;
    if (ulx != 0) {
        spAC += 2;
        ulx = 0x20 - ulx;
    }
    spA4 = 0x50;
    width = 0x500;

    for(i = 0; i < 2; i++) {
        gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, (img_width >> 1), gSkyboxImageOffset);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, ((s32) (((s32) (((spAC + spA4) - spAC) + 1) >> 1) + 7) >> 3), 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPLoadSync(gMasterDisplayList++);
        gDPLoadTile(gMasterDisplayList++, G_TX_LOADTILE, (spAC * 2), (spA8 * 4), ((spAC + spA4) * 2), ((spA8 + spA0) * 4));
        gDPPipeSync(gMasterDisplayList++);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_4b, ((s32) (((s32) (((spAC + spA4) - spAC) + 1) >> 1) + 7) >> 3), 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPSetTileSize(gMasterDisplayList++, G_TX_RENDERTILE, (spAC * 4), (spA8 * 4), ((spAC + spA4) * 4), ((spA8 + spA0) * 4));
        gDPSetTileSize(gMasterDisplayList++, G_TX_RENDERTILE, 0, 0, (spA4 * 4), (spA0 * 4));
        // wave effect
        for(j = 0; j < 0x78; j++) {
            sp84 = ((((i * 0x78) + j) * 0x168) / (0xF0 / D_80177A1E)) + D_80177A14;
            if (sp84 >= D_80105580) {
                sp84 = (sp84 - D_80105588);
            }
            sp84 = __sinf((sp84 * D_80105590)) * (D_80177A1A / 2);
            sp80 = (sp84 * 32.0f);
            D_80177A14 += D_80177A24;
            if (D_80177A14 >= D_80105598) {
                D_80177A14 = (D_80177A14 - D_801055A0);
            }
            gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_STRETCH);
            gSPTextureRectangle(gMasterDisplayList++, ulx, uly, (ulx + width), (uly + height), G_TX_RENDERTILE, sp80, (j << 5) / 4, 0x0100, 0x0100);
            gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_AUTO);
            uly += height;
        }
        spA8 += spA0;
    }
}

/**
 * load the img width/height from the skybox file and use the draw method for the BG.
 */
RECOMP_PATCH void Skybox_ProcessDraw(void) {
    s32 img_width;
    s32 img_height;

    if (gSkyboxType == 3) {
        // "wave" type skybox. fixed to 0 due to the effect. Only seems to be used in the VS Nitros fights.
        gSkyboxXCoord = 0.0f;
        gSkyboxYCoord = 0.0f;
        Skybox_GetWidthHeight(&img_width, &img_height);
        Skybox_DrawWave(gSkyboxXCoord, gSkyboxYCoord, img_width + 0x50, img_height);
    } else {
        // normal skybox. skybox moves around with camera
        Skybox_GetWidthHeight(&img_width, &img_height);
        Skybox_DrawStandard(gSkyboxXCoord, gSkyboxYCoord, img_width + 0x50, img_height);
    }
}

/**
 * (unused) Move the skybox with the d-pad. Was probably used for testing.
 */
RECOMP_PATCH void Skybox_MoveDpad(void) {
    f32 x = gSkyboxXCoord;
    f32 y = gSkyboxYCoord;

    if (gActiveContButton & L_JPAD) {
        x -= 1.0f;
    } else if (gActiveContButton & R_JPAD) {
        x += 1.0f;
    }
    if (gActiveContButton & U_JPAD) {
        y -= 1.0f;
    } else if (gActiveContButton & D_JPAD) {
        y += 1.0f;
    }
    if (x <= 0.0f) {
        x = 0.0f;
    } else if (x >= D_801055A8) {
        x = D_801055AC;
    }
    if (y <= 0.0f) {
        y = 0.0f;
    } else if (y >= D_801055B0) {
        y = D_801055B4;
    }
    gSkyboxXCoord = x;
    gSkyboxYCoord = y;
}

/**
 * The skybox rotates along the camera rotation; generally, the skybox does not move with the player unless it is along the height axis.
 */
RECOMP_PATCH void Skybox_MoveY(void) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    s32 width;
    s32 height;

    sp2C = ((gView.rot.y / 360.0f) * 1280.0f) + 640.0f;
    if (sp2C >= 1280.0f) {
        sp2C -= 1280.0f;
    }
    gSkyboxXCoord = sp2C;
    Skybox_GetWidthHeight(&width, &height);
    sp28 = ((height * 4u) - 0x12C) - ((80.0f * (gView.at.y - D_801778F0)) / 960.0f);
    if (gView.rot.x >= 180.0f) {
        sp24 = gView.rot.x - 360.0f;
    } else {
        sp24 = gView.rot.x;
    }
    sp24 = (sp24 - D_80177A58) * 2.0f;
    if (sp24 >= 60.0f) {
        sp24 = 60.0f;
    } else if (sp24 <= -60.0f) {
        sp24 = -60.0f;
    }
    gSkyboxYCoord = sp28 + sp24;
    if (gSkyboxYCoord <= 0.0f) {
        gSkyboxYCoord = 0.0f;
    }
}

/**
 * The skybox rotates with the camera movement; generally, the skybox moves with the player.
 */
RECOMP_PATCH void Skybox_MoveX(void) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    s32 width;
    s32 height;

    sp2C = gView.at.x;
    if (sp2C <= D_801777E8) {
        sp2C = D_801777E8;
    } else if (sp2C >= D_80177900) {
        sp2C = D_80177900;
    }
    gSkyboxXCoord = ((sp2C - D_801777E8) * 80.0f) / 960.0f;
    Skybox_GetWidthHeight(&width, &height);
    sp28 = ((height * 4u) - 0x12C) - ((80.0f * (gView.at.y - D_801778F0)) / 960.0f);
    if (gView.rot.x >= 180.0f) {
        sp24 = gView.rot.x - 360.0f;
    } else {
        sp24 = gView.rot.x;
    }
    sp24 = (sp24 - D_80177A58) * 2.0f;
    if (sp24 >= 60.0f) {
        sp24 = 60.0f;
    } else if (sp24 <= -60.0f) {
        sp24 = -60.0f;
    }
    gSkyboxYCoord = sp28 + sp24;
    if (gSkyboxYCoord <= 0.0f) {
        gSkyboxYCoord = 0.0f;
    }
}

/**
 * Process the skybox move depending on the skybox type.
 */
RECOMP_PATCH void Skybox_ProcessMove(void) {
    if (gSkyboxID != 0) {
        switch (gSkyboxType) {
            case 0:
                Skybox_MoveY();
                break;
            case 1:
                Skybox_MoveX();
                break;
            case 2:
                Skybox_MoveX();
                break;
        }
    }
}

RECOMP_PATCH void Skybox_Format(struct SkyboxFile* skybox) {
    s8* sp34;
    s8* sp30;
    u32 i;
    s32 j;
    u32 width;
    u32 height;
    s32 id;

    gSkyboxImageBase = &skybox->image; // set the base image
    // this is really doing ALIGN8(skybox->image[skybox->defaultTile]) .. but that wont match
    skybox = (struct SkyboxFile *)(((((skybox->defaultTile * 2) + (s32)skybox) + 7) & ~7) + 0x10);
    gSkyboxImageOffset = skybox; // set the tile offsetted image
    id = gSkyboxID - 1;
    Skybox_GetWidthHeight(&width, &height);
    sp34 = (s8*)(((width >> 1) * (height - 1)) + (u32)gSkyboxImageOffset);
    sp30 = (s8*)((((width >> 1) + 0x28) * height) + (u32)gSkyboxImageOffset);

    for (i = 0; i < height; i++) {
        for(j = 0; j < (width >> 1); j++) {
            *sp30 = *sp34;
            sp30++;
            sp34++;
        }
        sp34 -= width >> 1;
        for(j = 0; j < 0x28; j++) {
            *sp30 = *sp34;
            sp30++;
            sp34++;
        }
        sp34 = (sp34 - (width >> 1)) - 0x28;
        sp30 = (sp30 - width) - 0x50;
    }
}

/**
 * Load skybox to RAM from the set skybox ID.
 */
RECOMP_PATCH void Skybox_LoadFromID(void) {
    s32 tableIdx;
    u32 romStart;
    u32 romEnd;

    if (gSkyboxID != 0) {
        tableIdx = gSkyboxID - 1;
        romStart = gSkyboxROMTable[tableIdx].romStart;
        romEnd = gSkyboxROMTable[tableIdx].romEnd;
        DecompressFile(SKYBOX_FILE_ID, romStart, romEnd);
        Skybox_Format(SKYBOX_FILE);
    }
    D_80177A14 = 0.0f;
    D_80177A1A = 0xA;
    D_80177A1E = 4;
    D_80177A24 = D_801055B8;
}
