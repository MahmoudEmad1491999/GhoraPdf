#ifndef HIGHLEVEL_API_H
#define HIGHLEVEL_API_H
#include "helpers/macros.h"
#include "pdf/highlevel_defs.h"
#include <stdlib.h>

Document* makeDocument();

Page* makePage(uint32_t width, uint32_t hight, enum PageOrientation orientation);

void appendPage(Document* document, Page* page);

int8_t getPage(Document *document, uint32_t pageNumber, Page *out);

#endif
