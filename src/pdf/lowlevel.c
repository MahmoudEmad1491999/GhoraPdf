#include "pdf/lowlevel.h"
#include "helpers/macros.h"
#include <string.h>

PdfName *makePdfName(const char *NT_str /*Null Terminated string, fail otherwise.*/) {
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
  FAIL_IF_NULL(pdfName->NT_str, "memeory Allocation FAILED for pdfName's underlying string of characters\n");

  // copy the given string to the newly allocated string and check if the
  // copying succedded.
  memcpy(pdfName->NT_str, NT_str, strlen(NT_str) + 1);
  FAIL_IF_NOT_EQUAL(strlen(pdfName->NT_str), strlen(NT_str), "memory copy failed to copy the given string\n")

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
  FAIL_IF_NULL(NT_str, "Cannot construct pdfString object with null String.\n");

  // check if the given string is not of zero length.
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot create a pdfName object with zero lenght string.\n");

  // create a new pdfString object on the heap, and check if the allocation
  // failed.
  PdfString *pdfString = (PdfString *)malloc(sizeof(PdfString));
  FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString\n");

  // allocate a string on the heap to hold the given string.
  pdfString->NT_str = (char *)malloc(sizeof(char) * (strlen(NT_str) + 1));
  FAIL_IF_NULL(pdfString->NT_str, "memeory Allocation FAILED for pdfString underalying string of characters\n");

  // copy the given string to the newly allocated string and check if the
  // copying succedded.
  memcpy(pdfString->NT_str, NT_str, strlen(NT_str) + 1);
  FAIL_IF_NOT_EQUAL(strlen(pdfString->NT_str), strlen(NT_str), "memory copy failed to copy the given string\n")

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
  FAIL_IF_ZERO_OR_LESS(size, "cannot create an a zero or less size.\n");

  // allocate space for the newly created pdfArray object and check if the
  // allocation succeded.
  PdfArray *pdfArray = (PdfArray *)malloc(sizeof(PdfArray));
  FAIL_IF_NULL(pdfArray, "Failed to allocate memroy for pdfArray object.\n");

  // set the size of the newly created pdfArray object.
  pdfArray->size = size;

  // allocate space for the sub array of the pdfArry object and check if the
  // allocation succeded.
  pdfArray->arr = (void **)malloc(size * sizeof(void *));
  FAIL_IF_NULL(pdfArray->arr, "Failed to allocate memroy for the sub array pdfArray object.\n");

  // allocate space for the sub array for the types of the sub array of
  // pdfArray and check if the allocation succeded.
  pdfArray->values_types = (int32_t *)malloc(size * sizeof(int32_t));
  FAIL_IF_NULL(pdfArray->values_types, "Failed to allocate memroy for the types sub array of the pdfArray object.\n");

  return pdfArray;
}

void freePdfArray(PdfArray *pdfArray) {
  // check if the given pointer is null.
  if (NULL != pdfArray) {
    // free each element using the element type freeing function.
    for (int index = 0; index < pdfArray->size; index++) {
      freePdfValue(pdfArray->arr[index], pdfArray->values_types[index]);
    }

    // free the size allocated for the array of pointers to the elements.
    free(pdfArray->arr);
    // free the array of integers representing the types of elements inside the array.
    free(pdfArray->values_types);
    // free the pdfArray object itself.
    free(pdfArray);
  }
}

PdfValObj pdfArrayAtGet(PdfArray *pdfArray, int index) {
  // check if the given array pointer is null.
  FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

  // check if the given index is positive.
  FAIL_IF_NEGATIVE(index, "Index is less than zero.\n");

  // check if Index is within range.
  if (index < pdfArray->size) {
    // if at the specified index there is not element return null pointer and PDFNULL.
    if (pdfArray->arr[index] == NULL || pdfArray->values_types[index] == PDFNULL) {
      return (PdfValObj){.ptr = pdfArray->arr[index], .type = PDFNULL};
    } else {
      return (PdfValObj){.ptr = pdfArray->arr[index], .type = pdfArray->values_types[index]};
    }
  } else { // not withing range always return null without extending the underlying arrays.
    return (PdfValObj){.ptr = pdfArray->arr[index], .type = PDFNULL};
  }
}

void pdfArrayAtSet(PdfArray *pdfArray, int index, PdfValObj pdfObjVal) {
  // check if the given array pointer is null
  FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

  // check if the given index is positive.
  FAIL_IF_NEGATIVE(index, "Index is less than zero.\n");

  // check if the given pdfObjVal is in a consistant state or not.
  if ((pdfObjVal.ptr == NULL && pdfObjVal.type != PDFNULL)||
      (pdfObjVal.ptr != NULL && pdfObjVal.type == PDFNULL)   ) {
    fprintf(stderr, " pdfObjVal is in an inconsistant state.\n");
    exit(-1);
  }

  // check the index is in range or not.
  if (index < pdfArray->size) {
    // check if there was an already element in that place if so free it first.
    if (pdfArray->arr[index] != NULL) {
      freePdfValue(pdfArray->arr[index], pdfArray->values_types[index]);
    }
    // assign the new element to the place in the pointers array.
    pdfArray->arr[index] = (void *)pdfObjVal.ptr; // the cast here is required to supress the
                                                  // warning of missing qualifiers "i.e.const".
    // update the type.
    pdfArray->values_types[index] = pdfObjVal.type;
  } else { // index is not within range, so extend.
    // enlarge the underlying array until the index is within range.
    while (index >= pdfArray->size) {
      // enlarge the size of the underlying array by a factor of 2.
      pdfArray->arr = realloc(pdfArray->arr, (pdfArray->size * 2) * (sizeof(void *)));
      // enlarge the array that record the typed of elements as well.
      pdfArray->values_types = realloc(pdfArray->values_types, (pdfArray->size * 2) * (sizeof(int)));

      // check of the reallocation failed for ethier one.
      FAIL_IF_NULL(pdfArray->arr, "Failed to resize the underlying array.\n");
      FAIL_IF_NULL(pdfArray->values_types, "Failed to resize the underlying array that record types.\n");

      // assign the new size to the member size.
      pdfArray->size = pdfArray->size * 2;
    }

    // assign the new element to the place in the pointers array.
    pdfArray->arr[index] = (void *)pdfObjVal.ptr; // the cast here is required to supress the
                                                  // warning of missing qualifiers "i.e.const".
    // update the type.
    pdfArray->values_types[index] = pdfObjVal.type;
  }
}

PdfDictionary *makePdfDictionary(int32_t size) {
  // check if the given inital cap is larger than zero
  FAIL_IF_ZERO_OR_LESS(size, "cannot create an a zero or less size capacity.\n");

  // allocate space for the newly created pdfArray object and check if the
  // allocation succeded.
  PdfDictionary *pdfDictionary = (PdfDictionary *)malloc(sizeof(PdfDictionary));
  FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

  // set the size of the newly created pdfObject object.
  pdfDictionary->size = size;

  // allocate space for the keys array of pointers and check if the allocation
  // succeded
  pdfDictionary->keys = (PdfName **)malloc(sizeof(PdfName *) * size);
  FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

  // allocate space for the values array of pointer to the actual elements
  // representing values for the keys, and check if the allocation succeded.
  pdfDictionary->values = (void **)malloc(sizeof(void *) * size);
  FAIL_IF_NULL(pdfDictionary->values, "Failed to allocate memroy for pdfDictionary object values objects array.\n");

  // allocate space for the array of integers representing the type of the
  // objects pointed by the array of pointers. and check if the allocation
  // succeded
  pdfDictionary->values_types = (int32_t *)malloc(sizeof(int32_t) * size);
  FAIL_IF_NULL(pdfDictionary->values, "Failed to allocate memroy for pdfDictionary array of integers representing types..\n");

  return pdfDictionary;
}

void freePdfDictionary(PdfDictionary *pdfDictionary) {
  if (NULL != pdfDictionary) {
    // free each key and value pointed by the keys and values array of pointers.
    for (int index = 0; index < pdfDictionary->size; index++) {
      // free the key.
      freePdfName(pdfDictionary->keys[index]);
      // free the value.
      freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
    }

    // free the space allocated for the array of pointers to the keys
    // "PdfName*".
    free(pdfDictionary->keys);

    // free the space allocated for the arrya of pointer to the values "void*"
    free(pdfDictionary->values);

    // free the space allocated for the array of integers allocated to represent
    // the types of values in each key-value pair.
    free(pdfDictionary->values_types);
    // free the space allocated for the object itself.
    free(pdfDictionary);
  }
}

PdfValObj pdfDictionaryGet(PdfDictionary *pdfDictionary, const char *NT_str) {
  // check if the Dictionary pointer is not NULL.
  FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
  // check if the given String is null.
  FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
  // check if the given string length is zero.
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");

  for (int index = 0; index < pdfDictionary->size; index++) {
    // check if the current index has a key,value pair
    if (pdfDictionary->keys[index] == NULL ) {
      continue;
    } else {
      // compare the key of the current index with the given key string.
      if (strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0) {
        // if the key sub string matches the given key, return the value.
        return (PdfValObj){.ptr = pdfDictionary->values[index], .type = pdfDictionary->values_types[index]};
      }
    }
  }
  // the whole dictionary does not have a key NT-str. so return null.
  return (PdfValObj){.ptr = NULL, .type = PDFNULL};
}

void pdfDictionarySet(PdfDictionary *pdfDictionary, const char *NT_str, PdfValObj pdfObjVal) {
  // check if the Dictionary pointer is not NULL.
  FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
  // check if the given String is null.
  FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
  // check if the given string length is zero.
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");
  
  // to record the last free position in the dictionary array.
  int last_free_position = -1;
  // check if the given pdfObjVal is consistant.
  if ((pdfObjVal.ptr == NULL && pdfObjVal.type != PDFNULL)||
      (pdfObjVal.ptr != NULL && pdfObjVal.type == PDFNULL)   ) {
    fprintf(stderr, "pdfObjVal is not consistant, type is not null but the "
                    "pointer point to null.\n");
    exit(-1);
  }

  for (int index = 0; index < pdfDictionary->size; index++) {
    // check if the current index has a key value pair
    if (pdfDictionary->keys[index] == NULL) {
      last_free_position = index;
      continue;
    } else {

      if (strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0) {
        // check if the value point to NON-pdfNULL "NULL" if so free before
        // assigning the new pointer.
        if (pdfDictionary->values[index] != NULL) {
          freePdfValue(pdfDictionary->values[index], pdfDictionary->values_types[index]);
        }
        // assign the new pdf value to the key and update the type.
        pdfDictionary->values[index] = (void *)pdfObjVal.ptr; // the cast here is to suppress the
                                                              // warining of the missing const qualifier lhs
        pdfDictionary->values_types[index] = pdfObjVal.type;
        return;
      }
    }
  }

  // there is not a single key with the value NT_str.
  // try to add one without extending the space for the old underlying arrays.
  if (last_free_position >= 0) {
    // just in case there is any thing wrong with the logic free the old value.
    freePdfValue(pdfDictionary->values[last_free_position], pdfDictionary->values_types[last_free_position]);

    // assign the new key and value and value type.
    pdfDictionary->keys[last_free_position] = makePdfName(NT_str);
    pdfDictionary->values[last_free_position] = (void *)pdfObjVal.ptr;
    pdfDictionary->values_types[last_free_position] = pdfObjVal.type;

  } else {
    // there is no free spot for the element.
    // double the size of the underlaying arrays array.

    pdfDictionary->keys = realloc(pdfDictionary->keys, pdfDictionary->size * 2 * sizeof(PdfName *));
    FAIL_IF_NULL(pdfDictionary->keys, "Failed to resize the old keys array.\n");

    pdfDictionary->values = realloc(pdfDictionary->values, pdfDictionary->size * 2 * sizeof(void *));
    FAIL_IF_NULL(pdfDictionary->values, "Failed to resize the old values array.\n");

    pdfDictionary->values_types = realloc(pdfDictionary->values_types, pdfDictionary->size * 2 * sizeof(int));
    FAIL_IF_NULL(pdfDictionary->values_types, "Failed to resize the old values_types array.\n");

    // assign the new element at the first open position.
    pdfDictionary->keys[pdfDictionary->size] = makePdfName(NT_str);
    pdfDictionary->values[pdfDictionary->size] = (void *)pdfObjVal.ptr;
    pdfDictionary->values_types[pdfDictionary->size] = pdfObjVal.type;

    // update the internal size member to the new value size * 2.
    pdfDictionary->size *= 2;
  }
}

PdfStream *makePdfStream(int32_t len) {
  FAIL_IF_ZERO_OR_LESS(len, "cannot create a stream object a zero or less len capacity.\n");

  // create a stream object and check if the allocation succeded.
  PdfStream *pdfStream = (PdfStream *)malloc(sizeof(PdfStream));
  FAIL_IF_NULL(pdfStream, "Failed to allocate memroy for PdfStream object.\n");

  // initialize the len member to the initial cap value.
  pdfStream->len = len;

  // allocate space for the actual stream of bytes, and check if the
  // allocation failed or not.
  pdfStream->stream = (uint8_t *)malloc(sizeof(uint8_t) * len);
  FAIL_IF_NULL(pdfStream->stream, "failed to allocate memory for the underlaying array of uint8_t.\n")

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

uint8_t pdfStreamGet(PdfStream *pdfStream, int index) {
  // check if the given stream pointer is not NULL.
  FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
  // this structure cannot have a return value that represent abscence like the pdf array or dictionary.
  // check the index if it falls inside the range or not.
  CHECK_INDEX(index, pdfStream->len);

  return pdfStream->stream[index];
}

void pdfStreamSet(PdfStream *pdfStream, int index, uint8_t value) {
  // check if the given stream pointer is not NULL.
  FAIL_IF_NULL(pdfStream, "Given Stream Pointer is null.\n");
  // check the index if it falls inside the range or not.
  FAIL_IF_NEGATIVE(index, "Index of stream must not be negative.\n ");

  // check if index is within range if not keep resizeing the stream by factor of 2 until the new index is withen range.
  while(index >= pdfStream->len){
    pdfStream->stream = (uint8_t*) realloc(pdfStream->stream, pdfStream->len * sizeof(uint8_t) * 2);
    FAIL_IF_NULL(pdfStream->stream, "Failed to resize the stream.\n");
    pdfStream->len = pdfStream->len * 2;
  }

  pdfStream->stream[index] = value;

}


PdfRef *makePdfRef(uint64_t generationNumber, uint64_t objectNumber) {
  // allocate space for the object.
  PdfRef *ptr = (PdfRef *)malloc(sizeof(PdfRef));
  // check if the allocation succeded.
  FAIL_IF_NULL(ptr, "failed to allocate memory for PdfRef.\n");
  // set the Generation Number.
  ptr->generationNumber = generationNumber;
  // set the identifer Number.
  ptr->objectNumber = objectNumber;

  return ptr;
}

void freePdfValue(void *ele, enum PDFVALUETYPE pdfValueType) {
  // check if the given pointer to pdf element is null or not.
  if (NULL != ele) {
    // if it's a simple pdf type "non-composite or aggregate" just freeing it is
    // enough.
    if (pdfValueType == PDFNUMBER || pdfValueType == PDFBOOLEAN || pdfValueType == PDFREAL || pdfValueType == PDFNULL || pdfValueType == PDFREF) {
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
      FAIL_WITH_MESSAGE("THIS PART OF THE CODE SHOULD NOT BE REACHED EVER.\n");
    }
  }
}

PdfNumber* makePdfNumber(int64_t value) {
  PdfNumber* pdfNumber = (PdfNumber*) malloc(sizeof(PdfNumber));
  FAIL_IF_NULL(pdfNumber , "Failed to allocate space for the PdfNumber.\n");
  pdfNumber->value = value;
  return pdfNumber; 
}

PdfReal* makePdfReal(double value) {
  PdfReal* pdfReal = (PdfReal*) malloc(sizeof(PdfReal));
  FAIL_IF_NULL(pdfReal , "Failed to allocate space for the PdfReal.\n");
  pdfReal->value = value;
  return pdfReal; 
}

PdfBoolean* makePdfBoolean(uint8_t value) {
  PdfBoolean* pdfBoolean = (PdfBoolean*) malloc(sizeof(PdfBoolean));
  FAIL_IF_NULL(pdfBoolean , "Failed to allocate space for the PdfBoolean.\n");
  pdfBoolean->value = value;
  return pdfBoolean; 
}

PdfIndirectObject* makePdfIndirectObject(uint64_t generationNumber, uint64_t objectNumber, void* pdfValuePtr, int32_t pdfValueType)
{
  PdfIndirectObject* pdfIndirectObject = (PdfIndirectObject*) malloc(sizeof(PdfIndirectObject));
  FAIL_IF_NULL(pdfIndirectObject, "Failed to allocate space for the pdf indirect object.\n");
  
  // check if the given pdf value is consistant with the type provided.
  if( (NULL == pdfValuePtr && pdfValueType != PDFNULL) ||
      (NULL != pdfValuePtr && pdfValueType == PDFNULL))
  {
    FAIL_WITH_MESSAGE("the given pdf value pointer is inconsistant with the given pdf value type,\n"
        "so either the given pdf value pointer is null and the type is not null or the other way around.\n");
  }
  
  pdfIndirectObject->generationNumber = generationNumber;
  pdfIndirectObject->objectNumber = objectNumber;
  pdfIndirectObject->pdfValuePtr = pdfValuePtr;
  pdfIndirectObject->pdfValueType = pdfValueType;

  return pdfIndirectObject;
}

