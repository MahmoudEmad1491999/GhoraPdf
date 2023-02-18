#include "pdf/lowlevel_api.h"
#include "helpers/macros.h"
#include <string.h>

PdfName *makePdfName(const char *NT_str) {
  FAIL_IF_NULL(NT_str, "Cannot construct pdfName object with null String.\n");

  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot create a pdfName object with zero lenght string.\n");

  PdfName *pdfName = (PdfName *)malloc(sizeof(PdfName));
  FAIL_IF_NULL(pdfName, "memeory Allocation FAILED for pdfName\n");

  size_t given_string_lenth = strlen(NT_str);
  // allocate a string on the heap to hold the given string.
  pdfName->NT_str = (char *)malloc(sizeof(char) * (given_string_lenth + 1));
  FAIL_IF_NULL(pdfName->NT_str, "memeory Allocation FAILED for pdfName's underlying string of characters\n");

  /* copy the given string to the newly allocated string and check if the copying succeded. */
  memcpy(pdfName->NT_str, NT_str, given_string_lenth + 1);
  FAIL_IF_NOT_EQUAL(strlen(pdfName->NT_str),given_string_lenth , "memory copy failed to copy the given string\n")

  return pdfName;
}

void freePdfName(PdfName *pdfName) {
  if (NULL != pdfName) {
    // free the underlying string first.
    free(pdfName->NT_str);
    // free the object.
    free(pdfName);
  }
}

PdfString *makePdfString(const char *NT_str) {
  FAIL_IF_NULL(NT_str, "Cannot construct pdfString object with null String.\n");
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot create a pdfName object with zero lenght string.\n");

  PdfString *pdfString = (PdfString *)malloc(sizeof(PdfString));
  FAIL_IF_NULL(pdfString, "memeory Allocation FAILED for pdfString\n");

  size_t given_string_length = strlen(NT_str);
  
  /* allocate a string on the heap to hold the given string. */
  pdfString->NT_str = (char *)malloc(sizeof(char) * (given_string_length + 1));
  FAIL_IF_NULL(pdfString->NT_str, "memeory Allocation FAILED for pdfString underalying string of characters\n");

  /* copy the given string to the newly allocated string and check if the coping succeded. */
  memcpy(pdfString->NT_str, NT_str, given_string_length  + 1);
  FAIL_IF_NOT_EQUAL(strlen(pdfString->NT_str), given_string_length, "memory copy failed to copy the given string\n")

  return pdfString;
}

void freePdfString(PdfString *pdfString) {
  if (NULL != pdfString) {
    // free the underlying string first.
    free(pdfString->NT_str);
    // then free the object itself.
    free(pdfString);
  }
}

PdfArray *makePdfArray(int32_t size) {
  FAIL_IF_ZERO_OR_LESS(size, "cannot create an a zero or less size.\n");

  // allocate space for the newly created pdfArray object and check if the
  // allocation succeded.
  PdfArray *pdfArray = (PdfArray *)malloc(sizeof(PdfArray));
  FAIL_IF_NULL(pdfArray, "Failed to allocate memroy for pdfArray object.\n");

  pdfArray->size = size;

  /* allocate space for the sub array of the pdfArry object and check if the
   allocation succeded. */
  pdfArray->arr = (void **)calloc(size,  sizeof(void *));
  FAIL_IF_NULL(pdfArray->arr, "Failed to allocate memroy for the sub array pdfArray object.\n");

  // allocate space for the sub array for the types of the sub array of
  // pdfArray and check if the allocation succeded.
  pdfArray->values_types = (enum PDF_VALUE_TYPE *)calloc(size , sizeof(enum PDF_VALUE_TYPE));
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

PdfValue pdfArrayAtGet(PdfArray *pdfArray, int index) {
  FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

  FAIL_IF_NEGATIVE(index, "Index is less than zero.\n");

  // check if Index is within range.
  if (index < pdfArray->size) {
    // if at the specified index there is not element return null pointer and PDFNULL.
    if (pdfArray->arr[index] == NULL || pdfArray->values_types[index] == PDF_NULL) {
      return (PdfValue){.ptr = NULL, .type = PDF_NULL};
    } else {
      return (PdfValue){.ptr = pdfArray->arr[index], .type = pdfArray->values_types[index]};
    }
  } else { // not withing range always return null without extending the underlying arrays.
    return (PdfValue){.ptr = NULL, .type = PDF_NULL};
  }
}

void pdfArrayAtSet(PdfArray *pdfArray, int index, PdfValue pdfObjVal) {
  FAIL_IF_NULL(pdfArray, "Given array pointer is null.\n");

  FAIL_IF_NEGATIVE(index, "Index is less than zero.\n");

  // check if the given pdfObjVal is in a consistant state or not.
  if ((pdfObjVal.ptr == NULL && pdfObjVal.type != PDF_NULL)||
      (pdfObjVal.ptr != NULL && pdfObjVal.type == PDF_NULL)   ) {
    FAIL_WITH_MESSAGE(" pdfObjVal is in an inconsistant state.\n");
  }

  // check the index is in range or not.
  if (index < pdfArray->size) {
    // check if there was an already element in that place if so free it first.
    if (pdfArray->arr[index] != NULL) {
      freePdfValue(pdfArray->arr[index], pdfArray->values_types[index]);
    }
    pdfArray->arr[index] = (void *)pdfObjVal.ptr; 
    
    pdfArray->values_types[index] = pdfObjVal.type;
  } else { // index is not within range, so extend.
    // enlarge the underlying array until the index is within range.
    while (index >= pdfArray->size) {
      // enlarge the size of the underlying array by a factor of 2.
      pdfArray->arr = realloc(pdfArray->arr, (pdfArray->size * 2) * (sizeof(void *)));
      // enlarge the array that record the typed of elements as well.
      pdfArray->values_types = realloc(pdfArray->values_types, (pdfArray->size * 2) * (sizeof(enum PDF_VALUE_TYPE)));

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
  FAIL_IF_ZERO_OR_LESS(size, "cannot create an a zero or less size capacity.\n");

  PdfDictionary *pdfDictionary = (PdfDictionary *)malloc(sizeof(PdfDictionary));
  FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

  pdfDictionary->size = size;

  /* allocate space for the keys array of pointers and check if the allocation
   succeded */
  pdfDictionary->keys = (PdfName **)calloc(size, sizeof(PdfName *));
  FAIL_IF_NULL(pdfDictionary, "Failed to allocate memroy for pdfDictionary object.\n");

  /* allocate space for the values array of pointer to the actual elements
   representing values for the keys, and check if the allocation succeded. */
  pdfDictionary->values = (void **)calloc(size, sizeof(void *));
  FAIL_IF_NULL(pdfDictionary->values, "Failed to allocate memroy for pdfDictionary object values objects array.\n");

  // allocate space for the array of integers representing the type of the
  // objects pointed by the array of pointers. and check if the allocation
  // succeded
  pdfDictionary->values_types = (enum PDF_VALUE_TYPE*)calloc(size, sizeof(enum PDF_VALUE_TYPE));
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

PdfValue pdfDictionaryGet(PdfDictionary *pdfDictionary, const char *NT_str) {
  FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
  FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");

  for (int index = 0; index < pdfDictionary->size; index++) {
    // check if the current index has a key,value pair
    if (pdfDictionary->keys[index] == NULL ) {
      continue;
    } else {
      // compare the key of the current index with the given key string.
      if (strcmp(NT_str, pdfDictionary->keys[index]->NT_str) == 0) {
        // if the key sub string matches the given key, return the value.
        return (PdfValue){.ptr = pdfDictionary->values[index], .type = pdfDictionary->values_types[index]};
      }
    }
  }
  // the whole dictionary does not have a key NT-str. so return null.
  return (PdfValue){.ptr = NULL, .type = PDF_NULL};
}

void pdfDictionarySet(PdfDictionary *pdfDictionary, const char *NT_str, PdfValue pdfObjVal) {
  FAIL_IF_NULL(pdfDictionary, "Given dictionary pointer is null.\n");
  FAIL_IF_NULL(NT_str, "Given String is Null pointer.\n");
  FAIL_IF_STRINT_LEN_ZERO(NT_str, "Cannot search of an element with the key name with zero lenght.\n");
  
  // check if the given pdfObjVal is consistant.
  if ((pdfObjVal.ptr == NULL && pdfObjVal.type != PDF_NULL)||
      (pdfObjVal.ptr != NULL && pdfObjVal.type == PDF_NULL)   ) {
    FAIL_WITH_MESSAGE("pdfObjVal is not consistant, type is not null but the pointer point to null.\n");
  }
  // to record the last free position in the dictionary array.
  int last_free_position = -1;

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
        pdfDictionary->values[index] = (void *)pdfObjVal.ptr; 
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

    pdfDictionary->values_types = realloc(pdfDictionary->values_types, pdfDictionary->size * 2 * sizeof(enum PDF_VALUE_TYPE));
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
  pdfStream->stream = (uint8_t *)calloc(len, sizeof(uint8_t));
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

PdfIndirectObject* makePdfIndirectObject(uint64_t generationNumber, uint64_t objectNumber, void* pdfValuePtr, enum PDF_VALUE_TYPE pdfValueType)
{
  PdfIndirectObject* pdfIndirectObject = (PdfIndirectObject*) malloc(sizeof(PdfIndirectObject));
  FAIL_IF_NULL(pdfIndirectObject, "Failed to allocate space for the pdf indirect object.\n");
  
  // check if the given pdf value is consistant with the type provided.
  if( (NULL == pdfValuePtr && pdfValueType != PDF_NULL) ||
      (NULL != pdfValuePtr && pdfValueType == PDF_NULL))
  {
    FAIL_WITH_MESSAGE("invalid argument to indirect object construcutor function.\n");
  }
  
  pdfIndirectObject->generationNumber = generationNumber;
  pdfIndirectObject->objectNumber = objectNumber;
  pdfIndirectObject->pdfValuePtr = pdfValuePtr;
  pdfIndirectObject->pdfValueType = pdfValueType;

  return pdfIndirectObject;
}

void freePdfValue(void *ele, enum PDF_VALUE_TYPE pdfValueType) {
  // check if the given pointer to pdf element is null or not.
  if (NULL != ele) {
    // if it's a simple pdf type "non-composite or aggregate" just freeing it is
    // enough.
    if (pdfValueType == PDF_NUMBER || pdfValueType == PDF_BOOLEAN || pdfValueType == PDF_REAL || pdfValueType == PDF_REF || pdfValueType == PDF_NULL) {
      free(ele);
    } else if (pdfValueType == PDF_STRING) {
      freePdfString(ele);
    } else if (pdfValueType == PDF_NAME) {
      freePdfName(ele);
    } else if (pdfValueType == PDF_DICTIONARY) {
      freePdfDictionary(ele);
    } else if (pdfValueType == PDF_STREAM) {
      freePdfStream(ele);
    } else if (pdfValueType == PDF_ARRAY) {
      freePdfArray(ele);
    } 
    else {
      // this means that the pdf value type has an unknown value.
      FAIL_WITH_MESSAGE("THIS PART OF THE CODE SHOULD NOT BE REACHED EVER.\n");
    }
  }
}

void freePdfInDirectObject(PdfIndirectObject *pdfIndirectObject)
{
  if(NULL != pdfIndirectObject)
  {
    freePdfValue(pdfIndirectObject->pdfValuePtr, pdfIndirectObject->pdfValueType);
    free(pdfIndirectObject);
  }
}


Number32Pool* makeNumber32Pool(uint32_t initialFreeListSize)
{
  Number32Pool* number32Pool = (Number32Pool*) malloc(sizeof(Number32Pool));
  FAIL_IF_NULL(number32Pool, "Failed to allocate space for the number pool object.\n");
  
  number32Pool->nextToUse = 0;
  number32Pool->satuarated = 0;

  number32Pool->freedList = (uint32_t**) calloc(initialFreeListSize, sizeof(uint32_t*));
  FAIL_IF_NULL(number32Pool->freedList, "Failed to allocate space for the freed list.\n");
  
  number32Pool->freedListSize = initialFreeListSize;

  return number32Pool;
}

void freeNumber32Pool(Number32Pool* number32Pool)
{
  if(NULL != number32Pool)
  {
    // first: loop the whole space allocated for pointer to freed number and free the pointed to spaces.
    for(int index = 0; index < number32Pool->freedListSize; index++)
    {
      if(NULL != number32Pool->freedList[index])
      {
        free(number32Pool->freedList[index]);
      }
    }
    // second: free the array used to store the pointers to the freed numbers.
    free(number32Pool->freedList);
    // third free the whole number32Pool structure.
    free(number32Pool);
  }
}

uint32_t Number32PoolGetNext(Number32Pool *number32Pool, enum NumberPoolWithDrawalMode mode)
{
  // check if the given number pool is not null.
  FAIL_IF_NULL(number32Pool, "the given number pool is null.\n");
  
  // based on the mode of withdrawal proceed.
  if(UNUSEDFIRST == mode)
  {
    // if the withdrawal is performed from the unused list check if the pool is satuarated.
    if(number32Pool->satuarated == 1)
    {
      // query the freed list for a number to return cause the unsued list is fully consumed.
        uint32_t valueToReturn;
        for (uint32_t index = 0; index < number32Pool->freedListSize; index++) {
          // if the address at the element is not null this means this element is ready to be used.
          if (number32Pool->freedList[index] != NULL) {
            // get the value of the number.
            valueToReturn = *(number32Pool->freedList[index]);
            // free the space used to hold the freed number.
            free(number32Pool->freedList[index]);
            // set the recording address to NULL so it's not a dangling pointer.
            number32Pool->freedList[index] = NULL;
            // return the number;
            return valueToReturn;
          }
        }
        // reaching this line means that the bool is fully consumed some. so Failing is the option to go.
        FAIL_WITH_MESSAGE("The number pool is fully satuarated.\n");
    }
    else {
      // this branch means that the pool is not fully saturated. so increment the list of used.
      number32Pool->nextToUse++;
      // check if we reached saturation if so set the saturation flag.
      if(number32Pool->nextToUse == UINT32_MAX)
      {
        number32Pool->satuarated = 1;
      }
      // return the consumed number.
      return number32Pool->nextToUse - 1;
    }
  }
  else if(FREEDFIRST == mode){
      // if the withdrawal is performed from the freed list if possible, then
      // see if we have any freed members and loop til we find the index at which the pointer is not null.
      uint32_t valueToReturn;
      for(uint32_t index = 0; index < number32Pool->freedListSize; index++)
      {
        if(NULL != number32Pool->freedList[index])
        {
          // record the value pointed by the pointer.
          valueToReturn = * (number32Pool->freedList[index]);
          // free the space used to hold the previously freed number.
          free(number32Pool->freedList[index]);
          // set the pointer to null so it's not a dangling pointer.
          number32Pool->freedList[index] = NULL;
          // return the value to be consumed..
          return valueToReturn;
        }
      }
      // if we reached the following code this means that the free list is empty.
      // so we are going to get going with the unused list if it's not fully consumed.
      if(number32Pool->satuarated != 1) // the unused list is not fully consumed.
      {
        number32Pool->nextToUse++;
        // check if we reached saturation if so set the saturation flag.
        if (number32Pool->nextToUse == UINT32_MAX) {
          number32Pool->satuarated = 1;
        }
        // return the consumed number.
        return number32Pool->nextToUse - 1;
      }
      else { // the unused list is fully consumed aswell so the whole number pool is fully consumed.
        FAIL_WITH_MESSAGE("The number pool is fully satuarated.\n");
      }
  }
  else {
    FAIL_WITH_MESSAGE("THIS PART OF THE CODE SHOULD NOT BE REACHED AT ALL.");
  }
}



void NUmber32PoolFreeNumber(Number32Pool* number32Pool, uint32_t valueToFree)
{
  FAIL_IF_NULL(number32Pool, "The given pointer is null.\n");
  
  int32_t last_free_position = -1;
  
  // check if the given element is already freed.
  for(uint32_t index = 0; index < number32Pool->freedListSize; index++)
  {
    if(NULL != number32Pool->freedList[index])
    {
      if(*(number32Pool->freedList[index]) == valueToFree)
      {
        FAIL_WITH_MESSAGE("the number is already freed");
      }
    }else {
      last_free_position = index;
    }
  }
  
  // reaching this part of the code means that the element is not already freed.
  // check if there is space on the free numbers list.
  if(last_free_position >= 0)
  {
    // there is space to record so allocate space for the freed number and assign that space
    // to the last free postion on the array.
    number32Pool->freedList[last_free_position] = malloc(sizeof(uint32_t));
    // record the value freed in the allocated space from the previous line.
    *(number32Pool->freedList[last_free_position]) = valueToFree;
  }
  else 
  { 
    // there is not space for the to be freed number on the current free list.
    // resize the list of pointer used to record the address of the memeory which hold the freed numbers.
    number32Pool->freedList = realloc(number32Pool->freedList, number32Pool->freedListSize *  sizeof(uint32_t*) * 2);
    FAIL_IF_NULL(number32Pool->freedList, "resizing the freed number list failed.\n");
    
    // allocate space for the memory that hold the free number.
    number32Pool->freedList[number32Pool->freedListSize] = malloc(sizeof(uint32_t));
    FAIL_IF_NULL(number32Pool->freedList[number32Pool->freedListSize], "failed to allocate space for the to be freed number.\n");
    
    // put the value to be freed into the allocated space from the previous line.
    *(number32Pool->freedList[number32Pool->freedListSize]) = valueToFree;
    
    // update the size of the new free list to be twice the size.
    number32Pool->freedListSize *= 2;
  }
}

PdfFile* makePdfFile(uint32_t initialCapacity)
{
  FAIL_IF_ZERO(initialCapacity, "Size of any thing cannot be zero.\n");

  PdfFile* pdfFile = malloc(sizeof(PdfFile));

  // create a new number pool with a freed list of 50 numbers.
  // i know bad design choice is to hard code it but fuck it.
  pdfFile->number32Pool = makeNumber32Pool(50);
  // set the static members of the pdf file, i.e. capacity, next free spot.
  pdfFile->currentCapacity = initialCapacity;
  pdfFile->nextEmpty = 0;

  // allocate zeroed out space for the list of pointers to the indirect objects.
  pdfFile->pdfIndirectObjectsList = calloc(initialCapacity, sizeof(PdfIndirectObject*));
  FAIL_IF_NULL(pdfFile->pdfIndirectObjectsList, "Failed to allocate memory for the list of pointers.\n");

  return pdfFile;
}

void freePdfFile(PdfFile *pdfFile) {
  if (NULL != pdfFile) {
    // first free the number pool.
    freeNumber32Pool(pdfFile->number32Pool);
    for(int index = 0; index < pdfFile->currentCapacity;index++)
    {
      // free each and every indirect object using the freeing function.
      freePdfInDirectObject(pdfFile->pdfIndirectObjectsList[index]);
    }
    // free the list of pointers used to store the refreneces to the actual PdfIndirectObject objects.
    free(pdfFile->pdfIndirectObjectsList);
    // free the pdf file structure.
    free(pdfFile);
  }
}

uint64_t appendPdfValueToList(PdfFile *pdfFile, void* pdfValuePtr, enum PDF_VALUE_TYPE pdfValueType)
{
  FAIL_IF_NULL(pdfFile, "Cannot Append To A NULL PdfFile.\n");
  
  // check if the underlying list is full or not through the next available spot.
  if (pdfFile->nextEmpty < pdfFile->currentCapacity) {
    // there is still space.
    pdfFile->pdfIndirectObjectsList[pdfFile->nextEmpty] = makePdfIndirectObject(0, Number32PoolGetNext(pdfFile->number32Pool, UNUSEDFIRST), pdfValuePtr, pdfValueType);
    // increment the next spot indicator.
    pdfFile->nextEmpty++;
    return pdfFile->pdfIndirectObjectsList[pdfFile->nextEmpty - 1]->objectNumber;
  } else {
    // the list is out of space.
    pdfFile->pdfIndirectObjectsList = realloc(pdfFile->pdfIndirectObjectsList, pdfFile->currentCapacity * sizeof(PdfIndirectObject *) * 2);
    FAIL_IF_NULL(pdfFile->pdfIndirectObjectsList, "Resizing failed for the list of indirect objects.\n");
    
    // update the capacity.
    pdfFile->currentCapacity *= 2;

    pdfFile->pdfIndirectObjectsList[pdfFile->nextEmpty] = makePdfIndirectObject(0, Number32PoolGetNext(pdfFile->number32Pool, UNUSEDFIRST), pdfValuePtr, pdfValueType);
    // increment the next spot indicator.
    pdfFile->nextEmpty++;
    pdfFile->nextEmpty++;
    return pdfFile->pdfIndirectObjectsList[pdfFile->nextEmpty - 1]->objectNumber;
  }
}
