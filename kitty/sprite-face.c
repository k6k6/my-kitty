/*
 * sprite-face.c
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "sprite-face.h"
#include "state.h"

#include <stddef.h>

#define debug debug_fonts

// The glyph ids used for the sprites of a face are just hash keys for the
// sprite cache, they are never sent to the GPU, so they only need to be unique.
// Builtin faces get the low range, the range after it is reserved for faces
// added at runtime (by users) so that adding/removing such a face never changes
// the ids of an already cached sprite. The upper bound is dictated by
// glyph_index being a 16 bit type and 0xffff being reserved as an invalid id.
#define SPRITE_FACE_MAX_PYTHON_ID_BASE 0x8000
#define MAX_SPRITE_FACES 64

SPRITE_FACE_DECL(box)
SPRITE_FACE_DECL(block)
SPRITE_FACE_DECL(braille)
SPRITE_FACE_DECL(branch)
SPRITE_FACE_DECL(geometric_shapes)
SPRITE_FACE_DECL(powerline)
SPRITE_FACE_DECL(progress)
SPRITE_FACE_DECL(legacy_computing)
SPRITE_FACE_DECL(legacy_computing_supplement)
SPRITE_FACE_DECL(special)

typedef struct FaceSpec {
    const char *name;
    const SpriteRangeDef *ranges;
    void (*draw)(char_type cp, Canvas *canvas, Canvas *supersampled);
    bool is_special;
} FaceSpec;

static const FaceSpec builtin_faces[] = {
    {.name = "box", .ranges = sprite_face_ranges_box, .draw = sprite_face_draw_box},
    {.name = "block", .ranges = sprite_face_ranges_block, .draw = sprite_face_draw_block},
    {.name = "braille", .ranges = sprite_face_ranges_braille, .draw = sprite_face_draw_braille},
    {.name = "branch", .ranges = sprite_face_ranges_branch, .draw = sprite_face_draw_branch},
    {.name = "geometric_shapes", .ranges = sprite_face_ranges_geometric_shapes, .draw = sprite_face_draw_geometric_shapes},
    {.name = "powerline", .ranges = sprite_face_ranges_powerline, .draw = sprite_face_draw_powerline},
    {.name = "progress", .ranges = sprite_face_ranges_progress, .draw = sprite_face_draw_progress},
    {.name = "legacy_computing", .ranges = sprite_face_ranges_legacy_computing, .draw = sprite_face_draw_legacy_computing},
    {
        .name = "legacy_computing_supplement",
        .ranges = sprite_face_ranges_legacy_computing_supplement,
        .draw = sprite_face_draw_legacy_computing_supplement,
    },
    {.name = "special", .ranges = sprite_face_ranges_special, .draw = sprite_face_draw_special, .is_special = true},
};

static SpriteFace faces[MAX_SPRITE_FACES];
static size_t num_faces = 0, num_builtin_faces = 0;
static glyph_index next_builtin_id = 0, next_python_id = SPRITE_FACE_MAX_PYTHON_ID_BASE;
static bool initialized = false;

typedef struct LookupEntry {
    char_type first, last;
    uint16_t face_idx, range_idx;
} LookupEntry;

static LookupEntry *lookup = NULL;
static size_t lookup_count = 0, lookup_capacity = 0;
static bool lookup_is_dirty = true;

static size_t
count_ranges(const SpriteRangeDef *defs) {
    size_t ans = 0;
    while (defs[ans].first || defs[ans].last) ans++;
    return ans;
}

static glyph_index
alloc_ids(glyph_index *next, char_type first, char_type last) {
    const size_t count = (size_t)last - first + 1;
    if ((size_t)*next + count >= 0xffff) fatal("Too many sprite glyphs, out of glyph ids");
    glyph_index ans = *next;
    *next += count;
    return ans;
}

// Returns an index into faces or -1
static ssize_t
register_face(const char *name, const SpriteRangeDef *defs, void (*draw)(char_type, Canvas *, Canvas *), bool is_special) {
    if (num_faces >= MAX_SPRITE_FACES) fatal("Too many sprite faces, increase MAX_SPRITE_FACES");
    const size_t n = count_ranges(defs);
    if (!n) fatal("Sprite face %s has no codepoint ranges", name);
    SpriteFace *f = faces + num_faces;
    zero_at_ptr(f);
    f->name = name;
    f->draw = draw;
    f->enabled = true;
    f->is_special = is_special;
    f->num_ranges = n;
    f->ranges = calloc(n, sizeof(f->ranges[0]));
    if (!f->ranges) fatal("Out of memory allocating sprite face ranges");
    for (size_t i = 0; i < n; i++) {
        SpriteRange *r = f->ranges + i;
        if (defs[i].first > defs[i].last) fatal("Sprite face %s has an inverted range", name);
        r->first = defs[i].first;
        r->last = defs[i].last;
        // Special sprites are referenced by their sprite index, not by codepoint,
        // so their ranges are sprite indices and need no synthetic glyph ids.
        r->id_base = is_special ? 0 : alloc_ids(&next_builtin_id, r->first, r->last);
    }
    lookup_is_dirty = true;
    return (ssize_t)(num_faces++);
}

static int
cmpr_lookup_entry(const void *a, const void *b) {
    const LookupEntry *x = a, *y = b;
    if (x->first < y->first) return -1;
    if (x->first > y->first) return 1;
    return 0;
}

static void
rebuild_lookup(void) {
    lookup_count = 0;
    for (size_t i = 0; i < num_faces; i++) {
        const SpriteFace *f = faces + i;
        if (!f->enabled || f->is_special) continue;
        if (lookup_count + f->num_ranges > lookup_capacity) {
            lookup_capacity = MAX(64u, 2 * (lookup_count + f->num_ranges));
            lookup = realloc(lookup, lookup_capacity * sizeof(lookup[0]));
            if (!lookup) fatal("Out of memory growing the sprite face lookup table");
        }
        for (size_t r = 0; r < f->num_ranges; r++) {
            lookup[lookup_count].first = f->ranges[r].first;
            lookup[lookup_count].last = f->ranges[r].last;
            lookup[lookup_count].face_idx = (uint16_t)i;
            lookup[lookup_count].range_idx = (uint16_t)r;
            lookup_count++;
        }
    }
    qsort(lookup, lookup_count, sizeof(lookup[0]), cmpr_lookup_entry);
    for (size_t i = 1; i < lookup_count; i++) {
        if (lookup[i].first <= lookup[i - 1].last) {
            fatal(
                "Overlapping sprite face ranges: %s handles up to U+%x while %s handles from U+%x", faces[lookup[i - 1].face_idx].name,
                lookup[i - 1].last, faces[lookup[i].face_idx].name, lookup[i].first);
        }
    }
    lookup_is_dirty = false;
}

static const LookupEntry *
find_lookup_entry(char_type cp) {
    if (lookup_is_dirty) rebuild_lookup();
    // The lookup table is sorted by the first codepoint of each range, the last
    // entry is not necessarily the one with the largest codepoints in it.
    if (!lookup_count || cp < lookup[0].first) return NULL;
    size_t lo = 0, hi = lookup_count;
    while (lo < hi) {
        const size_t mid = lo + (hi - lo) / 2;
        if (lookup[mid].first <= cp) lo = mid + 1;
        else hi = mid;
    }
    if (!lo) return NULL;
    const LookupEntry *e = lookup + lo - 1;
    return cp <= e->last ? e : NULL;
}

static void
init_faces(void) {
    if (initialized) return;
    initialized = true;
    for (size_t i = 0; i < arraysz(builtin_faces); i++) {
        const FaceSpec *s = builtin_faces + i;
        if (register_face(s->name, s->ranges, s->draw, s->is_special) < 0) fatal("Failed to register sprite face %s", s->name);
    }
    num_builtin_faces = num_faces;
    rebuild_lookup();
}

void
sprite_faces_init(void) {
    init_faces();
}

void
sprite_faces_cleanup(void) {
    for (size_t i = 0; i < num_faces; i++) {
        free(faces[i].ranges);
        faces[i].ranges = NULL;
        Py_CLEAR(faces[i].py_draw);
        if (i >= num_builtin_faces) free((void *)faces[i].name);
        faces[i].name = NULL;
    }
    num_faces = 0;
    num_builtin_faces = 0;
    free(lookup);
    lookup = NULL;
    lookup_count = lookup_capacity = 0;
    lookup_is_dirty = true;
    initialized = false;
    next_builtin_id = 0;
    next_python_id = SPRITE_FACE_MAX_PYTHON_ID_BASE;
}

SpriteFace *
sprite_face_for_char(char_type cp) {
    init_faces();
    const LookupEntry *e = find_lookup_entry(cp);
    return e ? faces + e->face_idx : NULL;
}

bool
is_sprite_char(char_type cp) {
    return sprite_face_for_char(cp) != NULL;
}

glyph_index
sprite_face_glyph_id_for_char(char_type cp) {
    init_faces();
    const LookupEntry *e = find_lookup_entry(cp);
    if (!e) return 0xffff;
    const SpriteRange *r = faces[e->face_idx].ranges + e->range_idx;
    return r->id_base + (glyph_index)(cp - r->first);
}

// Draws using a python callable. The callable is invoked as
// draw(cp, width, height, dpi_x, dpi_y, scale) and must return a bytes object of
// length width*height, one alpha value per pixel.
static bool
render_python(SpriteFace *face, char_type cp, Canvas *canvas) {
    RAII_PyObject(ans, PyObject_CallFunction(face->py_draw, "Ikkddd", cp, (unsigned long)canvas->width, (unsigned long)canvas->height, canvas->dpi.x, canvas->dpi.y, canvas->scale));
    if (!ans) {
        PyErr_Print();
        return false;
    }
    if (!PyBytes_Check(ans)) {
        PyErr_Format(PyExc_TypeError, "The draw callback of sprite face %s returned a %s instead of a bytes object", face->name, Py_TYPE(ans)->tp_name);
        PyErr_Print();
        return false;
    }
    const size_t sz = (size_t)PyBytes_GET_SIZE(ans);
    const size_t needed = (size_t)canvas->width * canvas->height;
    if (sz != needed) {
        PyErr_Format(
            PyExc_ValueError, "The draw callback of sprite face %s returned %zu bytes, it must return %zu bytes (width * height)", face->name, sz, needed);
        PyErr_Print();
        return false;
    }
    memcpy(canvas->mask, PyBytes_AS_STRING(ans), needed);
    return true;
}

static bool
render_face(SpriteFace *face, char_type cp, Canvas *canvas, Canvas *supersampled) {
    if (!face->enabled) return false;
    memset(canvas->mask, 0, (size_t)canvas->width * canvas->height);
    if (face->py_draw) return render_python(face, cp, canvas);
    if (!face->draw) return false;
    // The face draws into the canvases, it does not own them, so the scratch
    // space allocated while drawing is freed by our caller.
    face->draw(cp, canvas, supersampled);
    return true;
}

bool
sprite_face_render_special(glyph_index id, Canvas *canvas) {
    init_faces();
    for (size_t i = 0; i < num_faces; i++) {
        SpriteFace *f = faces + i;
        if (!f->is_special || !f->enabled) continue;
        for (size_t r = 0; r < f->num_ranges; r++) {
            const SpriteRange *range = f->ranges + r;
            if (id >= range->first && id <= range->last) return render_face(f, id, canvas, NULL);
        }
    }
    return false;
}

void
render_sprite_char(char_type ch, Canvas *canvas, Canvas *supersampled) {
    SpriteFace *face = sprite_face_for_char(ch);
    if (face) {
        render_face(face, ch, canvas, supersampled);
        return;
    }
    log_error("Unknown box drawing character: U+%x rendered as blank", ch);
    memset(canvas->mask, 0, (size_t)canvas->width * canvas->height);
}

PyObject *
sprite_faces_as_python(void) {
    init_faces();
    RAII_PyObject(ans, PyList_New(0));
    if (!ans) return NULL;
    for (size_t i = 0; i < num_faces; i++) {
        const SpriteFace *f = faces + i;
        RAII_PyObject(ranges, PyList_New(0));
        if (!ranges) return NULL;
        for (size_t r = 0; r < f->num_ranges; r++) {
            RAII_PyObject(item, Py_BuildValue("(II)", f->ranges[r].first, f->ranges[r].last));
            if (!item) return NULL;
            if (PyList_Append(ranges, item) != 0) return NULL;
        }
        RAII_PyObject(entry, Py_BuildValue("{s:s, s:O, s:O, s:O}", "name", f->name, "ranges", ranges, "is_special", f->is_special ? Py_True : Py_False, "enabled", f->enabled ? Py_True : Py_False));
        if (!entry) return NULL;
        if (PyList_Append(ans, entry) != 0) return NULL;
    }
    Py_INCREF(ans);
    return ans;
}

static SpriteFace *
find_face_by_name(const char *name) {
    for (size_t i = 0; i < num_faces; i++) {
        if (faces[i].name && strcmp(faces[i].name, name) == 0) return faces + i;
    }
    return NULL;
}

PyObject *
add_python_sprite_face(PyObject *name, PyObject *ranges, PyObject *draw) {
    init_faces();
    if (!PyUnicode_Check(name)) {
        PyErr_SetString(PyExc_TypeError, "The name of a sprite face must be a string");
        return NULL;
    }
    const char *n = PyUnicode_AsUTF8(name);
    if (!n) return NULL;
    if (!PyCallable_Check(draw)) {
        PyErr_SetString(PyExc_TypeError, "The draw callback of a sprite face must be callable");
        return NULL;
    }
    RAII_PyObject(ranges_list, PySequence_List(ranges));
    if (!ranges_list) return NULL;
    const Py_ssize_t num_ranges = PyList_GET_SIZE(ranges_list);
    if (num_ranges < 1) {
        PyErr_SetString(PyExc_ValueError, "A sprite face must have at least one codepoint range");
        return NULL;
    }
    if (num_faces >= MAX_SPRITE_FACES) {
        PyErr_SetString(PyExc_RuntimeError, "Too many sprite faces");
        return NULL;
    }
    if (find_face_by_name(n)) {
        PyErr_Format(PyExc_ValueError, "A sprite face named %s already exists", n);
        return NULL;
    }
    RAII_ALLOC(SpriteRange, rs, calloc(num_ranges, sizeof(SpriteRange)));
    if (!rs) {
        PyErr_NoMemory();
        return NULL;
    }
    glyph_index next = next_python_id;
    for (Py_ssize_t i = 0; i < num_ranges; i++) {
        unsigned long first, last;
        if (!PyArg_ParseTuple(PyList_GET_ITEM(ranges_list, i), "kk", &first, &last)) return NULL;
        if (first > last) {
            PyErr_SetString(PyExc_ValueError, "The first codepoint of a range must be <= the last");
            return NULL;
        }
        if (last >= 0x110000) {
            PyErr_SetString(PyExc_ValueError, "Invalid codepoint in sprite face range");
            return NULL;
        }
        rs[i].first = (char_type)first;
        rs[i].last = (char_type)last;
        rs[i].id_base = alloc_ids(&next, rs[i].first, rs[i].last);
    }
    SpriteFace *f = faces + num_faces;
    zero_at_ptr(f);
    f->name = strdup(n);
    if (!f->name) {
        PyErr_NoMemory();
        return NULL;
    }
    f->ranges = malloc(num_ranges * sizeof(SpriteRange));
    if (!f->ranges) {
        free((void *)f->name);
        PyErr_NoMemory();
        return NULL;
    }
    memcpy(f->ranges, rs, num_ranges * sizeof(SpriteRange));
    f->num_ranges = (size_t)num_ranges;
    f->py_draw = Py_NewRef(draw);
    f->enabled = true;
    next_python_id = next;
    num_faces++;
    lookup_is_dirty = true;
    // The new face may claim codepoints that are already rendered/cached by the
    // regular fonts, so the caller must trigger a redraw.
    return Py_NewRef(name);
}

PyObject *
set_enabled_sprite_faces(PyObject *names) {
    init_faces();
    // The special sprites are part of the rendering model, not characters, so
    // they are always enabled.
    for (size_t i = 0; i < num_faces; i++) {
        if (!faces[i].is_special) faces[i].enabled = false;
    }
    if (names && names != Py_None) {
        RAII_PyObject(seq, PySequence_Fast(names, "the list of sprite face names must be a sequence"));
        if (!seq) return NULL;
        const Py_ssize_t n = PySequence_Fast_GET_SIZE(seq);
        for (Py_ssize_t i = 0; i < n; i++) {
            PyObject *item = PySequence_Fast_GET_ITEM(seq, i);
            if (!PyUnicode_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "The name of a sprite face must be a string");
                return NULL;
            }
            SpriteFace *f = find_face_by_name(PyUnicode_AsUTF8(item));
            if (!f) {
                PyErr_Format(PyExc_KeyError, "No sprite face named %s", PyUnicode_AsUTF8(item));
                return NULL;
            }
            f->enabled = true;
        }
    }
    lookup_is_dirty = true;
    Py_RETURN_NONE;
}

PyObject *
remove_sprite_face(PyObject *name) {
    init_faces();
    if (!PyUnicode_Check(name)) {
        PyErr_SetString(PyExc_TypeError, "The name of a sprite face must be a string");
        return NULL;
    }
    const char *n = PyUnicode_AsUTF8(name);
    if (!n) return NULL;
    SpriteFace *f = find_face_by_name(n);
    if (!f) {
        PyErr_Format(PyExc_KeyError, "No sprite face named %s", n);
        return NULL;
    }
    if (f < faces + num_builtin_faces) {
        // Builtin faces can only be disabled, as their glyph ids are baked into
        // the ordering of the builtin table.
        f->enabled = false;
        lookup_is_dirty = true;
        Py_RETURN_NONE;
    }
    // Note that the sprites already rendered for this face stay in the sprite
    // texture and in the glyph cache of the font groups. This is harmless, as
    // the glyph ids of a removed face are never reused, so they can only ever
    // be hit by the codepoints of the removed face, which now fall back to the
    // regular fonts. The only cost is a little wasted sprite texture space.
    free(f->ranges);
    Py_CLEAR(f->py_draw);
    free((void *)f->name);
    const size_t idx = (size_t)(f - faces);
    if (idx + 1 < num_faces) memmove(faces + idx, faces + idx + 1, (num_faces - idx - 1) * sizeof(SpriteFace));
    num_faces--;
    lookup_is_dirty = true;
    Py_RETURN_NONE;
}
