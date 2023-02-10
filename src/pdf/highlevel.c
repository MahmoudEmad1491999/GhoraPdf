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

  number32Pool->freedList = (uint32_t**) malloc(sizeof(uint32_t*) * initialFreeListSize);
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
      if(number32Pool->freedList[index] != NULL)
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
        if(number32Pool->freedList[index] != NULL)
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
    FAIL_WITH_MESSAGE("dangling else branch.\n");
  }
}



void NUmber32PoolFreeNumber(Number32Pool* number32Pool, uint32_t valueToFree)
{
  // check if the given number32Pool is null.
  FAIL_IF_NULL(number32Pool, "The given pointer is null.\n");
  
  // this vairable is used to record the last free position in the array of pointers.
  int32_t last_free_position = -1;
  
  // check if the given element is already freed.
  for(uint32_t index = 0; index < number32Pool->freedListSize; index++)
  {
    if(number32Pool->freedList[index] != NULL)
    {
      if(*(number32Pool->freedList[index]) == valueToFree)
      {
        fprintf(stdout, "the number \"%d\" is already freed", valueToFree);
        return;
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


