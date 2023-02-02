#include "pdf/lowlevel.h"
#include "helpers/macros.h"

PdfName *makePdfName(const char *str/*Null Terminated string, fail otherwise.*/) {
    // check if the given string is null.
    FAIL_IF_NULL( str, "Cannot construct pdfName object with null String.\n" );
    
    // make sure that the given string length is greater than zero.
    int len = strlen(str);
    FAIL_IF_ZERO(len, "Cannot create a pdfName object with zero lenght string.\n");

    // create a new pdfName object on the heap, and check if the allocation failed.
    PdfName *pdfName = (PdfName *)malloc(sizeof(PdfName));
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName\n");
    
    // allocate a string on the heap to hold the given string.
    pdfName->str = (char *)malloc(sizeof(char) * (len + 1));
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName underalying string of characters\n");
    
    // copy the given string to the newly allocated string and check if the copying succedded.
    memcpy(pdfName->str, str, len+1);
    FAIL_IF_NOT_EQUAL(strlen(pdfName->str), len, "memory copy failed to copy the given string\n") 
    
    return pdfName;
}

PdfString *makePdfString(const char *str) {
    // check if the given string is null.
    FAIL_IF_NULL( str, "Cannot construct pdfString object with null String.\n" );
    
    // make sure that the given string length is greater than zero.
    int len = strlen(str);
    FAIL_IF_ZERO(len, "Cannot create a pdfString object with zero lenght string.\n");

    // create a new pdfString object on the heap, and check if the allocation failed.
    PdfString *pdfString = (PdfString *)malloc(sizeof(PdfString));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString\n");
    
    // allocate a string on the heap to hold the given string.
    pdfString->str = (char *)malloc(sizeof(char) * (len + 1));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString underalying string of characters\n");
    
    // copy the given string to the newly allocated string and check if the copying succedded.
    memcpy(pdfString->str, str, len+1);
    FAIL_IF_NOT_EQUAL(strlen(pdfString->str), len, "memory copy failed to copy the given string\n") 
    
    return pdfString;
}

PdfArray *makePdfArray(int32_t initialCap) {
    // check if the given inital cap is larger than zero
    FAIL_IF_ZERO_OR_LESS(initialCap, "cannot create an a zero or less initialCap capacity.\n");
    
    // allocate space for the newly created pdfArray object and check if the allocation succeded.
    PdfArray *pdfArray = (PdfArray *)malloc(sizeof(PdfArray));
    FAIL_IF_NULL(pdfArray, "Failed to allocate memroy for pdfArray object.\n");
    
    // set the size and initial capacity of the newly created pdfArray object.
    pdfArray->size = 0;
    pdfArray->cap = initialCap;

    // allocate space for the sub array of the pdfArry object and check if the allocation succeded.
    pdfArray->arr = (void **)malloc(initialCap * sizeof(void *));
    FAIL_IF_NULL(pdfArray->arr,"Failed to allocate memroy for the sub array pdfArray object.\n");
    
    // allocate space for the sub array for the types of the sub array of pdfArray and check if the allocation succeded.
    pdfArray->values_types = (int32_t *)malloc(initialCap * sizeof(int32_t));
    FAIL_IF_NULL(pdfArray->values_types,"Failed to allocate memroy for the types sub array of the pdfArray object.\n");
    
    return pdfArray;
}

PdfDictionary *makePdfDictionary(int32_t initialCap) {
    // check if the given inital cap is larger than zero
    FAIL_IF_ZERO_OR_LESS(initialCap, "cannot create an a zero or less initialCap capacity.\n");

    // allocate space for the newly created pdfArray object and check if the allocation succeded.
    PdfDictionary *pdfDictionary =
        (PdfDictionary *)malloc(sizeof(PdfDictionary));
    FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

    // set the size and initial capacity of the newly created pdfObject object.
    pdfDictionary->cap = initialCap;
    pdfDictionary->len = 0;

    // allocate space for the keys array of pointers and check if the allocation succeded
    pdfDictionary->keys = (PdfName **)malloc(sizeof(PdfName *) * initialCap);
    FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

    // allocate space for the values array of pointer to the actual elements representing values for the keys,
    // and check if the allocation succeded.
    pdfDictionary->values = (void **)malloc(sizeof(void *) * initialCap);
    FAIL_IF_NULL(pdfDictionary->values, "Failed to allocate memroy for pdfDictionary object values objects array.\n");
    
    // allocate space for the array of integers representing the type of the objects pointed by the array of pointers.
    // and check if the allocation succeded
    pdfDictionary->values_types =
        (int32_t *)malloc(sizeof(int32_t) * initialCap);
    FAIL_IF_NULL(pdfDictionary->values, "Failed to allocate memroy for pdfDictionary array of integers representing types..\n");


    return pdfDictionary;
}

PdfStream *makePdfStream(int32_t initialCap) {
    FAIL_IF_ZERO_OR_LESS(initialCap, "cannot create a stream object a zero or less initialCap capacity.\n");
    
    //create a stream object and check if the allocation succeded.
    PdfStream *pdfStream = (PdfStream *)malloc(sizeof(PdfStream));
    FAIL_IF_NULL(pdfStream, "Failed to allocate memroy for PdfStream object.\n");
    
    //initialize the len member to the initial cap value.
    pdfStream->len = initialCap;

    // allocate space for the actual stream of bytes.
    pdfStream->stream = (uint8_t*) malloc(sizeof(uint8_t) * initialCap);
    
    return pdfStream;
}


void freePdfName(PdfName* pdfName)
{
    if(NULL != pdfName)
    {
        
    }
}
