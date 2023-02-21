#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel_defs.h"
#include <stdint.h>
/**
 * these enum values represent the position of the origin on the page.
 */
enum PageOrientation {
  LEFTBOTTOM,
  LEFTTOP,
  RIGHTBOTTOM,
  RIGHTTOP
};

/**
 * Purpose:       this strucutre represent single page in a document.
 * @horizontal:   Horizontal length of the page.
 * @vertical:     the vertical length of the page.
 * @orientation:  the position of the origin in the page.
 */
typedef struct
{
  int32_t horizontal, vertical;
  enum PageOrientation orientation;
} Page;


/**
 * Purpose:       this structure represent single self represented doucument.
 * @pages:        singlely linked list of pages.
 */
typedef struct
{
  struct PageNode
  {
    Page* page;
    struct PageNode* next;
  }* pages;
} Document;

#endif
