#include <stdio.h>
#include "pdf/lowlevel.h"
#include "pdf/highlevel.h"

int main(int argc, char **argv) {
    PdfFile* pdfFile = makePdfFile(100);
    uint64_t x = appendPdfValueToList(pdfFile ,makePdfNumber(100),PDF_NUMBER);
    freePdfFile(pdfFile);
    fprintf(stdout, "%lu\n", x);
    return 0;
}
