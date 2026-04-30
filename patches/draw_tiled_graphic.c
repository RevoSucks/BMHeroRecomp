#include "patches.h"

#include <ultra64.h>

// draw_tiled_graphic.c

// Patch the GFX tiler function

extern Gfx* gMasterDisplayList;

#define FILE_ID_1        1

struct ReplaceCoord {
    int use;
    f32 coords[2][2];
};

struct RomMatch {
    s32 rom_offset;
    s32 arr_offset;
    struct ReplaceCoord update;
};

#define NUM_ENTRIES 9
#define TRUE 1
#define FALSE 0

// for entries we do not need to update the coordinate, use this define
#define NO_COORD_REPLACE { FALSE, { /* old */ 0.0f, 0.0f, /* new */ 0.0f, 0.0f } }

struct RomMatch gEntries[NUM_ENTRIES] = {
    { 0x00B69820, 0x00000000, NO_COORD_REPLACE },                                         // Title screen BG
    { 0x00B7B260, 0x000030A0, NO_COORD_REPLACE },                                         // File select screen BG  (blue)
    { 0x00B7B260, 0x00004330, NO_COORD_REPLACE },                                         // File select screen BG  (orange)
    { 0x00B7B260, 0x000036D0, NO_COORD_REPLACE },                                         // File select screen BG  (green)
    { 0x00B7B260, 0x00003D00, NO_COORD_REPLACE },                                         // File select screen BG  (red)
    { 0x00B7B260, 0x00004960, NO_COORD_REPLACE },                                         // File select screen BG  (cyan)
    { 0x00B802E0, 0x00007020, NO_COORD_REPLACE },                                         // Options menu screen BG (cyan)
    { 0x00B22470, 0x00000000, { TRUE, { /* old */ 8.0f, 2.0f, /* new */ 0.0f, 0.0f } } }, // World select screen BG (clouds)
    { 0x00B234C0, 0x00000000, { TRUE, { /* old */ 8.0f, 2.0f, /* new */ 0.0f, 0.0f } } }  // World select screen BG (stars)
};

/**
 * Search the gEntries table to see if an entry matches. Report that it is tagged if it is in the database.
 */
RECOMP_EXPORT int isRomOffsetTagged(s32 rom_offset, s32 arr_offset) {
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (gEntries[i].rom_offset == rom_offset && gEntries[i].arr_offset == arr_offset) {
            return i;
        }
    }
    return -1;
}

// the first time file 1 is used, it is the boot logo. Suspend auto for it and then set this.
static int fileOneTagged = 0;

// draw a tiled graphic?
RECOMP_PATCH void func_80060F00(f32 x, f32 y, s32 width, s32 height, f32 scale_x, f32 scale_y, Gfx *arg6, Gfx *arg7, s16 arg8) {
    s32 spFC;
    s32 spF8;
    s32 spF4;
    s32 spF0;
    s32 size;
    s32 spE8;
    s16 count;
    s16 i;
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s32 spD0;
    s32 spCC;
    s32 spC8;
    f32 spC4;

    gDPPipeSync(gMasterDisplayList++);
    gDPSetCycleType(gMasterDisplayList++, G_CYC_1CYCLE);
    gSPClearGeometryMode(gMasterDisplayList++, G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH);
    gSPSetGeometryMode(gMasterDisplayList++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetTexturePersp(gMasterDisplayList++, G_TP_NONE);
    gDPSetRenderMode(gMasterDisplayList++, G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2);
    gDPSetCombineMode(gMasterDisplayList++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetColorDither(gMasterDisplayList++, G_CD_BAYER);
    gDPSetTextureFilter(gMasterDisplayList++, G_TF_BILERP);

    if (arg8 == 0) {
        gDPSetTextureLUT(gMasterDisplayList++, G_TT_RGBA16);
        gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, arg7);
        gDPTileSync(gMasterDisplayList++);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPLoadSync(gMasterDisplayList++);
        gDPLoadTLUTCmd(gMasterDisplayList++, G_TX_LOADTILE, 15);
        gDPPipeSync(gMasterDisplayList++);
    } else if (arg8 == 1) {
        gDPSetTextureLUT(gMasterDisplayList++, G_TT_RGBA16);
        gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, arg7);
        gDPTileSync(gMasterDisplayList++);
        gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        gDPLoadSync(gMasterDisplayList++);
        gDPLoadTLUTCmd(gMasterDisplayList++, G_TX_LOADTILE, 255);
        gDPPipeSync(gMasterDisplayList++);
    } else {
        gDPSetTextureLUT(gMasterDisplayList++, G_TT_NONE);
    }

    spD0 = (s32)(1024.0f / scale_x + 0.5f);
    spCC = (s32)(1024.0f / scale_y + 0.5f);
    spE8 = 0;
    spF0 = 0;
    spF4 = 0;
    spFC = width;
    spF8 = height;

    if (x != 0.0f && x < (2.0f * scale_x)) {
        spC4 = -(x - (2.0f * scale_x));
        spF4 = (spF4 + (s32)(spC4 / scale_x));
        width -= spF4;
        spC8 = (s32)(spC4 / (2.0f * scale_x));
        spC8 = (s32)(spC4 - (2.0f * scale_x * spC8));
        x = ((scale_x * 2.0f) - spC8);
    }
    if (y != 0.0f && y < scale_y) {
        spC4 = -(y - scale_y);
        spF0 = (spF0 + (s32)(spC4 / scale_y));
        height -= spF0;
        spC8 = (s32)(spC4 / scale_y);
        spC8 = (s32)(spC4 - (spC8 * scale_y));
        y = scale_y - spC8;
    }
    if (((width * scale_x) + x) > 328.0f) {
        width = (s32)(width - (((x + (width * scale_x)) - 328.0f) / scale_x));
    }
    if (((height * scale_y) + y) > 244.0f) {
        height = (s32)(height - (((y + (height * scale_y)) - 244.0f) / scale_y));
    }
    if (width < 0) {
        width = 0;
    }
    if (spF4 > spFC) {
        spF4 = spFC;
    }
    if (height < 0) {
        height = 0;
    }
    if (spF0 > spF8) {
        spF0 = spF8;
    }

    if ((width * height) >= 0x5DD) {
        size = 0x5DC / width;
        count = (height / size);
        if ((height % size) != 0) {
            count += 1;
        }
    } else {
        count = 1;
        size = height;
    }

    ulx = (s32)(x * 4.0f + 0.5f);
    lrx = (s32)(x * 4.0f + (width * scale_x) * 4.0f + 0.5f);
	
	s32 current_uly = (s32)(y * 4.0f + 0.5f);

    for (i = 0; i < count; i++) {

        if ((i + 1) == count) {
            size = height - (i * size);
        }

        if (arg8 == 0) {
            gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, spFC >> 1, arg6);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_8b, ((((((spF4 + width) - spF4) + 1) >> 1) + 7) >> 3), 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync(gMasterDisplayList++);
            gDPLoadTile(gMasterDisplayList++, G_TX_LOADTILE, spF4 * 2, spF0 * 4, ((spF4 + width) * 2) - 1, ((spF0 + size) * 4) - 4);
            gDPPipeSync(gMasterDisplayList++);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_CI, G_IM_SIZ_4b, ((((((spF4 + width) - spF4) + 1) >> 1) + 7) >> 3), 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        } else if (arg8 == 1) {
            gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_8b, spFC, arg6);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_8b, ((((spF4 + width) - spF4) + 8) >> 3), 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync(gMasterDisplayList++);
            gDPLoadTile(gMasterDisplayList++, G_TX_LOADTILE, (spF4 * 4), (spF0 * 4), ((spF4 + width) * 4) - 4, ((spF0 + size) * 4) - 4);
            gDPPipeSync(gMasterDisplayList++);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_8b, ((((spF4 + width) - spF4) + 8) >> 3), 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        } else {
            gDPSetTextureImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_32b, spFC, arg6);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_32b, (((((spF4 + width) - spF4) * 2) + 9) >> 3), 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadSync(gMasterDisplayList++);
            gDPLoadTile(gMasterDisplayList++, G_TX_LOADTILE, (spF4 * 4), (spF0 * 4), ((spF4 + width) * 4) - 4, ((spF0 + size) * 4) - 4);
            gDPPipeSync(gMasterDisplayList++);
            gDPSetTile(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_32b, (((((spF4 + width) - spF4) * 2) + 9) >> 3), 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
        }

        gDPSetTileSize(gMasterDisplayList++, G_TX_RENDERTILE, 0, 0, (width * 4) - 4, (size * 4) - 4);

        uly = current_uly;
        lry = (s32)(uly + (size * scale_y) * 4.0f + 0.5f);
        current_uly = lry;

        gSPTextureRectangle(gMasterDisplayList++, ulx, uly, lrx, lry, G_TX_RENDERTILE, 0, 0, spD0, spCC);
        spF0 += size;
        spE8 += size;
    }
}

extern f64 D_80103D00;
extern f32 D_80103D08;
extern f64 D_80103D10;
extern f32 D_80103D18;

struct UnkStruct8016C298 {
    /* 0x00 */ u32 unk0;
    char filler4[0x1];
    /* 0x05 */ u8 unk5;
    char filler6[0x2];
}; // size=0x8

extern struct UnkStruct8016C298 D_8016C298[];
extern u8 D_8016C29D[];
extern struct DecompressedFileEntry gFileArray[700];
extern struct ObjectStruct gObjects[207];

struct UnkFuncStruct800623E0_SP4C {
    char filler0[0x4];
    /* 0x04 */ u8 *unk4;
    char filler8[0x4];
    /* 0x0C */ u32 *unkC;
    char filler10[0x4];
    /* 0x14 */ s16 unk14;
};

extern u8 D_80177932;
extern u8 D_80177934;
extern u8 D_80177938;

// process tiled graphic call by loading arguments and calling draw
RECOMP_PATCH void func_800623E0(s32 objId) {
    f32 x;
    f32 y;
    s32 width;
    s32 height;
    s32 fileId;
    s32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 scale_x;
    f32 scale_y;
    struct UnkFuncStruct800623E0_SP4C* sp4C;
    struct ObjectStruct* sp48 = &gObjects[objId]; // sp48
    Gfx *sp44;
    Gfx *sp40;
    u8 sp3F;
    s32 sp38;
    s16 sp36;

    x = sp48->Pos.x;
    y = sp48->Pos.y;
    sp5C = sp48->unkD4;
    sp58 = sp48->unkD8;
    scale_x = sp48->Scale.x;
    scale_y = sp48->Scale.y;

    if (scale_x < D_80103D00) {
        scale_x = D_80103D08;
    }
    if (scale_y < D_80103D10) {
        scale_y = D_80103D18;
    }

    scale_x = 1024.0f / (1024.0f / scale_x);
    scale_y = 1024.0f / (1024.0f / scale_y);
    x -= sp5C * scale_x;
    y -= sp58 * scale_y;
    sp38 = gObjects[objId].Unk148[0];
    sp4C = (void*)D_8016C298[sp38].unk0;
    sp3F = D_8016C298[sp38].unk5;
    fileId = sp4C->unk14;
    sp60 = gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]];

    if (0) {
        recomp_printf("[func_800623E0] file ID 0x%08X %d will be drawn\n", fileId, fileId);
        recomp_printf("[func_800623E0] file ID ptr     0x%08X\n", (u32)&gFileArray[fileId].ptr);
        recomp_printf("[func_800623E0] file ID rom off 0x%08X\n", *(u32*)&gFileArray[fileId].pad4[0]);
        recomp_printf("[func_800623E0] file ID arr off 0x%08X\n", sp4C->unkC[sp4C->unk4[sp3F]]);
    }

    if (sp60 == 4) {
        sp36 = 2;
        sp44 = (Gfx *)(&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]] + 0x10);
        sp40 = 0;
    } else {
        sp60 = (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[4]) | (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[5] << 8);
        if (sp60 >= 0x12) {
            sp36 = 1;
        } else {
            sp36 = 0;
        }
        sp44 = (Gfx *)(&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]] + (((sp60 * 2) + 7) & ~7) + 0x10);
        sp40 = (Gfx *)(&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]] + 0x10);
    }

    width = (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[8]) | (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[9] << 8);
    height = (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[0xC]) | (((u8*)&gFileArray[fileId].ptr[sp4C->unkC[sp4C->unk4[sp3F]]])[0xD] << 8);

    if (0 && fileId == 2) {
        recomp_printf("File id 2 has x and y of %.6f and %.6f\n", x, y);
    }

    s32 rom_offset = *(u32*)&gFileArray[fileId].pad4[0];
    s32 arr_offset = sp4C->unkC[sp4C->unk4[sp3F]];
    s32 id = isRomOffsetTagged(rom_offset, arr_offset);

    if (id != -1) {
        // we have a coordinate override detected, so update the x/y with the new one.
        if (gEntries[id].update.use && x == gEntries[id].update.coords[0][0] && y == gEntries[id].update.coords[0][1]) {
            x = gEntries[id].update.coords[1][0];
            y = gEntries[id].update.coords[1][1];
        }
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_STRETCH);
        func_80060F00(x, y, width, height, scale_x, scale_y, sp44, sp40, sp36);
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_AUTO);
    } else if (fileId == 1 && rom_offset == 0 && arr_offset == 0) {
        // this is the boot logo. suspend auto
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_ADJUST);
        func_80060F00(x, y, width, height, scale_x, scale_y, sp44, sp40, sp36);
        gEXSetRectAspect(gMasterDisplayList++, G_EX_ASPECT_AUTO);
    } else {
        func_80060F00(x, y, width, height, scale_x, scale_y, sp44, sp40, sp36);
    }
}
