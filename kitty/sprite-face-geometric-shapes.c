/*
 * sprite-face-geometric-shapes.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Geometric Shapes | U+25A0...U+25FF

#include "sprite-face-private.h"

const SpriteRangeDef sprite_face_ranges_geometric_shapes[] = {
    {0x25c9, 0x25c9},
    {0x25cb, 0x25cb},
    {0x25cf, 0x25cf},
    {0x25d6, 0x25d7},
    {0x25dc, 0x25e5},
    {0x25f8, 0x25fa},
    {0x25ff, 0x25ff},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_geometric_shapes(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        S(L'◗', filled_D, true);
        S(L'◖', filled_D, false);
        S(L'◣', corner_triangle, BOTTOM_LEFT);
        S(L'◢', corner_triangle, BOTTOM_RIGHT);
        S(L'◤', corner_triangle, TOP_LEFT);
        S(L'◥', corner_triangle, TOP_RIGHT);
        C(L'○', spinner, 0, 0, 360);
        C(L'◜', spinner, 1, 180, 270);
        C(L'◝', spinner, 1, 270, 360);
        C(L'◞', spinner, 1, 360, 450);
        C(L'◟', spinner, 1, 450, 540);
        C(L'◠', spinner, 1, 180, 360);
        C(L'◡', spinner, 1, 0, 180);
        S(L'●', fill_circle, 1.0, 0, false);
        S(L'◉', draw_fish_eye, 0);
        SS(0x25f8, corner_triangle_outline(c, 1, TOP_LEFT));
        SS(0x25f9, corner_triangle_outline(c, 1, TOP_RIGHT));
        SS(0x25fa, corner_triangle_outline(c, 1, BOTTOM_LEFT));
        SS(0x25ff, corner_triangle_outline(c, 1, BOTTOM_RIGHT));
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
