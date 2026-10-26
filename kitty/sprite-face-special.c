/*
 * sprite-face-special.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

// The special sprites: things that are part of the rendering model rather than
// characters, such as the box drawn for a missing glyph and the text cursors.
// These are not looked up by codepoint, they are rendered once per font group
// and always live at fixed indices in the sprite texture, see
// send_prerendered_sprites() in fonts.c and SPRITE_IDX_* in sprite-face.h
// (which must be kept in sync with the shaders and with fonts.c)

#include "decorations.h"
#include "sprite-face-private.h"

const SpriteRangeDef sprite_face_ranges_special[] = {
    {SPRITE_IDX_BLANK, SPRITE_IDX_HOLLOW_CURSOR},
    SPRITE_RANGE_END,
};

void
sprite_face_draw_special(char_type ch, Canvas *canvas, Canvas *supersampled UNUSED) {
    switch (ch) {
        case SPRITE_IDX_MISSING_GLYPH: add_missing_glyph(canvas->mask, canvas->fcm); break;
        case SPRITE_IDX_BEAM_CURSOR: add_beam_cursor(canvas->mask, canvas->fcm, canvas->dpi.x, canvas->scale); break;
        case SPRITE_IDX_UNDERLINE_CURSOR: add_underline_cursor(canvas->mask, canvas->fcm, canvas->dpi.y, canvas->scale); break;
        case SPRITE_IDX_HOLLOW_CURSOR: add_hollow_cursor(canvas->mask, canvas->fcm, canvas->dpi.x, canvas->dpi.y, canvas->scale); break;
        default: break; // BLANK
    }
}
