#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// these structure are verys simple and represent the possible values that can be denoted in pdf format.
// but this implementation encourage the use of dynamically allocated variables of those structure types.
// so that no memory leaks happen.
//
typedef struct {
    int64_t value;
} PdfNumber;

typedef struct {
    double value;
} PdfReal;

typedef struct {
    unsigned char value;
} PdfBoolean;


/**
 * this structure store the string characters as null termainted string.
 */
typedef struct {
    char* str;
} PdfString;

/**
 * this structure store the string characters as null termainted string.
 */

typedef struct {
    char* str;
} PdfName;

typedef struct {
/**
 * represent dynamically allocated array structure the holds dynamically allocated pdf object types.
 * @arr             represent array of pointer to dynamically allocated pdfvalues.
 * @cap             the capacity of this array.
 * @size            the actual occupied size by the currently present elements in the array.
 * @values_types    due to the nature or pdf array construct it holds elements of differnet type, so this list
 *                  actually record the types of each individual element 
 */
    void** arr;
    int32_t cap, size;
    int32_t* values_types;
} PdfArray;

typedef struct {
/**
 * this structure represent the dictionary construct in the pdf standard. because the pdf states that the keys has 
 * the pdf value type Name we only need to store the types of the value structure.
 * @keys pointer to the array of pointers to pdf names.
 * @values pointer to an array of pointers to the values.
 * @valuetypes array of types representing the types of each individual value stored.
 */
    PdfName** keys;
    void** values;
    int32_t cap,len;
    int* values_types;
} PdfDictionary;

typedef struct 
{
    /**
     * stream is a sequence of bytes usually representing either text or graphics command.
     * @stream array of unsigned bytes.
     * @len the actual length of the array.
     */
    uint8_t* stream;
    int32_t len;
} PdfStream;

typedef struct 
{} PdfNull;

/**
 * this function will return the address of a newly created pdfName on the heap that is 
 * independent even from the str characters, which means it's the responsiblity of the client
 * to free the given characters if it was dynamically allocated.
 * NOTE:
 *      freePdfName() must be used to free the object or the underlaying character array
 *      will causei memory leak
 */
PdfName* makePdfName(const char* str);

/**
 * this function will return the address of a newly created pdfString on the heap that is 
 * independent even from the str characters, which means it's the responsiblity of the client
 * to free the given characters if it was dynamically allocated.
 * NOTE:
 *      freePdfString() must be used to free the object or the underlaying character array
 *      will causei memory leak
 */
PdfString* makePdfString(const char* str);

/**
 * this function is the sole point you should used to create pdf array values.
 * @initialCap is the intialCapacity of the Array.
 * NOTE:
 *      freePdfArray() must be used to free the Array or the underlaying elements will cause 
 *      a memory leak.
 */
PdfArray* makePdfArray(int32_t initialCap);

/**
 * this function is the sole point you should used to create pdf dictionary values.
 * @initialCap is the intialCapacity of the Array.
 * NOTE:
 *      freePdfDictionary() must be used to free the Dictionary or the underlaying keys/values will cause 
 *      a memory leak.
 */
PdfDictionary* makePdfDictionary(int32_t initialCap);


/**
 * this function is the sole point you should used to create pdf stream values.
 * @initialCap is the intialCapacity of the Array.
 * NOTE:
 *      freePdfStream() must be used to free the stream or the underlaying character array will cause 
 *      a memory leak.
 */
PdfStream* makePdfStream(int32_t initialCap);


/**
 * to mimec the behavior expected from an array this function is ued to get the element
 * at index index for the array pdfarray, or it will return NULL if the index is out of bounds.
 * 0 <= index < pdfArray->cap
 * @pdfArry is the array to try fetch the element from.
 * @index is the requested index.
 * NOTE:
 *      this function return the element on the heap by address in other words don't free
 *      address or the free array function will fail.
 *
 */
void* pdfArrayAtGet(PdfArray* pdfArray, int32_t index);

/**
 * to mimec the behavior expected from an array this function is ued to set the element
 * at index index for the array pdfarray, or it will return NULL if the index is out of bounds.
 * 0 <= index < pdfArray->cap
 * @pdfArry is the array to try fetch the element from.
 * @index is the requested index.
 * NOTE: 
 *      this function is used to set the element at the specified index 
 *      it will also free the previous element at the specified index so any references
 *      to the old elemenet will segfault.
 */
void pdfArrayAtSet(PdfArray* pdfArray, int32_t index, void* ele);




// this function shold be called to free object created by makePdfName to free the underlaying array of characters also.
void freePdfName(PdfName* pdfName);

// this function shold be called to free object created by makePdfString to free the underlaying array of characters also.
void freePdfString(PdfString pdfString);

// this function shold be called to free object created by makePdfArray to free the underlaying elements composing aswell.
void freePdfArray(PdfArray* pdfArray);

/** this function shold be called to free object created by makePdfDictionary to free the underalying keys and values and types array of 
  * elements using the correct functions.
  */
void freePdfDictionary(PdfDictionary* pdfDictionary);

// this function shold be called to free object created by makePdfStream to free the underlaying uint8_t array.
void freePdfStream(PdfStream* pdfStream);
