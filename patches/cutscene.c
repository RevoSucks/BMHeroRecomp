#include "patches.h"

#include <ultra64.h>

// .bss
extern s8 D_80134BF0;
extern s16 gCutsceneLength;
extern s16 D_80134BF4;
extern s16 D_80134BF6_unused;
extern s16 D_80134BF8;
extern s16 D_80134BFA;
extern s16 D_80134BFC;
extern s16* gCutsceneByteCode;
extern s16* D_80134C04_unused;
extern struct CutsceneString* gCutsceneString;
extern s16 D_80134C0C;
extern s16 D_80134C0E;
extern s16 D_80134C10;
extern s16 D_80134C12;
extern s16 D_80134C14;
extern s16 D_80134C16;
extern s16 D_80134C18;
extern s16 D_80134C1A;
extern s16 D_80134C1C;
extern s16 D_80134C1E;
extern s16 D_80134C20;
extern s16 D_80134C22;
extern s8 D_80134C24;
extern s8 D_80134C25;
extern s8 D_80134C26;
extern s16 D_80134C28[0x18];
extern s16 D_80134C58[0x18];
extern s16 D_80134C88[0x18];
extern s16 D_80134CB8[0x18];
extern s16 D_80134CE8[0x18];
extern s16 D_80134D18[0x18];

extern struct UnkStruct80134D48 D_80134D48[0x18];
extern struct MegaStruct gMegaStruct; // 80134F28

extern int skipInterpolationOneFrame;

extern void ByteParser_SetObjectScale(s16* arg0);

// process cutscene bytecode script, ran every frame during cutscenes
RECOMP_PATCH void func_80088ECC(void) {
    s16 exit;

    // @recomp unset skip if it was set previously.
    if (skipInterpolationOneFrame) {
        skipInterpolationOneFrame--;
    }

    if (gCutsceneLength == -1) {
        return;
    }
    if ((D_80134C0C == 0) && (D_80134BF8 == -1)) {
        D_80134C0C += 1;
        D_80134C12 = 0;
        D_80134C10 = 0;
        D_80134C0E = 0;
        D_80134C16 = 0;
        D_80134C14 = 0;
        D_80134BF8 = -1, D_80134BF4 = -1;
        gCutsceneByteCode = (s16*) 0x8033A000;
    }
    if (D_80134C14 != 0) {
        D_80134C14 -= 1;
        return;
    }
    exit = 0;
    while (exit == 0) {
        switch (*gCutsceneByteCode) {
            case 0x0:
                gCutsceneLength = -1;
                D_80134C0C = 0;
                exit = 1;
                break;
            case 0x1:
                exit = 1;
                break;
            case 0x2:
                D_80134BF8 = gCutsceneByteCode[1];
                D_80134BFA = gCutsceneByteCode[2];
                D_80134BFC = gCutsceneByteCode[3];
                gCutsceneByteCode += 3;
                break;
            case 0x5:
                D_80134C14 = gCutsceneByteCode[1] - 1;
                gCutsceneByteCode += 1;
                exit = 1;
                break;
            case 0x6:
                func_8008ABC4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x3C:
                func_8008ABF4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x7:
                func_8008AC08(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x8:
                gCameraType = 0;
                break;
            case 0x27:
                func_8008BB0C();
                break;
            case 0x25:
                func_8008AD58(gCutsceneByteCode + 1);
                gCutsceneByteCode += 7;
                break;
            case 0x26:
                func_8008AE64(gCutsceneByteCode + 1);
                gCutsceneByteCode += 6;
                break;
            case 0x9:
                gMegaStruct.D_80134F74 = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xA:
                gMegaStruct.D_80134F50.x = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xB:
                gMegaStruct.D_80134F50.y = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xC:
                gMegaStruct.D_80134F50.z = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xD:
                gMegaStruct.D_80134F5C.x = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xE:
                gMegaStruct.D_80134F5C.y = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0xF:
                func_8008B5D4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x10:
                func_8008B624(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x11:
                func_8008B6E0(gCutsceneByteCode + 1);
                gCutsceneByteCode += 5;
                break;
            case 0x12:
                func_8008BA3C();
                break;
            case 0x13:
                func_8008C61C();
                break;
            case 0x14:
                func_8008C638();
                break;
            case 0x15:
                func_8008C650();
                break;
            case 0x16:
                func_8008C66C();
                break;
            case 0x17:
                func_8008C684(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x18:
                func_8008C778(gCutsceneByteCode + 1);
                gCutsceneByteCode += 5;
                break;
            case 0x19:
                func_8008C9B4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x1A:
                func_8008CADC(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x1B:
                func_8008CC04(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x1F:
                func_8008CD2C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x20:
                func_8008CE8C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 5;
                break;
            case 0x32:
                func_8008D074(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x33:
                func_8008D114(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x21:
                func_8008D530(gCutsceneByteCode + 1);
                gCutsceneByteCode += 6;
                break;
            case 0x22:
                func_8008DC8C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x23:
                func_8008E328(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x34:
                func_8008E3B0(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x39:
                func_8008E418(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x24:
                func_8008E81C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x28:
                gMegaStruct.D_80134F68.x = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0x29:
                gMegaStruct.D_80134F68.y = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0x2A:
                gMegaStruct.D_80134F68.z = (f32) gCutsceneByteCode[1] / 10.0f;
                gCutsceneByteCode += 1;
                break;
            case 0x2B:
                func_8008BC04(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x2C:
                func_8008E8A4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x2D:
                func_8008E918(gCutsceneByteCode + 1);
                gCutsceneByteCode += 3;
                break;
            case 0x2E:
                func_8008EAE4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x2F:
                func_8008EB10(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x30:
                func_8008EB40(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x31:
                func_8008EB6C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x36:
                func_8008EB9C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x35:
                func_8008EBEC(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x37:
                func_8008EC3C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x38:
                func_8008ECD0(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x3A:
                func_8008ED64(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x3B:
                ByteParser_SetObjectScale(gCutsceneByteCode + 1);
                gCutsceneByteCode += 4;
                break;
            case 0x3D:
                func_8008F078(gCutsceneByteCode + 1);
                gCutsceneByteCode += 5;
                break;
            case 0x3E:
                func_8008F3D4(gCutsceneByteCode + 1);
                gCutsceneByteCode += 1;
                break;
            case 0x3F:
                func_8008F63C(gCutsceneByteCode + 1);
                gCutsceneByteCode += 5;
                break;
            case 0x4C:
                func_8008F6AC(gCutsceneByteCode + 1);
                gCutsceneByteCode += 2;
                break;
            case 0x40:
                D_80134C22 = 1;
                break;
            case 0x41:
                D_80134C22 = 0;
                break;
            case 0x42:
                D_80134C24 = 1;
                D_80134C25 = 0;
                break;
            case 0x44:
                D_80134C24 = 2;
                D_80134C25 = 0;
                break;
            case 0x43:
            case 0x45:
                D_80134C24 = 0;
                break;
            case 0x46:
                func_8001FB3C(gCutsceneByteCode[1], gCutsceneByteCode[2], gCutsceneByteCode[3]);
                gCutsceneByteCode += 3;
                break;
            case 0x47:
                D_80134C26 = 1;
                break;
            case 0x48:
                D_80134C26 = 0;
                break;
            case 0x49:
                func_8008884C(1);
                break;
            case 0x4A:
                func_8008884C(0);
                break;
            case 0x4B:
                D_80134BF0 = (s8) gCutsceneByteCode[1];
                gCutsceneByteCode += 1;
                break;
            default:
                exit = 1;
                break;
        }
        gCutsceneByteCode += 1;
    }
}

RECOMP_PATCH void func_8008AE64(s16* arg0) {
    s32 sp4;

    // @recomp Cutscenes have an interpolation issue where when the camera changes suddenly it
    // is glitched for a frame due to interpolation. Skip it for 3 frames so the right frame is skipped.
    skipInterpolationOneFrame = 3;

    sp4 = D_80134D48[arg0[0]].ObjectID;
    gMegaStruct.D_80134F28.x = gObjects[sp4].Pos.x;
    gMegaStruct.D_80134F28.y = gObjects[sp4].Pos.y + arg0[1];
    gMegaStruct.D_80134F28.z = gObjects[sp4].Pos.z;
    gMegaStruct.D_80134F34.x = arg0[2];
    gMegaStruct.D_80134F34.y = arg0[3];
    gMegaStruct.D_80134F34.z = arg0[4];
    gMegaStruct.D_80134F40 = arg0[5];
    gMegaStruct.D_80134FA4.unk2 = (s16) (s32) (f32) arg0[1]; // what the fuck
    gMegaStruct.D_80134FA8 = arg0[0];
    gMegaStruct.D_80134F50.x = 0.0f;
    gMegaStruct.D_80134F50.y = 0.0f;
    gMegaStruct.D_80134F50.z = 0.0f;
    gMegaStruct.D_80134F5C.x = 0.0f;
    gMegaStruct.D_80134F5C.y = 0.0f;
    gMegaStruct.D_80134F5C.z = 0.0f;
    gMegaStruct.D_80134F74 = 0.0f;
}
