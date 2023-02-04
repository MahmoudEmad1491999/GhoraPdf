#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

enum {PDF_TRUE, PDF_FALSE};

enum PDFVALUETYPE {
    PDFSTRING,
    PDFNAME,
    PDFARRAY,
    PDFSTREAM,
    PDFDICTIONARY,
    PDFREAL,
    PDFNUMBER,
    PDFBOOLEAN,
    PDFNULL
};

/**
 * This structure is used to represent a pdf value and it's type in  a single structure.
 * @ele             is a pointer to the element itself.
 * @type            is the type of the element/value.
 */
typedef struct {
    void* ele;
    enum PDFVALUETYPE type;
} PdfObjVal;

/**
 * this structure is used to represent integer values in pdf file. 
 * @value           as is says.
 */
typedef struct {
    int64_t value;
} PdfNumber;

/**
 * this structure is used to represent real numbers values in pdf file. 
 * @value           as is says.
 */
typedef struct {
    double value;
} PdfReal;

/**
 * this structure is used to represent boolean vaules in pdf file. 
 * @value           as is says "true not 0, false only zero".
 */
typedef struct {
    uint8_t value;
} PdfBoolean;


/**
 * this structure store the string characters as null termainted string.
 * @NT_str          the null termainated array of characters.
 */
typedef struct {
    char* NT_str;
} PdfString;

/**
 * this structure store the string characters as null termainted string.
 * @NT_str          the null termainated array of characters.
 */
typedef struct {
    char* NT_str;
} PdfName;

/**
 * represent dynamically allocated array structure the holds dynamically allocated pdf object types.
 * @arr             represent array of pointer to dynamically allocated pdfvalues.
 * @cap             the capacity of this array.
 * @size            the actual occupied size by the currently present elements in the array.
 * @values_types    due to the nature or pdf array construct it holds elements of differnet type, so this list
 *                  actually record the types of each individual element 
 */
 
typedef struct {
    void** arr;
    int32_t cap, size;
    int32_t* values_types;
} PdfArray;

typedef struct {
/**
 * this structure represent the dictionary construct in the pdf standard. because the pdf states that the keys has 
 * the pdf value type Name we only need to store the types of the value structure.
 * @keys            pointer to the array of pointers to pdf names.
 * @values          pointer to an array of pointers to the values.
 * @valuetypes      array of types representing the types of each individual value stored.
 */
    PdfName** keys;
    void** values;
    int32_t cap,size;
    int* values_types;
} PdfDictionary;

/**
 * stream is a sequence of bytes usually representing either text or graphics command.
 * @stream          array of unsigned bytes.
 * @len             the actual length of the array.
 */
typedef struct 
{
    uint8_t* stream;
    int32_t size;
} PdfStream;

/**
 * this function will return the address of a newly created pdfName on the heap that is 
 * independent even from the null terminated string of characters,
 * which means it's the responsiblity of the client to free the allocated copied characters
 * after being done with the whole object
 * NOTE:
 *      freePdfName() must be used to free the object or the underlaying character array
 *      will causei memory leak
 */
PdfName* makePdfName(const char* str);

/**
 * this function shold be called to free object created by makePdfName
 * to free the underlaying array of characters also.
 * @pdfName         address of the object to be freed.
*/
void freePdfName(PdfName* pdfName);

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
 * this function shold be called to free object created by makePdfString 
 * to free the underlaying array of characters also.
 * @pdfName         address of the object to be freed.
 */
void freePdfString(PdfString* pdfString);

/**
 * this function is the sole point you should used to create pdf array values.
 * @initialCap          is the intialCapacity of the Array.
 * NOTE:
 *      freePdfArray() must be used to free the Array or the underlaying elements will cause 
 *      a memory leak.
 */
PdfArray* makePdfArray(int32_t initialCap);


/* this function shold be called to free object created by makePdfArray 
 * to free the underlaying elements aswell.
 * @pdfArray        address of the PdfArray to be removed
*/
void freePdfArray(PdfArray* pdfArray);

/**
 * this function is the sole point you should used to create pdf dictionary values.
 * @initialCap          is the intialCapacity of the Array.
 * NOTE:
 *      freePdfDictionary() must be used to free the Dictionary or the underlaying keys/values will cause 
 *      a memory leak.
 */
PdfDictionary* makePdfDictionary(int32_t initialCap);


/** this function shold be called to free object created by makePdfDictionary to free
 * the underalying keys and values and types array of 
 * elements using the correct functions.
 * @pdfDictionary       address of the dictinoary to be freed with the underlaying composing elements.
 */
void freePdfDictionary(PdfDictionary* pdfDictionary);

/**
 * this function is the sole point you should used to create pdf stream values.
 * @initialCap          is the intialCapacity of the Array.
 * NOTE:
 *      freePdfStream() must be used to free the stream or the underlaying character array will cause 
 *      a memory leak.
 */
PdfStream* makePdfStream(int32_t initialCap);

// this function shold be called to free object created by makePdfStream 
// to free the underlaying uint8_t array aswell.
// @pdfStream address ot the PdfStream object to be freed with underlaying objects aswell.
void freePdfStream(PdfStream* pdfStream);

/**
 * to make it more like a normal array the this function is used as the subscription operator 
 * for the PdfArray type 
 * @pdfArray        the Array in question.
 * @index           the index of the element requested.
 * NOTE:
 *                  this function does not actually return an element but another structure so that
 *                  it can also return the type of the element but it return the address of the element 
 *                  and the type in a form of structure so don't funking free the element it's not your job
 *                  it's the job of the array freeing function.
 */
PdfObjVal pdfArrayAtGet(PdfArray* pdfArray, int32_t index);

/**
 * to make it more like a normal array the this function is used as the subscription operator 
 * for the PdfArray type 
 * @pdfArray        the Array in question.
 * @index           the index of the element requested.
 * @pdfObjVal       the element and the type to store as the specified index as requested.
 * NOTE:
 *                  this function does not actually set the element to a value it free the existing value if present 
 *                  and set the address of the specified index to the address given in the form of a structure which also hold the type.
 */
void pdfArrayAtSet(PdfArray* pdfArray, int32_t index, PdfObjVal pdfObjVal);



/**
 * this function is used to get the value of the element with the given string as a key if not presetn it fails.
 * @pdfDictionary       this is the dictionary to be considered when searching.
 * @NT_str              the null terminated string to be used as the key.
 * @return              return a PdfObjVal strucutre object holding the address and the type of the found element as members.
*/

PdfObjVal pdfDictionaryGet(PdfDictionary* pdfDictionary, const char* NT_str);

/**
 * this function is used to set the value of the element with the given string as a key if not presetn it fails.
 * @pdfDictionary       this is the dictionary to be considered when searching and setting.
 * @NT_str              the null terminated string to be used as the key.
 * @pdfObjVal           this object holds the address and type of the object pointed by the address to be set in place of the old one.
 * NOTE:
 *                      the old element is freed and the new one set to be freed when the whole strucutre is freed.
*/
void pdfDictionarySet(PdfDictionary* pdfDictionary, const char* NT_str, PdfObjVal pdfObjVal);
/*
 * thie funcion is used to get the value of the stream at the specified index.
 * @pdfStream           is the stream to use as the inquestion search place.
 * @index               is the index of the byte of interest.
 * @return              the value at the specified index.
 */
uint8_t  pdfStreamGet(PdfStream* pdfStream, int index);
/*
 * thie funcion is used to set the value of the stream at the specified index.
 * @pdfStream           is the stream to use as the inquestion search place.
 * @index               is the index of the byte of interest.
 * @value             the value at the to be inserted specified index.
 */
void pdfStreamSet(PdfStream* pdfStream, int index, uint8_t value);

/**
 * this function is a wrapper that calles the correct freeing function given the type of the element pointed by ele.
 * @ele                 ele to be freed from memory.
 * @pdfValueType        this is the type of the element pointed by ele. 
 * NOTE:
 *                      if ele is null noting happen.
 */
void freePdfValue(void* ele, enum PDFVALUETYPE pdfValueType);
