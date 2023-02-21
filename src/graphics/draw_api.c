#include "graphics/draw_api.h"

Canvas* makeCanvas(Diminsions dims, ORIGIN origin, uint32_t cap)
{
  FAIL_IF_ZERO(cap, "Buffers cannot have length of zero\n");

  Canvas* canvas = malloc(sizeof(Canvas));
  FAIL_IF_NULL(canvas, "Failed to allocate memory for the canvas strucutre.\n");

  canvas->dims = dims;
  canvas->origin = origin;

  canvas->len = 0;
  canvas->cap = cap;

  canvas->commands = calloc(cap, sizeof(void*));
  canvas->commandstypes = calloc(cap, sizeof(COMMAND_TYPE));

  FAIL_IF_NULL(canvas->commands, "Failed to allocate memory for the canvas commands array.\n");
  FAIL_IF_NULL(canvas->commandstypes, "Failed to allocate memory for the canvas commandstypes array.\n");


  return canvas;
}

void drawLine(Canvas* canvas, Point start, Point end, uint32_t lineWidth, RGBColor color)
{
  Line* line = malloc(sizeof(Line));
  FAIL_IF_NULL(line, "Failed to allocate memory for the line command.\n");

  line->color = color;
  line->start = start;
  line->end = end;
  line->lineWidth = lineWidth;

  FAIL_IF_NULL(canvas, "Cannot perform on null canvas pointer.\n");

  if(canvas->len < canvas->cap)
  {
    canvas->commands[canvas->len] = line; 
    canvas->commandstypes[canvas->len] = LINE;
    canvas->len++;
  }else 
  {
    canvas->commands = realloc(canvas->commands, canvas->cap * 2 * sizeof(void *));
    canvas->commandstypes = realloc(canvas->commandstypes, canvas->cap * 2 * sizeof(void *));
    FAIL_IF_NULL(canvas->commands, "Failed to resize the commands array.\n");
    FAIL_IF_NULL(canvas->commandstypes, "Failed to resize the commandstypes array.\n");

    canvas->commands[canvas->len] = line; 
    canvas->commandstypes[canvas->len] = LINE;
    canvas->len++;
  }
}

void drawRectangle(Canvas* canvas, Point start, int32_t width_v, int32_t height_v, RGBColor color, uint32_t lineWidth)
{
  Rectangle* rectangle = malloc(sizeof(Rectangle));
  FAIL_IF_NULL(rectangle, "Failed to allocate space for rectangle command.\n");
  rectangle->start = start;
  rectangle->width_v = width_v;
  rectangle->height_v = height_v;
  rectangle->color = color;
  rectangle->lineWidth = lineWidth;

  if(canvas->len < canvas->cap)
  {
    canvas->commandstypes[canvas->len] = RECTANGLE;
    canvas->commands[canvas->len] = rectangle;
    canvas->len++;
  }
  else
  {
    canvas->commands = realloc(canvas->commands, canvas->cap * 2 * sizeof(void *));
    canvas->commandstypes = realloc(canvas->commandstypes, canvas->cap * 2 * sizeof(void *));
    FAIL_IF_NULL(canvas->commands, "Failed to resize the commands array.\n");
    FAIL_IF_NULL(canvas->commandstypes, "Failed to resize the commandstypes array.\n");

    canvas->commandstypes[canvas->len] = RECTANGLE;
    canvas->commands[canvas->len] = rectangle;
    canvas->len++;
  }
}

void drawCurve(Canvas* canvas, Point start, Point c1, Point c2, Point end, RGBColor color, uint32_t lineWidth)
{
  Curve* curve = malloc(sizeof(Curve));
  FAIL_IF_NULL(curve, "Failed to allocate space for the curve command.\n");
  curve->start = start;
  curve->end = end;
  curve->c1 = c1;
  curve->c2 = c2;
  curve->color = color;
  curve->lineWidth = lineWidth;

  if(canvas->len < canvas->cap)
  {
    canvas->commandstypes[canvas->len] = CURVE;
    canvas->commands[canvas->len] = curve;
    canvas->len++;
  }
  else
  {
    canvas->commands = realloc(canvas->commands, canvas->cap * 2 * sizeof(void *));
    canvas->commandstypes = realloc(canvas->commandstypes, canvas->cap * 2 * sizeof(void *));
    FAIL_IF_NULL(canvas->commands, "Failed to resize the commands array.\n");
    FAIL_IF_NULL(canvas->commandstypes, "Failed to resize the commandstypes array.\n");

    canvas->commandstypes[canvas->len] = CURVE;
    canvas->commands[canvas->len] = curve;
    canvas->len++;
  }
}

void freeCommand(void* command, COMMAND_TYPE commandtype)
{
  free(command); 
}

void freeCanvas(Canvas* canvas)
{
  if(NULL != canvas)
  {
    for(uint32_t index = 0; index < canvas->len; index++)
    {
      freeCommand(canvas->commands[index],canvas->commandstypes[index]);
    }

    free(canvas->commands);
    free(canvas->commandstypes);
    free(canvas);
  
  }
}


