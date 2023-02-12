#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel.h"
#include <wchar.h>



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
  Number32Pool* number32Pool;                     // the number pool to use when constructing the file's indirect objects.
  PdfIndirectObject** pdfIndirectObjectsList;     // array of pointers to indirect pdf objects.
  uint32_t currentCapacity, nextEmpty;                                  // this is the current number of possible indirect object the file can hold.
} PdfFile;

typedef struct {
  uint64_t objectNumber;
  uint64_t generationNumber;                   // most of the time if not always zero.
}PdfObjectId;
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

/**
 * Purpose:                 this function is used to make a new pdf file object with the given number32Pool
 *
 * @size                    the intial number of indirect object this file object can have.
 */
PdfFile* makePdfFile(uint32_t size);


/*
 * Purpose                  this function is used to free a PdfFile object with all the underlying indirect object.
 * @pdfFile                 Pointer to the object to free can be null.
 */
void freePdfFile(PdfFile* pdfFile);

/**
 * Purpose:                 this function is used to append indirect ojbect to the list of pointer of a pdf file and 
 *                          double the length of the list if necessary.
 * @PdfFile                 pointer to the PdfFile object to append to.
 * @pdfValuePtr             pointer to the value to be appended.
 * @pdfValueType            the type of value to be recoreded in the encasulating PdfIndirectObject
 */
uint64_t appendPdfValueToList(PdfFile *pdfFile, void* pdfValuePtr, enum PDF_VALUE_TYPE pdfValueType);

#endif
