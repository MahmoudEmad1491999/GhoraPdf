#ifndef LOWLEVEL_API_H
#define LOWLEVEL_API_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pdf/lowlevel_defs.h"
/**
 * Purpose:     make a new PdfName object on the heap and initialize it by copying.
 * @str         the string of null terminated characters to copy.
 * @return      pointer to the created object.
 * Note:        free using the freePdfName() funciton
 */
PdfName* makePdfName(const char* str);

/**
 * this function shold be called to free object created by makePdfName
 * to free the underlaying array of characters aswell.
 * @pdfName         pointer of the object to be freed.
*/
void freePdfName(PdfName* pdfName);

/**
 * Purpose:     make a new PdfString object on the heap and initialize it by copying.
 * @str         the string of null termainted characters to copy from.
 * @return      pointer to the created object.
  */
PdfString* makePdfString(const char* str);

/** 
 * Purpose:         free the object pointed by pdfString and the underlying string.
 * @pdfName         pointer of the object to be freed.
 */
void freePdfString(PdfString* pdfString);

/**
 * Purpose:             create a new pdfArray object on the heap and allocate the space for the
 *                      elements.
 * @size                the maximus number of elements the array can hold.
 * @return              pointer to the create pdfArray object.
 */
PdfArray* makePdfArray(int32_t size);


/* Purpose:             this function shold be called to free object created by makePdfArray 
 *                      to free the underlaying elements aswell.
 * @pdfArray            pointer to the object to be freed.
*/
void freePdfArray(PdfArray* pdfArray);

/**
 * Purpose:             create a new pdfDictionary object on the heap and allocate space for the underlying 
 *                      key/value pairs given their number
 * @size                the maximum number of key/value pairs the dictionary can hold.
 * @return              pointer the newly created PdfDictionary object.
 */
PdfDictionary* makePdfDictionary(int32_t size);


/** Purpose:            this function shold be called to free object created by makePdfDictionary and to free
 *                      the underalying keys and values and types array of object using the correct functions.
 * @pdfDictionary       pointer the dictinoary to be freed with the underlaying composing elements.
 */
void freePdfDictionary(PdfDictionary* pdfDictionary);

/**
 * Purpose:             create a new PdfStream object on the heap and allocate space for the underlying 
 *                      array of bytes given the length.
 * @len                 the length of the underlying array.
 */
PdfStream* makePdfStream(int32_t len);

/**
 * Purpose:             this function shold be called to free object created by makePdfStream 
 *                      to free the underlaying uint8_t array aswell.
 * @pdfStream           pointer to the PdfStream object to be freed with underlaying array aswell.
 */
void freePdfStream(PdfStream* pdfStream);

/**
 * Purpose:             to make it more like a normal array the this function is used as the subscription operator 
 *                      for the PdfArray type.
 * @pdfArray            the Array to be queried.
 * @index               the index of the element requested.
 * @return              the PdfObjVal that holds the pointer to the element and the type, both as memebers.
 * 
 * Failure senarios:    
 *                      1) if the given array is null it will close the app.
 *                      2) if the index is negative will close the app.
 *                      
 * Notes:               1) if the index in question has a null as an element it will return 
 *                         Null as the Pointer to the element and PDFNULL as the type regardless of the value of pdfArray->values_types[index]
 *                      2) this function return the address of the element as a memeber to PdfObjVal so don't free it.
 *                      3) this function will return NULL pdfObjVal if the index is larger than the size without actual resizing.
 */
PdfValue pdfArrayAtGet(PdfArray* pdfArray, int32_t index);

/**
 * Purpose:             to make it more like a normal array the this function is used as the subscription operator 
 *                      for the PdfArray type.
 *
 * @pdfArray            the Array to be queried.
 * @index               the index of the element requested.
 * @pdfObjVal           the element and the type to store at the specified index as requested.
 * 
 * Failure Senarios:    
 *                      1) if the given array is null, it will close the app with error code.
 *                      2) if the index is negative, it will close the app with error code.
 *                      3) if the @pdfObjVal.ele is Null and the @pdfObjVal.type is not PDFNULL,
 *                         or @pdfObjVal.type is PDFNULL and @pdfObjVal.ptr is not NULL it will close the app with error code. 
 * 
 * Notes: 
 *                      1) this function does not only set the new element at the specified position but also it free the old element
 *                         using the correct freeing function unless ofcourse it's a PdfNull object no need to free anything.
 *                      2) this function will keep extending the size of the underlying arrays if the index is larger than the current size.
 *
 */
void pdfArrayAtSet(PdfArray* pdfArray, int32_t index, PdfValue pdfObjVal);

/**
 * Purpose:             to make it more like a normal associative array, a.k.a. dictionary, the this function is used as
 *                      the subscription operator for the PdfDictionary type.
 * @pdfDictionary       the pdfDictionary or associative array to be queried.
 * @NT_str              the key of the value requested.
 * @return              the PdfObjVal that holds the pointer to the value element and the type, both as memebers.
 * 
 * Failure senarios:    
 *                      1) if the given dictionary is null it will close the app with the correct error code.
 *                      2) if the given Null terminated string length is zero or the NT_str itself is NULL
 *                         it will close the application with the correct error code.
 *                      
 * Notes:               1) if the PdfName (NT_str) in question has a null as an element it will return 
 *                         Null as the Pointer to the element and PDFNULL as the type.
 *                      2) this function return the address of the element as a memeber to PdfObjVal so don't free it,
 *                         or it will put this structure in an invalid state.
 *                      3) if the NT_str is not found, that is it don't have a PdfName with the same value in the keys array,
 *                         it will always return PDFNULL as a type and null as a pointer.
 *                         this actually what the standard state about abscence of keys.
 */
PdfValue pdfDictionaryGet(PdfDictionary* pdfDictionary, const char* NT_str);

/**
 * Purpose:             to make it more like a normal associative array the this function is used as the subscription operator 
 *                      for the PdfArray type.
 *
 * @pdfDictionary       the pdfDictionary/associative array to be queried.
 * @NT_str              the key of the value requested.
 * @pdfObjVal           the element and the type to store at the specified index as requested.
 * 
 * Failure Senarios:    
 *                      1) if the given @pdfDictionary is null, it will close the app with error code.
 *                      2) if the given Null terminated string length is zero or the NT_str itself is NULL
 *                         it will close the application with the correct error code.
 *                      3) if the @pdfObjVal.ele is Null and the @pdfObjVal.type is not PDFNULL,
 *                         or @pdfObjVal.type is PDFNULL and @pdfObjVal.ptr is not NULL it will close the app with error code. 
 * 
 * Notes: 
 *                      1) this function does not only set the new element at the specified position but also it free the old element
 *                         using the correct freeing function unless ofcourse it's a PdfNull object no need to free anything.
 *                      2) if the dictionary does not have an element with the NT_str key it will set one in the last open key/value pair in the array.
 *                         if there is not suce place it will double the size of the underlying arrays and assign the new given object the first
 *                         open position.
 */
void pdfDictionarySet(PdfDictionary* pdfDictionary, const char* NT_str, PdfValue pdfObjVal);

/*
 * Purpose:             get the value of the pdfStream at the specified index.
 *
 * @pdfStream           is the stream to use as the inquestion search place.
 * @index               is the index of the byte of interest.
 * @return              the value at the specified index.
 *
 * Failure Senarios:    
 *                      1) if the given pdfStream  is null it will close the app with the correct error code.
 *                      2) if the given index is larger than the len of the stream it will close the app with 
 *                         correct error code.
 */
uint8_t  pdfStreamGet(PdfStream* pdfStream, int index);
/*
 * Purpose:             set the pdfStream to the given value at the specified index.
 *
 * @pdfStream           is the stream to use as the inquestion search place.
 * @index               is the index of the byte of interest.
 * @value               the value at the to be inserted specified index.
 * Failure Senarios:    
 *                      1) if the given pdfStream  is null it will close the app with the correct error code.
 *                      2) if the given index is negative, it will close the app with 
 *                         correct error code.
 *
 * Notes:               1) if the given index is larger than the stream len it will keep resizing,
 *                         the stream until index is less than the stream length, then set the @index position with
 *                         @value 
 */
void pdfStreamSet(PdfStream* pdfStream, int index, uint8_t value);

/**
 * Purpose              used to make a pdfnumber object on the heap with the provieded value.
 * @value               the value of the number.
 * @return              pointer to the created object.
 */
PdfNumber* makePdfNumber(int64_t value);

/**
 * Purpose              used to make a pdfRean object on the heap with the provieded value.
 * @value               the value of the Real Number.
 * @return              pointer to the created object.
 */
PdfReal* makePdfReal(double value);

/**
 * Purpose              used to make a pdfBoolean object on the heap with the provieded value.
 * @value               the value of the boolean.
 * @return              pointer to the created object.
 */
PdfBoolean* makePdfBoolean(uint8_t value);

/**
 * Purpose:             create a new pdf reference  object with the given object id 
 *                      "Note object id is the pair generation number and other object number"
 * generationNumber     the referenced object generationNumber
 */

PdfRef* makePdfRef(uint64_t generationNumber, uint64_t id);

/**
 * Purpose:             this function is used to create an pdf indirect object 
 *
 * @generationNumber    the generation number of the object.
 * @objectNumber        the object number of the object.
 * @pdfVal              pointer to the pdfValue object.
 * @objectType          the type of the pdf object.
 */
PdfIndirectObject* makePdfIndirectObject(uint64_t generationNumber, uint64_t objectNumber, void* pdfValue_ptr, enum PDF_VALUE_TYPE pdfValueType);
/**
 * Purpose:             this function is used to free the space occupied by the indirect object and PdfIndirectObject.
 * @pdfIndirectObject   pointer to the PdfIndirectObject object to free with the underlying pdfValue.
 */
void freePdfInDirectObject(PdfIndirectObject* pdfIndirectObject);

/**
 * Purpose:             free the pdfValue object pointed by the @ptr
 * @ptr                 pointer to the pdf object to be freed.
 * @pdfValueType        this is the type of the element pointed by ptr. 
 *
 * NOTE:
 *                      1) If ele is null noting happen.
 */
void freePdfValue(void* ptr, enum PDF_VALUE_TYPE pdfValueType);

// pdf file is a list of indirect objects. each object has a unique object id.
// so we need to implement a certain type of pool of integers for the object numbers to have.
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
