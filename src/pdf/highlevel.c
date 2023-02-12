#include "pdf/lowlevel.h"
#include "pdf/highlevel.h"
#include "helpers/macros.h"
#include <stdint.h>
#include <stdlib.h>


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
























