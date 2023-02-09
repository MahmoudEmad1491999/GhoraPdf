#ifndef LOWLEVEL_H
#define LOWLEVEL_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

enum PDFVALUETYPE {
    PDFSTRING,
    PDFNAME,
    PDFARRAY,
    PDFSTREAM,
    PDFDICTIONARY,
    PDFREAL,
    PDFNUMBER,
    PDFBOOLEAN,
    PDFREF,
    PDFNULL,

};

/**
 * This structure is used to represent a pdf value and it's type in  a single structure.
 * @ptr             is a pointer to one of the possible pdfvalues [PdfString, PdfName, PdfNumber, ...,etc]
 * @type            is the type of pdf value pointed by ele.
 */
typedef struct {
    const void * const ptr;         //const pointer to const void.
    enum PDFVALUETYPE type;
} PdfValObj;

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
 * this structure store the string characters as null termainted string and represent pdf Name object/value.
 * @NT_str          the null termainated array of characters.
 */
typedef struct {
    char* NT_str;
} PdfName;

/**
 * represent dynamically allocated array structure the holds dynamically allocated pdf object types.
 * @arr             represent array of pointer to dynamically allocated pdfvalues.
 * @size            represent the maximum number of element this array can hold.
 * @values_types    due to the nature or pdf array construct, it holds elements of differnet type, so this list
 *                  actually record the types of each individual element 
 */
 
typedef struct {
    void** arr;
    int32_t size;
    int32_t* values_types;
} PdfArray;

typedef struct {
/**
 * this structure represent the dictionary construct in the pdf standard. because the pdf states that the keys has 
 * the pdf value type Name we only need to store the types of the value structure.
 * @keys            pointer to the array of pointers to pdf names.
 * @values          pointer to an array of pointers to the values.
 * @size            represent the maximum number of key/value pair this dictionary can hold.
 * @valuetypes      due to the nature of the pdf dictionary object the keys is always of type pdf Names, but the values
 *                  can take all possible types.
 */
    PdfName** keys;
    void** values;
    int32_t size;
    int* values_types;
} PdfDictionary;

/**
 * A stream is a pdf object/value type that represent sequence of bytes usually representing either text or graphics command.
 * @stream          is an array to uint8_t bytes representing the actuall data in the stream
 * @len             the actual length of the array.
 */
typedef struct 
{
    uint8_t* stream;
    int32_t len;
} PdfStream;

/**
 * this structure represent a reference to an indirect object, e.g. /Length 6 0 R
 * referneced here mean read.
 * @generationNumber        Generation Number of the referneced indirect object.
 * @id                      identifier Number of the referenced indirect object.
 */
typedef struct
{
    uint64_t generationNumber;
    uint64_t objectNumber;
} PdfRef;

typedef struct
{
    uint64_t generationNumber;          // generation number of the indirect number.
    uint64_t objectNumber;              // the object number of the indirect number.
    void*    pdfValuePtr;              // pointer to the pdf object.
    int32_t  pdfValueType;              // the object type pointed by the pointer pdfValue.
} PdfIndirectObject;

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
PdfValObj pdfArrayAtGet(PdfArray* pdfArray, int32_t index);

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
void pdfArrayAtSet(PdfArray* pdfArray, int32_t index, PdfValObj pdfObjVal);

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
PdfValObj pdfDictionaryGet(PdfDictionary* pdfDictionary, const char* NT_str);

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
void pdfDictionarySet(PdfDictionary* pdfDictionary, const char* NT_str, PdfValObj pdfObjVal);

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
 * Purpose:             free the pdfValue object pointed by the @ptr
 * @ptr                 pointer to the pdf object to be freed.
 * @pdfValueType        this is the type of the element pointed by ptr. 
 *
 * NOTE:
 *                      1) If ele is null noting happen.
 */
void freePdfValue(void* ptr, enum PDFVALUETYPE pdfValueType);

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
PdfIndirectObject* makePdfIndirectObject(uint64_t generationNumber, uint64_t objectNumber, void* pdfValue_ptr, int32_t pdfValueType);
#endif
