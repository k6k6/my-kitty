/*
 * sprite-face-legacy-computing-supplement.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Symbols for Legacy Computing Supplement | U+1CC00...U+1CEFF

#include "sprite-face-private.h"

// The circle and ellipse outlines for U+1CE00, U+1CE01, U+1CE0B and U+1CE0C. The
// circle has a diameter equal to the cell width, the ellipse exactly inscribes
// the cell. Both are centered in the cell. Angles are in degrees, clockwise from
// the positive x axis, the y axis points down, so 0 = right, 90 = bottom,
// 180 = left, 270 = top.
static void
outline(Canvas *self, bool ellipse_shape, double start_deg, double end_deg) {
    const double lw = thickness_as_float(self, 1, true), half_lw = fmax(0.5, lw / 2.0);
    const double w = self->width, h = self->height, rx = w / 2.0 - half_lw;
    const double ry = ellipse_shape ? h / 2.0 - half_lw : rx;
    ellipse_arc_outline(self, w / 2.0, h / 2.0, rx, ry, start_deg, end_deg, lw);
}


const SpriteRangeDef sprite_face_ranges_legacy_computing_supplement[] = {
    {0x1cc1b, 0x1cc3f},
    {0x1cd00, 0x1cde5},
    {0x1ce00, 0x1ce01},
    {0x1ce0b, 0x1ce0c},
    {0x1ce16, 0x1ce19},
    {0x1ce51, 0x1ceaf},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_legacy_computing_supplement(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        case 0x1cd00 ... 0x1cde5: octant(c, ch - 0x1cd00); break;
        // U+1CC1B-1CC1E Box drawing variants
        // 1CC1B: HORIZONTAL AND UPPER RIGHT - full hline + half vline going up from right quarter
        CC(0x1cc1b, hline(c, 1); draw_vline(c, 0, c->height / 2, 3 * c->width / 4, 1));
        // 1CC1C: HORIZONTAL AND LOWER RIGHT - full hline + half vline going down from right quarter
        CC(0x1cc1c, hline(c, 1); draw_vline(c, c->height / 2, c->height, 3 * c->width / 4, 1));
        // 1CC1D: TOP AND UPPER LEFT - half vline from top to 1/4 + half hline going left from that point
        CC(0x1cc1d, draw_vline(c, 0, c->height / 4, c->width / 2, 1); draw_hline(c, 0, c->width / 2, c->height / 4, 1));
        // 1CC1E: BOTTOM AND LOWER LEFT - half vline from bottom to 3/4 + half hline going left from that point
        CC(0x1cc1e, draw_vline(c, 3 * c->height / 4, c->height, c->width / 2, 1); draw_hline(c, 0, c->width / 2, 3 * c->height / 4, 1));
        // U+1CC1F-1CC20 Double diagonal lines
        S(0x1cc1f, double_cross_line, 1, false);
        S(0x1cc20, double_cross_line, 1, true);
        // Symbols for Legacy Computing Supplement (U+1CC00–U+1CEBF)
        // Separated Block Quadrant (bit 0=TL, 1=TR, 2=BL, 3=BR)
        case 0x1cc21 ... 0x1cc21 + 14: draw_separated_block(c, 2, 2, ch - 0x1cc21 + 1); break;
        // U+1CC30-1CC3F Twelfth and quarter circle arcs
        case 0x1cc30 ... 0x1cc3f:
        twelfth_circle(c, 1, ch - 0x1cc30);
        break;
        // U+1CE16-1CE19 Box drawings light vertical with offset horizontal
        // 1CE16: VERTICAL AND TOP RIGHT - full vline + half hline going right from 1/4 height
        CC(0x1ce16, vline(c, 1); draw_hline(c, c->width / 2, c->width, c->height / 4, 1));
        // 1CE17: VERTICAL AND BOTTOM RIGHT - full vline + half hline going right from 3/4 height
        CC(0x1ce17, vline(c, 1); draw_hline(c, c->width / 2, c->width, 3 * c->height / 4, 1));
        // 1CE18: VERTICAL AND TOP LEFT - full vline + half hline going left from 1/4 height
        CC(0x1ce18, vline(c, 1); draw_hline(c, 0, c->width / 2, c->height / 4, 1));
        // 1CE19: VERTICAL AND BOTTOM LEFT - full vline + half hline going left from 3/4 height
        CC(0x1ce19, vline(c, 1); draw_hline(c, 0, c->width / 2, 3 * c->height / 4, 1));
        // Separated Block Sextant (same bit encoding as regular sextants)
        case 0x1ce51 ... 0x1ce51 + 62: draw_separated_block(c, 2, 3, ch - 0x1ce51 + 1); break;
        // One Sixteenth Block: individual 1/16 cells in a 4x4 grid, row-major
        case 0x1ce90 ... 0x1ce90 + 15: sixteenth_block(c, ch - 0x1ce90); break;
        // One Quarter Block partial fills: each is a sub-rectangle of one of the four quarter strips
        // Lower quarter (y: 3H/4 to H)
        case 0x1cea0: fill_rect(c, c->width / 2, 3 * c->height / 4, c->width, c->height); break;
        case 0x1cea1: fill_rect(c, c->width / 4, 3 * c->height / 4, c->width, c->height); break;
        case 0x1cea2: fill_rect(c, 0, 3 * c->height / 4, 3 * c->width / 4, c->height); break;
        case 0x1cea3: fill_rect(c, 0, 3 * c->height / 4, c->width / 2, c->height); break;
        // Left quarter (x: 0 to W/4)
        case 0x1cea4: fill_rect(c, 0, c->height / 2, c->width / 4, c->height); break;
        case 0x1cea5: fill_rect(c, 0, c->height / 4, c->width / 4, c->height); break;
        case 0x1cea6: fill_rect(c, 0, 0, c->width / 4, 3 * c->height / 4); break;
        case 0x1cea7: fill_rect(c, 0, 0, c->width / 4, c->height / 2); break;
        // Upper quarter (y: 0 to H/4)
        case 0x1cea8: fill_rect(c, 0, 0, c->width / 2, c->height / 4); break;
        case 0x1cea9: fill_rect(c, 0, 0, 3 * c->width / 4, c->height / 4); break;
        case 0x1ceaa: fill_rect(c, c->width / 4, 0, c->width, c->height / 4); break;
        case 0x1ceab: fill_rect(c, c->width / 2, 0, c->width, c->height / 4); break;
        // Right quarter (x: 3W/4 to W)
        case 0x1ceac: fill_rect(c, 3 * c->width / 4, 0, c->width, c->height / 2); break;
        case 0x1cead: fill_rect(c, 3 * c->width / 4, 0, c->width, 3 * c->height / 4); break;
        case 0x1ceae: fill_rect(c, 3 * c->width / 4, c->height / 4, c->width, c->height); break;
        case 0x1ceaf: fill_rect(c, 3 * c->width / 4, c->height / 2, c->width, c->height); break;
        SS(0x1ce00, outline(c, false, 0, 360));
        SS(0x1ce01, outline(c, false, 0, 360));
        SS(0x1ce0b, outline(c, true, 90, 270));
        SS(0x1ce0c, outline(c, true, 270, 450));
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
