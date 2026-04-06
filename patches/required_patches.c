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

RECOMP_PATCH void main_game(void* arg) {
    int i;
    u8 padding[0x44];
    u8* ptr;

    // clears code segment in RAM. Whats the need to do this, when a soft reset does this anyway?
    /*
    for (ptr = (u8*) game_VRAM; (u32) ptr < (u32) 0x80400000; ptr++) {
        *ptr = 0;
    }
    if (D_8004A280.unk0 != 0) {
        osTvType = 0;
    }
    */
    osInitialize();
    /*
    for (ptr = (u8*) 0x80200000; (u32) ptr < (u32) 0x80225800; ptr++) {
        *ptr = 0;
    }
    for (ptr = (u8*) 0x80225800; (u32) ptr < (u32) 0x8024B000; ptr++) {
        *ptr = 0;
    }
    for (i = 0; i < 0x200U; i++) {
        D_80050D38[i] = 0;
        gIdleThreadStack[i] = 0;
    }
    */

    D_8016524C = 0;

    //Parse_Args(NULL);
    osCreateThread(&gIdleThread, 1, &thread1_idle, arg, (u8*) &gIdleThreadStack + 0x1000, 10);
    osStartThread(&gIdleThread);
}
