/*
 * sprite-face-braille.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// Braille Patterns | U+2800...U+28FF

#include "sprite-face-private.h"

const SpriteRangeDef sprite_face_ranges_braille[] = {
    {0x2800, 0x28ff},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_braille(char_type ch, Canvas *canvas, Canvas *ss) {
    Canvas *c = canvas;
    (void)ss;
    START_ALLOW_CASE_RANGE
    switch (ch) {
        case 0x2800 ... 0x2800 + 255: braille(c, ch - 0x2800); break;
        default: break;
    }
    END_ALLOW_CASE_RANGE
}
