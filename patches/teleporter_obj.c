#include "patches.h"

#include <ultra64.h>

extern s32 D_80115808[];
extern s16 D_8016E318;

extern f32 D_8016E438;
extern f32 D_8016E43C;
extern f32 D_8016E440;
extern f32 D_8016E444;

extern void func_80073580(void);

extern int skipInterpolationOneFrame;
extern int skipInterpolationOneFrame_Camera;

// code for the teleporter object in levels.
RECOMP_PATCH void func_80284668_code_extra_0(void) {

    // @recomp We need to disable interpolation for 1 frame once the camera moves suddenly. It will get decremented before
    // this is processed, so we need to compensate with +1.
    skipInterpolationOneFrame_Camera = 2;
    skipInterpolationOneFrame = 2;

    func_8001A928(1);
    gPlayerObject->actionState = 0x2E;
    func_8001C0EC(0, 0, 7, 1, &D_80115808);
    D_8016E318 = 0;
    gPlayerObject->damageState = -1;
    func_800178D4(0, 0, 0x39, -1, 0);
    gPlayerObject->Pos.x = D_8016E438;
    gPlayerObject->Pos.y = D_8016E43C;
    gPlayerObject->Pos.z = D_8016E440;
    gPlayerObject->Rot.y = D_8016E444;
    func_80073580();
}
