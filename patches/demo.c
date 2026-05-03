#include "patches.h"

#include <ultra64.h>

extern void Demo_HandleSetAnimTextures(void);
extern void func_8001BD44(s32 objId, s32 arg1, s32 arg2, s32 arg3);
extern void Demo_SetView(void);
extern void Demo_PrepareStr(void);
extern void Demo_HandlePrintText(void);

extern int skipInterpolationOneFrame;
extern int skipInterpolationOneFrame_Camera;

void func_8005DA00(void);

extern float gRecompNewView[2][4][4] __attribute__((aligned(8)));
extern float gRecompNewView_Inv[2][4][4] __attribute__((aligned(8)));
extern int bufferID;

extern int gluInvertMatrix(float m[16], float invOut[16]);

// demo camera
RECOMP_PATCH void func_8005E040(void) {
    u16 sp3E;
    s32 i;

    func_8001D4D0();
    Debug_SetBg(1, 0, 0, 0);
    guPerspective(D_8016E104, &sp3E, 50.0f, 1.3333334f, 10.0f, 8000.0f, 1.0f);
    gSPPerspNormalize(gMasterDisplayList++, sp3E);
    // for the rt64 renderer
    guLookAtF(&gRecompNewView[bufferID], gView.eye.x, gView.eye.y, gView.eye.z, gView.at.x, gView.at.y, gView.at.z,
                 gView.up.x, gView.up.y, gView.up.z);
    guLookAt(&D_8016E104->unk00[2], gView.eye.x, gView.eye.y, gView.eye.z, gView.at.x, gView.at.y, gView.at.z,
             gView.up.x, gView.up.y, gView.up.z);

    // we need to invert the MTX before using it
    gluInvertMatrix(&gRecompNewView[bufferID], &gRecompNewView_Inv[bufferID]);
    gEXSetViewMatrixFloat(gMasterDisplayList++, &gRecompNewView_Inv[bufferID]);

    // the camera needs interpolation detected and applied as well.
#define CAMERA_ID 0xF000

    if (skipInterpolationOneFrame_Camera == 0) {
        gEXMatrixGroupSimpleNormal(gMasterDisplayList++, CAMERA_ID, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);
    } else {
        gEXMatrixGroupNoInterpolate(gMasterDisplayList++, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_EDIT_NONE);
        recomp_printf("skipping interpolation for camera for 1 frame\n");
    }

    gSPMatrix(gMasterDisplayList++, D_8016E104, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    D_8016E3A4 = 0;
    func_8001838C();
    for (i = 0; i < 8; i++) {
        if ((gObjects[i].actionState != 0)) {
            if (func_8001C1A8(i, 0) != 0) {
                func_8001B014(i, 0);
                func_8001C384(i, 0);
            }
        }
    }
    Demo_HandlePrintText();
}

RECOMP_PATCH void func_8005DD80(void) {
    s32 sp24;

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
    skipInterpolationOneFrame = 2; // skip for the next time
    skipInterpolationOneFrame_Camera = 2; // skip for the next time
    gDemoSceneID += 1;
    D_80134794++;
    func_8001EBE8();
}
