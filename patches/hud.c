#include "patches.h"
#include "graphics.h"

#include <ultra64.h>

// hud.c

extern void Score_UpdateTimer(s32*, s32*, s32*);
extern s32 func_8001D000(s32, s32, s32, s16);                      /* extern */
extern void func_8006F570(s32 arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4);
extern s32 func_8006F664(s32, s32, s32, s32, f32, f32, s32);   /* extern */

extern Gfx D_801053D0[];
extern s8 D_80165250;
extern s8 D_80165258;
extern s8 D_80165260;
extern s8 D_80165268;
extern s8 D_8016E110;
extern s8 D_80177630;
extern s8 D_801779F8;
extern s16 D_80177A10;
extern s32 D_80177A4C;
extern s16 D_80177A50;
extern s32 D_80177A54;
extern s8 gBombCount;
extern s8 gFireCount;
extern u8 gGemCount;
extern s8 gHealthCount;
extern Gfx* gMasterDisplayList;
extern s8 gMaxHealth;
extern s16 gScore;

// most likely draws the HUD
RECOMP_PATCH void func_80070B1C(void) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;

    if (D_801779F8 == 0) {
        return;
    }
    if (D_80177A50 != 0) {
        func_8001D000(0, 0, 0, D_80177A50);
    }
    gSPDisplayList(gMasterDisplayList++, D_801053D0);

    // the score is treated like a timer (for bosses).
    if (D_80177630 == 2) {
        Score_UpdateTimer(&sp3C, &sp38, &sp34);
        gEXPushScissor(gMasterDisplayList++);
        gEXSetScissor(gMasterDisplayList++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, 240);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(320) * 4, 0, -(320) * 4, 0);
        func_8006F664(sp3C, 0xA, 0xC4, 0x18, 1.0f, 1.0f, 2);
        func_8006F570(0x14, 0xD4, 0x18, 1.0f, 1.0f);
        func_8006F664(sp38, 0xA, 0xEC, 0x18, 1.0f, 1.0f, 2);
        func_8006F570(0x14, 0xFC, 0x18, 1.0f, 1.0f);
        func_8006F664(sp34, 0xA, 0x114, 0x18, 1.0f, 1.0f, 2);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
        gEXPopScissor(gMasterDisplayList++);
    } else {

        // health

        gEXPushScissor(gMasterDisplayList++);
        gEXSetScissor(gMasterDisplayList++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, 240);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);

        sp4C = (s32) D_80177A10;
        sp50 = 0x1E;

        for(sp54 = 0; sp54 < gHealthCount; sp54++) {
            if ((D_8016E110 == 1) && (gHealthCount == (sp54 + 1))) {
                D_8016E115++;
                if ((((D_8016E115 & 1) != 0)))
                    break;
            }
            func_8006F570(0x15, sp50, 0x18 - sp4C, 1.0f, 1.0f);
            sp50 += 0xC;
        }
        for(; sp54 < gMaxHealth; sp54++) {
            sp30 = 1;
            if ((gMaxHealth == (sp54 + 1)) && (D_80177A54 != 0)) {
                sp30 = (D_80177A54 & 1) ^ 1;
            }
            if (sp30 != 0) {
                func_8006F570(0x16, sp50, 0x18 - sp4C, 1.0f, 1.0f);
            }
            sp50 += 0xC;
        }
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
        gEXPopScissor(gMasterDisplayList++);

        // bomb and fire counts
        gEXPushScissor(gMasterDisplayList++);
        gEXSetScissor(gMasterDisplayList++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, 240);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(320) * 4, 0, -(320) * 4, 0);
        func_8006F570(0x17, 0xF0, sp4C + 0xC0, 1.0f, 1.0f);
        func_8006F664(gBombCount + 1, 0, 0xF4, sp4C + 0xC4, 1.0f, 1.0f, 1);
        func_8006F570(0x18, 0x110, sp4C + 0xC0, 1.0f, 1.0f);
        func_8006F664(gFireCount + 1, 0, 0x114, sp4C + 0xC4, 1.0f, 1.0f, 1);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
        gEXPopScissor(gMasterDisplayList++);

        // gem count
        gEXPushScissor(gMasterDisplayList++);
        gEXSetScissor(gMasterDisplayList++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, 240);
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
        func_8006F570(0x19, 0x1A, sp4C + 0xC0, 1.0f, 1.0f);
        func_8006F664((s32) gGemCount, 0, 0x40, sp4C + 0xC4, 1.0f, 1.0f, 2);
        if ((s32) gGemCount >= 0x64) {
            func_8006F664(1, 0, 0x1E, sp4C + 0xC4, 1.0f, 1.0f, 1);
        }
        gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
        gEXPopScissor(gMasterDisplayList++);

        f32 aspect_ratio = recomp_get_target_aspect_ratio(320.0f/240.0f);

        //recomp_printf("Hud Aspect %.6f\n", aspect_ratio);

        if (D_80177A4C > 8) {
            aspect_ratio = (320.0f)/(240.0f); // HACK. Used when moving the score scissor to the center screen
        }

        // calculate the offsets for the score slide at end of level
        sp48 = (((f32) D_80177A4C * (f32)((f32)((f32)(116.0f / 4.0f) * 3.0f) * aspect_ratio)) / 15.0f);
        sp44 = ((f32) D_80177A4C * (104.0f)) / 15.0f;
        sp40 = (f32) D_80177A4C / 15.0f;

        // score
        if (D_80177A4C <= 8) {
            gEXPushScissor(gMasterDisplayList++);
            gEXSetScissor(gMasterDisplayList++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, 240);
            gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(320) * 4, 0, -(320) * 4, 0);
        }
        func_8006F664(gScore * 0xA, 0xA, (s32) (276.0f - sp48), (s32) (24.0f + sp44), 1.0f + sp40, 1.0f + sp40, 5);
        if (D_80177A4C <= 8) {
            gEXSetRectAlign(gMasterDisplayList++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
            gEXPopScissor(gMasterDisplayList++);
        }

        if (D_80165250 != 0) {
            sp54 = D_80165250 + 0x19;
        } else if (D_80165258 != 0) {
            sp54 = 0x1D;
        } else if (D_80165260 != 0) {
            sp54 = 0x1E;
        } else if (D_80165268 != 0) {
            sp54 = 0x2D;
        } else {
            sp54 = 0;
        }

        if (sp54 != 0) {
            func_8006F570(sp54, 0x90, 0x10 - sp4C, 1.0f, 1.0f);
        }
    }
}
