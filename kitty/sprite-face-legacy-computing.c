/*
 * sprite-face-legacy-computing.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Symbols for Legacy Computing | U+1FB00...U+1FBFF

#include "sprite-face-private.h"

// Additional drawing macros used by the cases below

#define SH(ch, ...) C(ch, shade, (Shade){__VA_ARGS__});

#define M(ch, corner)                                                                    \
    SB(ch, corner_triangle(c, corner));                                                  \
    memcpy(ss->mask, canvas->mask, sizeof(canvas->mask[0]) * canvas->width * canvas->height); \
    fill_canvas(canvas, 0);                                                             \
    shade(canvas, (Shade){.xnum = 12});                                                 \
    apply_mask(canvas, ss->mask);                                                        \
    break;

#define DL(x1, y1, x2, y2) diagonal_line(c, 1, x1, y1, x2, y2)

#define W (int)minus(c->width, 1)

#define H (int)minus(c->height, 1)

#define HW ((int)(c->width / 2))

#define HH ((int)(c->height / 2))


const SpriteRangeDef sprite_face_ranges_legacy_computing[] = {
    {0x1fb00, 0x1fbaf},
    {0x1fbbd, 0x1fbbf},
    {0x1fbce, 0x1fbef},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_legacy_computing(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        SH(L'🮌', .xnum = 12, .which_half = LEFT_EDGE);
        SH(L'🮍', .xnum = 12, .which_half = RIGHT_EDGE);
        SH(L'🮎', .xnum = 12, .which_half = TOP_EDGE);
        SH(L'🮏', .xnum = 12, .which_half = BOTTOM_EDGE);
        SH(L'🮐', .xnum = 12, .invert = true);
        SH(L'🮑', .xnum = 12, .invert = true, .fill_blank = true, .which_half = BOTTOM_EDGE);
        SH(L'🮒', .xnum = 12, .invert = true, .fill_blank = true, .which_half = TOP_EDGE);
        SH(L'🮓', .xnum = 12, .invert = true, .fill_blank = true, .which_half = RIGHT_EDGE);
        SH(L'🮔', .xnum = 12, .invert = true, .fill_blank = true, .which_half = LEFT_EDGE);
        SH(L'🮕', .xnum = 4, .ynum = 4);
        SH(L'🮖', .xnum = 4, .ynum = 4, .invert = true);
        SH(L'🮗', .xnum = 1, .ynum = 4, .invert = true);
        M(L'🮜', TOP_LEFT);
        M(L'🮝', TOP_RIGHT);
        M(L'🮞', BOTTOM_RIGHT);
        M(L'🮟', BOTTOM_LEFT);
        S(L'🮘', cross_shade, false);
        S(L'🮙', cross_shade, true);
        S(L'🬼', smooth_mosaic, true, 0, 2. / 3, 0.5, 1);
        S(L'🬽', smooth_mosaic, true, 0, 2. / 3, 1, 1);
        S(L'🬾', smooth_mosaic, true, 0, 1. / 3, 0.5, 1);
        S(L'🬿', smooth_mosaic, true, 0, 1. / 3, 1, 1);
        S(L'🭀', smooth_mosaic, true, 0, 0, 0.5, 1);
        S(L'🭁', smooth_mosaic, true, 0, 1. / 3, 0.5, 0);
        S(L'🭂', smooth_mosaic, true, 0, 1. / 3, 1, 0);
        S(L'🭃', smooth_mosaic, true, 0, 2. / 3, 0.5, 0);
        S(L'🭄', smooth_mosaic, true, 0, 2. / 3, 1, 0);
        S(L'🭅', smooth_mosaic, true, 0, 1, 0.5, 0);
        S(L'🭆', smooth_mosaic, true, 0, 2. / 3, 1, 1. / 3);
        S(L'🭇', smooth_mosaic, true, 0.5, 1, 1, 2. / 3);
        S(L'🭈', smooth_mosaic, true, 0, 1, 1, 2. / 3);
        S(L'🭉', smooth_mosaic, true, 0.5, 1, 1, 1. / 3);
        S(L'🭊', smooth_mosaic, true, 0, 1, 1, 1. / 3);
        S(L'🭋', smooth_mosaic, true, 0.5, 1, 1, 0);
        S(L'🭌', smooth_mosaic, true, 0.5, 0, 1, 1. / 3);
        S(L'🭍', smooth_mosaic, true, 0, 0, 1, 1. / 3);
        S(L'🭎', smooth_mosaic, true, 0.5, 0, 1, 2. / 3);
        S(L'🭏', smooth_mosaic, true, 0, 0, 1, 2. / 3);
        S(L'🭐', smooth_mosaic, true, 0.5, 0, 1, 1);
        S(L'🭑', smooth_mosaic, true, 0, 1. / 3, 1, 2. / 3);
        S(L'🭒', smooth_mosaic, false, 0, 2. / 3, 0.5, 1);
        S(L'🭓', smooth_mosaic, false, 0, 2. / 3, 1, 1);
        S(L'🭔', smooth_mosaic, false, 0, 1. / 3, 0.5, 1);
        S(L'🭕', smooth_mosaic, false, 0, 1. / 3, 1, 1);
        S(L'🭖', smooth_mosaic, false, 0, 0, 0.5, 1);
        S(L'🭗', smooth_mosaic, false, 0, 1. / 3, 0.5, 0);
        S(L'🭘', smooth_mosaic, false, 0, 1. / 3, 1, 0);
        S(L'🭙', smooth_mosaic, false, 0, 2. / 3, 0.5, 0);
        S(L'🭚', smooth_mosaic, false, 0, 2. / 3, 1, 0);
        S(L'🭛', smooth_mosaic, false, 0, 1, 0.5, 0);
        S(L'🭜', smooth_mosaic, false, 0, 2. / 3, 1, 1. / 3);
        S(L'🭝', smooth_mosaic, false, 0.5, 1, 1, 2. / 3);
        S(L'🭞', smooth_mosaic, false, 0, 1, 1, 2. / 3);
        S(L'🭟', smooth_mosaic, false, 0.5, 1, 1, 1. / 3);
        S(L'🭠', smooth_mosaic, false, 0, 1, 1, 1. / 3);
        S(L'🭡', smooth_mosaic, false, 0.5, 1, 1, 0);
        S(L'🭢', smooth_mosaic, false, 0.5, 0, 1, 1. / 3);
        S(L'🭣', smooth_mosaic, false, 0, 0, 1, 1. / 3);
        S(L'🭤', smooth_mosaic, false, 0.5, 0, 1, 2. / 3);
        S(L'🭥', smooth_mosaic, false, 0, 0, 1, 2. / 3);
        S(L'🭦', smooth_mosaic, false, 0.5, 0, 1, 1);
        S(L'🭧', smooth_mosaic, false, 0, 1. / 3, 1, 2. / 3);
        S(L'🭨', half_triangle, LEFT_EDGE, true);
        S(L'🭩', half_triangle, TOP_EDGE, true);
        S(L'🭪', half_triangle, RIGHT_EDGE, true);
        S(L'🭫', half_triangle, BOTTOM_EDGE, true);
        S(L'🭬', half_triangle, LEFT_EDGE, false);
        SS(L'🮛', half_triangle(c, LEFT_EDGE, false), half_triangle(c, RIGHT_EDGE, false));
        S(L'🭭', half_triangle, TOP_EDGE, false);
        S(L'🭮', half_triangle, RIGHT_EDGE, false);
        S(L'🭯', half_triangle, BOTTOM_EDGE, false);
        SS(L'🮚', half_triangle(c, BOTTOM_EDGE, false), half_triangle(c, TOP_EDGE, false));
        CC(L'🭼', eight_bar(c, 0, false); eight_bar(c, 7, true));
        CC(L'🭽', eight_bar(c, 0, false); eight_bar(c, 0, true));
        CC(L'🭾', eight_bar(c, 7, false); eight_bar(c, 0, true));
        CC(L'🭿', eight_bar(c, 7, false); eight_bar(c, 7, true));
        CC(L'🮀', eight_bar(c, 0, true); eight_bar(c, 7, true));
        CC(L'🮁', eight_bar(c, 0, true); eight_bar(c, 2, true); eight_bar(c, 4, true); eight_bar(c, 7, true));
        C(L'🮂', eight_block, true, 0, 1, -1);
        C(L'🮃', eight_block, true, 0, 1, 2, -1);
        C(L'🮄', eight_block, true, 0, 1, 2, 3, 4, -1);
        C(L'🮅', eight_block, true, 0, 1, 2, 3, 4, 5, -1);
        C(L'🮆', eight_block, true, 0, 1, 2, 3, 4, 5, 6, -1);
        C(L'🮇', eight_block, false, 6, 7, -1);
        C(L'🮈', eight_block, false, 5, 6, 7, -1);
        C(L'🮉', eight_block, false, 3, 4, 5, 6, 7, -1);
        C(L'🮊', eight_block, false, 2, 3, 4, 5, 6, 7, -1);
        C(L'🮋', eight_block, false, 1, 2, 3, 4, 5, 6, 7, -1);
        S(L'🮠', mid_lines, 1, TOP_LEFT, 0);
        S(L'🮡', mid_lines, 1, TOP_RIGHT, 0);
        S(L'🮢', mid_lines, 1, BOTTOM_LEFT, 0);
        S(L'🮣', mid_lines, 1, BOTTOM_RIGHT, 0);
        S(L'🮤', mid_lines, 1, TOP_LEFT, BOTTOM_LEFT, 0);
        S(L'🮥', mid_lines, 1, TOP_RIGHT, BOTTOM_RIGHT, 0);
        S(L'🮦', mid_lines, 1, BOTTOM_RIGHT, BOTTOM_LEFT, 0);
        S(L'🮧', mid_lines, 1, TOP_RIGHT, TOP_LEFT, 0);
        S(L'🮨', mid_lines, 1, BOTTOM_RIGHT, TOP_LEFT, 0);
        S(L'🮩', mid_lines, 1, BOTTOM_LEFT, TOP_RIGHT, 0);
        S(L'🮪', mid_lines, 1, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT, 0);
        S(L'🮫', mid_lines, 1, BOTTOM_LEFT, TOP_LEFT, BOTTOM_RIGHT, 0);
        S(L'🮬', mid_lines, 1, TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, 0);
        S(L'🮭', mid_lines, 1, TOP_RIGHT, TOP_LEFT, BOTTOM_LEFT, 0);
        S(L'🮮', mid_lines, 1, TOP_RIGHT, BOTTOM_RIGHT, TOP_LEFT, BOTTOM_LEFT, 0);
        case 0x1fb00 ... 0x1fb00 + 19: sextant(c, ch - 0x1fb00 + 1); break;
        case 0x1fb14 ... 0x1fb14 + 19: sextant(c, ch - 0x1fb00 + 2); break;
        case 0x1fb28 ... 0x1fb28 + 19: sextant(c, ch - 0x1fb00 + 3); break;
        case 0x1fb70 ... 0x1fb70 + 5: eight_bar(c, ch - 0x1fb6f, false); break;
        case 0x1fb76 ... 0x1fb76 + 5: eight_bar(c, ch - 0x1fb75, true); break;
        case 0x1fbe6: octant(c, 0xe6); break;
        case 0x1fbe7: octant(c, 0xe7); break;
        // U+1FBCE LEFT TWO THIRDS BLOCK
        case 0x1fbce: fill_rect(c, 0, 0, 2 * c->width / 3, c->height); break;
        // U+1FBCF LEFT ONE THIRD BLOCK
        case 0x1fbcf: fill_rect(c, 0, 0, c->width / 3, c->height); break;
        // U+1FBE4 UPPER CENTRE ONE QUARTER BLOCK
        case 0x1fbe4: fill_rect(c, c->width / 4, 0, 3 * c->width / 4, c->height / 2); break;
        // U+1FBE5 LOWER CENTRE ONE QUARTER BLOCK
        case 0x1fbe5:
        fill_rect(c, c->width / 4, c->height / 2, 3 * c->width / 4, c->height);
        break;
        // U+1FBD0-1FBDF Diagonal box drawings (supersampled for anti-aliasing)
        // Key points used: UL=(0,0), UC=(w/2,0), UR=(w,0), ML=(0,h/2), MC=(w/2,h/2), MR=(w,h/2),
        //                  LL=(0,h), LC=(w/2,h), LR=(w,h)
        // 1FBD0: middle right to lower left
        SS(0x1fbd0, DL(W, HH, 0, H));
        // 1FBD1: upper right to middle left
        SS(0x1fbd1, DL(W, 0, 0, HH));
        // 1FBD2: upper left to middle right
        SS(0x1fbd2, DL(0, 0, W, HH));
        // 1FBD3: middle left to lower right
        SS(0x1fbd3, DL(0, HH, W, H));
        // 1FBD4: upper left to lower centre
        SS(0x1fbd4, DL(0, 0, HW, H));
        // 1FBD5: upper centre to lower right
        SS(0x1fbd5, DL(HW, 0, W, H));
        // 1FBD6: upper right to lower centre
        SS(0x1fbd6, DL(W, 0, HW, H));
        // 1FBD7: upper centre to lower left
        SS(0x1fbd7, DL(HW, 0, 0, H));
        // 1FBD8: upper left to middle centre to upper right (V open down)
        SS(0x1fbd8, DL(0, 0, HW, HH); DL(HW, HH, W, 0));
        // 1FBD9: upper right to middle centre to lower right (> shape)
        SS(0x1fbd9, DL(W, 0, HW, HH); DL(HW, HH, W, H));
        // 1FBDA: lower left to middle centre to lower right (^ shape)
        SS(0x1fbda, DL(0, H, HW, HH); DL(HW, HH, W, H));
        // 1FBDB: upper left to middle centre to lower left (< shape)
        SS(0x1fbdb, DL(0, 0, HW, HH); DL(HW, HH, 0, H));
        // 1FBDC: upper left to lower centre to upper right (V with apex at bottom-center)
        SS(0x1fbdc, DL(0, 0, HW, H); DL(HW, H, W, 0));
        // 1FBDD: upper right to middle left to lower right (> with apex at middle-left)
        SS(0x1fbdd, DL(W, 0, 0, HH); DL(0, HH, W, H));
        // 1FBDE: lower left to upper centre to lower right (^ with apex at upper-center)
        SS(0x1fbde, DL(0, H, HW, 0); DL(HW, 0, W, H));
        // 1FBDF: upper left to middle right to lower left (< with apex at middle-right)
        SS(0x1fbdf, DL(0, 0, W, HH); DL(W, HH, 0, H));
        // U+1FBE0-1FBE3 Justified half white circles (outlines)
        S(0x1fbe0, justified_half_circle_outline, 1, TOP_EDGE);
        S(0x1fbe1, justified_half_circle_outline, 1, RIGHT_EDGE);
        S(0x1fbe2, justified_half_circle_outline, 1, BOTTOM_EDGE);
        S(0x1fbe3, justified_half_circle_outline, 1, LEFT_EDGE);
        // U+1FBE8-1FBEB Justified half black circles (filled)
        S(0x1fbe8, justified_half_circle, TOP_EDGE);
        S(0x1fbe9, justified_half_circle, RIGHT_EDGE);
        S(0x1fbea, justified_half_circle, BOTTOM_EDGE);
        S(0x1fbeb, justified_half_circle, LEFT_EDGE);
        // U+1FBEC-1FBEF Justified quarter black circles (filled)
        S(0x1fbec, justified_quarter_circle, TOP_RIGHT);
        S(0x1fbed, justified_quarter_circle, BOTTOM_LEFT);
        S(0x1fbee, justified_quarter_circle, BOTTOM_RIGHT);
        S(0x1fbef, justified_quarter_circle, TOP_LEFT);
        CC(0x1fbaf, cross(c, 6));
        SB(0x1fbbd, cross_line(c, 1, false); cross_line(c, 1, true)); invert_mask(canvas); break;
        SB(0x1fbbe, mid_lines(c, 1, BOTTOM_RIGHT, 0)); invert_mask(canvas); break;
        SB(0x1fbbf, mid_lines(c, 1, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, 0)); invert_mask(canvas); break;
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
