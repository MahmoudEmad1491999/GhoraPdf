#include "pdf/lowlevel.h"
#include "helpers/macros.h"
#include <string.h>

PdfName *
makePdfName(const char *NT_str /*Null Terminated string, fail otherwise.*/) {
    // check if the given string is null.
    FAIL_IF_NULL(NT_str, "Cannot construct pdfName object with null String.\n");

    // make sure that the given string length is greater than zero.
    int len = strlen(NT_str);
    FAIL_IF_ZERO(len,
                 "Cannot create a pdfName object with zero lenght string.\n");

    // create a new pdfName object on the heap, and check if the allocation
    // failed.
    PdfName *pdfName = (PdfName *)malloc(sizeof(PdfName));
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName\n");

    // allocate a string on the heap to hold the given string.
    pdfName->NT_str = (char *)malloc(sizeof(char) * (len + 1));
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName's underlying "
                          "string of characters\n");

    // copy the given string to the newly allocated string and check if the
    // copying succedded.
    memcpy(pdfName->NT_str, NT_str, len + 1);
    FAIL_IF_NOT_EQUAL(strlen(pdfName->NT_str), len,
                      "memory copy failed to copy the given string\n")

    return pdfName;
}

void freePdfName(PdfName *pdfName) {
    if (NULL != pdfName) {
        free(pdfName->NT_str);
        free(pdfName);
    }
}

PdfString *makePdfString(const char *NT_str) {
    // check if the given string is null.
    FAIL_IF_NULL(NT_str,
                 "Cannot construct pdfString object with null String.\n");

    // make sure that the given string length is greater than zero.
    int len = strlen(NT_str);
    FAIL_IF_ZERO(len,
                 "Cannot create a pdfString object with zero lenght string.\n");

    // create a new pdfString object on the heap, and check if the allocation
    // failed.
    PdfString *pdfString = (PdfString *)malloc(sizeof(PdfString));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString\n");

    // allocate a string on the heap to hold the given string.
    pdfString->NT_str = (char *)malloc(sizeof(char) * (len + 1));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString "
                            "underalying string of characters\n");

    // copy the given string to the newly allocated string and check if the
    // copying succedded.
    memcpy(pdfString->NT_str, NT_str, len + 1);
    FAIL_IF_NOT_EQUAL(strlen(pdfString->NT_str), len,
                      "memory copy failed to copy the given string\n")

    return pdfString;
}

void freePdfString(PdfString *pdfString) {
    if (NULL != pdfString) {
        free(pdfString->NT_str);
        free(pdfString);
    }
}

PdfArray *makePdfArray(int32_t initialCap) {
    // check if the given inital cap is larger than zero
    FAIL_IF_ZERO_OR_LESS(
        initialCap, "cannot create an a zero or less initialCap capacity.\n");

    // allocate space for the newly created pdfArray object and check if the
    // allocation succeded.
    PdfArray *pdfArray = (PdfArray *)malloc(sizeof(PdfArray));
    FAIL_IF_NULL(pdfArray, "Failed to allocate memroy for pdfArray object.\n");

    // set the size and initial capacity of the newly created pdfArray object.
    pdfArray->size = 0;
    pdfArray->cap = initialCap;

    // allocate space for the sub array of the pdfArry object and check if the
    // allocation succeded.
    pdfArray->arr = (void **)malloc(initialCap * sizeof(void *));
    FAIL_IF_NULL(
        pdfArray->arr,
        "Failed to allocate memroy for the sub array pdfArray object.\n");

    // allocate space for the sub array for the types of the sub array of
    // pdfArray and check if the allocation succeded.
    pdfArray->values_types = (int32_t *)malloc(initialCap * sizeof(int32_t));
    FAIL_IF_NULL(pdfArray->values_types,
                 "Failed to allocate memroy for the types sub array of the "
                 "pdfArray object.\n");

    return pdfArray;
}

void freePdfArray(PdfArray *pdfArray) {
    if (NULL != pdfArray) {
        enum PDFVALUETYPE temp;

        for (int index = 0; index < pdfArray->size; index++) {
            freePdfValue(pdfArray->arr[index],pdfArray->values_types[index]);
        }

        free(pdfArray->values_types);
        free(pdfArray);
    }
}
PdfDictionary *makePdfDictionary(int32_t initialCap) {
    // check if the given inital cap is larger than zero
    FAIL_IF_ZERO_OR_LESS(
        initialCap, "cannot create an a zero or less initialCap capacity.\n");

    // allocate space for the newly created pdfArray object and check if the
    // allocation succeded.
    PdfDictionary *pdfDictionary =
        (PdfDictionary *)malloc(sizeof(PdfDictionary));
    FAIL_IF_NULL(pdfDictionary,
                 "Failed to allocate memroy for pdfDictionary object.\n");

    // set the size and initial capacity of the newly created pdfObject object.
    pdfDictionary->cap = initialCap;
    pdfDictionary->size = 0;

    // allocate space for the keys array of pointers and check if the allocation
    // succeded
    pdfDictionary->keys = (PdfName **)malloc(sizeof(PdfName *) * initialCap);
    FAIL_IF_NULL(pdfDictionary,
                 "Failed to allocate memroy for pdfDictionary object.\n");

    // allocate space for the values array of pointer to the actual elements
    // representing values for the keys, and check if the allocation succeded.
    pdfDictionary->values = (void **)malloc(sizeof(void *) * initialCap);
    FAIL_IF_NULL(pdfDictionary->values,
                 "Failed to allocate memroy for pdfDictionary object values "
                 "objects array.\n");

    // allocate space for the array of integers representing the type of the
    // objects pointed by the array of pointers. and check if the allocation
    // succeded
    pdfDictionary->values_types =
        (int32_t *)malloc(sizeof(int32_t) * initialCap);
    FAIL_IF_NULL(pdfDictionary->values,
                 "Failed to allocate memroy for pdfDictionary array of "
                 "integers representing types..\n");

    return pdfDictionary;
}

void freePdfDictionary(PdfDictionary *pdfDictionary) {
    if (NULL != pdfDictionary) {
        enum PDFVALUETYPE temp;
        for (int index = 0; index < pdfDictionary->size; index++) {
            freePdfName(pdfDictionary->keys[index]);

            freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
        }
        free(pdfDictionary->values_types);
        free(pdfDictionary);
    }
}
PdfStream *makePdfStream(int32_t initialCap) {
    FAIL_IF_ZERO_OR_LESS(
        initialCap,
        "cannot create a stream object a zero or less initialCap capacity.\n");

    // create a stream object and check if the allocation succeded.
    PdfStream *pdfStream = (PdfStream *)malloc(sizeof(PdfStream));
    FAIL_IF_NULL(pdfStream,
                 "Failed to allocate memroy for PdfStream object.\n");

    // initialize the len member to the initial cap value.
    pdfStream->size = initialCap;

    // allocate space for the actual stream of bytes, and check if the
    // allocation failed or not.
    pdfStream->stream = (uint8_t *)malloc(sizeof(uint8_t) * initialCap);
    FAIL_IF_NULL(
        pdfStream->stream,
        "failed to allocate memory for the underlaying array of uint8_t.\n")

    return pdfStream;
}

void freePdfStream(PdfStream *pdfStream) {
    if (NULL != pdfStream) {
        free(pdfStream->stream);
        free(pdfStream);
    }
}

PdfObjVal pdfArrayAtGet(PdfArray* pdfArray, int index)
{
    FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

    if(index < pdfArray->size)
    {
        PdfObjVal result = {.ele = pdfArray->arr[index], .type = pdfArray->values_types[index]};
        return result;
    }
    else 
    {
        fprintf(stderr, "Index_OUT_OF_BOUNDS, index: %d, size: %d", index, pdfArray->size);
        exit(-1);
    }

}

void pdfArrayAtSet(PdfArray* pdfArray, int index, PdfObjVal pdfObjVal)
{
    FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

    if(index < pdfArray->size)
    {
        pdfArray->arr[index] = pdfObjVal.ele;
        pdfArray->values_types[index] = pdfObjVal.type;
    }
    else 
    {
        fprintf(stderr, "Index_OUT_OF_BOUNDS, index: %d, size: %d", index, pdfArray->size);
        exit(-1);
    }
}



PdfObjVal pdfDictionaryGet(PdfDictionary* pdfDictionary, const char* NT_str)
{
    FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
    PdfObjVal temp;
    for(int index = 0; index < pdfDictionary->size; index++)
    {
        if(strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0)
        {
            temp.ele = pdfDictionary->values[index];
            temp.type = pdfDictionary->values_types[index];
            return temp;
        }
    }

    fprintf(stderr, "the dictionary does not have a entry with the key: %s", NT_str);
    exit(-1);
}

void pdfDictionarySet(PdfDictionary* pdfDictionary, const char* NT_str, PdfObjVal pdfObjVal)
{
    FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
    for(int index = 0; index < pdfDictionary->size; index++)
    {
        if(strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0)
        {
            freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
            pdfDictionary->values[index] = pdfObjVal.ele;
            pdfDictionary->values_types[index] = pdfObjVal.type;
            return;
        }
    }

    fprintf(stderr, "the dictionary does not have a entry with the key: %s", NT_str);
    exit(-1);
}

uint8_t pdfStreamGet(PdfStream* pdfStream, int index)
{
    FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
    if(index < pdfStream->size)
    {
        return pdfStream->stream[index];
    }
    else 
    {
        fprintf(stderr, "Index_OUT_OF_BOUNDS, index: %d, size: %d", index, pdfStream->size);
        exit(-1);
    }
}

void pdfStreamSet(PdfStream* pdfStream, int index, uint8_t value)
{
    FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
    if(index < pdfStream->size)
    {
        pdfStream->stream[index] = value;
    }
    else 
    {
        fprintf(stderr, "Index_OUT_OF_BOUNDS, index: %d, size: %d", index, pdfStream->size);
        exit(-1);
    }
}
void freePdfValue(void* ele, enum PDFVALUETYPE pdfValueType)
{
    if (NULL != ele) {
        if (pdfValueType == PDFNUMBER || pdfValueType == PDFBOOLEAN ||
            pdfValueType == PDFREAL || pdfValueType == PDFNULL) {
            free(ele);
        } else if (pdfValueType == PDFSTRING) {
            freePdfString(ele);
        } else if (pdfValueType == PDFNAME) {
            freePdfName(ele);
        } else if (pdfValueType == PDFDICTIONARY) {
            freePdfDictionary(ele);
        } else if (pdfValueType == PDFSTREAM) {
            freePdfStream(ele);
        } else if (pdfValueType == PDFARRAY) {
            freePdfArray(ele);
        } else {
            FAIL_WITH_MESSAGE(
                "THIS PART OF THE CODE SHOULD NOT BE REACHED EVER.\n");
        }
    }
}
