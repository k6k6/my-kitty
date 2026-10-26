/*
 * sprite-canvas.h
 * Copyright (C) 2024 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#include "data-types.h"
#include "state.h"

#include <stdarg.h>

// A canvas of alpha values (0 = transparent, 255 = opaque) that sprite faces
// draw in to. The primitives below are implemented in decorations.c and are
// shared between the text decorations (underlines, cursors, etc.) and the
// sprite faces (box drawing, blocks, braille, powerline, ...). They are
// internal to the sprite rendering subsystem: keep their names free of
// collisions with the rest of the C code.
//
// Cells are drawn in a coordinate system whose origin is the top left corner
// of the terminal cell being rendered. Line thicknesses are specified in
// points and are converted to pixels using dpi and scale, so that they look
// correct on high dpi screens and track the font size. Primitives that need to
// produce smooth (anti-aliased) output are drawn on a supersampled canvas (a
// Canvas whose supersample_factor is > 1, see sprite_canvas_supersampled())
// and then reduced to cell resolution with downsample().

typedef uint32_t uint;

typedef struct Range {
    uint start, end;
} Range;

typedef struct Limit {
    double upper, lower;
} Limit;
typedef struct FloatPoint {
    double x, y;
} FloatPoint;

typedef struct Canvas {
    uint8_t *mask;
    uint width, height, supersample_factor;
    struct {
        double x, y;
    } dpi;
    double scale; // used to scale line thickness with font size changes and for multicell rendering
    Range *holes;
    uint holes_count, holes_capacity;
    Limit *y_limits;
    uint y_limits_count, y_limits_capacity;
    // Metrics of the cell being drawn. Only needed by faces that draw things
    // whose thickness/position is defined by the font (cursors, decorations).
    FontCellMetrics fcm;
} Canvas;

typedef union Point {
    struct {
        int32_t x : 32, y : 32;
    };
    int64_t val;
} Point;

typedef struct StraightLine {
    double m, c;
} StraightLine;

typedef enum Corner {
    TOP_LEFT = LEFT_EDGE | TOP_EDGE,
    TOP_RIGHT = TOP_EDGE | RIGHT_EDGE,
    BOTTOM_LEFT = BOTTOM_EDGE | LEFT_EDGE,
    BOTTOM_RIGHT = BOTTOM_EDGE | RIGHT_EDGE,
} Corner;

typedef enum Segment { LEFT, MIDDLE, RIGHT } Segment;

typedef struct CubicBezier {
    Point start, c1, c2, end;
} CubicBezier;

typedef double (*curve_func)(const void *, double t);

typedef struct ClipRect {
    uint left, top, x_end, y_end;
} ClipRect;

typedef struct Circle {
    double x, y, radius;
    double start, end, amt;
} Circle;

typedef struct Ellipse {
    double x, y, rx, ry;
    double start, end, amt;
} Ellipse;

typedef struct Shade {
    bool light, invert, fill_blank;
    Edge which_half;
    uint xnum, ynum;
} Shade;

// The mask is followed in memory by space for a supersampled canvas, see
// sprite_canvas_supersampled(). All the space for both is owned by the caller.
static inline Canvas
sprite_canvas(uint8_t *mask, unsigned width, unsigned height, double dpi_x, double dpi_y, double scale) {
    Canvas ans = {.mask = mask, .width = width, .height = height, .supersample_factor = 1u, .dpi = {.x = dpi_x, .y = dpi_y}, .scale = scale};
    return ans;
}

// A canvas of SUPERSAMPLE_FACTOR x SUPERSAMPLE_FACTOR resolution that draws into
// the space immediately after canvas->mask. Draw into it and then call
// downsample(&ss, canvas) to get an anti-aliased result in canvas.
Canvas sprite_canvas_supersampled(Canvas *canvas);
// Free the scratch space (holes/y_limits) allocated while drawing into a canvas.
void sprite_canvas_free_scratch(Canvas *canvas);

void fill_canvas(Canvas *self, int byte);
void append_hole(Canvas *self, Range hole);
void append_limit(Canvas *self, double upper, double lower);
double thickness_as_float(Canvas *self, uint level, bool horizontal);
uint thickness(Canvas *self, uint level, bool horizontal);
void get_holes(Canvas *self, uint sz, uint hole_sz, uint num);
void add_hholes(Canvas *self, uint level, uint num);
void add_vholes(Canvas *self, uint level, uint num);
Range hline_limits(Canvas *self, uint y, uint level);
void draw_hline(Canvas *self, uint x1, uint x2, uint y, uint level);
Range vline_limits(Canvas *self, uint x, uint level);
void draw_vline(Canvas *self, uint y1, uint y2, uint x, uint level);
uint half_width(Canvas *self);
uint half_height(Canvas *self);
double unit_double(double x);
double smoothstep(double edge0, double edge1, double x);
void half_hline(Canvas *self, uint level, bool right_half, uint extend_by);
Point half_dhline(Canvas *self, uint level, bool right_half, Edge which);
Point half_dvline(Canvas *self, uint level, bool bottom_half, Edge which);
Point dhline(Canvas *self, uint level, Edge which);
Point dvline(Canvas *self, uint level, Edge which);
void half_vline(Canvas *self, uint level, bool bottom_half, uint extend_by);
void hline(Canvas *self, uint level);
void vline(Canvas *self, uint level);
void hholes(Canvas *self, uint level, uint num);
void vholes(Canvas *self, uint level, uint num);
uint8_t plus(uint8_t a, uint8_t b);
uint8_t average_intensity(const Canvas *src, uint dest_x, uint dest_y);
void downsample(const Canvas *src, Canvas *dest);
void invert_mask(Canvas *self);
StraightLine line_from_points(double x1, double y1, double x2, double y2);
double line_y(StraightLine l, int x);
void fill_region(Canvas *self, bool inverted);
void triangle(Canvas *self, bool left, bool inverted);
void thick_line(Canvas *self, uint thickness_in_pixels, Point p1, Point p2);
void frame(Canvas *self, uint level, Edge edges);
void progress_bar(Canvas *self, Segment which, bool filled);
uint diagonal_thickness(uint base, Point p1, Point p2);
void half_cross_line(Canvas *self, uint level, Corner corner);
void cross_line(Canvas *self, uint level, bool left);
double bezier_x(const void *v, double t);
double bezier_y(const void *v, double t);
double bezier_prime_x(const void *v, double t);
double bezier_prime_y(const void *v, double t);
int find_bezier_for_D(int width, int height);
double find_t_for_x(const CubicBezier *cb, int x, double start_t);
void get_bezier_limits(Canvas *self, const CubicBezier *cb);
void filled_D(Canvas *self, bool left);
uint64_t hash_point(Point p);
bool cmpr_point(Point a, Point b);
void draw_parametrized_curve_with_derivative_and_antialiasing(
    Canvas *self, void *curve_data, double line_width, curve_func xfunc, curve_func yfunc, curve_func x_prime, curve_func y_prime, double x_offset,
    double y_offset, const ClipRect *clip_to);
void rounded_separator(Canvas *self, uint level, bool left);
void corner_triangle(Canvas *self, const Corner corner);
void corner_triangle_outline(Canvas *self, uint level, Corner corner);
Circle circle(double x, double y, double radius, double start_at, double end_at);
double circle_x(const void *v, double t);
double circle_y(const void *v, double t);
double circle_prime_x(const void *v, double t);
double circle_prime_y(const void *v, double t);
Ellipse ellipse(double x, double y, double rx, double ry, double start_at, double end_at);
double ellipse_x(const void *v, double t);
double ellipse_y(const void *v, double t);
double ellipse_prime_x(const void *v, double t);
double ellipse_prime_y(const void *v, double t);
void spinner(Canvas *self, uint level, double start_degrees, double end_degrees);
void fill_circle_of_radius(Canvas *self, double origin_x, double origin_y, double radius, uint8_t alpha);
void fill_circle(Canvas *self, double scale, double gap, bool invert);
void draw_fish_eye(Canvas *self, uint level);
void inner_corner(Canvas *self, uint level, Corner corner);
Range fourth_range(uint size, uint which);
Range eight_range(uint size, uint which);
void eight_bar(Canvas *self, uint which, bool horizontal);
void octant_segment(Canvas *self, uint8_t which, bool left);
void octant(Canvas *self, uint8_t which);
void eight_block(Canvas *self, int horizontal, ...);
void shade(Canvas *self, Shade s);
void apply_mask(Canvas *self, uint8_t *mask);
void cross_shade(Canvas *self, bool rotate);
void quad(Canvas *self, Corner which);
void quads(Canvas *self, ...);
void smooth_mosaic(Canvas *self, bool lower, double ax, double ay, double bx, double by);
void half_triangle(Canvas *self, Edge which, bool inverted);
void mid_lines(Canvas *self, uint level, ...);
Point *get_fading_lines(uint total_length, uint num, Edge fade);
void fading_hline(Canvas *self, uint level, uint num, Edge fade);
void fading_vline(Canvas *self, uint level, uint num, Edge fade);
void rounded_corner(Canvas *self, uint level, Corner which);
void commit(Canvas *self, Edge lines, bool solid);
void corner(Canvas *self, uint hlevel, uint vlevel, Corner which);
void cross(Canvas *self, uint which);
void vert_t(Canvas *self, uint base_char, uint variation);
void horz_t(Canvas *self, uint base_char, uint variation);
void dvcorner(Canvas *self, uint level, Corner which);
void dhcorner(Canvas *self, uint level, Corner which);
void dcorner(Canvas *self, uint level, Corner which);
void dpip(Canvas *self, uint level, Edge which);
void braille_dot(Canvas *self, uint col, uint row);
void braille(Canvas *self, uint8_t which);
void fill_rect(Canvas *self, uint x_start, uint y_start, uint x_end, uint y_end);
void fill_triangle(Canvas *self, FloatPoint a, FloatPoint b, FloatPoint c);
void draw_separated_block(Canvas *self, uint num_cols, uint num_rows, uint which);
void sixteenth_block(Canvas *self, uint pos);
void draw_sextant(Canvas *self, uint row, uint col);
void sextant(Canvas *self, uint which);
void double_cross_line(Canvas *self, uint level, bool left);
void diagonal_line(Canvas *self, uint level, int x1, int y1, int x2, int y2);
void justified_half_circle(Canvas *self, Edge edge);
void justified_quarter_circle(Canvas *self, Corner corner);
void justified_half_circle_outline(Canvas *self, uint level, Edge edge);
void ellipse_arc_outline(Canvas *self, double cx, double cy, double rx, double ry, double start_deg, double end_deg, double line_width);
void twelfth_circle(Canvas *self, uint level, uint pos);
