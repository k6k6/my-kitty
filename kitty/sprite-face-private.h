/*
 * sprite-face-private.h
 * Copyright (C) 2026 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#include "sprite-canvas.h"
#include "sprite-face.h"

// Macros shared by all the sprite face implementations. They expand to case
// labels of a switch (ch) inside a face draw function and expect the parameters
// canvas (the Canvas to draw into), ss (its supersampled companion) and a
// Canvas * named c which is the canvas the current case draws on.
//
// X(ch, ...) draws the single glyph ch. The lowercase variants draw on the plain
// canvas, the uppercase ones (and the B suffix) draw on the supersampled canvas
// and then downsample, which gives anti-aliased edges. CC/SS end the case, SB
// does not, so that more code can be run after the drawing.
//
// The canvases are owned by the caller, the faces must not free them.

#define SB(ch, ...)           \
    case ch:                  \
        fill_canvas(ss, 0);   \
        c = ss, __VA_ARGS__;  \
        downsample(ss, canvas);

#define CC(ch, ...) \
    case ch: __VA_ARGS__; break

#define SS(ch, ...)      \
    SB(ch, __VA_ARGS__); \
    break

#define C(ch, func, ...) CC(ch, func(c, __VA_ARGS__))
#define S(ch, func, ...) SS(ch, func(c, __VA_ARGS__))

static inline uint
sprite_minus(uint a, uint b) { // saturating subtraction (a > b ? a - b : 0)
    uint res = a - b;
    res &= -(res <= a);
    return res;
}

#define minus sprite_minus
