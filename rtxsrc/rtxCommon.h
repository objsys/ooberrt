/**
 * Copyright (c) 1997-2016 by Objective Systems, Inc.
 * http://www.obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
/**
 * @file rtxCommon.h
 * Common runtime constants, data structure definitions, and run-time functions
 * to support various data encoding standards.
 */
#ifndef _RTXCOMMON_H_
#define _RTXCOMMON_H_

#include <string.h>
#include "rtxsrc/systypes.h"

/* Error Code Constants */

#define RTERR_BUFOVFLW   -1      /* encode buffer overflow               */
#define RTERR_ENDOFBUF   -2      /* unexpected end of buffer on decode   */
#define RTERR_IDNOTFOU   -3      /* identifer not found                  */
#define RTERR_INVOBJID   -4      /* invalid object identifier            */
#define RTERR_INVLEN     -5      /* invalid field length                 */
#define RTERR_INVENUM    -6      /* enumerated value not in defined set  */
#define RTERR_SETDUPL    -7      /* duplicate element in set             */
#define RTERR_SETMISRQ   -8      /* missing required element in set      */
#define RTERR_NOTINSET   -9      /* element not part of set              */
#define RTERR_SEQOVFLW   -10     /* sequence of field overflow           */
#define RTERR_INVOPT     -11     /* invalid option encountered in choice */
#define RTERR_NOMEM      -12     /* no dynamic memory available          */
#define RTERR_INVHEXS    -14     /* invalid hex string                   */
#define RTERR_INVBINS    -15     /* invalid binary string                */
#define RTERR_INVREAL    -16     /* invalid real value                   */
#define RTERR_STROVFLW   -17     /* octet or bit string field overflow   */
#define RTERR_BADVALUE   -18     /* invalid value specification          */
#define RTERR_UNDEFVAL   -19     /* no def found for ref'd defined value */
#define RTERR_UNDEFTYP   -20     /* no def found for ref'd defined type  */
#define RTERR_BADTAG     -21     /* invalid tag value                    */
#define RTERR_TOODEEP    -22     /* nesting level is too deep            */
#define RTERR_CONSVIO    -23     /* value constraint violation           */
#define RTERR_RANGERR    -24     /* invalid range (lower > upper)        */
#define RTERR_ENDOFFILE  -25     /* end of file on file decode           */
#define RTERR_INVUTF8    -26     /* invalid UTF-8 encoding               */
#define RTERR_CONCMODF   -27     /* Concurrent list modification         */
#define RTERR_ILLSTATE   -28     /* Illegal state error                  */
#define RTERR_OUTOFBND   -29     /* out of bounds (of array, etc)        */
#define RTERR_INVPARAM   -30     /* invalid parameter                    */
#define RTERR_INVFORMAT  -31     /* invalid time string format           */
#define RTERR_NOTINIT    -32     /* not initialized                      */
#define RTERR_TOOBIG     -33     /* value is too big for given data type */
#define RTERR_INVCHAR    -34     /* invalid character (not in char set)  */
#define RTERR_XMLSTATE   -35     /* XML state error                      */
#define RTERR_XMLPARSE   -36     /* XML parse error                      */
#define RTERR_SEQORDER   -37     /* SEQUENCE elements not in order       */
#define RTERR_INVINDEX   -38     /* invalid index for TC id              */
#define RTERR_INVTCVAL   -39     /* invalid value for TC field           */
#define RTERR_FILNOTFOU  -40     /* file not found                       */
#define RTERR_READERR    -41     /* error occurred reading source        */
#define RTERR_WRITEERR   -42     /* error occurred writing target        */
#define RTERR_INVBASE64  -43     /* invalid base64 encoding              */
#define RTERR_INVSOCKET  -44     /* invalid socket operation             */
#define RTERR_HSTUNAVAIL -45     /* Local host info is unavailable       */
#define RTERR_CANTOPEN   -46     /* can't open file                      */
#define RTERR_ARRAYSIZE  -47     /* message violates the config option   */
#define RTERR_EXPIRED    -48     /* eval license is expired              */
#define RTERR_NULLPTR    -49     /* null pointer                         */
#define RTERR_TOOMANY    -50     /* too many sub elements                */
#define RTERR_LOOPDETECTED -51   /* loop detected, test code generation  */
#define RTERR_NOTPDU     -52     /* calling EncodeTo/From with non-PDU   */
#define RTERR_NOTSUPP    -99     /* non-supported ASN construct          */

#define ASN_E_INVLEN    -101
#define ASN_E_INVINDEX  -104
#define ASN_E_NOTINSEQ  -111

#define LOG_RTERR(pctxt,stat) \
rtxErrSetData(pctxt,stat,__FILE__,__LINE__)

#define LOG_RTERRNEW(pctxt,stat) \
rtxErrSetNewData(pctxt,stat,__FILE__,__LINE__)

#ifndef EXTERNRT
#ifdef ASN1DLL
#define EXTERNRT __declspec(dllexport)
#elif defined (USEASN1DLL)
#define EXTERNRT __declspec(dllimport)
#else
#define EXTERNRT
#endif /* ASN1DLL */
#endif /* EXTERNRT */

#ifndef EXTERN
#define EXTERN EXTERNRT
#endif

#define OSCRTLMEMSET    memset
#define OSCRTLMEMCMP    memcmp
#define OSCRTLMEMCPY    memcpy
#define OSCRTLMEMMOVE   memmove
#define OSCRTLSTRCHR    strchr
#define OSCRTLSTRCPY    strcpy
#define OSCRTLSTRNCPY   strncpy
#define OSCRTLSTRCAT    strcat
#define OSCRTLSTRNCAT   strncat
#define OSCRTLSTRCMP    strcmp
#define OSCRTLSTRNCMP   strncmp
#define OSCRTLSTRLEN    strlen
#define OSCRTLMALLOC    malloc
#define OSCRTLREALLOC   realloc
#define OSCRTLFREE      free

#define OSRTINDENTSPACES        3       /* number of spaces for indent  */

#define RTXCTXTPUSHARRAYELEMNAME(pctxt,name,idx)
#define RTXCTXTPOPARRAYELEMNAME(pctxt)
#define RTXCTXTPUSHELEMNAME(pctxt,name)
#define RTXCTXTPOPELEMNAME(pctxt)
#define RTXCTXTPUSHTYPENAME(pctxt,name)
#define RTXCTXTPOPTYPENAME(pctxt)

/* Ctype substitution macros */

#define OS_ISASCII(c) ((unsigned)(c) < 0x80)

#define OS_ISUPPER(c) (c >= 'A' && c <= 'Z')
#define OS_ISLOWER(c) (c >= 'a' && c <= 'z')
#define OS_ISDIGIT(c) (c >= '0' && c <= '9')
#define OS_ISALPHA(c) (OS_ISUPPER(c) || OS_ISLOWER(c))
#define OS_ISSPACE(c) ((c >= 0x09 && c <= 0x0d) || (c == ' '))
#define OS_ISPUNCT(c) (c >= 0 && c <= 0x20)
#define OS_ISALNUM(c) (OS_ISALPHA(c) || OS_ISDIGIT(c))
#define OS_ISPRINT(c) (c >= ' ' && c <= '~')
#define OS_ISGRAPH(c) (c >= '!' && c <= '~')
#define OS_ISCNTRL(c) ((c >= 0 && c <= 0x1F) || c == 0x7F)
#define OS_ISXDIGIT(c) \
(OS_ISDIGIT(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define OS_ISBASE64(c) (OS_ISALNUM(c) || c == '+' || c == '/' || c == '=')

#define OS_TOLOWER(c) (OS_ISUPPER(c) ? (c) - 'A' + 'a' : (c))
#define OS_TOUPPER(c) (OS_ISLOWER(c) ? (c) - 'a' + 'A' : (c))

#ifdef __cplusplus
extern "C" {
#endif

/* ASN.1 encode/decode buffer info structure */

typedef struct {
   OSOCTET*     data;           /* pointer to start of data buffer      */
   size_t       byteIndex;      /* byte index                           */
   size_t       size;           /* current buffer size                  */
   OSINT16      bitOffset;      /* current bit offset (8 - 1)           */
   OSBOOL       dynamic;        /* is buffer dynamic?                   */
} ASN1BUFFER;

/* This structure is used to save the current state of the buffer */

typedef struct {
   size_t       byteIndex;      /* byte index                           */
   OSINT16      bitOffset;      /* current bit offset (8 - 1)           */
   OSUINT16     flags;          /* flag bits                            */
} ASN1BUFSAVE;

/* ASN.1 run-time error info structures */

#define ASN_K_MAXERRP   5       /* maximum error parameters             */
#define ASN_K_MAXERRSTK 8       /* maximum levels on error ctxt stack   */

typedef struct {
   const char*  module;
   int          lineno;
} ASN1ErrLocn;

typedef struct {
   ASN1ErrLocn  stack[ASN_K_MAXERRSTK];
   int          stkx;
   int          status;
   int          parmcnt;
   const char*  parms[ASN_K_MAXERRP];
} ASN1ErrInfo;

/* Flag mask constant values */

#define ASN1DYNCTXT     0x8000
#define ASN1INDEFLEN    0x4000
#define ASN1TRACE       0x2000
#define ASN1LASTEOC     0x1000
#define ASN1FASTCOPY    0x0800  /* turns on the "fast copy" mode        */
#define ASN1CONSTAG     0x0400  /* form of last parsed tag              */
#define ASN1CANXER      0x0200  /* canonical XER                        */
#define ASN1SAVEBUF     0x0100  /* do not free dynamic encode buffer    */
#define ASN1OPENTYPE    0x0080  /* item is an open type field           */

/* ASN.1 encode/decode context block structure */

#ifndef ASN_K_ENCBUFSIZ
#define ASN_K_ENCBUFSIZ 16*1024 /* dynamic encode buffer extent size    */
#endif

typedef struct OSCTXT {         /* ASN.1 context block                  */
   void*        pMemHeap;       /* internal message memory heap         */
   ASN1BUFFER   buffer;         /* data buffer                          */
   ASN1BUFSAVE  savedInfo;      /* saved buffer info                    */
   ASN1ErrInfo  errInfo;        /* run-time error info                  */
   OSUINT16     flags;          /* flag bits                            */
} OSCTXT;

#define ASN1BUFCUR(cp) (cp)->buffer.data[(cp)->buffer.byteIndex]
#define OSRTBUFPTR(cp) &(cp)->buffer.data[(cp)->buffer.byteIndex]
#define OSRTMAX(a,b)   (((a)>(b))?(a):(b))
#define OSRTMIN(a,b)   (((a)<(b))?(a):(b))
#define OS_ISEMPTY(s)  (s == 0 || *s == '\0')

#define HEXCHARTONIBBLE(ch,b) \
if (ch >= '0' && ch <= '9') b = (unsigned char)(ch - '0'); \
else if (ch >= 'a' && ch <= 'f') b = (unsigned char)((ch - 'a') + 10); \
else if (ch >= 'A' && ch <= 'F') b = (unsigned char)((ch - 'A') + 10); \
else b = 0xFF;

#define NIBBLETOHEXCHAR(b,ch) \
if (b >= 0 && b <= 9) ch = (char)(b + '0'); \
else if (b >= 0x0a && b <= 0x0f) ch = (char)((b - 10)+ 'a'); \
else ch = '?';

#define OSRTBUFRESTORE(pctxt) { \
(pctxt)->buffer.byteIndex = (pctxt)->savedInfo.byteIndex; \
(pctxt)->flags = (pctxt)->savedInfo.flags; }

/* Doubly-linked list */

typedef struct _OSRTDListNode {
   void* data;
   struct _OSRTDListNode* next;
   struct _OSRTDListNode* prev;
} OSRTDListNode;

typedef struct _OSRTDList {
   OSUINT32 count;
   OSRTDListNode* head;
   OSRTDListNode* tail;
} OSRTDList;

#define OSRTDLISTNODESIZE ((sizeof(OSRTDListNode)+7)&(~7))

#define rtxDListAllocNodeAndData(pctxt,type,ppnode,ppdata) do { \
*ppnode = (OSRTDListNode*) \
rtxMemAlloc (pctxt, sizeof(type)+OSRTDLISTNODESIZE); \
if (0 != *ppnode) { \
(*ppnode)->data = (void*)((char*)(*ppnode)+OSRTDLISTNODESIZE); \
*ppdata = (type*)((*ppnode)->data); \
} else { *ppdata = 0; } \
} while (0)

/* Singly-linked list */

typedef struct _OSRTSListNode {
   void* data;                   /**< Pointer to list data item. */
   struct _OSRTSListNode* next;  /**< Pointer to next node in list. */
} OSRTSListNode;

typedef struct _OSRTSList {
   OSUINT32 count;              /**< Count of items in the list. */
   OSRTSListNode* head;         /**< Pointer to first entry in list. */
   OSRTSListNode* tail;         /**< Pointer to last entry in list. */
   struct OSCTXT* pctxt;
} OSRTSList;

/**
 * @defgroup llfuns Linked List Utility Functions
 *
 * Linked list and stack utility functions are used to maintain linked lists
 * and stacks used within the ASN.1 run-time library functions. @{
 */
/**
 * This function appends an item to the linked list structure. The data item is
 * passed into the function as a void pointer that can point to any object of
 * any type. The rtxMemAlloc function is used to allocated the memory for the
 * list node structure; therefore, all internal list memory will be released
 * whenever memFree is called. The pointer to the data item itself is stored
 * in the node structure - a copy is not made.
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pList        A pointer to a linked list structure onto which the data
 *                       item is to be appended. A pointer to an updated linked
 *                       list structure.
 * @param pData        A pointer to a data item to be appended to the list.
 * @return             A pointer to an allocated node structure used to link
 *                       the given data value into the list.
 */
EXTERNRT OSRTDListNode* rtxDListAppend
(OSCTXT* pctxt, OSRTDList* pList, const void* pData);

EXTERNRT OSRTDListNode* rtxDListAppendNode
(OSRTDList* pList, OSRTDListNode* pListNode);

/**
 * This function initializes a doubly linked list structure. It sets the number
 * of elements to zero and sets al internal pointer values to NULL. A doubly
 * linked-list structure is described by the OSRTDList type defined in
 * ooasn1.h. Nodes of the list are of type OSRTDListNode.
 *
 * Memory for the structures is allocated using the rtxMemAlloc run-time
 * function and is maintained within the context structure that is a required
 * parameter to all rtxDList functions. This memory is released when memFree
 * is called or the Context is released. Unless otherwise noted, all data
 * passed into the list functions is simply stored on the list by value (i.e. a
 * deep-copy of the data is not done).
 *
 * @param pList        A pointer to a linked list structure to be initialized.
 */
EXTERNRT void  rtxDListInit (OSRTDList* pList);

#define rtxDListFastInit rtxDListInit

/**
 * This function removes a node from the linked list structure. The rtxMemAlloc
 * function was used to allocate the memory for the list node structure,
 * therefore, all internal list memory will be released whenever memFree or
 * memFreePtr is called.
 *
 * @param pList        A pointer to a linked list structure onto which the data
 *                       item is to be appended. A pointer to an updated linked
 *                       list structure.
 * @param node         A pointer to the node that is to be removed. It should
 *                       already be in the linked list structure.
 */
EXTERNRT void rtxDListRemove (OSRTDList* pList, OSRTDListNode* node);

/**
 * This function removes all nodes from the linked list structure and releases
 * the memory that was allocated for storing nodes' structures
 * (OSRTDListNode) and for data. The memory for data in each node must have
 * been previously allocated with calls to rtxMemAlloc, rtxMemAllocZ, or
 * rtxMemRealloc functions.
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pList        A pointer to a linked list structure onto which the data
 *                       item is to be appended. A pointer to an updated linked
 *                       list structure. A pointer to an empty linked list
 *                       structure.
 */
EXTERNRT void rtxDListFreeAll (OSCTXT* pctxt, OSRTDList* pList);

/**
 * This function initializes a singly linked list structure.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pList        A pointer to a linked list structure to be initialized.
 */
EXTERNRT void  rtxSListInitEx (OSCTXT* pctxt, OSRTSList* pList);

/**
 * This function appends an item to a linked list structure. The data item is
 * passed into the function as a void parameter that can point to an object of
 * any type.
 *
 * @param pList        A pointer to a linked list onto which the data item is
 *                       to be appended.
 * @param pData        A pointer to a data item to be appended to the list.
 * @return             A pointer to the allocated linked list structure.
 */
EXTERNRT OSRTSListNode* rtxSListAppend (OSRTSList* pList, void* pData);

/**
 * @defgroup errfp Error Formatting and Print Functions
 * @{
 *
 * Error formatting and print functions allow information about the
 * encode/decode errors to be added to a context block structure and then
 * printed out when the error is propagated to the top level.
 */

/**
 * This function adds an integer parameter to an error information structure.
 * Parameter substitution is done in much the same way as it is done in C
 * printf statments. The base error message specification that goes along with
 * a particular status code may have variable fields built in using '%'
 * modifiers. These would be replaced with actual parameter data.
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param errParm      The typed error parameter.
 * @return             The status of the operation.
 */
EXTERNRT int rtxErrAddIntParm (OSCTXT* pctxt, int errParm);

/**
 * This function adds a 64-bit integer parameter to an error information
 * structure.
 * Parameter substitution is done in much the same way as it is done in C
 * printf statments. The base error message specification that goes along with
 * a particular status code may have variable fields built in using '%'
 * modifiers. These would be replaced with actual parameter data.
 *
 * @param pctxt        A pointer to a context structure.
 * @param errParm      The 64-bit integer error parameter.
 * @return             The status of the operation (TRUE if the parameter was
 *                       sucessfully added).
 */
EXTERNRT OSBOOL rtxErrAddInt64Parm (OSCTXT* pctxt, OSINT64 errParm);

/**
 * This function adds an unsigned integer parameter to an error information
 * structure. Parameter substitution is done in much the same way as it is done
 * in C printf statments. The base error message specification that goes along
 * with a particular status code may have variable fields built in using '%'
 * modifiers. These would be replaced with actual parameter data.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param errParm      The typed error parameter.
 * @return             The status of the operation.
 */
EXTERNRT int rtxErrAddUIntParm (OSCTXT* pctxt, unsigned int errParm);

/**
 * This function adds an unsigned 64-bit integer parameter to an error
 * information structure.
 *
 * @param pctxt        A pointer to a context structure.
 * @param errParm      The unsigned 64-bit integer error parameter.
 * @return             The status of the operation (TRUE if the parameter was
 *                       sucessfully added).
 */
EXTERNRT OSBOOL rtxErrAddUInt64Parm (OSCTXT* pctxt, OSUINT64 errParm);

/**
 * Add an element name parameter to the context error information structure
 * (not supported in OO version since element stack not maintained).
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @return            True if element name was added.
 */
EXTERNRT OSBOOL rtxErrAddElemNameParm (OSCTXT* pctxt);

/**
 * This function adds an string parameter to an error information structure.
 * Parameter substitution is done in much the same way as it is done in C
 * printf statments. The base error message specification that goes along with
 * a particular status code may have variable fields built in using '%'
 * modifiers. These would be replaced with actual parameter data.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param errprm_p     The typed error parameter.
 * @return             The status of the operation.
 */
EXTERNRT int rtxErrAddStrParm (OSCTXT* pctxt, const char* errprm_p);

/**
 * This function frees memory associated with the storage of parameters
 * associated with an error message. These parameters are maintained on an
 * internal linked list maintained within the error information structure. The
 * list memory must be freed when error processing is complete. This function
 * is called from within errPrint after teh error has been printed out. It is
 * also called in teh rtFreeContext function.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 */
EXTERNRT void rtxErrFreeParms (OSCTXT* pctxt);

/**
 * This function gets the text of the error
 *
 * @param pctxt       A pointer to a context structure.
 */
EXTERNRT char* rtxErrGetText (OSCTXT* pctxt);

/**
 * This function prints error information to the standard output device. The
 * error information is stored in a structure of type ASN1ErrInfo. A structure
 * of the this type is part f the OSCTXT structure. This is where error
 * information is stored within the ASN1C generated and low-level encode/decode
 * functions.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 */
EXTERNRT void rtxErrPrint (OSCTXT* pctxt);

/**
 * This function resets the error information in the error information
 * sturcture.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 */
EXTERNRT int rtxErrReset (OSCTXT* pctxt);

/**
 * This function sets error information in an error information structure. The
 * information set includes status code, module name, and line number. Location
 * information (i.e. module name and line number) is pushed onto a stack within
 * the error information structure to provide a complete stack trace when the
 * information is printed out.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param status       The error status code. This is one of the negative error
 *                       status codes.
 * @param module       The name of the module (C or C++ source file) in which
 *                       the module occurred. This is typically obtained by
 *                       using the _FILE_ macro.
 * @param lno          The line number at which the error occurred. This is
 *                       typically obtained by using the _LINE_ macro.
 * @return             The status value passed to the operation in the third
 *                       argument. This makes it possible to set the error
 *                       information and return the status value in one line of
 *                       code.
 */
EXTERNRT int rtxErrSetData (OSCTXT* pctxt, int status,
                            const char* module, int lno);

/**
 * @}
 */
/**
 * @defgroup rtmem Memory Allocation Macros and Functions
 * @ingroup cruntime
 *
 * Memory allocation functions and macros handle memory management for the
 * ASN1C run-time. Special algorithms are used for allocation and deallocation
 * of memory to improve the run-time performance. @{
 */
/**
 * Allocate a dynamic array. This macro allocates a dynamic array of records of
 * the given type. This version of the macro will return the RTERR_NOMEM error
 * status if the memory request cannot be fulfilled.
 *
 * @param pctxt        - Pointer to a context block
 * @param pseqof       - Pointer to a generated SEQUENCE OF array structure.
 *                       The <i>n</i> member variable must be set to the number
 *                       of records to allocate.
 * @param type         - Data type of an array record
 */
#define ALLOC_ASN1ARRAY(pctxt,pseqof,type) do {\
if (sizeof(type)*(pseqof)->n < (pseqof)->n) return RTERR_NOMEM; \
if (((pseqof)->elem = (type*) rtxMemAlloc \
(pctxt, sizeof(type)*(pseqof)->n)) == 0) return RTERR_NOMEM; \
} while (0)

/**
 * Get array size. This macro returns the number of elements in an array.
 *
 * @param x            - Array varaible
 */
#define ASN1ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

/**
 * Reallocate an array. This macro reallocates an array (either expands or
 * contracts) to hold the given number of elements. The number of elements is
 * specified in the <i>n</i> member variable of the pseqof argument.
 *
 * @param pctxt        - Pointer to a context block
 * @param pseqof       - Pointer to a generated SEQUENCE OF array structure.
 *                       The <i>n</i> member variable must be set to the number
 *                       of records to allocate.
 * @param type         - Data type of an array record
 */
#define REALLOC_ASN1ARRAY(pctxt,pseqof,type) do {\
if (sizeof(type)*(pseqof)->n < (pseqof)->n) return RTERR_NOMEM; \
if (((pseqof)->elem = (type*) rtxMemRealloc \
(pctxt, (void*)(pseqof)->elem, sizeof(type)*(pseqof)->n)) == 0) \
return RTERR_NOMEM; \
} while (0)

/**
 * Free memory associated with a context. This macro frees all memory held
 * within a context. This is all memory allocated using the rtxMemAlloc (and
 * similar macros) and the mem memory allocation functions using the given
 * context variable.
 *
 * @param pctxt        - Pointer to a context block
 */
#define MEMFREE(pctxt) do {\
memFree(pctxt); \
if ((pctxt)->buffer.dynamic && (pctxt)->buffer.data) { \
(pctxt)->buffer.data = 0; \
} \
} while (0)

/**
 * Allocate memory.  This function allocates the given number of bytes.  It is
 * similar to the C \c malloc run-time function.
 *
 * @param pctxt - Pointer to a context block
 * @param nbytes - Number of bytes of memory to allocate
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.
 */
EXTERNRT void* rtxMemAlloc (OSCTXT* pctxt, size_t nbytes);

/**
 * Allocate and zero memory.  This function allocates the given number of bytes
 * and then initializes the memory block to zero.
 *
 * @param pctxt - Pointer to a context block
 * @param nbytes - Number of bytes of memory to allocate
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.
 */
EXTERNRT void* rtxMemAllocZ (OSCTXT* pctxt, size_t nbytes);

/**
 * Free memory pointer. This macro frees memory at the given pointer. The
 * memory must have been allocated using the rtxMemAlloc (or similar) macros or
 * the mem memory allocation functions. This macro is similar to the C \c
 * free function.
 *
 * @param pctxt        - Pointer to a context block
 * @param pmem         - Pointer to memory block to free. This must have been
 *                       allocated using the rtxMemAlloc macro or the
 *                       memHeapAlloc function.
 */
EXTERNRT void rtxMemFreePtr2 (OSCTXT* pctxt, void* pmem);

#define rtxMemFreePtr(pctxt,mem_p) rtxMemFreePtr2(pctxt,(void*)mem_p)

/**
 * Free memory associated with a context.  This macro frees all memory
 * held within a context.  This is all memory allocated using the
 * rtxMemAlloc (and similar macros) and the rtxMem memory allocation
 * functions using the given context variable.
 *
 * @param pctxt - Pointer to a context block
 */
EXTERNRT void rtxMemFree (OSCTXT* pctxt);

/**
 * Reallocate memory.  This macro reallocates a memory block (either
 * expands or contracts) to the given number of bytes.  It is
 * similar to the C \c realloc run-time function.
 *
 * @param pctxt - Pointer to a context block
 * @param mem_p - Pointer to memory block to reallocate.  This must have been
 *   allocated using the rtxMemAlloc macro or the rtxMemHeapAlloc function.
 * @param nbytes - Number of bytes of memory to which the block is to be
 *   resized.
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.  This may be the same as the mem_p
 *   pointer that was passed in if the block did not need to be relocated.
 */
EXTERNRT void* rtxMemRealloc (OSCTXT* pctxt, void* pmem, size_t nbytes);

/**
 * Reset memory associated with a context. This macro resets all memory held
 * within a context. This is all memory allocated using the rtxMemAlloc (and
 * similar macros) and the mem memory allocation functions using the given
 * context variable.
 *
 * <p>The difference between this and the MEMFREE macro is that the memory
 * blocks held within the context are not actually freed. Internal pointers are
 * reset so the existing blocks can be reused. This can provide a performace
 * improvement for repetitive tasks such as decoding messages in a loop.
 *
 * @param pctxt        - Pointer to a context block
 */
EXTERNRT void rtxMemReset (OSCTXT* pctxt);

#define rtxMemAllocType(pctxt,ctype) \
(ctype*)rtxMemAlloc(pctxt,sizeof(ctype))

#define rtxMemAllocTypeZ(pctxt,ctype) \
(ctype*)rtxMemAllocZ(pctxt,sizeof(ctype))

/**
 * Determine if any memory records exist in the memory heap.
 *
 * @param pctxt        - Pointer to a context block
 * @return             - True if records exist
 */
EXTERNRT OSBOOL rtxMemHeapIsEmpty (OSCTXT* pctxt);

EXTERNRT void rtxMemFreeOpenSeqExt (OSCTXT* pctxt, OSRTDList* pElemList);

/**
 * @}
 */
/* Context management functions */

/**
 * @defgroup cmfun Context Management Functions
 * @{
 *
 * Context initialization functions handle the allocation, initialization, and
 * destruction of ASN.1 context variables (variables of type OSCTXT). These
 * variables hold all of the working data used during the process of encoding
 * or decoding a message. The context provides thread safe operation by
 * isolating what would otherwise be global variables within this structure.
 * The context variable is passed from function to function as a message is
 * encoded or decoded and maintains state information on the encoding or
 * decoding process.
 */

/**
 * This function assigns a buffer to an OSCTXT block. The block should have
 * been previously initialized by rtInitContext.
 *
 * @param pctxt       The pointer to the context structure variable to be
 *                       initialized.
 * @param bufaddr      For encoding, the address of a memory buffer to receive
 *                       and encode a message. For decoding the address of a
 *                       buffer that contains the message data to be decoded.
 *                       This address will be stored within the context
 *                       structure. For encoding it might be zero, the dynamic
 *                       buffer will be used in this case.
 * @param bufsiz       The size of the memory buffer. For encoding, it might be
 *                       zero; the dynamic buffer will be used in this case.
 *                       If bufaddr is NULL, this parameter specifies the
 *                       initial size of the dynamic buffer; if 0 - the
 *                       default size will be used.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxInitContextBuffer
(OSCTXT* pctxt, const OSOCTET* bufaddr, size_t bufsiz);

/**
 * This function initializes an OSCTXT block. It makes sure that if the block
 * was not previously initialized, that all key working parameters are set to
 * their correct initial state values (i.e. declared within a function as a
 * normal working variable).  A user must call this function or
 * newContext before making any other run-time library calls.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 */
#ifndef RTKEY_H
EXTERNRT int rtInitContext (OSCTXT* pctxt);
#endif

/**
 * This function frees all dynamic memory associated with a context. This
 * includes all memory inside the block (in particular, the list of memory
 * blocks used by the mem functions) as well as the block itself if allocated
 * with the newContext function
 *
 * @param pctxt       A pointer to a context structure.
 */
EXTERNRT void  rtFreeContext (OSCTXT* pctxt);

EXTERNRT int rtInitContextUsingKey
(OSCTXT* pctxt, const OSOCTET* key, size_t keylen);

/**
 * This function creates a copy of a context structure. The copy is a "shallow
 * copy" (i.e. new copies of dynamic memory blocks held within the context are
 * not made, only the pointers are transferred to the new context structure).
 * This function is mainly for use from within compiler-generated code.
 *
 * @param pdest        - Context structure to which data is to be copied.
 * @param psrc         - Context structure from which data is to be copied.
 */
EXTERNRT void rtxCopyContext (OSCTXT* pdest, OSCTXT* psrc);

/**
 * @} cmfun
 */
/* UTF-8 string functions */

EXTERNRT int rtxUTF8Len (const OSUTF8CHAR* inbuf);
EXTERNRT size_t rtxUTF8LenBytes (const OSUTF8CHAR* inbuf);

EXTERNRT const char* rtBMPToCString
(ASN1BMPString* pBMPString, char* cstring, OSSIZE cstrsize);

EXTERNRT const char* rtUCSToCString
(ASN1UniversalString* pUCSString, char* cstring, OSSIZE cstrsize);

EXTERNRT int rtValidateStr (OSUINT32 tag, const char *pdata);

/**
 * @defgroup rtxEnum Enumeration utility functions
 * @{
 * Enumeration utility function provide run-time functions for handling
 * enumerations defined within a schema.
 */
typedef struct OSEnumItem {
   const OSUTF8CHAR* name;
   OSINT32           value;
   OSUINT16          namelen;
   OSUINT16          transidx;
} OSEnumItem;

/**
 * This function will return the numeric value for the given enumerated
 * identifier string.
 *
 * @param strValue Enumerated identifier value
 * @param strValueSize Length of enumerated identifier
 * @param enumTable Table containing the defined enumeration
 * @param enumTableSize Number of rows in the table
 * @return Index to enumerated item if found; otherwise, negative
 *   status code (RTERR_INVENUM).
 */
EXTERNRT OSINT32 rtxLookupEnum
(const OSUTF8CHAR* strValue, size_t strValueSize,
 const OSEnumItem enumTable[], OSUINT16 enumTableSize);

/**
 * Lookup enum by integer value.  Required for ASN.1 because enumerated
 * values do not need to be sequential.
 *
 * @param value  Integer value of the enumerated item.
 * @param enumTable Table containing the defined enumeration
 * @param enumTableSize Number of rows in the table
 * @return Index to enumerated item if found; otherwiae, negative
 *   status code (RTERR_INVENUM).
 */
EXTERNRT OSINT32 rtxLookupEnumByValue
(OSINT32 value, const OSEnumItem enumTable[], size_t enumTableSize);

/**
 * @} rtxEnum
 */
/**
 * This function compares two OID values for equality.
 *
 * @param pOID1         Pointer to first OID value to compare.
 * @param pOID2         Pointer to second OID value to compare.
 * @return              True if OID's are equal.
 */
EXTERNRT OSBOOL rtOIDsEqual (const ASN1OBJID* pOID1, const ASN1OBJID* pOID2);

/**
 * This function copies one OID value to another.
 *
 * @param srcOID        Pointer to OID to be copied.
 * @param dstOID        Pointer to OID to which data is to be copied.
 * @return              True if copy operation was successful.
 */
EXTERNRT OSBOOL rtCopyOID (const ASN1OBJID* srcOID, ASN1OBJID* dstOID);

/**
 * Encode binary data into base64 string form to a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to binary data to encode.
 * @param srcDataSize  Length of the binary data in octets.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer for the encoded
 *                       base64 string.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64EncodeData (OSCTXT* pctxt, const char* pSrcData,
                                    size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Decode base64 string to binary form into a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to base64 string to decode.
 * @param srcDataSize  Length of the base64 string.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer to hold data.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64DecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * @defgroup ccfDateTime Date/time conversion functions
 * @{
 *
 * These functions handle the conversion of date/time to and from various
 * internal formats to XML schema standard string forms.
 */
/**
 * This function compares the date part of two OSNumDateTime structures and
 * returns the result of the comparison.
 *
 * @param pvalue1     Pointer to OSNumDateTime structure.
 * @param pvalue2     Pointer to OSNumDateTime structure.
 * @return            Completion status of operation:
 *                      - 0 Dates are same
 *                      - +1 First Date/Time is greater than second.
 *                      - -1 First Date/Time is less than second.
 */
EXTERNRT int rtxCmpDate
   (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2);

/**
 * This function compares the time part of two OSNumDateTime structures and
 * returns the result of the comparison.
 *
 * @param pvalue1     Pointer to OSNumDateTime structure.
 * @param pvalue2     Pointer to OSNumDateTime structure.
 * @return            Completion status of operation:
 *                      - 0 Times are same
 *                      - +1 First Date/Time is greater than second.
 *                      - -1 First Date/Time is less than second.
 */
EXTERNRT int rtxCmpTime
   (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2);

/**
 * This function compares two OSNumDateTime structures and
 * returns the result of the comparison.
 *
 * @param pvalue1     Pointer to OSNumDateTime structure.
 * @param pvalue2     Pointer to OSNumDateTime structure.
 * @return            Completion status of operation:
 *                      - 0 Dates are same
 *                      - +1 First Date/Time is greater than second.
 *                      - -1 First Date/Time is less than second.
 */
EXTERNRT int rtxCmpDateTime
   (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2);

/**
 * This function verifies that date members (year, month, day, timezone)
 * of the OSNumDateTime structure contains valid values.
 *
 * @param pvalue      Pointer to OSNumDateTime structure to be checked.
 * @return            Boolean result: true means data is valid.
 */
EXTERNRT OSBOOL rtxDateIsValid (const OSNumDateTime* pvalue);

/**
 * This function verifies that time members (hour, minute, second, timezone)
 * of the OSNumDateTime structure contains valid values.
 *
 * @param pvalue      Pointer to OSNumDateTime structure to be checked.
 * @return            Boolean result: true means data is valid.
 */
EXTERNRT OSBOOL rtxTimeIsValid (const OSNumDateTime* pvalue);

/**
 * This function verifies that all members of the OSNumDateTime structure
 * contains valid values.
 *
 * @param pvalue      Pointer to OSNumDateTime structure to be checked.
 * @return            Boolean result: true means data is valid.
 */
EXTERNRT OSBOOL rtxDateTimeIsValid (const OSNumDateTime* pvalue);

EXTERNRT int rtParseGeneralizedTime
(OSCTXT *pctxt, const char* value, OSNumDateTime* dateTime);

EXTERNRT int rtParseUTCTime
(OSCTXT *pctxt, const char* value, OSNumDateTime* dateTime);

EXTERNRT int rtxUIntToCharStr
(OSUINT32 value, char* dest, size_t bufsiz, char padchar);

EXTERNRT const char* rtxStrJoin
(char* dest, size_t bufsiz, const char* str1, const char* str2,
 const char* str3, const char* str4, const char* str5);

/**
 * @} ccfDateTime
 */

#ifdef __cplusplus
}
#endif

#endif
