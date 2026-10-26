/*
 * sprite-face-branch.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Box Drawing (branch/nerd font) | U+F5D0...U+F60D

#include "sprite-face-private.h"

// Additional drawing macros used by the cases below

#define t 1u

#define P(ch, lines)            \
    S(ch, commit, lines, true); \
    S(ch + 1, commit, lines, false);


const SpriteRangeDef sprite_face_ranges_branch[] = {
    {0xf5d0, 0xf60d},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_branch(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        C(L'', hline, 1);
        C(L'', vline, 1);
        C(L'', fading_hline, 1, 4, RIGHT_EDGE);
        C(L'', fading_hline, 1, 4, LEFT_EDGE);
        C(L'', fading_vline, 1, 5, BOTTOM_EDGE);
        C(L'', fading_vline, 1, 5, TOP_EDGE);
        C(L'', rounded_corner, 1, TOP_LEFT);
        C(L'', rounded_corner, 1, TOP_RIGHT);
        C(L'', rounded_corner, 1, BOTTOM_LEFT);
        C(L'', rounded_corner, 1, BOTTOM_RIGHT);
        CC(L'', vline(c, 1); rounded_corner(c, 1, BOTTOM_LEFT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, TOP_LEFT));
        CC(L'', rounded_corner(c, 1, BOTTOM_LEFT), rounded_corner(c, 1, TOP_LEFT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, TOP_RIGHT));
        CC(L'', rounded_corner(c, 1, TOP_RIGHT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, TOP_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, TOP_LEFT));
        CC(L'', rounded_corner(c, 1, TOP_LEFT), rounded_corner(c, 1, TOP_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, BOTTOM_LEFT));
        CC(L'', rounded_corner(c, 1, BOTTOM_LEFT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, BOTTOM_LEFT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, TOP_LEFT), rounded_corner(c, 1, TOP_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, TOP_RIGHT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, BOTTOM_LEFT), rounded_corner(c, 1, TOP_LEFT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, TOP_LEFT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', vline(c, 1); rounded_corner(c, 1, TOP_RIGHT), rounded_corner(c, 1, BOTTOM_LEFT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, TOP_LEFT), rounded_corner(c, 1, BOTTOM_RIGHT));
        CC(L'', hline(c, 1); rounded_corner(c, 1, TOP_RIGHT), rounded_corner(c, 1, BOTTOM_LEFT));
        P(L'', 0);
        P(L'', RIGHT_EDGE);
        P(L'', LEFT_EDGE);
        P(L'', LEFT_EDGE | RIGHT_EDGE);
        P(L'', BOTTOM_EDGE);
        P(L'', TOP_EDGE);
        P(L'', BOTTOM_EDGE | TOP_EDGE);
        P(L'', RIGHT_EDGE | BOTTOM_EDGE);
        P(L'', LEFT_EDGE | BOTTOM_EDGE);
        P(L'', RIGHT_EDGE | TOP_EDGE);
        P(L'', LEFT_EDGE | TOP_EDGE);
        P(L'', TOP_EDGE | BOTTOM_EDGE | RIGHT_EDGE);
        P(L'', TOP_EDGE | BOTTOM_EDGE | LEFT_EDGE);
        P(L'', LEFT_EDGE | RIGHT_EDGE | BOTTOM_EDGE);
        P(L'', LEFT_EDGE | RIGHT_EDGE | TOP_EDGE);
        P(L'', LEFT_EDGE | RIGHT_EDGE | TOP_EDGE | BOTTOM_EDGE);
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
