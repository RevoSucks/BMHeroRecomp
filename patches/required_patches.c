#include "patches.h"

#include <ultra64.h>

#include "misc_funcs.h"
#include "PR/os_pi.h"
#include "PR/os_message.h"
#include "PR/os_cont.h"
#include "ultramodern/extensions.h"
#include "code/71AA0.h"

unsigned long long dummy = 0x0123456789ABCDEFULL;

RECOMP_PATCH int GetSi_Status(void) {
    return 0;
}

// ---------------------------------------
// patch rom loader to load overlays
// ---------------------------------------

extern OSMesg D_8004D728;

/**
 * Load a given ROM area to a specific virtual address.
 */
RECOMP_PATCH void load_from_rom_to_addr(void* start, void* addr, s32 size) {
    OSIoMesg ioMesg;
    OSMesg dummy;
    u8 padding[4];
    s32 size_loc;
    s32 size_amount;
    u8* start_loc;
    u8* addr_loc;

    recomp_printf("[load_from_rom_to_addr] start 0x%08X addr 0x%08X size 0x%08X\n", (u32)start, (u32)addr, size);

    osWritebackDCache(addr, size);
    osInvalICache(addr, size);
    osInvalDCache(addr, size);
    osPiStartDma(&ioMesg, 0, 0, (u32)start, addr, size, &D_8004D728);
    osRecvMesg(&D_8004D728, &dummy, 1);
    /*
    start_loc = start;
    addr_loc = addr;
    size_loc = size;
    if (size_loc != 0) {
        do {
            // load in 0x4000 chunks.
            if (size_loc <= 0x4000) {
                size_amount = size_loc;
            } else {
                size_amount = 0x4000;
            }
            osPiStartDma(&ioMesg, 0, 0, (u32)start_loc, addr_loc, size_amount, &D_8004D728);
            osRecvMesg(&D_8004D728, &dummy, 1);
            size_loc -= size_amount;
            start_loc += size_amount;
            addr_loc += size_amount;
        } while (size_loc != 0);
    }
    */
    osInvalICache(addr, size);
    osInvalDCache(addr, size);

    // detect overlays and map them.
    switch((u32)start) {
        case 0x04DFF0: recomp_load_overlays((u32)start, addr, size); break; // main area
        case 0x128D20: recomp_load_overlays((u32)start, addr, size); break; // code_extra_0
        case 0x134440: recomp_load_overlays((u32)start, addr, size); break; // code_extra_1
        case 0x138360: recomp_load_overlays((u32)start, addr, size); break; // code_extra_2
        case 0x13AC20: recomp_load_overlays((u32)start, addr, size); break; // code_extra_3
        case 0x13C4C0: recomp_load_overlays((u32)start, addr, size); break; // code_extra_4
        case 0x13DAD0: recomp_load_overlays((u32)start, addr, size); break; // code_extra_5
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // code_extra_6
        // unk_bin
        case 0x147BB0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_1
        case 0x14C540: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_2
        case 0x1528A0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_3
        case 0x153B70: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_4
        case 0x156F90: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_5
        case 0x157520: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_6
        case 0x157A00: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_7
        case 0x15A0F0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_8
        case 0x15A7D0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_9
        case 0x15C0D0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_10
        case 0x160560: recomp_load_overlays((u32)start, addr, size); break; // ovl_endol
        case 0x167950: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_12
        case 0x16D650: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_13
        case 0x175420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_14
        case 0x184EE0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_15
        case 0x189940: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_16
        case 0x194F10: recomp_load_overlays((u32)start, addr, size); break; // ovl_bagular_1
        case 0x198140: recomp_load_overlays((u32)start, addr, size); break; // ovl_bagular_2
        case 0x19C610: recomp_load_overlays((u32)start, addr, size); break; // ovl_bagular_3
        case 0x1A89C0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_20
        case 0x1ABC60: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_21
        case 0x1AECD0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_22
        case 0x1B3930: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_23
        case 0x1B6C30: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_24
        case 0x1BC4E0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_25
        // not code
        /*
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_26
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_27
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_28
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_29
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_30
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_31
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_32
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_33
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_34
        */
        case 0x1CCCE0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_35
        case 0x1D01D0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_36
        case 0x1D1720: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_37
        case 0x1D9110: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_38
        case 0x1E3FF0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_39
        case 0x1E7490: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_40
        case 0x1ED860: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_41
        case 0x1EF7F0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_42
        case 0x1F0900: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_43
        case 0x1F6790: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_44
        case 0x1F7E80: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_45
        case 0x1F8D70: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_46
        case 0x1FACD0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_47
        case 0x1FE7A0: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_48
        case 0x201C00: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_49
        case 0x205F30: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_50
        case 0x20DA60: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_51
        // Not code
        /*
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_30
        case 0x144420: recomp_load_overlays((u32)start, addr, size); break; // unk_bin_30
        */
    }
}

extern u64 D_80050D38[];
extern OSThread gIdleThread;
extern u64 gIdleThreadStack[];

extern void thread1_idle(void *arg);

extern void Parse_Args(void *);

extern void func_8001D4D0(void);
extern void Skybox_ProcessDraw(void);
extern void func_80087C58(void);
extern void func_8007E678(void);
extern void func_8001C464(void);
extern void func_8001C70C(void);
extern void func_8001C96C(void);
extern void func_8006E7CC(void);
extern void func_80087D70(void);
extern void func_8001C5B8(void);
extern void func_8007F3F0(void);
extern void func_800657E8(void);
extern void func_800818CC(void);
extern void func_80077528(void);
extern void Cutscene_HandlePrintText(void);
extern void func_80087C58(void);
extern void func_80087D70(void);
extern void func_80070B1C(void);
extern void func_80071240(void);
extern void func_8007070C(void);
extern void func_8006F780(void);
extern void func_80064120(void);
extern void func_800FF7B4(void);

extern void yield_self_1ms(void);
extern void func_8008B030(void);

RECOMP_PATCH void func_800821E0(void) {
    u16 sp3E;
    s32 temp_t8;
    Gfx* sp34;
    Gfx* sp30;

    func_8001D4D0();
    if (D_80177A20 < 2) {
        if (gSkyboxID == 0) {
            Debug_SetBg(1, (s32) D_80177932, (s32) D_80177934, (s32) D_80177938);
        } else {
            Debug_SetBg(0, 0, 0, 0);
            Skybox_ProcessDraw();
        }
    } else {
        Debug_SetBg(1, 0, 0, 0);
    }
    guPerspective(D_8016E104->unk00, &sp3E, 50.0f, 1.3333334f, 100.0f, D_801779C8.raw, 1.0f);
    sp34 = gMasterDisplayList++;
    sp34->words.w0 = 0xBC00000E;
    sp34->words.w1 = (u32) sp3E;

    // if any of these will cause a NaN, just pass temporary values in.
    if ( (gView.eye.x == 0.0f && gView.eye.y == 0.0f && gView.eye.z == 0.0f) ||
         (gView.at.x  == 0.0f && gView.at.y  == 0.0f && gView.at.z  == 0.0f) ||
         (gView.up.x  == 0.0f && gView.up.y  == 0.0f && gView.up.z  == 0.0f)) {
            gView.at.x = 0.0f;
            gView.at.y = 0.0f;
            gView.at.z = 0.0f;
            gView.eye.x = 0.0f;
            gView.eye.y = 0.0f;
            gView.eye.z = 1000.0f;
            gView.rot.x = 0.0f;
            gView.rot.y = 0.0f;
            gView.rot.z = 0.0f;
            gView.up.x = 0.0f;
            gView.up.y = 1.0f;
            gView.up.z = 0.0f;
            gView.dist = 1000.0f;
        }

    guLookAt(&D_8016E104->unk00[2], gView.eye.x, gView.eye.y, gView.eye.z, gView.at.x, gView.at.y, gView.at.z,
             gView.up.x, gView.up.y, gView.up.z);
    sp30 = gMasterDisplayList++;
    sp30->words.w0 = 0x01030040;
    sp30->words.w1 = (u32) D_8016E104;
    D_8016E3A4 = 0;
    if (D_80177A20 < 2) {
        func_80087C58();
        func_8007E678();
        func_8001C464();
        func_8001C70C();
        func_8001C96C();
        func_8006E7CC();
        func_80087D70();
        func_8001C5B8();
        func_8007F3F0();
        func_800657E8();
        func_800818CC();
        func_80077528();
        Cutscene_HandlePrintText();
    } else {
        func_80087C58();
        func_80087D70();
    }
    func_80070B1C();
    func_80071240();
    func_8007070C();
    func_8006F780();
    func_80064120();
    func_800FF7B4();
    if (gDebugShowTimerBar != 0) {
        Debug_DrawProfiler(0x2E, 0xD0);
    }
}

extern s32 D_80165254;
extern Gfx *gMasterDisplayList;
extern void func_8005F0F4(void);
extern void Create_GfxTask(void);
extern void func_8001D3CC(void);

static int frameCount = 0;

RECOMP_PATCH void func_8001D9E4(void* arg0) {
    func_8005F0F4();
    D_8016E10C = arg0;
    D_8016E104 = &D_8016E10C->unk68;
    gMasterDisplayList = &D_8016E10C->unk68.gfxWork;

    gEXEnable(gMasterDisplayList++);
    gEXSetRefreshRate(gMasterDisplayList, 60);

    gSPSegment(gMasterDisplayList++, 0x00, 0x00000000);
    gSPSegment(gMasterDisplayList++, 0x01, osVirtualToPhysical(gFileArray[0].ptr));
    
    gSPDisplayList(gMasterDisplayList++, D_1000C68);

    if (D_80165254 == 1) {
        gSPDisplayList(gMasterDisplayList++, D_1000B78);
    }

    gSPDisplayList(gMasterDisplayList++, D_1000C50);
    if (D_80165254 == 0) {
        if ((D_8016E0A8 != 0) && (gDebugRoutine1 != NULL)) {
            gDebugRoutine1();
        }
    } else if (D_80165254 == 2) {
        D_80165254 = 0;
    } else {
        D_80165254++;
    }
    func_8001D3CC();
    gDPFullSync(gMasterDisplayList++);
    gSPEndDisplayList(gMasterDisplayList++);

    //recomp_printf("Test word: 0x%08X 0x%08X\n", ((u32*)D_8016E10C->unk68.gfxWork)[4], ((u32*)D_8016E10C->unk68.gfxWork)[5]);

    frameCount++;

    osWritebackDCacheAll();
    Create_GfxTask();
    
}

RECOMP_PATCH void func_8001D000(s32 red, s32 green, s32 blue, s32 alpha) {
    gDPPipeSync(gMasterDisplayList++);
    gDPSetCycleType(gMasterDisplayList++, G_CYC_1CYCLE);
    gSPClearGeometryMode(gMasterDisplayList++, G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                                                   G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH);
    gSPSetGeometryMode(gMasterDisplayList++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gDPSetCombineMode(gMasterDisplayList++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(gMasterDisplayList++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
    gDPSetPrimColor(gMasterDisplayList++, 0, 0, red, green, blue, alpha);
    gDPFillRectangle(gMasterDisplayList++, 0, 0, 320, 240); // patch the rect coords
}

extern UNK_TYPE D_801DA800;

RECOMP_PATCH void func_8001D4D0(void) {
    gDPPipeSync(gMasterDisplayList++);
    gDPSetDepthImage(gMasterDisplayList++, osVirtualToPhysical(&D_801DA800));
    gDPSetCycleType(gMasterDisplayList++, G_CYC_FILL);
    gDPSetColorImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, osVirtualToPhysical(&D_801DA800));
    gDPSetFillColor(gMasterDisplayList++, 0xFFFCFFFC);
    gDPFillRectangle(gMasterDisplayList++, 0, 0, 320, 240);
}

// Debug_SetBackgroundColor
RECOMP_PATCH void Debug_SetBg(s32 setColor, char red, char green, char blue) {

    gDPPipeSync(gMasterDisplayList++);
    gDPSetColorImage(gMasterDisplayList++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, osVirtualToPhysical((void*)D_8016E10C->unk18168));

    if (setColor) {
        gDPSetFillColor(gMasterDisplayList++,
                        (GPACK_RGBA5551(red, green, blue, 1) << 16) | GPACK_RGBA5551(red, green, blue, 1));

        gDPFillRectangle(gMasterDisplayList++, 0, 0, 320, 240);
    }
    gDPPipeSync(gMasterDisplayList++);
    gDPSetCycleType(gMasterDisplayList++, G_CYC_1CYCLE);
}

// Patch the GFX tiler function

extern Gfx* gMasterDisplayList;

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

    if (0) {
        recomp_printf("[func_80060F00] x %.6f\n", x);
        recomp_printf("[func_80060F00] y %.6f\n", y);
        recomp_printf("[func_80060F00] width 0x%08X\n", width);
        recomp_printf("[func_80060F00] height 0x%08X\n", height);
    }

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

    // hack
    if (x >= 0.0f && x <= 8.0f) {
        x = 0.0f;
    }
    if (y >= 0.0f && y <= 8.0f) {
        y = 0.0f;
    }

    if (x != 0 && x < (2.0f * scale_x)) {
        spC4 = -(x - (2.0f * scale_x));
        spF4 = (spF4 + (s32)(spC4 / scale_x));
        width -= spF4;
        spC8 = (s32)(spC4 / (2.0f * scale_x));
        spC8 = (s32)(spC4 - (2.0f * scale_x * spC8));
        x = ((scale_x * 2.0f) - spC8);
        recomp_printf("x was less than (2.0f * scale_x). x became %.6f!\n", x);
    }
    if (y != 0 && y < scale_y) {
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

    if (0) {
        recomp_printf("ulx calculated: 0x%08X\n", ulx);
        recomp_printf("lrx calculated: 0x%08X\n", lrx);
    }
	
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

typedef struct UnkStruct_F280_s {
    UNK_TYPE unk0;
    Vec3f unk4;
    Vec3f unk10;
    Vec3f unk1C;
    struct UnkStruct_F280_s* unk28;
    s32 unk2C;
}UnkStruct_F280;

typedef struct  {
    s32 pad[2];
    struct UnkInputStruct8000EEE8_SPE4 unk8;
    struct UnkInputStruct8000EEE8_SPEC unkC;
}UnkStruct_F280_1;

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
} UnkStruct_8000F888;

extern Matrix D_800557E0;
extern s32 D_80055820;
extern Matrix D_80055828[20];
extern s32 BssPad_80055d28;
extern u32 D_80055D30[3];
extern u32 D_80055D40[3];
extern s32 D_80055D4C;

s32 tagged = 0;
s32 taggedID = 0;

s32 func_8000EEE8(Gfx** gfx, UnkStruct_F280_1* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_8000FC08(struct UnkInputStruct8000FC08* arg0, Gfx** arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 func_8000F888(UnkStruct_8000F888* arg0, Gfx** arg1, s32 arg2, UNUSED s32 arg3, UNUSED s32* unused, s32* arg5);

int bufferID = 0;

RECOMP_PATCH s32 func_8000FD9C(struct UnkInputStruct8000FC08* arg0, Gfx** arg1, UnkStruct_F280_1* arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    D_80055820 = 0;
    guMtxL2F(D_80055828[D_80055820], (Mtx*) &D_8016E104->unk00[1]);

    if (arg0->unk0 == 0) {
        arg6 = func_8000EEE8(arg1, arg2, arg3, arg4, arg5, arg0->unk28, arg6);
    } else if (arg0->unk0 == 1) {
        // objects (bomberman, platform, doors, etc)
        arg6 = func_8000FC08((void*)arg0->unk28, arg1, (s32)arg2, arg3, arg4, arg5, arg6);
    }

    return arg6;
}

extern void Math_Mat3f_Inverse(Matrix mf, Matrix mf1);
s32 func_8000E944(Gfx** gfx, UnkStruct_F280_1* arg1, s32 arg2, s32 arg3, s32 arg4, Gfx* arg5, s32 arg6, s32 arg7);

RECOMP_PATCH s32 func_8000EEE8(Gfx** gfx, UnkStruct_F280_1* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 id) {
    struct UnkInputStruct8000EEE8_SPEC* spEC;
    Gfx* dlist;
    struct UnkInputStruct8000EEE8_SPE4* spE4;
    Matrix spA4;
    s32 spA0;

    spA0 = 0;
    spE4 = &arg1->unk8;
    spEC = &arg1->unkC;
    dlist = *gfx;
    guMtxIdentF(spA4);

    gSPSegment(dlist++, spE4->unk0, OS_PHYSICAL_TO_K0(arg1));
    gSPSegment(dlist++, spE4->unk1, OS_PHYSICAL_TO_K0(arg2));
    gSPSegment(dlist++, spE4->unk2, OS_PHYSICAL_TO_K0(arg3));
    gSPSegment(dlist++, spE4->unk3, OS_PHYSICAL_TO_K0(arg4));
    gDPPipelineMode(dlist++, G_PM_1PRIMITIVE);

    if ((spEC[arg5].unk0 == 0) || (spEC[arg5].unk0 == 5) || (spEC[arg5].unk0 == 6) || (spEC[arg5].unk0 == 8)) {
        switch (spEC[arg5].unk0) { /* irregular */
            case 5:
                //recomp_printf("case 5\n");
                guMtxIdentF(D_800557E0);
                Math_Mat3f_Inverse(D_800557E0, D_80055828[D_80055820]);
                Math_Mat3f_Scale((float*)D_800557E0, D_8016E3B4, D_8016E3BC, D_8016E3C4);
                D_80055820 += 1;
                guMtxCatF(D_800557E0, D_80055828[D_80055820 - 1], D_80055828[D_80055820]);
                spA0 += 1;
                break;
            case 6:
                //recomp_printf("case 6\n");
                D_8004A390 = TRUE;
                gDPSetTextureLUT(dlist++, G_TT_NONE);
                gSPSetGeometryMode(dlist++, G_TEXTURE_GEN);
                gSPTexture(dlist++, 0x07C0, 0x07C0, 0, G_TX_RENDERTILE, G_ON);
                gDPSetTexturePersp(dlist++, G_TP_PERSP);
                gDPSetPrimColor(dlist++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(dlist++, 64, 64, 64, 255);
                gDPSetCombineMode(dlist++, G_CC_HILITERGBA, G_CC_HILITERGBA);
                gDPLoadTextureBlock(dlist++, D_1000768, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                    G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
                gDPSetHilite1Tile(dlist++, G_TX_RENDERTILE, &D_8016E104->hilites[0], 32, 32);
                break;
            case 8:
                //recomp_printf("case 6\n");
                D_8004A394 = TRUE;
                gSPSetGeometryMode(dlist++, G_TEXTURE_GEN);
                break;
            default:
                //recomp_printf("case default\n");
                if (D_8004A390) {
                    gSPClearGeometryMode(dlist++, G_TEXTURE_GEN);
                    gSPTexture(dlist++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
                    D_8004A390 = FALSE;
                } else if (D_8004A394) {
                    gSPClearGeometryMode(dlist++, G_TEXTURE_GEN);
                    D_8004A394 = FALSE;
                }
                break;
        }
        if (spEC[arg5].u.unk4_as_f != 1.0f) {
            //recomp_printf("scale\n");
            Math_Mat3f_Scale((float*)spA4, spEC[arg5].u.unk4_as_f, spEC[arg5].u.unk4_as_f, spEC[arg5].u.unk4_as_f);
            D_80055820 += 1;
            guMtxCatF(spA4, D_80055828[D_80055820 - 1], D_80055828[D_80055820]);
            spA0 += 1;
        }
        guMtxF2L(D_80055828[D_80055820], &D_8016E104->unkE0[id]);

#define gEXMatrixGroupDecomposedNormal(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO)

        if (tagged) {
            Mtx *m = &D_8016E104->unkE0[id];
            if (id != 0xFFFFFFFF)
                gEXMatrixGroupDecomposedNormal(dlist++, taggedID, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        }
        gSPMatrix(dlist++, osVirtualToPhysical(&D_8016E104->unkE0[id++]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(dlist++, (s32) (spEC[arg5].dlist) + (u8*)arg1);
        if (tagged) {
            if (id != 0xFFFFFFFF)
                gEXPopMatrixGroup(dlist++, G_MTX_MODELVIEW);
        }
        D_80055820 -= spA0;
    } else if (spEC[arg5].unk0 == 1) {
        id = func_8000E944(&dlist, arg1, arg2, arg3, arg4, spEC[arg5].dlist, spEC[arg5].u.unk4_as_s, id);
    }
    *gfx = dlist;
    return id;
}

RECOMP_PATCH s32 func_8000FC08(struct UnkInputStruct8000FC08* arg0, Gfx** arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 sp34;
    Gfx* sp30;
    s32 sp2C;

    sp30 = *arg1;
    arg6 = func_8000F888((void*) ((u32) arg0 + 0x14), &sp30, arg6, arg2, (void*)arg0->unk0, &sp2C);
    if (arg0->unk0 >= 0) {
        arg6 = func_8000EEE8((Gfx**) &sp30, (void*)arg2, arg3, arg4, arg5, (s32) arg0->unk0, arg6);
    }
    if (arg0->unk10 != 0) {
        for (sp34 = 0; sp34 < arg0->unk10; sp34++) {
            arg6 = func_8000FC08((void*)arg0->unkC[sp34], &sp30, arg2, arg3, arg4, arg5, arg6);
        }
    }
    if (sp2C != 0) {
        D_80055820 -= 1;
    }
    *arg1 = sp30;
    return arg6;
}

// .bss
extern f32 D_80177680;
extern f32 D_801776A8;
extern f32 D_801776C8;
extern f32 D_801776D0;
extern u8 D_801776D8;
extern u8 D_801776DC;
extern s8 D_801775EF;
extern u8 D_80177920;
extern s32 D_8017763C;
extern s32 D_80177644;
extern s32 D_8017764C;
extern s32 D_80177660;

extern void func_80072358(void);

RECOMP_PATCH void func_800723EC(void) {
    s32 sp24;
    s32 sp20;
    s32 sp1C;

    if (gCameraType < 2) {
        return;
    }
    sp1C = 0;
    if (D_8016523E == 0) {
        if ((gPlayerObject->Vel.x == 0.0f) && (gPlayerObject->Vel.y == 0.0f) && (gPlayerObject->Vel.z == 0.0f)) {
            sp1C = 1;
        }
    } else if (D_8016523E == 6) {
        if ((gPlayerObject->Vel.x == 0.0f) && (gPlayerObject->Vel.y == 0.0f) && (gPlayerObject->Vel.z == 0.0f)) {
            sp1C = 1;
        }
    } else if (D_8016523E == 4) {
        sp1C = 1;
    }
    sp24 = 0;
    sp20 = 0;
    if (sp1C != 0) {
        if (gActiveContButton & L_CBUTTONS) {
            sp24 = 1;
            if (D_80177680 < 30.0f) {
                D_80177680 += 2.0f;
            }
        } else if (gActiveContButton & R_CBUTTONS) {
            sp24 = 1;
            if (D_80177680 > -30.0f) {
                D_80177680 -= 2.0f;
            }
        }
        if (gActiveContButton & U_CBUTTONS) {
            sp20 = 1;
            if (D_801776A8 < 20.0f) {
                D_801776A8 += 1.0f;
            }
        }
    }
    if (sp24 == 0) {
        if (D_80177680 > 0.0f) {
            D_80177680 -= 2.0f;
        } else if (D_80177680 < 0.0f) {
            D_80177680 += 2.0f;
        }
    }
    if (sp20 == 0) {
        if (D_801776A8 > 0.0f) {
            D_801776A8 -= 1.0f;
        } else if (D_801776A8 < 0.0f) {
            D_801776A8 += 1.0f;
        }
    }
    if ((D_801776A8 != 0.0f) || (D_80177680 != 0.0f)) {
        func_80072358();
        gDebugDispType = gLevelInfo[gCurrentLevel]->unk5;
        gView.rot.x = Math_WrapAngle(gView.rot.x, D_801776A8);
        gView.rot.y = Math_WrapAngle(gView.rot.y, D_80177680);
    }
}

void Math_Mat3f_Inverse(Matrix, Matrix); /* extern */
extern void func_80019050(s32);

RECOMP_PATCH void func_80019510(s32 arg0, s32 arg1, s32 arg2) {
    Mtx spA8;
    Matrix sp68;
    Matrix sp28;

    if (arg2 != 0) {
        func_80019050(arg0);
    }
    guMtxL2F(sp68, &D_8016E104->unk00[2]);
    guMtxCatF(gObjects[arg0].unk64, sp68, sp68);
    guMtxF2L(sp68, &spA8);
    if (arg1 == 0) {
        //recomp_printf("Setting mtx of type 0\n");
        D_8016E104->unkE0[D_8016E3A4] = spA8;
        gSPMatrix(gMasterDisplayList++, &D_8016E104->unkE0[D_8016E3A4++], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    } else if (arg1 == 1) {
        //recomp_printf("Setting mtx of type 1\n");
        D_8016E3B4 = gObjects[arg0].Scale.x;
        D_8016E3BC = gObjects[arg0].Scale.y;
        D_8016E3C4 = gObjects[arg0].Scale.z;
        D_8016E104->unk00[1] = spA8;
    } else if (arg1 == 2) {
        
       // recomp_printf("Setting mtx of type 2\n");
        guMtxIdentF(sp28);
        Math_Mat3f_Inverse(sp28, sp68);
        Math_Mat3f_Scale(sp28, gObjects[arg0].Scale.x, gObjects[arg0].Scale.y, gObjects[arg0].Scale.z);
        guMtxCatF(sp28, sp68, sp68);
        guMtxF2L(sp68, &spA8);
        D_8016E104->unkE0[D_8016E3A4] = spA8;
        gSPMatrix(gMasterDisplayList++, &D_8016E104->unkE0[D_8016E3A4++], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    } else if (arg1 == 3) {
        //recomp_printf("Setting mtx of type 3\n");
        guMtxF2L(sp68, &spA8);
        D_8016E104->unkE0[D_8016E3A4] = spA8;
        gSPMatrix(gMasterDisplayList++, &D_8016E104->unkE0[D_8016E3A4++], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
}

extern void Check_PakState(void);

RECOMP_PATCH void func_80000964(void) {
    s32 sp34 = 0;
    u32 sp30 = 0;
    OSMesg sp2C = 0;
    s32 sp28;
    s16 temp_s0;

    // why did you do this twice...?
    sp34 = 0;
    sp30 = 0;
    sp2C = 0;

    D_80165254 = 1;
    D_8016525C = 1;
    D_80165284 = 0;
    D_80165264 = 0;
    sp28 = -1;
    if (D_8016524C == 1) {
        sp30 = 2;
        osViSetYScale(1.0f);
        osViBlack(TRUE);
        Check_PakState();
        func_8001ECA0();
    }
    while (sp28 != 0) {
        osRecvMesg(&D_8016E0B8, &sp2C, 1);
        switch (*(s16*) sp2C) {
            case 1:
                if (D_8016E0A0 != 0) {
                    func_8001E80C();
                }
                if ((sp30 < 2) && (D_8016E098 != 0)) {
                    bufferID = sp34;
                    func_8001D9E4(&D_80340000[sp34]);
                    sp30 += 1;
                    sp34 ^= 1;
                }
                if ((D_80165264 != 0) && (sp28 == -1)) {
                    sp28 = 0x10;
                    func_8001ECA0();
                    sp30 += 2;
                }
                if (sp28 > 0) {
                    sp28 -= 1;
                }
                break;
            case 2:
                sp30 -= 1;
                break;
            case 4:
                sp30 += 2;
                osViSetYScale(1.0f);
                osViBlack(TRUE);
                Check_PakState();
                func_8001ECA0();
                break;
        }
    }
    func_8001EE64();
}

// -----------------------------------------------
// Main game loop
// -----------------------------------------------

extern void func_80082E38(s32);
extern void func_80000DAC(void);
extern void func_803303A4_unk_bin_6(void);
extern void func_80000D4C(void);
extern void func_80330440_unk_bin_5(void);
extern void func_80000E6C(void);
extern s32 func_80330594_unk_bin_8(void);
extern s32 func_803316CC_unk_bin_9(s32);
extern s32 func_8033148C_unk_bin_10(void);
extern void func_8008279C(void);
extern void func_800829AC(void);
extern s32 func_80335DE4_unk_bin_2(void);
extern void func_80082CC4(void);
extern void func_80081C50(void);
extern void func_800824A8(void);
extern void func_80082BBC(void);
extern void func_80082678(void);
extern void func_800828A4(void);
extern void func_80082AB4(void);
extern void func_80082500(void);
extern void func_8033248C_unk_bin_7(s32, s8);
extern void func_80332BDC_unk_bin_10(void);
extern s32 func_80331154_unk_bin_3(void);
extern s32 func_80333164_unk_bin_4(void);

// used for hotfix to reverse bits of gControllerBits
static int patched = 0;

RECOMP_PATCH void func_80083180(s32 arg0) {
    s32 sp2C_pad;
    s8 pad2B;
    s8 pad2A;
    s8 pad29;
    s8 sp28;
    s8 sp27;
    s8 pad26;
    s8 pad25;
    s8 pad24;
    s32 sp20;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    if (arg0 != 0) {
        func_80082E38(arg0);
        if (arg0 == 0x64) {
            D_8016E3CC = 0;
            arg0 = 0;
            D_8016E3D4 = 0;
            D_80177630 = 0;
            goto loop_67;
        }
    }
    func_80016D74(0);

    // HOTFIX: the library commit used is currently broken, and for public build reasons, we
    // cannot at the moment update it for live builds. reverse the bits for the controller and
    // mark it as patched.
    if (patched == 0) {
        u8 contPort[4];
        u8 newVal = 0;

        if(0) recomp_printf("gControllerBits: 0x%02X\n", gControllerBits);
        for(int i = 0; i < 4; i++) {
            contPort[i] = gControllerBits & (1 << i);
        }

        // doing it manually cause whatever
        if (contPort[0]) newVal |= (1 << 3);
        if (contPort[1]) newVal |= (1 << 2);
        if (contPort[2]) newVal |= (1 << 1);
        if (contPort[3]) newVal |= (1 << 0);

        gControllerBits = newVal;
        if(0) recomp_printf("gControllerBits: 0x%02X\n", gControllerBits);
        patched = 1;
    }

    if (!(gControllerBits & 1)) {
        PlayTrack_WithVolLoop(-1, -1, 0);
        func_80000DAC();
        func_803303A4_unk_bin_6();
    }

block_5:
    PlayTrack_WithVolLoop(-1, -1, 0);
    func_80000D4C();
    func_80330440_unk_bin_5();

demo_init:
    Demo_Start(0);

block_7:
    func_80000E6C();

    if ((sp20 = func_80330594_unk_bin_8()) == 1) {
        goto demo_init;
    }

    if (gShowDebugMenu == 1) {
        gShowDebugMenu = 1;
    } else if (gShowDebugMenu == 2) {
        gShowDebugMenu = 1;
        if (arg0 == 0) {
            return;
        }
    }

    D_80134888 = 0;
    D_8013488C = 0;
block_15:
    PlayTrack_WithVolLoop(0x1C, -1, 1);
loop_16:
    func_80000CEC();
    sp20 = func_80333164_unk_bin_4();
    if (sp20 == 0) {
        goto CASE_0;
    } else if (sp20 == 1) {
        goto CASE_1;
    } else if (sp20 == 2) {
        D_8013488D = 0;
        goto loop_37;
    } else if (sp20 == 3) {
        goto CASE_3;
    } else {
        PlayTrack_WithVolLoop(-1, -1, 1);
        goto block_7;
    }
CASE_0:
    D_80177630 = 0;
    goto block_49;

CASE_1:
    sp20 = 0;

    while (TRUE) {
        func_80000ECC();
        sp20 = func_803316CC_unk_bin_9(sp20);
        if (sp20 != 6) {
            func_80000E0C();
            func_8033248C_unk_bin_7(sp20, 1);
        } else {
            break;
        }
    }

    goto loop_16;

CASE_3:
    func_8005FBD0();
    func_800880E4();
    D_8016E3D4 = 0;
    gCurrentLevel = MAP_BOMBER_STAR_HUB_CUTSCENE;
    D_80177630 = 4;
    goto block_49;

loop_37:
    func_80000F2C();
    sp20 = func_8033148C_unk_bin_10();

    if (sp20 == 0) {
        PlayTrack_WithVolLoop(-1, -1, 0);
        func_80000F2C();
        func_80332BDC_unk_bin_10();
        PlayTrack_WithVolLoop(0x1C, -1, 1);
        goto loop_37;
    } else if (sp20 == 1) {
        func_80088094();
        D_8016E3D4 = 0;
        gCurrentLevel = 0xAC;
        D_80177630 = 2;
        goto block_49;
    } else if (sp20 == 2) {
        func_800880E4();
        D_8016E3D4 = 0;
        gCurrentLevel = 0xB1;
        D_80177630 = 3;
        goto block_49;
    } else if (sp20 == 3) {
        D_80134801 = 0;
        D_80177630 = 1;
        goto block_49;
    }
    goto loop_16;
block_49:
    D_8016E3CC = 0;
    if (D_80177630 >= 2) {
        D_80177600 = 0;
        goto loop_133;
    }
block_51:
    func_800880E4();
    if (D_80134801 == -1) {
        func_80025D4C(D_8013488C);
        func_8002598C(D_8013488C);
        D_80134800 = 0;
        D_80134803 = 0;
        D_80134802 = 0;
        D_80134801 = 0;
        func_8006A2BC();
        func_800250A0((s32) D_8013488C);
        func_8002536C(D_8013488C, 0, 0, 0, 2);
        func_80024EF4(D_8013488C);
        D_80134801 = 0;
        D_80134802 = 0;
        D_80134803 = 0;
        D_80134800 = 0;
        func_8008279C();
    } else {
        func_80025978(D_8013488C);
        func_800252AC(D_8013488C, &D_80134801, &D_80134802, &D_80134803, &D_80134800);
    }
block_54:
    func_800251D4(D_8013488C);
    if (D_80177630 == 1) {
        D_80134801 = 0;
        D_80134802 = 0;
        D_80134803 = 0;
        D_80134800 = 0;
        func_800829AC();
    }

block_56:
    PlayTrack_WithVolLoop(0x1A, -1, 1);
    func_80000C2C();
    sp20 = func_80335DE4_unk_bin_2();
    if (sp20 != 0) {
        if (D_80177630 == 0) {
            goto block_15;
        } else {
            goto loop_37;
        }
    }

    if (func_800600B8(0, D_80134801) != 0 || func_800600B8(1, D_80134802) != 0 || func_8006A054() != 0) {
        D_8016E3D4 = 1;
    } else {
        D_8016E3D4 = 0;
    }

    func_80069F0C(D_8016E3D4);
    func_80082CC4();
loop_67:
    func_80081C50();
loop_68:
    sp28 = func_800253EC(D_8013488C) & 2;
    sp27 = func_80025764(D_8013488C);
    func_80025608(D_8013488C);
    func_800258A0(D_8013488C);
    func_800882C8();
    D_80177628 = 0;
    func_800824A8();
    if (D_8016E3CC == -1) {
        goto end;
    } else if (D_8016E3CC == 4) {
        func_80025674(D_8013488C);
        func_8002590C(D_8013488C);
        func_80088338();
        func_8002598C(D_8013488C);
        func_80024EF4(D_8013488C);
        goto block_56;
    } else if (D_8016E3CC == 1) {
        func_80025674(D_8013488C);
        func_8002590C(D_8013488C);

        if (gCurrentLevel == MAP_BOMBER_BASE_ENTRANCE || gCurrentLevel == MAP_AIR_ROOM_SELECT || gCurrentLevel == MAP_ZERO_G_ENTRANCE || gCurrentLevel == MAP_MIRROR_ROOM_ENTRANCE ||
            gCurrentLevel == MAP_NATIA_1_ENTRANCE) {
        } else {
            func_80069FD8();
            func_80069F0C(1);
        }

        func_80088248(-1);
        if (gLifeCount == 0) {
            func_80000C8C();
            if (func_80331154_unk_bin_3() == 0) {
                func_80088134();
                func_8002598C(D_8013488C);
                func_80024EF4(D_8013488C);
                goto block_56;
            } else {
                func_80088184();
                func_8002598C(D_8013488C);
                func_80024EF4(D_8013488C);
                if (D_80177630 == 0) {
                    PlayTrack_WithVolLoop(-1, -1, 1);
                    goto block_7;
                } else {
                    goto loop_37;
                }
            }
        } else {
            func_800881D4();
            func_8002598C(D_8013488C);
            func_80024EF4(D_8013488C);
            goto loop_67;
        }
    } else if (D_8016E3CC == 2 || D_8016E3CC == 3) {
        func_80024EF4(D_8013488C);

        if (D_8016E3CC == 3 || (D_8016E3D4 != 0 && D_80177620 != 0)) {
            if (gCurrentLevel == 0x9E) {
                gHealthCount = gMaxHealth;
                gBombCount = 3;
                gFireCount = 3;
                func_8002598C(D_8013488C);
                func_80024EF4(D_8013488C);
            }

            func_80069FD8();
            if (func_800600B8(0, D_80134801) != 0) {
                func_80000E0C();
                if (D_8016E3D4 == 0) {
                    func_8033248C_unk_bin_7((s32) D_80134801, 0);
                } else {
                    func_8033248C_unk_bin_7((s32) D_80134801, 1);
                }
            }

            if (D_80177630 == 0) {
                if (gCurrentLevel == MAP_CUTSCENE_BAGULAR_3_WIN) {

                    if (D_8016E3D4 == 0 || !(func_800253EC(D_8013488C) & 2)) {
                        Demo_Start(1);
                        func_80082BBC();
                    }

                    if (D_8016E3D4 == 0 && (func_800253EC(D_8013488C) & 2)) {
                        func_80082678();
                    }

                    D_80134803 = 0;
                    D_80134802 = 0;
                    D_80134801 = 0;
                    D_80134800 = 0;
                    goto block_5;
                } else if (gCurrentLevel == 0x55) {
                    func_800828A4();
                    D_80134803 = 0;
                    D_80134802 = 0;
                    D_80134801 = 0;
                    D_80134800 = 0;
                    goto block_5;
                }
            }

            if (((D_80177630 == 0) && func_800600B8(0, 4) && sp28 == 0 && (func_800253EC(D_8013488C) & 2))) {
                func_80082678();
            }

            if ((D_80177630 == 1 && sp27 == 0 && func_80025764(D_8013488C) != 0)) {
                func_80082AB4();
                goto loop_37;
            }

            if (!(D_8016E3D4 != 0 || D_80134801 >= 4 || func_800600B8(0, D_80134801) == 0)) {
                func_80082500();
                func_8002598C(D_8013488C);
                func_80024EF4(D_8013488C);
            }

            func_800252AC(D_8013488C, &D_80134801, &D_80134802, &D_80134803, &D_80134800);
            func_8006A168();
            func_80069FD8();
            goto block_56;
        } else {
            goto loop_68;
        }
    }

    goto block_7;
loop_133:
    func_80081C50();
loop_134:
    D_80177628 = 0;
    func_800824A8();
    if (D_8016E3CC == -1) {
        goto end;
    } else if (D_8016E3CC == 4) {
        if (D_80177630 == 2) {
            goto loop_37;
        } else if (D_80177630 == 3) {
            goto loop_37;
        } else {
            goto block_15;
        }
    } else if (D_8016E3CC == 1) {
        func_80069FD8();
        func_80069F0C(1);
        func_80088248(-1);
        if (gLifeCount == 0) {
            func_80000C8C();
            if (func_80331154_unk_bin_3() == 0) {
                if (D_80177630 == 2) {
                    func_80088094();
                } else {
                    func_80088134();
                }
                goto loop_133;
            } else if (D_80177630 == 2) {
                goto loop_37;
            } else if (D_80177630 == 3) {
                goto loop_37;
            } else {
                goto block_15;
            }
        } else {
            func_800881D4();
            goto loop_133;
        }
    } else if (D_8016E3CC == 2 || D_8016E3CC == 3) {

        if (D_80177630 == 2) {
            if (D_8016E432 == 0xAD) {
                if (D_80177640 == 1) {
                    D_8016E432 = 0xAD;
                } else if (D_80177640 == 2) {
                    D_8016E432 = 0xAE;
                } else if (D_80177640 == 3) {
                    D_8016E432 = 0xAE;
                }
            } else if (D_8016E432 == 0x7F) {
                goto loop_37;
            }
        } else if (D_80177630 == 3) {
            if (D_8016E432 == 0x7F) {
                goto loop_37;
            }
        } else if (D_8016E432 == 0x7F) {
            goto block_15;
        }
        goto loop_134;
    }
end:;
}

// -------------------------------------
// Create Model Tag
// -------------------------------------

extern s32 func_8001A988(void);
extern void func_8001A2A0(void);
extern void *func_800122F0(s32);
extern void func_8001A300(s32);

struct ModelTag* func_80010408(struct UnkInputStruct80010408* arg0, u32 arg1);

RECOMP_PATCH void func_8001BD44(s32 objId, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp1C;

    if (gObjects[objId].Unk140[arg1] != -1) {
        return;
    }
    sp1C = func_8001A988();
    gObjects[objId].Unk140[arg1] = sp1C;
    D_80165290[sp1C].unk0 = arg3;
    func_8001A2A0();
    D_8016E3AC = func_800122F0(arg3);
    if (0) {
        if (objId >= 0xE && objId < 0x4E) {
            recomp_printf("[func_8001BD44] creating object with objID 0x%08X (%d)\n", objId, objId);
        } else if (objId >= 0x4E && objId <= 0x8E) {
            recomp_printf("[func_8001BD44] creating geometry with objID 0x%08X (%d)\n", objId, objId);
        } else {
            recomp_printf("[func_8001BD44] unknown creation with objID 0x%08X (%d)\n", objId, objId);
        }
    }
    
    D_80165290[sp1C].modelTag = func_80010408((void*)arg3, arg2);
    func_8001A300(sp1C);
}

extern void *malloc_game(s32 size);
extern void func_8000FEB0(void *verts);
extern void *func_800100E8(void *, s32);

RECOMP_EXPORT void breakpoint_me(int blah) {
    for(int i = 0; i < blah; i++) {
        // ;
    }
}

// create Model Tag struct
RECOMP_PATCH struct ModelTag* func_80010408(struct UnkInputStruct80010408* arg0, u32 arg1) {
    struct ModelTag* modelTag;
    struct UnkInputStruct80010408_Inner* sp28;
    s32 i;

    sp28 = arg0->unkC;
    if (arg0->unk4 <= arg1) {
        return NULL;
    }

    modelTag = malloc_game(sizeof(struct ModelTag));
    func_8000FEB0(&modelTag->verts);
    switch (sp28[arg1].unk0) { /* irregular */
        case 0:
        case 5:
        case 6:
            modelTag->type = 0;
            modelTag->data.dl = (void*) arg1;
            breakpoint_me(100);
            break;
        case 1:
            
            D_80055D4C = 0;
            modelTag->type = 1;
            modelTag->data.dl = func_800100E8(NULL, sp28[arg1].unk4);
            break;
        default:
            break;
    }

    for (i = 0; i < 3; i++) {
        D_80055D30[i] = D_8016E3AC[i + 3];
    }

    for (i = 0; i < 3; i++) {
        D_80055D40[i] = D_8016E3AC[i + 6];
    }

    return modelTag;
}

extern void func_800183E8(s32 r, s32 g, s32 b, s32 a, s32 arg4, s32 arg5);
extern void func_80018794(s32 r, s32 g, s32 b, s32 a, s32 arg4, s32 arg5);

// iterate over the geometry chunks
RECOMP_PATCH void func_8001C70C(void) {
    struct ObjectStruct* sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;

    for (sp3C = &gObjects[0x4E], sp38 = 0x4E; sp38 < 0x8E; sp3C++, sp38++) {
        if (sp3C->actionState != 0) {
            if ((char) gObjects[sp38].unk139 != 0) {
                func_80019510(sp38, 1, 0);
            } else {
                func_80019510(sp38, 1, 1);
            }
            for (sp2C = 0; sp2C < 2; sp2C++) {
                if (!((u8) sp3C->unk130 & 1) && ((sp34 = sp3C->Unk140[sp2C]) != -1)) {
                    if (D_8017792C == 0) {
                        func_8001838C();
                    } else if (D_8017792C == 1) {
                        func_800183E8((s32) D_8017793A, (s32) D_8017793E, (s32) D_80177940, 0xFF, (s32) D_80177944,
                                      (s32) D_80177948);
                    } else {
                        func_80018794((s32) D_8017793A, (s32) D_8017793E, (s32) D_80177940, 0xFF, (s32) D_80177944,
                                      (s32) D_80177948);
                    }
                    func_8001B014(sp38, sp2C);
                    func_8001A488(sp34);
                    sp30 = D_80165290[sp34].unk0;
                    tagged = 1;
                    recomp_printf("[func_8001C70C] tagging geometry with 0x%08X\n", sp38);
                    taggedID = sp38;
                    D_8016E3A4 =
                        func_8000FD9C(D_80165290[sp34].modelTag, &gMasterDisplayList, (void*)sp30, sp30, sp30, sp30, D_8016E3A4);
                    tagged = 0;
                }
            }
        }
    }
}

// some other geometry iterator. This will also need patching.
RECOMP_PATCH void func_8001C96C(void) {
    struct ObjectStruct* sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;

    func_8001838C();
    for (sp34 = &gObjects[0x4E], sp30 = 0x4E; sp30 < 0x8E; sp34++, sp30++) {
        if ((sp34->actionState != 0)) {
            if (!((u8) sp34->unk130 & 1) && ((sp2C = (s32) sp34->Unk140[3]) != -1)) {
                func_80019510(sp30, 1, 0);
                func_8001B014(sp30, 3);
                func_8001A488(sp2C);
                sp28 = D_80165290[sp2C].unk0;
                tagged = 1;
                recomp_printf("[func_8001C96C] tagging geometry with 0x%08X\n", sp30);
                taggedID = sp30;
                D_8016E3A4 =
                    func_8000FD9C(D_80165290[sp2C].modelTag, &gMasterDisplayList, (void*)sp28, sp28, sp28, sp28, D_8016E3A4);
                tagged = 0;
            }
        }
    }
}

// objects
RECOMP_PATCH void func_8001C464(void) {
    struct ObjectStruct* sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;

    for (sp34 = &gObjects[0xE], sp30 = 14; sp30 < 0x4E; sp34++, sp30++) {
        if (sp34->actionState != 0) {
            func_80019510(sp30, 1, 1);
            if (!((u8) sp34->unk130 & 1) && !(sp34->unk131 & 2) && (((sp2C = sp34->Unk140[0]) != -1))) {
                func_8001838C();
                func_8001B014(sp30, 0);
                func_8001A488(sp2C);
                sp28 = D_80165290[sp2C].unk0;
                tagged = 1;
                recomp_printf("[func_8001C464] tagging object with 0x%08X\n", sp30);
                taggedID = sp30;
                D_8016E3A4 =
                    func_8000FD9C(D_80165290[sp2C].modelTag, &gMasterDisplayList, (void*)sp28, sp28, sp28, sp28, D_8016E3A4);
                tagged = 0;
            }
        }
    }
}

RECOMP_PATCH void func_8001C5B8(void) {
    struct ObjectStruct* sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;

    for (sp34 = &gObjects[0xE], sp30 = 14; sp30 < 0x4E; sp34++, sp30++) {
        if (sp34->actionState != 0) {
            if (!((u8) sp34->unk130 & 1) && !(sp34->unk131 & 2) && (((sp2C = sp34->Unk140[3]) != -1))) {
                func_80019510(sp30, 1, 0);
                func_8001838C();
                func_8001B014(sp30, 3);
                func_8001A488(sp2C);
                sp28 = D_80165290[sp2C].unk0;
                tagged = 1;
                recomp_printf("[func_8001C5B8] tagging object with 0x%08X\n", sp30);
                taggedID = sp30;
                D_8016E3A4 =
                    func_8000FD9C(D_80165290[sp2C].modelTag, &gMasterDisplayList, (void*)sp28, sp28, sp28, sp28, D_8016E3A4);
                tagged = 0;
            }
        }
    }
}

// manual processing of a specific object ID
RECOMP_PATCH void func_8001C384(s32 objID, s32 arg1) {
    s32 sp2C;
    s32 sp28;

    sp2C = (s32) gObjects[objID].Unk140[arg1];
    func_8001A488(sp2C);
    sp28 = D_80165290[sp2C].unk0;
    tagged = 1;
    recomp_printf("[func_8001C384] tagging misc with 0x%08X\n", objID);
    taggedID = objID;
    D_8016E3A4 = func_8000FD9C(D_80165290[sp2C].modelTag, &gMasterDisplayList, (void*)sp28, sp28, sp28, sp28, D_8016E3A4);
    tagged = 0;
}
