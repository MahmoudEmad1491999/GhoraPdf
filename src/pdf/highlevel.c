#include "pdf/lowlevel.h"
#include "pdf/highlevel.h"
#include "helpers/macros.h"

PdfFileContent* makePdfFileContent(int32_t initialCapacity)
{
    FAIL_IF_ZERO_OR_LESS(initialCapacity, "initialCapacity cannot be non-poisitive number.\n");
    // allocate space for the PdfFileContent Object.
    PdfFileContent* ptr = (PdfFileContent*) malloc(sizeof(PdfFileContent));
    // set the currentCapacity to the initialCapacity.
    ptr->currentCapacity = initialCapacity;
    // allocate space for the indirect object poiters array.
    ptr->inObjarr = (PdfInObj **)malloc(
        sizeof(PdfInObj *) * initialCapacity);

    return ptr;
}



PdfInObj* PdfFileContentAtGet(PdfFileContent* pdfFileContent,int32_t index)
{
    // check if the given pointer is null.
    FAIL_IF_NULL(pdfFileContent , "the given pdf file content object is null");
    // check if the index is negative.
    FAIL_IF_NEGATIVE(index, "Index is Negative.\n");
    
    // check if the given index is out of the range.
    if(index >= pdfFileContent->currentCapacity)
    {
        return NULL;
    }
    else {
        return pdfFileContent->inObjarr[index];
    }
}

void PdfFileContentAtSet(PdfFileContent *pdfFileContent, int32_t index, PdfInObj *pdfInObj)
{
    // check if the given pointer is null.
    FAIL_IF_NULL(pdfFileContent , "the given pdf file content object is null");
    // check if the index is negative.
    FAIL_IF_NEGATIVE(index, "Index is Negative.\n");
    
    // check if the given index is out of the range.
    if(index >= pdfFileContent->currentCapacity)
    {
        fprintf(stderr, "Not Implemented yet!\n");
        exit(-1);
    }
    else {
        // free the exisiting pdf indirect object.
        if(pdfFileContent->inObjarr[index] != NULL)
        {
            freePdfInObj(pdfFileContent->inObjarr[index]);
        }
        pdfFileContent->inObjarr[index] = pdfInObj;
    }
}


void freePdfInObj(PdfInObj *ptr)
{
    if(NULL != ptr)
    {
        // free the underlying pdf value.
        freePdfValue(ptr->value, ptr->type);
        // free the indirect object itself.
        free(ptr);
    }
}
