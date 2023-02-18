#include <stdio.h>
#include <stdlib.h>
#include "pdf/highlevel.h"
#include "helpers/macros.h"

PdfPage* makePdfPage(int media_box[4])
{
  PdfPage* pdfPage = (PdfPage*) malloc(sizeof(PdfPage));
  FAIL_IF_NULL(pdfPage, "Failed to allocate space for the PdfPage.\n");
  //
  // validate the given array here.
  //
  pdfPage->media_box[0] = media_box[0];
  pdfPage->media_box[1] = media_box[1];
  pdfPage->media_box[2] = media_box[2];
  pdfPage->media_box[3] = media_box[3];

  return NULL;
}
