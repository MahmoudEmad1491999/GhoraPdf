#ifndef DRAW_DEFS_H
#define DRAW_DEFS_H
#include <stdint.h>
#include <wchar.h>

/**
 * Purpose:
 * represent the origin and the directions of increase for the main axis.
 * LB: Left Bottom  : x increase to the right and y increase to the top.
 * LT: left Top     : x increase to the right and y increase to the bottom.
 * RB: Right Bottom : x increase to the Left and y increase to the top.
 * RT: Right Top    : x increase to the left and y increase to the bottom.
 */
typedef enum { LB, RB, LT, RT } ORIGIN;

/**
 * Purpose:
 * Represent the possible opertaion that can be performed on the canvas.
 * LINE:            add subpath segment of type line.
 * RECTANGLE:       add a closed subpath of type rectangle.
 * CURVE:           add subpath of type curve.
 */
typedef enum { LINE,RECTANGLE,CURVE } COMMAND_TYPE;

/** 
 * Purpose:                  
 * descripe the possible ways to end a line.
 * BUTT_CAP                   simply end the line with sharp non extended edges.
 * ROUNDED_CAP                end the line with a rounded edge with radius half the line width.
 * PROJECTING_SQUARE_CAP      end the line with sharp extended edge of length half the line width.
 */
typedef enum { BUTT_CAP, ROUNDED_CAP, PROJECTING_SQUARE_CAP } LINE_CAP;

/**
 * Purpose:         describe the possible ways to join multiple lines or paths or curves.
 *
 * ROUNDED_JOIN      join path segments with rounded joints of radius.
 * BEVEL_JOIN        any extension beyound the both segments length is cut out.
 * MITER_JOIN        the extendsions is sharp and long.
 */

typedef enum { ROUNDED_JOIN, BEVEL_JOIN, MITER_JOIN } JOIN_STYLE;


/**
 * represent rgb color values. [0-255]
 */
typedef struct {
  uint8_t red,green,blue;
} RGBColor;

/*
 * Purpose:         this strucutre represent a point in any possible coordinate system.
 * @x,y             can be positive and negative.
 */
typedef struct {
  int32_t x,y;
} Point;

/**
 * Purpose:         this structure represent the space occupied by an area/canvas.
 * @width           the horizontal length of the array.
 * @height          the vertical length of the array.
 */
typedef struct
{
  uint32_t width, height;
} Diminsions;


/**
 * Purpose:
 * this strucutre represent a drawing canvas.
 * @dims          the diminsions of the canvas.
 * @orgin         origin of the canvas.
 * @commands      pointer array to Command objects that should be applied sequentially to the canvas.
 */
typedef struct
{
  Diminsions dims;
  ORIGIN origin;
  void** commands;
  COMMAND_TYPE* commandstypes;
  uint32_t len, cap;
}Canvas;

/**
 * Purpose:
 * this structure represent a Line segment.
 * @start       the starting point of the line.
 * @end         the ending point of the line.
 * @lineCap     the capping style of the line.
 */
typedef struct
{
  Point start, end;
  LINE_CAP lineCap;
  RGBColor color;
  uint32_t lineWidth;
} Line;

/**
 * Purpose:
 * this structure represent a rectangle.
 * @start       the origin of the rectangle
 * @width_v     the width vector of the rectangle "negative values means
 *              in the negative direction of x."
 * @height_v    the height vector of the rectangle "negative values means
 *              in the negative direction of y."
 * @JOIN_STYLE  how the right angles should be drawn.
 */
typedef struct
{
  Point start;
  int32_t width_v, height_v;
  JOIN_STYLE joinStyle;
  uint32_t lineWidth;
  RGBColor color;
} Rectangle;

/**
 * Purpose:
 * represent bezier curve.
 * @start       the start point of the curve.
 * @c1, c2      control point one and two.
 * @end         end point of the curve.
 */
typedef struct
{
  Point start, c1, c2, end;
  LINE_CAP lineCap;
  RGBColor color;
  uint32_t lineWidth;
} Curve;
#endif
