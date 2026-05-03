#include "patches.h"

#include <ultra64.h>

extern void Demo_HandleSetAnimTextures(void);
extern void func_8001BD44(s32 objId, s32 arg1, s32 arg2, s32 arg3);
extern void Demo_SetView(void);
extern void Demo_PrepareStr(void);

extern int skipInterpolationOneFrame;

void func_8005DA00(void);

RECOMP_PATCH void func_8005DD80(void) {
    s32 sp24;

    // unset skip if it was set since this will be called the frame after.
    if (skipInterpolationOneFrame == 1) {
        skipInterpolationOneFrame = 0;
    }

    if ((gContButtonPressed[0] & 0x1000) && (func_8001D1D4() != -1) && (func_8001D1D4() != 1)) {
        D_801347E5 = 1;
        Set_BgColor(0xFF, 0xFF, 0xFF, 0x10);
        func_8001D2C0();
        func_80016FC8(0x20);
    }
    if ((func_8001D1D4() == 2) && (D_801347E5 != 0)) {
        D_80165264 = 1;
    }
    func_8005DA00();
    if (D_801347E6 == 0) {
        Demo_PrepareStr();
        if (D_80134790 == 0) {
            Demo_SetView();
            for (sp24 = 0; sp24 < 8; sp24++) {
                if (gObjects[sp24].actionState != 0) {
                    func_8001CD20(sp24);
                    func_8001AD6C(sp24);
                }
            }
            for (sp24 = 0; sp24 < 8; sp24++) {
                if (gObjects[sp24].actionState != 0) {
                    if ((func_8001B44C(sp24, 0) != 0)) {
                        D_80134790 = 1;
                        if (gDemoID == 1) {
                            D_80134791 = 3;
                        } else {
                            D_80134791 = 2;
                        }
                        break;
                    }
                }
            }
        }
    }
    if (D_801347E4 != 0) {
        if (func_8001B62C(0, 0) == func_8001B580(0, 0) - 48.0f) {
            if (D_801347E4 == 1) {
                Set_BgColor(0, 0, 0, 0x10);
            } else {
                Set_BgColor(0xFF, 0xFF, 0xFF, 0x10);
            }
            func_8001D2C0();
        }
    }
    func_8001994C();
}

// we need to patch this function to tag when to skip interpolation for 1 frame between demo scene changes.
RECOMP_PATCH void func_8005DA00(void) {
    s32 i;

    if (D_80134790 == 0) {
        return;
    } else if (D_80134790 < D_80134791) {
        func_8001EC04();
        D_80134790 += 1;
        return;
    }
    D_80134790 = 0;
    if (D_80134794->unk0 == 0) {
        D_801347E5 = 1;
        D_801347E6 = 1;
        Set_BgColor(0xFF, 0xFF, 0xFF, 0x10);
        func_8001D2C0();
        func_8001EBE8();
        return;
    }
    for (i = 0; i < 8; i++) {
        func_8001A928(i);
    }
    Set_DecompressHeap((void*)0x80280000);
    if (D_80134794->unk0 != 0) {
        DecompressFile(1, D_80134794->unk0, D_80134794->unk4);
    }
    if (D_80134794->unk8 != 0) {
        DecompressFile(2, D_80134794->unk8, D_80134794->unkC);
    }
    if (D_80134794->unk10 == -1) {
        D_8013479C = NULL;
    } else {
        D_8013479C = (void*) (D_80134794->unk10 + gFileArray[1].ptr);
        D_801347A8 = 0.0f;
        D_801347AC = 0;
        D_801347B0 = 0;
        D_801347A4 = (void*) (D_8013479C->unk14[3] + (u32) D_8013479C);
        D_801347A0.arr = (void*) (D_8013479C->unk14[6] + (u32) D_8013479C);
    }
    for (i = 0; i < 8; i++) {
        if ((D_80134794->unk14[i] != -1) && (D_80134794->unk34[i] != -1)) {
            func_8001BD44(i, 0, D_80134794->unk34[i], (s32) (gFileArray[1].ptr + D_80134794->unk14[i]));
            func_8001BE6C(i, 0, 0, 0);
            func_8001B754(0, 0);
            gObjects[i].actionState = 1;
        }
    }
    Demo_HandleSetAnimTextures();
    if ((D_801347E4 != 0) && (func_8001D1D4() == 2)) {
        func_8001D284();
    }
    D_801347E4 = D_80134794->unk54;
    skipInterpolationOneFrame = 1; // skip for the next time
    gDemoSceneID += 1;
    D_80134794++;
    func_8001EBE8();
}
