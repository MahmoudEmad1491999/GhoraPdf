#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel.h"


/*
 * this structure represent pdf indirect object.
 * @objectNumber             is the object number.
 * @generationNumber         is the genreation number of the object most of the time 0.
 * @ptr                      is a pointer to the actual value of the indirect object.
 * @type                     is the type of the actual value pointed by the indirect object.
 * */
typedef struct {
    uint32_t objectNumber;
    uint32_t generationNumber;
    void* value;
    enum PDFVALUETYPE type;
} PdfInObj;

/**
 * each pdf file content can be interpreted as a list of indirect objects.
 * @indirect_objects_list           array of pointers to indirect pdf objects.
 * @currentCapacity                current Capacity of the PdfFileContent, or how many indirect object it can have.
*/
typedef struct {
   PdfInObj** inObjarr;
   uint32_t currentCapacity;
} PdfFileContent;
/**
 * Purpose: allocate space for the PdfFileContent on the heap and allocate space for the underlying
 *          array of pointers with the initialCapacity as currnet capacity.
 * @initialCapacity         the current maximum number of indirect object this file can hold without expanding.
 * @return                  pointer to the newly created logical file.
 *
 * Failure Senarios:
 *                  1) this function will exit the application if the given capacity is negative or zero.
 *                  2) this funciton will exit if the application failed to allocate memory for the object or
 *                      the underlying arrya of pointers.
 */
PdfFileContent* makePdfFileContent(int32_t initialCapacity);

/**
 * Purpose: get the Pdf indirect object pointer at the specified index from the list or return Null if index is upbove 
 *          current capacity.
 * @pdfFileContent:         the logical file in question.          
 * @index:                  the index in question. 
 * @return:                 indirect object pointer.
 *
 * Failure Senarios:        
 *                      1) if the index is negative it will fail and close the application.
 *                      2) if the given logical file is null, it will fail and close the application.
 * Notes:
 *                      1) if the index is out of the range it will return Null without extending the pdffilecontent.
 */
PdfInObj* PdfFileContentAtGet(PdfFileContent* pdfFileContent, int32_t index);

/**
 * Purpose: set the Pdf indirect object pointer at the specified index by the given value 
 *
 *
 * @pdfFileContent          pointer to the logical file in question.
 * @index                   the index we want to set.
 * @pdf_Indirect_Obj        the new object that should be in place.
 *
 * Failure Senarios:    
 *                      1) if the index is negative it will fail and close the application.
 *                      2) if the given logical file is null, it will fail and close the application.
 * Notes:               
 *                      1) if the index is out of range it will extend the file till the index is less than the current cap.
 *                      2) the old indirect object pointed by the pointer at the index is freed.
 */
void PdfFileContentAtSet(PdfFileContent* pdfFileContent, int32_t index, PdfInObj* pdf_Indirect_Obj);


/**
 * free the given object and the underlying pdf value"@ptr.value" in question.
 * @ptr         pointer to the element to free.
 * Notes:       
 *              1) if ptr is null nothing happens.
 */
void freePdfInObj(PdfInObj* ptr);

#endif
