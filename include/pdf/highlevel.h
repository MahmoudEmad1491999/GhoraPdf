#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel.h"



enum NumberPoolWithDrawalMode { FREEDFIRST, UNUSEDFIRST};
// pdf file is a list of indirect objects. each object has a unique object id.
// so we need to implement a certain type of pool of integers for the object numbers to have.
typedef struct
{
  uint32_t   nextToUse;                     // hold the next value to use.
  uint32_t** freedList;                      // holds the list of pointer to free number after being used.
  uint32_t   freedListSize;                 // the maximum number of free numbers that the number pool can have as a whole.
  uint8_t satuarated;                       // 0 mean that the unused list still have elements to offer.
                                            // 1 means that the unused list is fully consumed.
                                            // 2 means that the unused list is fully consumed and 
                                            //   freed list is fully consumed.
} Number32Pool;

typedef struct {
  Number32Pool* number32Pool;
  PdfIndirectObject** pdfIndirectObjectsList;
} PdfFile;


/**
 * Purpose:                 create a new number pool. 
 *
 * @initialFreeListSize     the size of the freelist before any freeing.
*/
Number32Pool* makeNumber32Pool(uint32_t initialFreeListSize);
/*
 * Purpose:                 used to free the whole Number32Pool strucutre and the underlying
 *                          array of pointers and the pointed to numbers by those pointers.
 * @number32Pool            pointer to the number32Pool strucutre itself.
 * */
void freeNumber32Pool(Number32Pool* number32Pool);

/**
 * Purpose:                get the next number to use from a certain Number32Pool.
 *
 * @number32Pool            the number pool to perform the operation on.
 * @mode                    determine the source of the number is it the unused list,
 *                          or the freed list if possible.
 * @return                  the number to use.
 */

uint32_t Number32PoolGetNext(Number32Pool* number32Pool,enum NumberPoolWithDrawalMode mode);      

/**
 * Purpose:                 free a certain number from a number pool.
 * @number32Pool            the number pool to perform the operation on.
 * @value                   the value to free.
 *
 *
 * Note:
 *                          1) if the number is already freed nothing happens.
 */
void Number32PoolFreeNumber(Number32Pool* number32Pool, uint32_t valueToFree); // function used to free the number @valueToFree.

#endif
