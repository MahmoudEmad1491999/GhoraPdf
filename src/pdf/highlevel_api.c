#include <stdio.h>
#include <stdlib.h>
#include "pdf/highlevel_defs.h"
#include "helpers/macros.h"
#include "pdf/highlevel_api.h"

Document* makeDocument()
{
  Document* document = malloc(sizeof(Document));
  FAIL_IF_NULL(document, "Failed to allocate space for the document.\n");
  document->pageNodes = NULL;
  return document;
}

Page* makePage(uint32_t width, uint32_t height, enum PageOrientation orientation)
{
  Page* page = malloc(sizeof(Page));
  FAIL_IF_NULL(page, "Failed to allocate space for the document.\n");

  page->width = width;
  page->height = height;
  page->orientation = orientation;

  return page;
}

void appendPage(Document* document, Page* page)
{
  FAIL_IF_NULL(document, "cannot operate on NULL document.\n");
  FAIL_IF_NULL(page, "cannot operate on NULL pages.\n");
  
  struct PageNode* head = document->pageNodes;

  if(head != NULL)
  {
    while(head->next != NULL) head = head->next;    

    head->next = malloc(sizeof(struct PageNode));
    head->next->page = page;
    head->next->next = NULL;
  }
  else 
  {
    document->pageNodes = malloc(sizeof(struct PageNode));
    document->pageNodes->page = page;
    document->pageNodes->next = NULL;
  }
}

int8_t getPage(Document *document, uint32_t pageNumber, Page *out) {
  FAIL_IF_NULL(document, "cannot operate on null document.\n");

  // this variable is to keep track of the count of the pages.
  uint32_t count;
  // this variable is to traverse the linked list of pages.
  struct PageNode *head = document->pageNodes;
  // ckeck if the page list is empty.
  if (head == NULL) {
    fprintf(stderr, "there is no pages in the document.\n");
    return -1;
  } else {
    // keep advancing the head untill you either reach the requested page,
    // or run out of pages.
    while (head->next != NULL && count != pageNumber) {
      head = head->next;
      count++;
    }
    // reached the page??
    if (count == pageNumber) { // Yes.
      out = head->page;
      return 0;
    } else { //NO.
      fprintf(stderr, "page number is out of range.\n");
      return -1;
    }
  }
}
