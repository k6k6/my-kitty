/*
 * sprite-face-progress.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Progress bars and spinners (fira code) | U+EE00...U+EE0B

#include "sprite-face-private.h"

const SpriteRangeDef sprite_face_ranges_progress[] = {
    {0xee00, 0xee0b},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_progress(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    (void)ss;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        C(L'', progress_bar, LEFT, false);
        C(L'', progress_bar, MIDDLE, false);
        C(L'', progress_bar, RIGHT, false);
        C(L'', progress_bar, LEFT, true);
        C(L'', progress_bar, MIDDLE, true);
        C(L'', progress_bar, RIGHT, true);
        C(L'', spinner, 1, 235, 305);
        C(L'', spinner, 1, 270, 390);
        C(L'', spinner, 1, 315, 470);
        C(L'', spinner, 1, 360, 540);
        C(L'', spinner, 1, 80, 220);
        C(L'', spinner, 1, 170, 270);
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
