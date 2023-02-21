#ifndef LOWLEVEL_DEFS_H
#define LOWLEVEL_DEFS_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Domain Concepts:
// 1) pdf values: 
//                these are the values that an indirect object can take on it's life time.
//                i.e. numbers, real, boolean, null, string, name, dictionary, array, stream,
//                and indirect reference.
// 2) pdf value types:
//                    these are the categories for the values that can be present in the pdf indirect object.
//
enum PDF_VALUE_TYPE {
    PDF_NULL = 0,
    PDF_STRING,
    PDF_NAME,
    PDF_ARRAY,
    PDF_STREAM,
    PDF_DICTIONARY,
    PDF_REAL,
    PDF_NUMBER,
    PDF_BOOLEAN,
    PDF_REF
};

/**
 * This structure is used to represent a pdf value and it's type in  a single structure.
 * @ptr             is a pointer to one of the possible pdfvalues [PdfString, PdfName, PdfNumber, ...,etc]
 * @type            is the type of pdf value pointed by ele.
 */
typedef struct {
    const void * const ptr;         //const pointer to const void.
    enum PDF_VALUE_TYPE type;
} PdfValue;

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
    enum PDF_VALUE_TYPE* values_types;
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
    enum PDF_VALUE_TYPE* values_types;
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
    enum PDF_VALUE_TYPE  pdfValueType;              // the object type pointed by the pointer pdfValue.
} PdfIndirectObject;

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
enum NumberPoolWithDrawalMode { FREEDFIRST, UNUSEDFIRST};
#endif
