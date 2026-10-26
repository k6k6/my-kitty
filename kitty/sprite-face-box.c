/*
 * sprite-face-box.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Box Drawing | U+2500...U+257F

#include "sprite-face-private.h"

// Additional drawing macros used by the cases below

#define t 1u

#define f 3u

#define Q(ch, which)                \
    C(ch, corner, t, t, which);     \
    C(ch + 1, corner, f, t, which); \
    C(ch + 2, corner, t, f, which); \
    C(ch + 3, corner, f, f, which);

#define T(q, func) \
    case q ... q + 7: func(c, q, ch - q); break;


const SpriteRangeDef sprite_face_ranges_box[] = {
    {0x2500, 0x257f},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_box(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        C(L'─', hline, 1);
        C(L'━', hline, 3);
        C(L'│', vline, 1);
        C(L'┃', vline, 3);
        C(L'╌', hholes, 1, 1);
        C(L'╍', hholes, 3, 1);
        C(L'┄', hholes, 1, 2);
        C(L'┅', hholes, 3, 2);
        C(L'┈', hholes, 1, 3);
        C(L'┉', hholes, 3, 3);
        C(L'╎', vholes, 1, 1);
        C(L'╏', vholes, 3, 1);
        C(L'┆', vholes, 1, 2);
        C(L'┇', vholes, 3, 2);
        C(L'┊', vholes, 1, 3);
        C(L'┋', vholes, 3, 3);
        C(L'╴', half_hline, 1, false, 0);
        C(L'╵', half_vline, 1, false, 0);
        C(L'╶', half_hline, 1, true, 0);
        C(L'╷', half_vline, 1, true, 0);
        C(L'╸', half_hline, 3, false, 0);
        C(L'╹', half_vline, 3, false, 0);
        C(L'╺', half_hline, 3, true, 0);
        C(L'╻', half_vline, 3, true, 0);
        CC(L'╾', half_hline(c, 3, false, 0); half_hline(c, 1, true, 0));
        CC(L'╼', half_hline(c, 1, false, 0); half_hline(c, 3, true, 0));
        CC(L'╿', half_vline(c, 3, false, 0); half_vline(c, 1, true, 0));
        CC(L'╽', half_vline(c, 1, false, 0); half_vline(c, 3, true, 0));
        S(L'╲', cross_line, 1, true);
        S(L'╱', cross_line, 1, false);
        SS(L'╳', cross_line(c, 1, false); cross_line(c, 1, true));
        C(L'═', dhline, 1, TOP_EDGE | BOTTOM_EDGE);
        C(L'║', dvline, 1, LEFT_EDGE | RIGHT_EDGE);
        CC(L'╞', vline(c, 1); half_dhline(c, 1, true, TOP_EDGE | BOTTOM_EDGE));
        CC(L'╡', vline(c, 1); half_dhline(c, 1, false, TOP_EDGE | BOTTOM_EDGE));
        CC(L'╥', hline(c, 1); half_dvline(c, 1, true, LEFT_EDGE | RIGHT_EDGE));
        CC(L'╨', hline(c, 1); half_dvline(c, 1, false, LEFT_EDGE | RIGHT_EDGE));
        CC(L'╪', vline(c, 1); dhline(c, 1, TOP_EDGE | BOTTOM_EDGE));
        CC(L'╫', hline(c, 1), dvline(c, 1, LEFT_EDGE | RIGHT_EDGE));
        CC(L'╬', inner_corner(c, 1, TOP_LEFT); inner_corner(c, 1, TOP_RIGHT); inner_corner(c, 1, BOTTOM_LEFT); inner_corner(c, 1, BOTTOM_RIGHT));
        CC(L'╠', inner_corner(c, 1, TOP_RIGHT); inner_corner(c, 1, BOTTOM_RIGHT); dvline(c, 1, LEFT_EDGE));
        CC(L'╣', inner_corner(c, 1, TOP_LEFT); inner_corner(c, 1, BOTTOM_LEFT); dvline(c, 1, RIGHT_EDGE));
        CC(L'╦', inner_corner(c, 1, BOTTOM_LEFT); inner_corner(c, 1, BOTTOM_RIGHT); dhline(c, 1, TOP_EDGE));
        CC(L'╩', inner_corner(c, 1, TOP_LEFT); inner_corner(c, 1, TOP_RIGHT); dhline(c, 1, BOTTOM_EDGE));
        Q(L'┌', BOTTOM_RIGHT);
        Q(L'┐', BOTTOM_LEFT);
        Q(L'└', TOP_RIGHT);
        Q(L'┘', TOP_LEFT);
        C(L'╭', rounded_corner, 1, TOP_LEFT);
        C(L'╮', rounded_corner, 1, TOP_RIGHT);
        C(L'╰', rounded_corner, 1, BOTTOM_LEFT);
        C(L'╯', rounded_corner, 1, BOTTOM_RIGHT);
        case L'┼' ... L'┼' + 15: cross(c, ch - L'┼'); break;
        T(L'├', vert_t);
        T(L'┤', vert_t);
        T(L'┬', horz_t);
        T(L'┴', horz_t);
        C(L'╒', dvcorner, 1, TOP_LEFT);
        C(L'╕', dvcorner, 1, TOP_RIGHT);
        C(L'╘', dvcorner, 1, BOTTOM_LEFT);
        C(L'╛', dvcorner, 1, BOTTOM_RIGHT);
        C(L'╓', dhcorner, 1, TOP_LEFT);
        C(L'╖', dhcorner, 1, TOP_RIGHT);
        C(L'╙', dhcorner, 1, BOTTOM_LEFT);
        C(L'╜', dhcorner, 1, BOTTOM_RIGHT);
        C(L'╔', dcorner, 1, TOP_LEFT);
        C(L'╗', dcorner, 1, TOP_RIGHT);
        C(L'╚', dcorner, 1, BOTTOM_LEFT);
        C(L'╝', dcorner, 1, BOTTOM_RIGHT);
        C(L'╟', dpip, 1, RIGHT_EDGE);
        C(L'╢', dpip, 1, LEFT_EDGE);
        C(L'╤', dpip, 1, BOTTOM_EDGE);
        C(L'╧', dpip, 1, TOP_EDGE);
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
