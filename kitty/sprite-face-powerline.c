/*
 * sprite-face-powerline.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Powerline separators and progress | U+E0A0...U+E0FF

#include "sprite-face-private.h"

// The "thick half circle" separators from powerline extra, U+E0D2 and U+E0D4.
// Two wedges that meet at the middle of the cell, with a gap of one line
// thickness between them, so that the cells above and below line up.
static void
thick_half_circle(Canvas *self, bool flip) {
    const double w = self->width, h = self->height;
    const double t = (double)thickness(self, 1, true);
    const double top_end = h / 2.0 - t / 2.0, bottom_end = h / 2.0 + t / 2.0;
#define X(x) (flip ? w - (x) : (x))
    fill_triangle(self, (FloatPoint){.x = X(0), .y = 0}, (FloatPoint){.x = X(w), .y = 0}, (FloatPoint){.x = X(w / 2), .y = top_end});
    fill_triangle(self, (FloatPoint){.x = X(0), .y = 0}, (FloatPoint){.x = X(w / 2), .y = top_end}, (FloatPoint){.x = X(0), .y = top_end});
    fill_triangle(self, (FloatPoint){.x = X(0), .y = h}, (FloatPoint){.x = X(w), .y = h}, (FloatPoint){.x = X(w / 2), .y = bottom_end});
    fill_triangle(self, (FloatPoint){.x = X(0), .y = h}, (FloatPoint){.x = X(w / 2), .y = bottom_end}, (FloatPoint){.x = X(0), .y = bottom_end});
#undef X
}


const SpriteRangeDef sprite_face_ranges_powerline[] = {
    {0xe0b0, 0xe0bf},
    {0xe0d2, 0xe0d2},
    {0xe0d4, 0xe0d4},
    {0xe0d6, 0xe0d7},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_powerline(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        S(L'', triangle, true, false);
        S(L'', triangle, true, true);
        SS(L'', half_cross_line(c, 1, TOP_LEFT); half_cross_line(c, 1, BOTTOM_LEFT));
        S(L'', triangle, false, false);
        S(L'', triangle, false, true);
        SS(L'', half_cross_line(c, 1, TOP_RIGHT); half_cross_line(c, 1, BOTTOM_RIGHT));
        S(L'', filled_D, true);
        S(L'', filled_D, false);
        C(L'', rounded_separator, 1, true);
        C(L'', rounded_separator, 1, false);
        S(L'', cross_line, 1, true);
        S(L'', cross_line, 1, true);
        S(L'', cross_line, 1, false);
        S(L'', cross_line, 1, false);
        S(L'', corner_triangle, BOTTOM_LEFT);
        S(L'', corner_triangle, BOTTOM_RIGHT);
        S(L'', corner_triangle, TOP_LEFT);
        S(L'', corner_triangle, TOP_RIGHT);
        SS(0xe0d2, thick_half_circle(c, false));
        SS(0xe0d4, thick_half_circle(c, true));
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
