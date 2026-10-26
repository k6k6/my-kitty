/*
 * sprite-face-block.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Block Elements | U+2580...U+259F

#include "sprite-face-private.h"

// Additional drawing macros used by the cases below

#define EH(ch, ...) C(ch, eight_block, true, __VA_ARGS__, -1);

#define EV(ch, ...) C(ch, eight_block, false, __VA_ARGS__, -1);


const SpriteRangeDef sprite_face_ranges_block[] = {
    {0x2580, 0x259f},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_block(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    (void)ss;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        case L'█':
        fill_canvas(c, 255);
        break;
        EH(L'▔', 0);
        EH(L'▀', 0, 1, 2, 3);
        EH(L'▁', 7);
        EH(L'▂', 6, 7);
        EH(L'▃', 5, 6, 7);
        EH(L'▄', 4, 5, 6, 7);
        EH(L'▅', 3, 4, 5, 6, 7);
        EH(L'▆', 2, 3, 4, 5, 6, 7);
        EH(L'▇', 1, 2, 3, 4, 5, 6, 7);
        EV(L'▉', 0, 1, 2, 3, 4, 5, 6);
        EV(L'▊', 0, 1, 2, 3, 4, 5);
        EV(L'▋', 0, 1, 2, 3, 4);
        EV(L'▌', 0, 1, 2, 3);
        EV(L'▍', 0, 1, 2);
        EV(L'▎', 0, 1);
        EV(L'▏', 0);
        EV(L'▕', 7);
        EV(L'▐', 4, 5, 6, 7);
        // The shade characters are rendered as uniform blocks of gray, see
        // SHADE_*_ALPHA in sprite-canvas.h
        C(L'░', fill_canvas, SHADE_LIGHT_ALPHA);
        C(L'▒', fill_canvas, SHADE_MEDIUM_ALPHA);
        C(L'▓', fill_canvas, SHADE_DARK_ALPHA);
        C(L'▖', quad, BOTTOM_LEFT);
        C(L'▗', quad, BOTTOM_RIGHT);
        C(L'▘', quad, TOP_LEFT);
        C(L'▝', quad, TOP_RIGHT);
        C(L'▙', quads, TOP_LEFT, BOTTOM_LEFT, BOTTOM_RIGHT, 0);
        C(L'▚', quads, TOP_LEFT, BOTTOM_RIGHT, 0);
        C(L'▛', quads, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, 0);
        C(L'▜', quads, TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, 0);
        C(L'▞', quads, TOP_RIGHT, BOTTOM_LEFT, 0);
        C(L'▟', quads, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, 0);
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
