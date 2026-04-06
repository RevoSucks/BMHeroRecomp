#include "patches.h"
#include "misc_funcs.h"
#include "PR/os_pi.h"
#include "PR/os_message.h"
#include "PR/os_cont.h"

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

RECOMP_PATCH void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye,
	       float xAt,  float yAt,  float zAt,
	       float xUp,  float yUp,  float zUp)
{
	float	len, xLook, yLook, zLook, xRight, yRight, zRight;

	guMtxIdentF(mf);

	xLook = xAt - xEye;
	yLook = yAt - yEye;
	zLook = zAt - zEye;

	/* Negate because positive Z is behind us: */

	len = -1.0 / sqrtf (xLook*xLook + yLook*yLook + zLook*zLook);
	xLook *= len;
	yLook *= len;
	zLook *= len;

	/* Right = Up x Look */

    recomp_printf("xUp 0x%08X\n", *(u32*)&xUp);
    recomp_printf("yUp 0x%08X\n", *(u32*)&yUp);
    recomp_printf("zUp 0x%08X\n", *(u32*)&zUp);

    recomp_printf("xLook 0x%08X\n", *(u32*)&xLook);
    recomp_printf("yLook 0x%08X\n", *(u32*)&yLook);
    recomp_printf("zLook 0x%08X\n", *(u32*)&zLook);

	xRight = yUp * zLook - zUp * yLook;
	yRight = zUp * xLook - xUp * zLook;
	zRight = xUp * yLook - yUp * xLook;

    f32 check = xRight*xRight + yRight*yRight + zRight*zRight;
    f32 check_sqrtf = sqrtf (xRight*xRight + yRight*yRight + zRight*zRight);

    if (check_sqrtf == 0.0f) {
        // uh oh
        recomp_printf("NaN was calculated. Args that were passed in:\n");
        recomp_printf("0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\n",
        *(u32*)&xEye, *(u32*)&yEye, *(u32*)&zEye, *(u32*)&xAt, *(u32*)&yAt, *(u32*)&zAt, *(u32*)&xUp, *(u32*)&yUp, *(u32*)&zUp);
    }

	len = 1.0 / sqrtf (xRight*xRight + yRight*yRight + zRight*zRight);
	xRight *= len;
	yRight *= len;
	zRight *= len;

	/* Up = Look x Right */

	xUp = yLook * zRight - zLook * yRight;
	yUp = zLook * xRight - xLook * zRight;
	zUp = xLook * yRight - yLook * xRight;
	len = 1.0 / sqrtf (xUp*xUp + yUp*yUp + zUp*zUp);
	xUp *= len;
	yUp *= len;
	zUp *= len;

	mf[0][0] = xRight;
	mf[1][0] = yRight;
	mf[2][0] = zRight;
	mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);

	mf[0][1] = xUp;
	mf[1][1] = yUp;
	mf[2][1] = zUp;
	mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);

	mf[0][2] = xLook;
	mf[1][2] = yLook;
	mf[2][2] = zLook;
	mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);

	mf[0][3] = 0;
	mf[1][3] = 0;
	mf[2][3] = 0;
	mf[3][3] = 1;
}

RECOMP_PATCH void guLookAt (Mtx *m, float xEye, float yEye, float zEye,
	       float xAt,  float yAt,  float zAt,
	       float xUp,  float yUp,  float zUp)
{
	Matrix	mf;

	guLookAtF(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);

	guMtxF2L(mf, m);
}

extern void func_8001D4D0(void);
extern void func_8006D6F4(void);
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

RECOMP_PATCH void func_800821E0(void) {
    u16 sp3E;
    s32 temp_t8;
    Gfx* sp34;
    Gfx* sp30;

    func_8001D4D0();
    if (D_80177A20 < 2) {
        if (D_8017792E == 0) {
            Debug_SetBg(1, (s32) D_80177932, (s32) D_80177934, (s32) D_80177938);
        } else {
            Debug_SetBg(0, 0, 0, 0);
            func_8006D6F4();
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

    osWritebackDCacheAll();
    Create_GfxTask();
}
