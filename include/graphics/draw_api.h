#ifndef DRAW_API_H
#define DRAW_API_H
#include <stdio.h>
#include <stdlib.h>
#include "graphics/draw_defs.h"
#include "helpers/macros.h"

Canvas* makeCanvas(Diminsions dims, ORIGIN origin, uint32_t cap);

void drawLine(Canvas* canvas, Point start, Point end, uint32_t lineWidth, RGBColor color);
void drawRectangle(Canvas* canvas, Point start, int32_t width_v, int32_t height_v, RGBColor color, uint32_t lineWidth);
void drawCurve(Canvas* canvas, Point start, Point c1, Point c2, Point end, RGBColor color, uint32_t lineWidth);
void freeCommand(void* command, COMMAND_TYPE commandtype);
void freeCanvas(Canvas* canvas);
#endif
