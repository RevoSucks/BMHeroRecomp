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
    osInvalICache(addr, size);
    osInvalDCache(addr, size);

    // detect overlays and map them.
    switch((u32)start) {
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

extern u8 gControllerBits;
extern u16 gContPlugged[4];
extern u16 gContCurrButton[4];
extern u16 gContLastButton[4];
extern u16 gContButtonPressed[4];
extern s8 gContStickX[4];
extern s8 gContStickY[4];
extern u16 gContCurrDir[4];
extern u16 gContLastDir[4];
extern u16 gContDirPressed[4];
extern u16 gContRawPlugged[4];
extern u16 gContRawCurrButton[4];
extern u16 gContRawLastButton[4];
extern u16 gContRawButtonPressed[4];
extern s8 gContRawStickX[4];
extern s8 gContRawStickY[4];
extern u16 gContRawCurrDir[4];
extern u16 gContRawLastDir[4];
extern u16 gContRawDirPressed[4];

extern void func_8001DC78(void);
/**
 * Setup the mesg queue for the controllers and init them.
 */
RECOMP_PATCH void InitControllers(void) {
    s32 status;
    u16 i;

    gControllerBits = 0;
    osCreateMesgQueue(&gContMesgQueue, &D_801776CC, 1);
    osSetEventMesg(5U, &gContMesgQueue, (void*) 1);
    status = osContInit(&gContMesgQueue, &gControllerBits, &sContStatus);
    recomp_printf("[InitControllers] bits is 0x%02X\n", gControllerBits);
    if (status != 0) {
        // presumedly for handling if the controller had an error, but there's nothing here.
        // missing assert?
    }
    for (i = 0; i < MAXCONTROLLERS; i++) {
        gContRawPlugged[i] = 0;
        gContRawLastButton[i] = 0;
        gContRawCurrButton[i] = 0;
        gContRawButtonPressed[i] = 0;
        gContRawStickX[i] = 0;
        gContRawStickY[i] = 0;
        gContRawCurrDir[i] = 0;
        gContRawLastDir[i] = 0;
        gContRawDirPressed[i] = 0;
        gContLastButton[i] = 0;
        gContCurrButton[i] = 0;
        gContButtonPressed[i] = 0;
        gContStickX[i] = 0;
        gContStickY[i] = 0;
        gContCurrDir[i] = 0;
        gContLastDir[i] = 0;
        gContDirPressed[i] = 0;
    }
    UpdateActiveController(0);
    func_8001DC78();
}

/**
 * Perform the reading of the raw arrays front the OSContPad data itself. This will be later read
 * into the buffers to be used during gameplay.
 */
RECOMP_PATCH void UpdateRawControllers(void) {
    OSContPad* pad;
    u16 dir;
    u16 i;
    s32 status;

    status = 1;
    if (gControllerBits & 1) {
        status = osContStartReadData(&gContMesgQueue);
        if (status == 0) {
            osRecvMesg(&gContMesgQueue, NULL, 1);
            osContGetReadData(gContPads);
        }
    }
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((gControllerBits >> i) & 1) {
            pad = &gContPads[i];
            // if any errors occurred or if no controllers are plugged in, treat the controller as if its not
            // plugged in.
            if ((pad->errno != 0) || (status != 0)) {
                gContRawPlugged[i] = 0;
                ;
            } else {
                gContRawPlugged[i] = 1;
                dir = 0;
                gContRawStickX[i] = pad->stick_x;
                gContRawStickY[i] = pad->stick_y;
                if (gContRawStickX[i] >= 0x32) {
                    dir |= CONT_RIGHT;
                } else if (gContRawStickX[i] < -0x31) {
                    dir |= CONT_LEFT;
                }
                if (gContRawStickY[i] >= 0x32) {
                    dir |= CONT_UP;
                } else if (gContRawStickY[i] < -0x31) {
                    dir |= CONT_DOWN;
                }
                if (D_8016525C != 0) {
                    gContRawCurrButton[i] = 0;
                    gContRawCurrDir[i] = 0;
                } else if (D_80165284 == 1) {
                    gContRawLastButton[i] = gContRawCurrButton[i];
                    gContRawCurrButton[i] = pad->button;
                    gContRawLastDir[i] = gContRawCurrDir[i];
                    gContRawCurrDir[i] = dir;
                } else {
                    gContRawCurrButton[i] |= pad->button;
                    gContRawCurrDir[i] |= dir;
                }
            }
        }
    }
}
