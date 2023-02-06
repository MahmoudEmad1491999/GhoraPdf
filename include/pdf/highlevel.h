#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel.h"

typedef struct {
    uint64_t generation_number; // this represent the generation number of the
                                // indirect object. always non-negative. >= 0;
    uint64_t object_number;     // this is the identifier of the object always
                                // positive. > 0;
    void *ptr; // pointer to the object "PDFNAME, PDFSTRING, ..., etc"

} Pdf_Indirect_Obj;


typedef struct {
   Pdf_Indirect_Obj** indirect_objects_list;                // indirect objects list

}PdfFileContent;

#endif
