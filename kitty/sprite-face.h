/*
 * sprite-face.h
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#include "data-types.h"
#include "sprite-canvas.h"

// A sprite face is not really a font face. It is a lookup table that maps
// codepoints to hard coded drawing routines. It is used for characters that are
// better drawn by us than by a font: box drawing characters (so that lines meet
// seamlessly across cells), block elements, braille, powerline separators,
// symbols for legacy computing and so on. The equivalent in ghostty is
// src/font/sprite/Face.zig
//
// Each face owns a set of codepoint ranges and a draw function. The draw
// function is given a canvas to draw the glyph for a single codepoint into. The
// canvas is in terms of pixels of a single terminal cell, so the same code works
// for any font size/dpi. Faces are looked up in sorted order by codepoint, so
// adding support for a new range of codepoints is just a matter of adding the
// range to a face and a case to its draw function.
//
// Glyph ids: every codepoint handled by a face is assigned a synthetic glyph id
// (unique across all faces) which is used as the key for the sprite cache, see
// sprite_position_for() in fonts.c. The ids are assigned by the registry at
// registration time, so they are stable for the lifetime of the process, but
// must never be persisted anywhere.

// The sprites that are always present in the sprite texture, they are rendered
// once when the font group is created, see send_prerendered_sprites() in
// fonts.c. These indices are hardcoded in the shaders as well, so do not
// reorder them.
enum {
    SPRITE_IDX_BLANK = 0,
    SPRITE_IDX_MISSING_GLYPH = 1,
    SPRITE_IDX_BEAM_CURSOR = 2,
    SPRITE_IDX_UNDERLINE_CURSOR = 3,
    SPRITE_IDX_HOLLOW_CURSOR = 4,
    NUM_OF_SPECIAL_SPRITES = 5,
};

typedef struct SpriteRange {
    char_type first, last; // inclusive
    glyph_index id_base;   // synthetic glyph id assigned to first, ids are contiguous
} SpriteRange;

typedef struct SpriteRangeDef {
    char_type first, last; // inclusive
} SpriteRangeDef;

// Sentinel that terminates the range table of a face.
#define SPRITE_RANGE_END {0, 0}

typedef struct SpriteFace {
    const char *name;
    SpriteRange *ranges;
    size_t num_ranges;
    void (*draw)(char_type cp, Canvas *canvas, Canvas *supersampled);
    PyObject *py_draw;  // when set, drawing is delegated to this python callable
    bool enabled;
    bool is_special;    // internal sprites, not looked up by codepoint (cursors, etc.)
} SpriteFace;

// Implemented by each face file. The name of the face is used for the names of
// both its draw function and its range table, so adding a face is just a matter
// of implementing the two and adding one entry to builtin_faces in sprite-face.c
#define SPRITE_FACE_DECL(nm)                                                      \
    void sprite_face_draw_##nm(char_type cp, Canvas *canvas, Canvas *supersampled); \
    extern const SpriteRangeDef sprite_face_ranges_##nm[];

void sprite_faces_init(void);
void sprite_faces_cleanup(void);

// Returns NULL if the codepoint is not handled by any enabled face.
SpriteFace *sprite_face_for_char(char_type cp);
// True if the codepoint is drawn by one of the sprite faces
bool is_sprite_char(char_type cp);
glyph_index sprite_face_glyph_id_for_char(char_type cp);
// Render one of the special sprites (used for the pre-rendered sprites such as
// the missing glyph box and the cursors). See sprite-face-special.c for the
// mapping of ids to sprites.
bool sprite_face_render_special(glyph_index id, Canvas *canvas);
// Renders ch using the sprite faces, falling back to a blank cell (and an error
// in the log) if it is not handled by any face. This is the entrypoint used by
// the font rendering code and by the python API.
void render_sprite_char(char_type ch, Canvas *canvas, Canvas *supersampled);

// The codepoints that use a sprite face are exposed to python for tests and for
// users that want to know what kitty draws itself.
PyObject *sprite_faces_as_python(void);

// Add a face whose drawing is implemented in python. The callable is invoked as
// draw(cp, width, height, dpi_x, dpi_y, scale) and must return a bytes object of
// length width*height, one alpha value per pixel. Returns NULL on failure.
PyObject *add_python_sprite_face(PyObject *name, PyObject *ranges, PyObject *draw);
PyObject *remove_sprite_face(PyObject *name);
// Enable only the faces named in the sequence (a name that is not a face is an
// error). Passing NULL or an empty sequence disables all the codepoint based
// faces, in which case the regular fonts are used for those codepoints instead.
// The special sprites (cursors and the missing glyph box) are not affected, as
// they are part of the rendering model rather than characters.
PyObject *set_enabled_sprite_faces(PyObject *names);
