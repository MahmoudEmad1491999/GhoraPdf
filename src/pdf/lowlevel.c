#include "pdf/lowlevel.h"
#include "helpers/macros.h"
#include <string.h>

PdfName *
makePdfName(const char *NT_str /*Null Terminated string, fail otherwise.*/) {
    // check if the given string is null.
    FAIL_IF_NULL(NT_str, "Cannot construct pdfName object with null String.\n");

    // make sure that the given string length is greater than zero.
    FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot create a pdfName object with zero lenght string.\n");
    // create a new pdfName object on the heap, and check if the allocation
    // failed.
    PdfName *pdfName = (PdfName *)malloc(sizeof(PdfName));
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName\n");

    // allocate a string on the heap to hold the given string.
    pdfName->NT_str = (char *)malloc(sizeof(char) * strlen(NT_str) + 1);
    FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName's underlying "
                          "string of characters\n");

    // copy the given string to the newly allocated string and check if the
    // copying succedded.
    memcpy(pdfName->NT_str, NT_str, strlen(NT_str) + 1);
    FAIL_IF_NOT_EQUAL(strlen(pdfName->NT_str), strlen(NT_str),
                      "memory copy failed to copy the given string\n")

    return pdfName;
}

void freePdfName(PdfName *pdfName) {
    // check if the given pointer is not null.
    if (NULL != pdfName) {
        // free the underlying string first.
        free(pdfName->NT_str);
        // free the object.
        free(pdfName);
    }
}

PdfString *makePdfString(const char *NT_str) {
    // check if the given string is null.
    FAIL_IF_NULL(NT_str,
                 "Cannot construct pdfString object with null String.\n");

    // check if the given string is not of zero length.
    FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot create a pdfName object with zero lenght string.\n");

    // create a new pdfString object on the heap, and check if the allocation
    // failed.
    PdfString *pdfString = (PdfString *)malloc(sizeof(PdfString));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString\n");

    // allocate a string on the heap to hold the given string.
    pdfString->NT_str = (char *)malloc(sizeof(char) * (strlen(NT_str) + 1));
    FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString "
                            "underalying string of characters\n");

    // copy the given string to the newly allocated string and check if the
    // copying succedded.
    memcpy(pdfString->NT_str, NT_str, strlen(NT_str) + 1);
    FAIL_IF_NOT_EQUAL(strlen(pdfString->NT_str), strlen(NT_str),
                      "memory copy failed to copy the given string\n")

    return pdfString;
}

void freePdfString(PdfString *pdfString) {
    // check if the given pointer is null
    if (NULL != pdfString) {
        // free the underlying string first.
        free(pdfString->NT_str);
        // then free the object itself.
        free(pdfString);
    }
}

PdfArray *makePdfArray(int32_t size) {
    // check if the given size is larger than zero
    FAIL_IF_ZERO_OR_LESS(
        size, "cannot create an a zero or less size.\n");

    // allocate space for the newly created pdfArray object and check if the
    // allocation succeded.
    PdfArray *pdfArray = (PdfArray *)malloc(sizeof(PdfArray));
    FAIL_IF_NULL(pdfArray, "Failed to allocate memroy for pdfArray object.\n");

    // set the size of the newly created pdfArray object.
    pdfArray->size = size;

    // allocate space for the sub array of the pdfArry object and check if the
    // allocation succeded.
    pdfArray->arr = (void **)malloc(size * sizeof(void *));
    FAIL_IF_NULL(
        pdfArray->arr,
        "Failed to allocate memroy for the sub array pdfArray object.\n");

    // allocate space for the sub array for the types of the sub array of
    // pdfArray and check if the allocation succeded.
    pdfArray->values_types = (int32_t *)malloc(size * sizeof(int32_t));
    FAIL_IF_NULL(pdfArray->values_types,
                 "Failed to allocate memroy for the types sub array of the "
                 "pdfArray object.\n");

    return pdfArray;
}

void freePdfArray(PdfArray *pdfArray) {
    // check if the given pointer is null.
    if (NULL != pdfArray) {
        // free each pdf element in the array.
        enum PDFVALUETYPE temp;

        for (int index = 0; index < pdfArray->size; index++) {
            freePdfValue(pdfArray->arr[index],pdfArray->values_types[index]);
        }
        // free the size allocated for the array of pointers to the elements.
        free(pdfArray->arr);
        // free the array of integers representing the types of elements inside the array.
        free(pdfArray->values_types);
        // free the pdfArray object itself.
        free(pdfArray);
    }
}
PdfDictionary *makePdfDictionary(int32_t size) {
    // check if the given inital cap is larger than zero
    FAIL_IF_ZERO_OR_LESS(
        size, "cannot create an a zero or less size capacity.\n");

    // allocate space for the newly created pdfArray object and check if the
    // allocation succeded.
    PdfDictionary *pdfDictionary =
        (PdfDictionary *)malloc(sizeof(PdfDictionary));
    FAIL_IF_NULL(pdfDictionary,
                 "Failed to allocate memroy for pdfDictionary object.\n");

    // set the size of the newly created pdfObject object.
    pdfDictionary->size = size;

    // allocate space for the keys array of pointers and check if the allocation
    // succeded
    pdfDictionary->keys = (PdfName **)malloc(sizeof(PdfName *) * size);
    FAIL_IF_NULL(pdfDictionary,
                 "Failed to allocate memroy for pdfDictionary object.\n");

    // allocate space for the values array of pointer to the actual elements
    // representing values for the keys, and check if the allocation succeded.
    pdfDictionary->values = (void **)malloc(sizeof(void *) * size);
    FAIL_IF_NULL(pdfDictionary->values,
                 "Failed to allocate memroy for pdfDictionary object values "
                 "objects array.\n");

    // allocate space for the array of integers representing the type of the
    // objects pointed by the array of pointers. and check if the allocation
    // succeded
    pdfDictionary->values_types =
        (int32_t *)malloc(sizeof(int32_t) * size);
    FAIL_IF_NULL(pdfDictionary->values,
                 "Failed to allocate memroy for pdfDictionary array of "
                 "integers representing types..\n");

    return pdfDictionary;
}

void freePdfDictionary(PdfDictionary *pdfDictionary) {
    if (NULL != pdfDictionary) {
        // free each key and value pointed by the keys and values array of pointers.
        enum PDFVALUETYPE temp;
        for (int index = 0; index < pdfDictionary->size; index++) {
            // free the key.
            freePdfName(pdfDictionary->keys[index]);
            // free the value.
            freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
        }
        
        // free the space allocated for the array of pointers to the keys "PdfName*".
        free(pdfDictionary->keys);

        // free the space allocated for the arrya of pointer to the values "void*"
        free(pdfDictionary->values);
        

        // free the space allocated for the array of integers allocated to represent the types of values
        // in each key-value pair.
        free(pdfDictionary->values_types);
        // free the space allocated for the object itself.
        free(pdfDictionary);
    }
}


PdfStream *makePdfStream(int32_t len) {
    FAIL_IF_ZERO_OR_LESS(
        len,
        "cannot create a stream object a zero or less len capacity.\n");

    // create a stream object and check if the allocation succeded.
    PdfStream *pdfStream = (PdfStream *)malloc(sizeof(PdfStream));
    FAIL_IF_NULL(pdfStream,
                 "Failed to allocate memroy for PdfStream object.\n");

    // initialize the len member to the initial cap value.
    pdfStream->len = len;

    // allocate space for the actual stream of bytes, and check if the
    // allocation failed or not.
    pdfStream->stream = (uint8_t *)malloc(sizeof(uint8_t) * len);
    FAIL_IF_NULL(
        pdfStream->stream,
        "failed to allocate memory for the underlaying array of uint8_t.\n")

    return pdfStream;
}

void freePdfStream(PdfStream *pdfStream) {
    // check if the given pointer is NULL
    if (NULL != pdfStream) {
        // first free the space for the stream data.
        free(pdfStream->stream);
        // second free the space for the pdfStream Object itself.
        free(pdfStream);
    }
}

PdfObjVal pdfArrayAtGet(PdfArray* pdfArray, int index)
{
    // check if the given array pointer is null.
    FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");
    
    // check if the given index falls within the range.
    CHECK_INDEX(index, pdfArray->size);
   
    // if at the specified index there is not element return PDFNULL. 
    if(pdfArray->arr[index] == NULL)
    {

    return (PdfObjVal){
        .ptr = pdfArray->arr[index],
        .type = PDFNULL};
    }
    else {
    return (PdfObjVal) {
        .ptr = pdfArray->arr[index],
        .type = pdfArray->values_types[index]};
    }
    

}

void pdfArrayAtSet(PdfArray* pdfArray, int index, PdfObjVal pdfObjVal)
{
    // check if the given array pointer is null
    FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");
    //
    // check if the given index falls in the accepts range.
    CHECK_INDEX(index, pdfArray->size);
    
    // check if the given pdfObjVal is in a consistant state or not.
    if(pdfObjVal.ptr == NULL && pdfObjVal.type != PDFNULL){
        fprintf(stderr, " pdfObjVal is in an inconsistant state.\n");
        exit(-1);
    }
    // check if there was an already element in that place if so free it first.
    if(pdfArray->arr[index] != NULL)
    {
        freePdfValue(pdfArray->arr[index],pdfArray->values_types[index]);    
    }
    // assign the new element to the place in the pointers array.
    pdfArray->arr[index] = (void*) pdfObjVal.ptr; // the cast here is required to supress the warning of missing qualifiers "i.e.const".
    // update the type.
    pdfArray->values_types[index] = pdfObjVal.type;
    
}



PdfObjVal pdfDictionaryGet(PdfDictionary* pdfDictionary, const char* NT_str)
{
    // check if the Dictionary pointer is not NULL.
    FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
    // check if the given String is null.
    FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
    // check if the given string length is zero.
    FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");

    for(int index = 0; index < pdfDictionary->size; index++)
    {
        // check if the current index has a key value pair
        if(pdfDictionary->keys[index] == NULL){
            continue;
        }
        else {

            if(strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0)
            {
                return (PdfObjVal) {
                    .ptr = pdfDictionary->values[index] ,
                    .type =pdfDictionary->values_types[index]
                };
            }
        }
    }
    // the whole dictionary does not have a key NT-str.
    fprintf(stderr, "the dictionary does not have a entry with the key: %s", NT_str);
    exit(-1);
}

void pdfDictionarySet(PdfDictionary* pdfDictionary, const char* NT_str, PdfObjVal pdfObjVal)
{
    // check if the Dictionary pointer is not NULL.
    FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
    // check if the given String is null.
    FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
    // check if the given string length is zero.
    FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");

    //check if the given pdfObjVal is consistant.
    if(pdfObjVal.ptr == NULL && pdfObjVal.type != PDFNULL)
    {
        fprintf(stderr, "pdfObjVal is not consistant, type is not null but the pointer point to null.\n");
        exit(-1);
    }

    for(int index = 0; index < pdfDictionary->size; index++)
    {
        // check if the current index has a key value pair
        if(pdfDictionary->keys[index] == NULL){
            continue;
        }
        else {

            if(strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0)
            {
                // check if the value point to NON-pdfNULL "NULL" if so free before assigning the new pointer.
                if(pdfDictionary->values[index] != NULL)
                {
                    freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
                }
                // assign the new pdf value to the key and update the type.
                pdfDictionary->values[index] = (void*) pdfObjVal.ptr;   // the cast here is to suppress the  
                                                                        // warining of the missing const qualifier lhs
                pdfDictionary->values_types[index] = pdfObjVal.type; 
                return;
            }
        }
    }
    // there is not a single key with the value NT_str.
    fprintf(stderr, "the dictionary does not have a entry with the key: %s", NT_str);
    exit(-1);
}

uint8_t pdfStreamGet(PdfStream* pdfStream, int index)
{
    // check if the given stream pointer is not NULL.
    FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
    // check the index if it falls inside the range or not.
    CHECK_INDEX(index,pdfStream->len);

    return pdfStream->stream[index];
}

void pdfStreamSet(PdfStream* pdfStream, int index, uint8_t value)
{
    // check if the given stream pointer is not NULL.
    FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
    // check the index if it falls inside the range or not.
    CHECK_INDEX(index,pdfStream->len);

    pdfStream->stream[index] = value;
}

void freePdfValue(void* ele, enum PDFVALUETYPE pdfValueType)
{
    // check if the given pointer to pdf element is null or not.
    if (NULL != ele) {
        // if it's a simple pdf type "non-composite or aggregate" just freeing it is enough.
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
            // this means that the pdf value type has an unknown value.
            FAIL_WITH_MESSAGE(
                "THIS PART OF THE CODE SHOULD NOT BE REACHED EVER.\n");
        }
    }
}
