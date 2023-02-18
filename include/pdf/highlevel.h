#ifndef HIGHLEVEL_H
#define HIGHLEVEL_H
#include "pdf/lowlevel_defs.h"
#include <stdint.h>

typedef struct
{} PdfNameTreeRoot;
typedef struct
{} PdfNameTreeInter;

/*
 * This Structure represent the leaf node of a name tree, it can also represent single level name tree
 * instead of PdfNameTreeRoot.
 *
 * currentLen represent the current length of the implicit array of key value pairs.
 * cap        represent the current capacity of the PdfNameTree.
 * first      represent the lowest pdfString name in the list
 * last       represent the last pdfString name in the list.
 * keys       array of references/pointers to the PdfString that represent the keys of the key value pair.
 * values     array of references/pointer to the PdfValues that represent the values of the key value pairs.
 * */
typedef struct 
{
  int32_t currentLen;
  int32_t currentCap;
  PdfString* first, *last;
  PdfString** keys;
  void** values;
} PdfNameTreeLeaf;

/**
 * 
 */
typedef struct
{} PdfNumberTreeInter;

typedef struct
{} PdfNumberTreeRoot;

typedef struct 
{} PdfNumberTreeLeaf;

typedef struct
{} PdfPageLeaf;
typedef struct
{} PdfPageRoot;
typedef struct 
{} PdfPageInter;

typedef struct
{} PdfCatalog;
typedef struct
{} PdfContentStream;
typedef struct
{} PdfResource;

typedef struct
{} PdfGraphicStateDict;

#endif
