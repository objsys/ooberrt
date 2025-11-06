/**
 * Copyright (c) 1997-2025 by Objective Systems, Inc.
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
 * @file asn1ber.h
 * ASN.1 runtime constants, data structure definitions, and functions to
 * support the Basic Encoding Rules (BER) and Distinguished Encoding Rules
 * (DER) as defined in the ITU-T X.690 standard.
 */
#ifndef _ASN1BER_H_
#define _ASN1BER_H_

#include "rtxsrc/rtxCommon.h"

#define ASN1TAG_LSHIFT  24

/* tag mask values - these can be logically or'd together to    */
/* form tags compatible with the ASNTAG structure..             */

#define TM_UNIV         0x00000000  /* universal class          */
#define TM_APPL         0x40000000  /* application-wide class   */
#define TM_CTXT         0x80000000  /* context-specific class   */
#define TM_PRIV         0xC0000000  /* private-use class        */

#define TM_PRIM         0x00000000  /* primitive form           */
#define TM_CONS         0x20000000  /* constructor form         */
#define TM_IDCODE       0x1FFFFFFF  /* ID code mask             */

#define ASN_K_BADTAG    0xFFFFFFFF  /* invalid tag code         */
#define ASN_K_NOTAG     0xFFFFFFFF  /* no tag input parameter   */

#define TM_CLASS        0xC0    /* class mask                   */
#define TM_FORM         0x20    /* form mask                    */
#define TM_CLASS_FORM   0xE0    /* class/form mask              */
#define TM_B_IDCODE     0x1F    /* id code mask (byte)          */

#define ASN1BUF_SAVE(cp) { \
(cp)->savedInfo.byteIndex = (cp)->buffer.byteIndex; \
(cp)->savedInfo.flags = (cp)->flags; }

#define ASN1BUF_RESTORE(cp) { \
(cp)->buffer.byteIndex = (cp)->savedInfo.byteIndex; \
(cp)->flags = (cp)->savedInfo.flags; }

#define ASN1TAG2BYTE(tag) \
((OSOCTET)(((tag)&TM_B_IDCODE)|((tag)>>ASN1TAG_LSHIFT)))

typedef OSUINT32 ASN1TAG;

typedef enum { ASN1IMPL, ASN1EXPL } ASN1TagType;

/* flag mask values */

#define XM_SEEK         0x01    /* seek match until found or end-of-buf */
#define XM_ADVANCE      0x02    /* advance pointer to contents on match */
#define XM_DYNAMIC      0x04    /* alloc dyn mem for decoded variable   */
#define XM_SKIP         0x08    /* skip to next field after parsing tag */
#define XM_OPTIONAL     0x10    /* tag test is for optional element     */

/* universal built-in type ID code value constants */

#define ASN_ID_EOC      0       /* end of contents              */
#define ASN_ID_BOOL     1       /* boolean                      */
#define ASN_ID_INT      2       /* integer                      */
#define ASN_ID_BITSTR   3       /* bit string                   */
#define ASN_ID_OCTSTR   4       /* byte (octet) string          */
#define ASN_ID_NULL     5       /* null                         */
#define ASN_ID_OBJID    6       /* object ID                    */
#define ASN_ID_OBJDSC   7       /* object descriptor            */
#define ASN_ID_EXTERN   8       /* external type                */
#define ASN_ID_REAL     9       /* real                         */
#define ASN_ID_ENUM     10      /* enumerated value             */
#define ASN_ID_EPDV     11      /* EmbeddedPDV type             */
#define ASN_ID_RELOID   13      /* relative object ID           */
#define ASN_ID_SEQ      16      /* sequence, sequence of        */
#define ASN_ID_SET      17      /* set, set of                  */

#define ASN_SEQ_TAG     0x30    /* SEQUENCE universal tag byte  */
#define ASN_SET_TAG     0x31    /* SET universal tag byte       */

/* Restricted character string type ID's */

#define ASN_ID_UTF8String       12
#define ASN_ID_NumericString    18
#define ASN_ID_PrintableString  19
#define ASN_ID_TeletexString    20
#define ASN_ID_T61String        ASN_ID_TeletexString
#define ASN_ID_VideotexString   21
#define ASN_ID_IA5String        22
#define ASN_ID_UTCTime          23
#define ASN_ID_GeneralTime      24
#define ASN_ID_GraphicString    25
#define ASN_ID_VisibleString    26
#define ASN_ID_GeneralString    27
#define ASN_ID_UniversalString  28
#define ASN_ID_BMPString        30

#define ASN_K_INDEFLEN   -9999   /* indefinite length message indicator  */

/**
 * Buffer location descriptor
 */
typedef struct _Asn1BufLocDescr {
   OSUINT32 numocts;
   OSINT32  offset;
} Asn1BufLocDescr;

/* ASN.1 context control block - this structure is used to keep track   */
/* of pointer and length context values when decoding sets or sequence  */
/* of constructs.                                                       */

#define ASN1_K_CCBMaskSize      32
#define ASN1_K_NumBitsPerMask   16
#define ASN1_K_MaxSetElements   (ASN1_K_CCBMaskSize*ASN1_K_NumBitsPerMask)

typedef struct {                /* context control block        */
   OSOCTET*     ptr;            /* constructor pointer          */
   long         len;            /* constructor length           */
   int          seqx;           /* sequence element index       */
   OSUINT16     mask[ASN1_K_CCBMaskSize];  /* set mask value  */
   size_t       bytes;          /* bytes processed before block (for stream) */
   int          stat;           /* status, returned by BS_CHKEND */
} ASN1CCB;

#ifdef __cplusplus
extern "C" {

#endif

/**
 * This function adds a tag parameter to an error information structure.
 * Parameter substitution is done in much the same way as it is done in C
 * printf statments. The base error message specification that goes along with
 * a particular status code may have variable fields built in using '%'
 * modifiers. These would be replaced with actual parameter data.
 *
 * @param pctxt        A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param errParm      The typed error parameter.
 * @return             The status of the operation.
 */
EXTERNRT int berErrAddTagParm (OSCTXT* pctxt, ASN1TAG errParm);

/** @defgroup berdecruntime BER/DER C Decode Functions.
 * @{
 *
 * BER/DER C decode functions handle the decoding of the primitive ASN.1 data
 * types and ASN.1 length and tag fields within a message. Calls to these
 * functions are assembled in the C source code generated by the ASN1C compiler
 * to decode complex ASN.1 structures. These functions are also directly
 * callable from within a user's application program if the need to decode a
 * primitive data item exists.
 *
 * The procedure to decode a primitive data item is as follows:
 *   -# Call the xd_setp low-level decode function to specify the address of
 * the buffer containing the encoded ASN.1 data to be decoded.
 *   -# Call the specific decode function to decode the value. The tag value
 * obtained in the first step can be used to determine which decode function to
 * call for decoding the variable.
 */
/**
 * This function decodes an ASN.1 tag into a standard 32-bit unsigned integer
 * type. The bits used to represent the components of a tag are as follows:
 *
 * Bit Fields:
 *   - 31-30 Class (00 = UNIV, 01 = APPL, 10 = CTXT, 11 = PRIV)
 *   - 29 Form (0 = primitive, 1 = constructed)
 *   - 28-0 ID code value
 *
 * @param pctxt       Pointer to context block structure.
 * @param tag_p        Pointer to variable to receive decoded tag info.
 * @return             Completion status of operation: 0 (ASN_OK) = success,
 *                       negative return value is error.
 */
EXTERNRT int xd_tag (OSCTXT* pctxt, ASN1TAG *tag_p);

/**
 * This function parses the ASN.1 tag and length fields located at the current
 * message pointer position.
 *
 * xd_tag_len monitors indefinite length messages as follows: Each time a
 * length is parsed, it is checked to see if it is an indefinite length value.
 * If it is, an indefinite length section counter is incremented. Each time an
 * end-of-contents (EOC) identifier is parsed, this counter is decremented.
 * When the counter goes to zero, end of message is signaled.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tag_p        Pointer to variable to receive decoded tag info.
 * @param len_p        Length of message component. Returned as follows: >= 0
 *                       component is fixed length ASN_K_INDEFLEN component is
 *                       indefinite length
 * @param flags        Bit flags used to set the following options: XM_ADVANCE:
 *                       Advance decode pointer on match.
 * @return             Completion status of operation: 0 (ASN_OK) = success,
 *                       negative return value is error.
 */
EXTERNRT int xd_tag_len
(OSCTXT *pctxt, ASN1TAG *tag_p, int *len_p, OSOCTET flags);

/**
 * This function compares the tag at the current message pointer position with
 * the given tag for a match. If a match occurs, the length field is decoded
 * and the length is returned to the caller. If the input parameter 'advance'
 * is set to TRUE, the message pointer is advanced to the beginning of the
 * contents field.
 *
 * If a match does not occur, the routine will skip to subsequent fields in
 * search of a match. If a match is eventually found, the processing described
 * above is done; otherwise, a not found status is returned to the caller.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tag          Tag variable to match.
 * @param len_p        Length of message component. Returned as follows: >= 0
 *                       component is fixed length ASN_K_INDEFLEN component is
 *                       indefinite length
 * @param flags        Bit flags used to set the following options:
 *                       - XM_ADVANCE: Advance decode pointer on match.
 *                       - XM_SEEK : Seek until match found or EOM.
 *                       - XM_SKIP : Skip to next tag before search
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_match
(OSCTXT *pctxt, ASN1TAG tag, int *len_p, OSOCTET flags);

/**
 * This function parses an ASN.1 BOOLEAN tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * The function first checks to see if explicit tagging is specified. If yes,
 * the universal tag for this message type is checked to make sure it is of the
 * expected value. If the match is not successful, a negative value is returned
 * to indicate the parse was not successful. Otherwise, the pointer is advanced
 * to the length field and the length parsed.
 *
 * The length value is then check to see if it is equal to one which is the
 * only valid length for boolean. If it is equal, the boolean data value is
 * parsed; otherwise, and error is returned.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Decoded boolean data value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_INVLEN invalid length (!= 1)
 *                       - RTERR_IDNOTFOU unexpected tag value (not UNIV 1)
 */
EXTERNRT int xd_boolean
(OSCTXT *pctxt, OSBOOL *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an ASN.1 INTEGER tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * The function first checks to see if explicit tagging is specified. If yes,
 * the universal tag value is parsed and checked to make sure it matches the
 * expected tag for this message type. If not, a negative value is returned to
 * indicate the parse was not successful. Otherwise, the length value is
 * parsed.
 *
 * If the match is successful or implicit tagging is specified, the integer
 * data value is parsed.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_integer
(OSCTXT *pctxt, OSINT32 *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an ASN.1 INTEGER tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * This function is similar to ::xd_integer but it is used to parse 8-bit
 * integer values.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded 8-bit integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_int8
(OSCTXT *pctxt, OSINT8 *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an ASN.1 INTEGER tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * This function is similar to ::xd_integer but it is used to parse 16-bit
 * integer values.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded 16-bit integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_int16
(OSCTXT *pctxt, OSINT16 *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an unsigned variant of ASN.1 INTEGER tag/length/value
 * at the current message pointer location and advances the pointer to the next
 * field.
 *
 * The function first checks to see if explicit tagging is specified. If yes,
 * the universal tag value is parsed and checked to make sure it matches the
 * expected tag for this message type. If not, a negative value is returned to
 * indicate the parse was not successful. Otherwise, the length value is
 * parsed.
 *
 * If the match is successful or implicit tagging is specified, the integer
 * data value is parsed.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded unsigned integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_uint
(OSCTXT *pctxt, OSUINT32 *pvalue, ASN1TagType tagging, int length);

#define xd_unsigned xd_uint

/**
 * This function parses an unsigned variant of ASN.1 INTEGER tag/length/value
 * at the current message pointer location and advances the pointer to the next
 * field.
 *
 * This function is similar to ::xd_uint but it is used to parse 8-bit
 * unsigned integer values.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded 8-bit unsigned integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_uint8
(OSCTXT *pctxt, OSUINT8 *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an unsigned variant of ASN.1 INTEGER tag/length/value
 * at the current message pointer location and advances the pointer to the next
 * field.
 *
 * This function is similar to ::xd_uint but it is used to parse 16-bit
 * unsigned integer values.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to decoded 16-bit unsigned integer value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_uint16
(OSCTXT *pctxt, OSUINT16 *pvalue, ASN1TagType tagging, int length);

/**
 * This function parses an ASN.1 INTEGER tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * The function is similar to ::xd_integer but it can be used to parse integer
 * values with sizes up to 64 bits (if platform supports such integer's size).
 *
 * If the match is successful or implicit tagging is specified, the integer
 * data value is parsed.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param object_p     Pointer to decoded 64-bit integer value.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_int64 (OSCTXT *pctxt, OSINT64 *object_p,
                       ASN1TagType tagging, int length);

/**
 * This function parses an unsigned variant of ASN.1 INTEGER tag/length/value
 * at the current message pointer location and advances the pointer to the next
 * field.
 *
 * The function is similar to ::xd_unsigned but it can be used to parse integer
 * values with sizes up to 64 bits (if platform supports such integer's size).
 *
 * If the match is successful or implicit tagging is specified, the integer
 * data value is parsed.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param object_p     Pointer to decoded 64-bit unsigned integer value.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_uint64 (OSCTXT *pctxt, OSUINT64 *object_p,
                        ASN1TagType tagging, int length);

/**
 * This function parses an ASN.1 INTEGER tag/length/value at the current
 * message pointer location and advances the pointer to the next field.
 *
 * This function will decode a variable of the ASN.1 INTEGER type. In this
 * case, the integer is assumed to be of a larger size than can fit in a C or
 * C++ long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes. These
 * variables are stored in character string constant variables. They are
 * represented as decimal strings starting with no prefix. If it is necessary
 * to convert a decimal string to another radix, then use ::rtxBigIntSetStr /
 * ::rtxBigIntToString functions.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @param pvalue     Pointer to a character pointer variable to receive the
 *                       decoded unsigned value. Dynamic memory is allocated
 *                       for the variable using the rtxMemAlloc function. The
 *                       decoded variable is represented as a decimal string
 *                       starting with no prefix.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_bigint
(OSCTXT *pctxt, const char** pvalue, ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 BIT STRING type into a static
 * memory structure. This function call is generated by ASN1C to decode a sized
 * bit string production.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       The length, in OCTETs, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @param numbits_p    Pointer to an integer variable containing the size (in
 *                       bits) of the sized ASN.1 bit string. An error will
 *                       occur if the number of bits in the decoded string is
 *                       larger than this value. Note that this is also used as
 *                       an output variable - the actual number of decoded bits
 *                       will be returned in this variable.
 * @param pvalue     Pointer to a variable to receive the decoded bit string.
 *                       This is assumed to be a static array large enough to
 *                       hold the number of bits specified in the *numbits_p
 *                       input parameter.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_bitstr_s
(OSCTXT *pctxt, OSOCTET* pvalue, OSUINT32* numbits_p,
 ASN1TagType tagging, int length);

/**
 * This function decodes a variable of the ASN.1 BIT STRING. This function will
 * allocate dynamic memory to store the decoded result.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       The length, in OCTETs, of the contents field to be
 *                       decoded. This parameter only has meaning if the
 *                       tagging parameter specifies implicit decoding. If
 *                       explicit, the length is obtained from the decoded
 *                       length field.
 * @param numbits_p    Pointer to an integer value to receive the decoded
 *                       number of bits.
 * @param pvalue2    Pointer to a pointer variable to receive the decoded bit
 *                       string. Dynamic memory is allocated to hold the
 *                       string.Pointer to a variable to receive the decoded
 *                       bit string.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_bitstr
(OSCTXT *pctxt, const OSOCTET** pvalue2,
 OSUINT32* numbits_p, ASN1TagType tagging, int length);

/**
 * This function decodes the octet string at the current message pointer
 * location and returns its value. The value is returned in the buffer pointed
 * to by the given character buffer pointer. This is a static buffer that must
 * be large enough to hold the decoded data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param pvalue     Pointer to static octet array to receive decoded data
 * @param numocts_p    Pointer to an integer variable to receive the length of
 *                       the decoded octet string. On input, this parameter is
 *                       used to specify the size of the octet array.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_octstr_s
(OSCTXT *pctxt, OSOCTET* pvalue, OSUINT32* numocts_p,
 ASN1TagType tagging, int length);

/**
 * This function decodes the octet string at the current message pointer
 * location and returns its value. This version of the function allocates
 * memory for the decoded string and returns a pointer to the data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param pvalue2    Pointer to a pointer to receive the address of the
 *                       allocated memory into which the decoded data will be
 *                       stored.
 * @param numocts_p    Pointer to an integer variable to receive length of the
 *                       decoded octet string.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_octstr
(OSCTXT *pctxt, const OSOCTET** pvalue2, OSUINT32* numocts_p,
 ASN1TagType tagging, int length);

/**
 * This function is the base function for decoding any of the 8-bit character
 * string useful types such as IA5String, VisibleString, etc. This function
 * allocates memory for the decoded string and returns a pointer to the data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param pvalue     Pointer to a pointer to receive the address of the
 *                       allocated memory into which the decoded character
 *                       string data will be stored. The string is assumed to
 *                       be a normal C string containing non-null characters. A
 *                       null terminator is automatically added to the end of
 *                       the string by this function.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param tag          Tag variable to match
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_charstr
(OSCTXT* pctxt, const char** pvalue, ASN1TagType tagging,
 ASN1TAG tag, int length);

/**
 * This function is used to decode a variable of the ASN.1 UTF-8 string
 * type.  This function allocates memory for the decoded string and returns
 * a pointer to the data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param object_p     Pointer to a pointer to receive the address of the
 *                       allocated memory into which the decoded character
 *                       string data will be stored. The string is assumed to
 *                       be a normal C string containing non-null characters. A
 *                       null terminator is automatically added to the end of
 *                       the string by this function.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
#define xd_utf8str(pctxt,object_p,tagging,length) \
xd_charstr (pctxt, (const char**)object_p, tagging, ASN_ID_UTF8String, length)

/**
 * This function is the base function for decoding a 16-bit character string
 * useful type. In the current version of ASN.1, the only string type based on
 * 16-bit Unicode characters is the BMPString type. This function allocates
 * memory for the decoded string and returns a pointer to the data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param pvalue     Pointer to a pointer to receive the address of the
 *                       allocated memory into which the decoded character
 *                       string data will be stored. The string is assumed to
 *                       contain all non-null 16-bit unicode characters. A null
 *                       terminator is automatically added to the end of the
 *                       string by this function.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param tag          Tag variable to match
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_16BitCharStr
(OSCTXT* pctxt, Asn116BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag, int length);

/**
 * This function is the base function for decoding a 32-bit character string
 * useful type. In the current version of ASN.1, the only string type based on
 * 32-bit characters is the UniversalString type. This function allocates
 * memory for the decoded string and returns a pointer to the data.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param pvalue     Pointer to a pointer to receive the address of the
 *                       allocated memory into which the decoded character
 *                       string data will be stored. The string is assumed to
 *                       contain all non-null 32-bit unicode characters. A null
 *                       terminator is automatically added to the end of the
 *                       string by this function.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param tag          Tag variable to match.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_32BitCharStr
(OSCTXT* pctxt, Asn132BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag, int length);

/**
 * This function decoded the ASN.1 NULL placeholder type. The null data type
 * contains no data; however, if explicit tagging is specified, it will contain
 * a universal tag value (5) and zero length. This function will parse those
 * values.
 *
 * @param pctxt       Pointer to ASN.1 context block structure
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged. The function will do nothing if implicit
 *                       tagging is specified.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_null (OSCTXT *pctxt, ASN1TagType tagging);

/**
 * This function decodes a value of the ASN.1 object identifier type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue     Pointer to value to receive decoded result. The
 *                       ASN1OBJID structure contains an integer to hold the
 *                       number of subidentifiers and an array to hold the
 *                       subidentifier values.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_objid
(OSCTXT *pctxt, ASN1OBJID *pvalue, ASN1TagType tagging, int length);

/**
 * This function decodes a value of the ASN.1 relative object identifier type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue     Pointer to value to receive decoded result. The
 *                       ASN1OBJID structure contains an integer to hold the
 *                       number of subidentifiers and an array to hold the
 *                       subidentifier values.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
int decodeRelOID
(OSCTXT *pctxt, ASN1OBJID *pvalue, ASN1TagType tagging, int length);

/**
 * This function decodes a value of the ASN.1 ENUMERATED type. This function is
 * identical to the integer decode function (xd_integer) except that the
 * enumerated universal tag value is validated.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue     Pointer to value to receive decoded result.
 * @param tagging      Specifies whether element is implicitly or explicitly
 *                       tagged.
 * @param length       Length of data to retrieve. Valid for implicit case
 *                       only.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_enum
(OSCTXT *pctxt, OSINT32 *pvalue, ASN1TagType tagging, int length);

EXTERNRT int xd_enumInt8
(OSCTXT *pctxt, OSINT8 *object_p, ASN1TagType tagging, int length);

EXTERNRT int xd_enumInt16
(OSCTXT *pctxt, OSINT16 *object_p, ASN1TagType tagging, int length);

#define xd_enumInt xd_enum

EXTERNRT int xd_enumUnsigned
(OSCTXT *pctxt, OSUINT32 *object_p, ASN1TagType tagging, int length);

EXTERNRT int xd_enumUInt8
(OSCTXT *pctxt, OSUINT8 *object_p, ASN1TagType tagging, int length);

EXTERNRT int xd_enumUInt16
(OSCTXT *pctxt, OSUINT16 *object_p, ASN1TagType tagging, int length);

#define xd_enumUInt xd_enumUnsigned

/**
 * This function decodes a value of an ASN.1 open type. An open type is used to
 * model the old ASN.1 ANY and ANY DEFINED BY types. It is also used to model
 * variable type references within information objects (for example,
 * TYPE-IDENTIFER.&Type). Dynamic memory is allocated to hold the decoded
 * result.
 *
 * @param pctxt       Pointer to context block structure.
 * @param pvalue2    Pointer to value to receive decoded result.
 * @param numocts_p    Pointer to an integer variable to receive length of the
 *                       decoded octet string.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_OpenType
(OSCTXT *pctxt, const OSOCTET** pvalue2, OSUINT32* numocts_p);

EXTERNRT int xd_OpenTypeExt
(OSCTXT* pctxt, ASN1CCB* ccb_p, ASN1TAG* tags, int tagCount,
 OSRTDList *pElemList);

/**
 * This function sets the decode pointer (cursor) to point at the beginning of
 * the encoded ASN.1 message that is to be decoded. This function must be
 * called prior to calling any of the other decode functions.
 *
 * @param pctxt       Pointer to context block structure.
 * @param msg_p        Pointer to message buffer containing data to be decoded.
 * @param msglen       Pointer to size of the message data buffer. This is an
 *                       optional parameter. It is used to verify that the
 *                       length of the data encoded in the message is less than
 *                       or equal to the given size of the message buffer. If
 *                       the message size is unknown at the time of decoding,
 *                       this argument can be set to zero and the size check
 *                       will be bypassed.
 * @param tag_p        Pointer to an ASN.1 tag variable to recieve the value of
 *                       the initial tag parsed from a message. This is an
 *                       optional parameter. It can be set to NULL is the user
 *                       does not require the initial tag value.
 * @param len_p        Pointer to an integer variable to receive the decoded
 *                       message length. Note that this is the total length of
 *                       the message from the start of message, NOT the actual
 *                       length decoded after the initial tag. In other words
 *                       the length of the initial tag and length are added on
 *                       to the parsed length. This is an optional parameter.
 *                       It can be set to NULL if the user does not require the
 *                       message length value.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_setp
(OSCTXT *pctxt, const OSOCTET* msg_p, int msglen,
 ASN1TAG *tag_p, int *len_p);

/**
 * This function decodes an ASN.1 length value.
 *
 * @param pctxt       Pointer to context block structure.
 * @param len_p        Pointer to integer variable to receive the the decoded
 *                       length value. If the length is indefinite, the
 *                       constant ASN_K_INDEFLEN is returned.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_len (OSCTXT *pctxt, int *len_p);

/**
 * This function checks for the end of a constructed element. It is typically
 * used by the ASN1C compiler to set up a loop for decoding a constructed type
 * such as a SEQUENCE or SET.
 *
 * @param pctxt       Pointer to context block structure.
 * @param ccb_p        Pointer to a context control block (ccb). This is a
 *                       structure added to compiler generated code to keep
 *                       track of the current position within nested
 *                       structures.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_chkend (OSCTXT *pctxt, ASN1CCB* ccb_p);

/**
 * This function determines the count of elements within a constructed type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param length       Length of the constructed type.
 * @param count_p      Pointer to an integer variable to receive the element
 *                       count.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_count (OSCTXT *pctxt, int length, int *count_p);

/**
 * This function skips to the next element in the decode buffer.
 *
 * @param pctxt       Pointer to context block structure.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_NextElement (OSCTXT* pctxt);

/**
 * This function is an optimized version of the xd_tag_len function.
 * If the ASN1C compiler determines the tag at a given location to be parsed
 * is only one byte long (a typical case) then it will use this function.
 * It reads a single tag byte and then immediately parses the length field.
 *
 * @param pctxt       Pointer to context block structure.
 * @param len_p        Length of message component. Returned as follows: >= 0
 *                       component is fixed length ASN_K_INDEFLEN component is
 *                       indefinite length
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_Tag1AndLen (OSCTXT* pctxt, OSINT32* len_p);

/* internal decode function to do memory copies */
/**
 * This function copies data from the contents field of a message component
 * into the target object.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param             *pvalue A pointer to a memory structure to receive the
 *                       copied data.
 * @param length       The number of bytes to copy from the contents field.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_memcpy (OSCTXT* pctxt, OSOCTET *pvalue, int length) ;

/**
 * The xd_match1 function does a comparison between the given tag and the tag
 * at the current decode pointer position to determine if they match. It then
 * returns the result of the match operation. In contrast to ::xd_match
 * function xd_match1 is an optimized version and can be used only to compare
 * primitive tags (with number less than 31). It is always advance the decode
 * pointer to the contents field if matching is successful. Note, that the tag
 * should be specified as an octet, like it is used in BER encoding.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tag          Tag variable to match in octet format.
 * @param len_p        Length of message component. Returned as follows: >= 0
 *                       component is fixed length ASN_K_INDEFLEN component is
 *                       indefinite length
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xd_match1 (OSCTXT *pctxt, OSOCTET tag, int *len_p);


/** @} berdecruntime */

/* encode functions */
/** @defgroup berencruntime BER/DER C Encode Functions.
 * @{
 *
 * BER/DER C encode functions handle the BER encoding of the primitive ASN.1
 * data types and ASN.1 length and tag fields within a message. Calls to these
 * functions are assembled in the C source code generated by the ASN1C complier
 * to accomplish the encoding of complex ASN.1 structures. These functions are
 * also directly callable from within a user's application program if the need
 * to accomplish a low level encoding function exists.
 *
 * The procedure to call the encode function that encodes a primitive type is
 * the same as the procedure to call a compiler generated encode function
 * described above. The rtInitContext and xe_setp functions must first be
 * called to initialize a context variable and set a pointer to the buffer into
 * which the variable is to be encoded. A static encode buffer is specified by
 * specifying a pointer to a buffer and buffer size. Setting the buffer address
 * to NULL and buffer size to 0 specifies a dynamic buffer. The primitive
 * encode function is invoked. Finally, xe_getp is called to retrieve a
 * pointer to the encoded message component.
 */
/**
 * This function is used to encode ASN.1 tag and length fields that preface
 * each block of message data. The ANS1C complier generates calls to this
 * function to handle teh encoding of user-defined tags within an ASN.1
 * specification. The function is also called from within the runtime library
 * functions to handle the addition of the universal tags defined for each of
 * the ASN.1 primitive data types.
 *
 * @param             *pctxt A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param tag          The ASN.1 tag to be encoded in the message. This
 *                       parameter is passed using the ASN1C internal tag
 *                       representation. It is passed as an unsigned 32-bit
 *                       integer.
 * @param length       The length of the contents field previously encoded.
 *                       This parameter can be used to specify the actual
 *                       length, or the special constant ASN_K_INDEFLEN can be
 *                       used to specify that an indefinite length
 *                       specification should be encoded.
 * @return             Length of the encoded message component. This is equal
 *                       to the given length plus the additional bytes that are
 *                       added for the tag and length fields. A negative status
 *                       will be returned if the encoding is not successful.
 */
EXTERNRT int xe_tag_len (OSCTXT *pctxt, ASN1TAG tag, int length) ;

/**
 * This function will encode a variable of the ASN.1 BOOLEAN type.
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param             *pvalue A pointer to the BOOLEAN value to be encoded
 *                       (note that pointer to the BOOLEAN is passed, not the
 *                       BOOLEAN itself. This may seem awkward, but to keep the
 *                       calling sequence of all encode functions the same,
 *                       pointers were used in all cases). A BOOLEAN is defined
 *                       as a single OCTET whose value is 0 for FALSE and any
 *                       other value for TRUE.
 * @param tagging      An enumerated type whose value is set to either ASN1EXPL
 *                       (for explicit tagging) or ASN1IMPL (for implicit).
 *                       Controls whether the universal tag value for this type
 *                       is added or not. Users will generally always set this
 *                       value to ASN1EXPL.
 * @return             The length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_boolean
(OSCTXT* pctxt, OSBOOL *pvalue, ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the INTEGER value to be encoded (note that
 *                       a pointer to the INTEGER is passed, not the INTEGER
 *                       value itself. This may seem awkward, but to keep the
 *                       calling sequence of all encode functions the same,
 *                       pointers were used in all cases). The OSINT32 type is
 *                       set to the C type 'int' in the ooasn1.h file. This
 *                       is assumed to represent a 32-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_integer
(OSCTXT* pctxt, int *pvalue, ASN1TagType tagging);

/**
 * This function encodes an unsigned variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the unsigned INTEGER value to be encoded
 *                       (note that a pointer to the unsigned INTEGER is
 *                       passed, not the INTEGER value itself. This may seem
 *                       awkward, but to keep the calling sequence of all
 *                       encode functions the same, pointers were used in all
 *                       cases). The OSUINT32 type is set to the C type
 *                       'unsigned int' in the ooasn1.h file. This is assumed
 *                       to represent a 32-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_uint
(OSCTXT* pctxt, OSUINT32 *pvalue, ASN1TagType tagging);

#define xe_unsigned xe_uint

/**
 * This function encodes an 8-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the 8-bit INTEGER value to be encoded (note
 *                       that a pointer is passed, not the INTEGER value
 *                       itself. This may seem awkward, but to keep the calling
 *                       sequence of all encode functions the same, pointers
 *                       were used in all cases). The OSINT8 type is set to
 *                       the C type 'signed char' in the ooasn1.h file. This
 *                       is assumed to represent an 8-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_int8
(OSCTXT* pctxt, OSINT8 *pvalue, ASN1TagType tagging);

/**
 * This function encodes a 16-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the 16-bit INTEGER value to be encoded
 *                       (note that a pointer is passed, not the INTEGER value
 *                       itself. This may seem awkward, but to keep the calling
 *                       sequence of all encode functions the same, pointers
 *                       were used in all cases). The OSINT16 type is set to
 *                       the C type 'short' in the ooasn1.h file. This is
 *                       assumed to represent a 16-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_int16
(OSCTXT* pctxt, OSINT16 *pvalue, ASN1TagType tagging);

/**
 * This function encodes an unsigned 8-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the unsigned 8-bit INTEGER value to be
 *                       encoded (note that a pointer is passed, not the
 *                       INTEGER value itself. This may seem awkward, but to
 *                       keep the calling sequence of all encode functions the
 *                       same, pointers were used in all cases). The OSOCTET
 *                       type is set to the C type 'unsigned char' in the
 *                       ooasn1.h file. This is assumed to represent an 8-bit
 *                       integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_uint8
(OSCTXT* pctxt, OSUINT8 *pvalue, ASN1TagType tagging);

/**
 * This function encodes an unsigned 16-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to the unsigned 16-bit INTEGER value to be
 *                       encoded (note that a pointer is passed, not the
 *                       INTEGER value itself. This may seem awkward, but to
 *                       keep the calling sequence of all encode functions the
 *                       same, pointers were used in all cases). The OSUINT16
 *                       type is set to the C type 'unsigned short' in the
 *                       ooasn1.h file. This is assumed to represent a 16-bit
 *                       integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_uint16
(OSCTXT* pctxt, OSUINT16 *pvalue, ASN1TagType tagging);

/**
 * This function encodes a 64-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param object_p     A pointer to the 64-bit INTEGER value to be encoded
 *                       (note that a pointer to the INTEGER is passed, not the
 *                       INTEGER value itself. This may seem awkward, but to
 *                       keep the calling sequence of all encode functions the
 *                       same, pointers were used in all cases). The OSINT64
 *                       type is set to the C type '__int64', 'long long' or
 *                       'long' in the rtxsrc/rtxCommon.h file (depends on the
 *                       platform and the compiler). This is assumed to
 *                       represent a 64-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_int64 (OSCTXT* pctxt, OSINT64 *object_p, ASN1TagType tagging);

/**
 * This function encodes an unsigned 64-bit variable of the ASN.1 INTEGER type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param object_p     A pointer to the unsigned 64-bit INTEGER value to be
 *                       encoded (note that a pointer to the unsigned INTEGER
 *                       is passed, not the INTEGER value itself. This may seem
 *                       awkward, but to keep the calling sequence of all
 *                       encode functions the same, pointers were used in all
 *                       cases). The OSUINT64 type is set to the C type
 *                       'unsigned __int64', 'unsigned long long' or 'unsigned
 *                       long' in the rtxsrc/rtxCommon.h file (depends on the
 *                       platform and the compiler). This is assumed to
 *                       represent a 64-bit integer value.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_uint64 (OSCTXT* pctxt, OSUINT64 *object_p,
                        ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 INTEGER type. In this case,
 * the integer is assumed to be of a larger size than can fit in a C or C++
 * long type (normally 32 or 64 bits). For example, parameters used to
 * calculate security values are typically larger than these sizes.
 *
 * Items of this type are stored in character string constant variables. They
 * can be represented as decimal strings (with no prefixes), as hexadecimal
 * strings starting with a "0x" prefix, as octal strings starting with a "0o"
 * prefix or as binary strings starting with a "0b" prefix. Other radixes
 * currently are not supported. It is highly recommended to use the hexadecimal
 * or binary strings for better performance.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     A pointer to a character string containing the value to
 *                       be encoded.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_bigint
(OSCTXT* pctxt, const char* pvalue, ASN1TagType tagging);

/**
 * This function will encode a variable of the ASN.1 BIT STRING type.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue     A pointer to an OCTET string containing the bit data to
 *                       be encoded. The string contains bytes having the
 *                       actual bit settings as they are to be encoded in the
 *                       message.
 * @param numbits      The number of bits within the bit string to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_bitstr
(OSCTXT* pctxt, const OSOCTET* pvalue,
 OSUINT32 numbits, ASN1TagType tagging);

/**
 * This function will encode a variable of the ASN.1 OCTET STRING type.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue     A pointer to an OCTET string containing the bit data to
 *                       be encoded. The string contains bytes having the
 *                       actual bit settings as they are to be encoded in the
 *                       message.
 * @param numocts      The number of octets (bytes) within the OCTET STRING to
 *                       be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_octstr
(OSCTXT* pctxt, const OSOCTET* pvalue,
 OSUINT32 numocts, ASN1TagType tagging);

/**
 * This function will encode a variable one of the ASN.1 character string types
 * that are based 8-bit character sets. This includes IA5String, VisibleString,
 * PrintableString, and NumericString
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue     A pointer to a null-terminated C character string to be
 *                       encoded
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The Universal ASN.1 tag to be encoded in the message.
 *                       This parameter is passed using the internal
 *                       representation discussed in Section 4.1.2. It is
 *                       passed as an unsigned 16-bit integer. The tag value
 *                       must represent one of the 8-bit character string types
 *                       documented in the X.680 standard.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_charstr
(OSCTXT* pctxt, const char* pvalue, ASN1TagType tagging, ASN1TAG tag);

/**
 * This function will encode a variable one of ASN.1 UTF-8 character string
 * type.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param object_p     A pointer to a null-terminated UTF-8 C character
 *                       string to be encoded.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
#define xe_utf8str(pctxt,object_p,tagging) \
xe_charstr (pctxt, (const char*)object_p, tagging, ASN_ID_UTF8String)

/**
 * This function will encode a variable one of the ASN.1 character string types
 * that are based on a 16-bit character set. This includes the BMPString type.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue     A pointer t a structure representing a 16-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       16-bit character elements represented as 16-bit short
 *                       integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The Universal ASN.1 tag to be encoded in the message.
 *                       This parameter is passed using the internal
 *                       representation discussed in Section 4.1.2. It is
 *                       passed as an unsigned 16-bit integer. The tag value
 *                       must represent one of the 16-bit character string
 *                       types documented in the X.680 standard.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_16BitCharStr
(OSCTXT* pctxt, Asn116BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag);

/**
 * This function will encode a variable one of the ASN.1 character string types
 * that are based on a 32-bit character set. This includes the UniversalString
 * type.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param pvalue     A pointer t a structure representing a 32-bit character
 *                       string to be encoded. This structure contains a
 *                       character count element and a pointer to an array of
 *                       32-bit character elements represented as 16-bit short
 *                       integers.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param tag          The Universal ASN.1 tag to be encoded in the message.
 *                       This parameter is passed using the internal
 *                       representation discussed in Section 4.1.2. It is
 *                       passed as an unsigned 32-bit integer. The tag value
 *                       must represent one of the 32-bit character string
 *                       types documented in the X.680 standard.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_32BitCharStr
(OSCTXT* pctxt, Asn132BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag);

/**
 * This function will encode an ASN.1 NULL placeholder.
 *
 * @param pctxt       Pointer to a context block structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Length of the encoded message component. A negative
 *                       status value will be returned if encoding is not
 *                       successful.
 */
EXTERNRT int xe_null (OSCTXT* pctxt, ASN1TagType tagging);

/**
 * This function encodes a value of the ASN.1 object identifier type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     Pointer to value to be encoded. The ASN1OBJID structure
 *                       contains a numids fields to hold the number of
 *                       subidentifiers and an array to hold the subidentifier
 *                       values.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xe_objid
(OSCTXT* pctxt, ASN1OBJID *pvalue, ASN1TagType tagging);

/**
 * This function encodes a value of the ASN.1 relative object identifier type.
 *
 * @param pctxt       Pointer to context block structure.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @param pvalue     Pointer to value to be encoded. The ASN1OBJID structure
 *                       contains a numids fields to hold the number of
 *                       subidentifiers and an array to hold the subidentifier
 *                       values.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int encodeRelOID
(OSCTXT* pctxt, ASN1OBJID *pvalue, ASN1TagType tagging);

/**
 * This function encodes a variable of the ASN.1 ENUMERATED type.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param pvalue     A pointer to an integer containing the enumerated value
 *                       to be encoded (Note that a pointer to the value is
 *                       passed not the value itself. This may seem awkward,
 *                       but to keep the calling sequence of all the encode
 *                       function the same, pointers were used in all cases.)
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xe_enum
(OSCTXT* pctxt, OSINT32 *pvalue, ASN1TagType tagging);

EXTERNRT int xe_enumInt8
(OSCTXT* pctxt, OSINT8 *object_p, ASN1TagType tagging);

EXTERNRT int xe_enumInt16
(OSCTXT* pctxt, OSINT16 *object_p, ASN1TagType tagging);

#define xe_enumInt xe_enum

EXTERNRT int xe_enumUnsigned
(OSCTXT* pctxt, OSUINT32 *object_p, ASN1TagType tagging);

EXTERNRT int xe_enumUInt8
(OSCTXT* pctxt, OSUINT8 *object_p, ASN1TagType tagging);

EXTERNRT int xe_enumUInt16
(OSCTXT* pctxt, OSUINT16 *object_p, ASN1TagType tagging);

#define xe_enumUInt xe_enumUnsigned

/**
 * This function will encode a variable of the old (pre- 1994) ASN.1 ANY type
 * or other elements defined in the later standards to be Open Types (for
 * example, a variable type declaration in a CLASS construct as defined in
 * X.681).
 *
 * A variable of this type is considered to be previously encoded ASN.1 message
 * component.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue     A pointer to a buffer containing an encoded ASN.1
 *                       message component.
 * @param numocts      The number of octets to be encoded.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xe_OpenType (OSCTXT* pctxt, const OSOCTET* pvalue,
                           OSUINT32 numocts);

/**
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pElemList    A pointer to open type to be encoded.
 */
EXTERNRT int xe_OpenTypeExt (OSCTXT* pctxt, OSRTDList* pElemList);

/**
 * This function is used to set the internal buffer within the runtime library
 * encoding context. It must be called after the context variable is
 * initialized by the rtInitContext function and before any other compiler
 * generated or runtime library encode function.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param             *buf_p A pointer to a memory buffer to use to encode a
 *                       message. The buffer should be declared as an array of
 *                       unsigned characters (OSOCTETs). This parameter can
 *                       be set to NULL to specify dynamic encoding (i.e., the
 *                       encode functions will dynamically allocate a buffer
 *                       for the message).
 * @param bufsiz       The length of the memory buffer in bytes.
 */
EXTERNRT int xe_setp (OSCTXT* pctxt, OSOCTET *buf_p, int bufsiz);

/**
 * This function is used to obtain a pointer to the start of an encoded message
 * after calls to the encode function(s) are complete. ANS.1 messages are
 * encoded from the end of a given buffer toward the beginning. Therefore, in
 * practically all cases, the start of the message will not be at the beginning
 * of the buffer.
 *
 * @param pctxt       A pointer to a context structure. This provides a
 *                       storage area for the function to store all working
 *                       variables that must be maintained between function
 *                       calls.
 * @return             A pointer to the beginning of the encoded message.
 */
EXTERNRT OSOCTET* xe_getp (OSCTXT* pctxt);

/**
 * This function will free a dynamic encode buffer.
 *
 * The dynamic encode buffer is the buffer that is allocated if dynamic
 * encoding of a message is enabled (passing NULL as the buffer pointer
 * argument to xe_setp enables dynamic encoding.)
 *
 * Note that this is different than the xu_freeall function associated with
 * freeing decoder memory. This function only releases the memory associated
 * with a dynamic encoded buffer. The xu_freeall will not release this memory.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls. The
 *                       dynamic encode buffer pointer is contained within the
 *                       structure.
 */
EXTERNRT void xe_free (OSCTXT* pctxt);

/**
 * This function is used to copy bytes into the encode buffer. BER and DER
 * messages are encoded from back-to-front and this function will take this
 * into account when copying bytes. It will also check to ensure that enough
 * space is available in the buffer for the bytes to be copied. If the encode
 * buffer is dynamic, it will be expanded to hold the number of bytes to be
 * copied if not enough space is available. If the buffer is static and enough
 * space is not available, an error (RTERR_BUFOVFLW) will be returned.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls. The
 *                       dynamic encode buffer pointer is contained within the
 *                       structure.
 * @param pvalue     A pointer to a buffer containing the bytes to be copied.
 * @param length       The number of bytes to copy.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xe_memcpy
(OSCTXT *pctxt, const OSOCTET* pvalue, size_t length) ;

/**
 * This function is added to the generated code for SEQUENCE OF/SET OF
 * constructs to ensure the elements are in the required canonical order for
 * DER.
 *
 * If the elements are not in the right order, they are sorted to be in the
 * correct order prior to encoding.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param pList        Linked List of message components to be sorted. The
 *                       elements of this list are offsets to encoded
 *                       components within the encode buffer.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int xe_derCanonicalSort (OSCTXT* pctxt, OSRTSList* pList);

/**
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param length       The actual length of existing message components.
 * @param pDescr       A pointer to a buffer location descriptor.
 */
EXTERNRT void xe_getBufLocDescr
(OSCTXT *pctxt, OSUINT32 length, Asn1BufLocDescr* pDescr);

/**
 * This function encodes a BIT STRING value in accordance with the
 * Distinguished Encoding Rules (DER).  It adjusts the bit count to remove
 * trailing zero bits and then invokes the BER encode function.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store al working variables
 *                       that must be maintained between function calls.
 * @param pvalue       Bit string to be encoded.
 * @param numbits      Number of bits in the bit string.
 * @param tagging      An enumerated type whose value is set to either
 *                       'ASN1EXPL' (for explicit tagging) or 'ASN1IMPL' (for
 *                       implicit). Controls whether the universal tag value
 *                       for this type is added or not. Users will generally
 *                       always set this value to 'ASN1EXPL'.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int derEncBitString
(OSCTXT* pctxt, const OSOCTET* pvalue, OSUINT32 numbits, ASN1TagType tagging);

/** @} berencruntime */

/**
 * Macro definitions
 */
#define ASN1RTDLISTNODESZ ((sizeof(OSRTDListNode)+7)&(~7))

#define ALLOC_ASN1ELEMDNODE(pctxt,type) \
(type*) (((char*)rtxMemAllocZ (pctxt, sizeof(type) + \
ASN1RTDLISTNODESZ )) + ASN1RTDLISTNODESZ);

#define ASN1BUF_INDEFLEN(cp) (((cp)->flags&ASN1INDEFLEN)!=0)

#ifndef ASN1MAX
#define ASN1MAX(a,b)        (((a)>(b))?(a):(b))
#endif

#ifndef ASN1MIN
#define ASN1MIN(a,b)        (((a)<(b))?(a):(b))
#endif

#define _GENASN1TAG(t) \
(((ASN1TAG)((t) & TM_CLASS_FORM) << ASN1TAG_LSHIFT) | ((t) & TM_B_IDCODE))

/* These macros set the constructed tag bit in the context flags based on
   the parsed value */

#define SET_ASN1CONSTAG(pctxt,tag) \
((pctxt)->flags = (OSUINT16)(((pctxt)->flags & (~ASN1CONSTAG)) \
| ((!(((tag) & TM_CONS) >> (ASN1TAG_LSHIFT - 8 + 5)) - 1) & ASN1CONSTAG)))

#define SET_ASN1CONSTAG_BYTE(pctxt,tag) \
((pctxt)->flags = (OSUINT16)(((pctxt)->flags & (~ASN1CONSTAG)) | \
((!(((tag) & TM_FORM) >> 5) - 1) & ASN1CONSTAG)))

/* This macro will fetch the next byte in the input stream.  A test is  */
/* done for buffer overrun.                                             */

#define XD_MEMCPY1(pctxt, pvalue) \
(((ASN1BUF_INDEFLEN(pctxt)) || \
((pctxt)->buffer.byteIndex < (pctxt)->buffer.size)) ? \
(*pvalue = *OSRTBUFPTR(pctxt), (pctxt)->buffer.byteIndex ++, 0) : \
RTERR_ENDOFBUF)

/* This macro will fetch the next byte in the input stream.  No test is */
/* done for buffer overrun..                                            */

#define XD_FETCH1(pctxt) ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex++])

/* This macro will match a single-byte tag. It does not advance the     */
/* decode cursor.                                                       */

#define XD_PEEKTAG(pctxt, tag) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] & (~0x20)) == \
(tag & (~0x20)))

/* This macro will match a single-byte tag (assumes it is primitive only)
   and advances the decode cursor if it matches. It also clears the
   ASN1CONSTAG flag bit in the context. */

#define XD_MATCH1(pctxt, tag) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == tag) ? \
((pctxt)->flags &= (~ASN1CONSTAG), (pctxt)->buffer.byteIndex++, TRUE) : FALSE)

/* This macro will test for an EOC (end-of-context) marker */

#define XD_MATCHEOC(pctxt) \
( ( (pctxt)->buffer.byteIndex + 2 <= (pctxt)->buffer.size ) && \
( (pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == 0 ) && \
( (pctxt)->buffer.data[(pctxt)->buffer.byteIndex + 1] == 0 ) )

/* These macros will test a series of bytes in the decode buffer for a  */
/* match.                                                               */
#define XD_MATCHBYTES1(pctxt, b1) \
((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == b1)

#define XD_MATCHBYTES2(pctxt, b1, b2) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == b1) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+1] == b2))

#define XD_MATCHBYTES3(pctxt, b1, b2, b3) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == b1) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+1] == b2) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+2] == b3))

#define XD_MATCHBYTES4(pctxt, b1, b2, b3, b4) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == b1) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+1] == b2) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+2] == b3) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+3] == b4))

#define XD_MATCHBYTES5(pctxt, b1, b2, b3, b4, b5) \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == b1) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+1] == b2) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+2] == b3) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+3] == b4) && \
 ((pctxt)->buffer.data[(pctxt)->buffer.byteIndex+4] == b5))

/* This macro will advance the decode buffer index by the given number  */
/* number of bytes.                                                     */

#define XD_BUMPIDX(pctxt, nbytes) ((pctxt)->buffer.byteIndex += nbytes)

/* This macro will test for buffer overrun */

#define XD_CHKBUFEND(pctxt) \
(((ASN1BUF_INDEFLEN(pctxt)) || \
((pctxt)->buffer.byteIndex <= (pctxt)->buffer.size)) ? \
0 : RTERR_ENDOFBUF)

/* This macro will test if the given length value is within the current */
/* bounds of the message buffer.  The given length value is assumed to  */
/* be a definite length (i.e != ASN_K_INDEFLEN)..                       */

#define XD_CHKDEFLEN(pctxt,len) \
(((ASN1BUF_INDEFLEN(pctxt)) || \
(((pctxt)->buffer.byteIndex + len) <= (pctxt)->buffer.size)) ? \
0 : RTERR_ENDOFBUF)

/* This macro will test for EOB */

#define XD_CHKEOB(pctxt) \
(((pctxt)->buffer.byteIndex + 2 > (pctxt)->buffer.size) ? TRUE : \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == 0 && \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex + 1] == 0 ) ? \
TRUE : FALSE))

/* This macro will test for CCB overrun */

#define XD_CHKEND(pctxt,ccb_p) \
(((ccb_p)->len == ASN_K_INDEFLEN) ? XD_CHKEOB(pctxt) : \
((OSRTBUFPTR(pctxt) - (ccb_p)->ptr >= (ccb_p)->len) || \
((pctxt)->buffer.byteIndex >= (pctxt)->buffer.size)))

/* This macro is optimized version of xd_len function */

#define XD_LEN(pctxt,len_p) \
(((pctxt)->buffer.byteIndex < (pctxt)->buffer.size) ? \
(((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] < 0x80) ? \
(*(len_p)=(pctxt)->buffer.data[(pctxt)->buffer.byteIndex++], 0) : \
 xd_len ((pctxt),len_p)) : RTERR_ENDOFBUF)

/* This macro is an optimized version of the xd_tag function */

#define XD_TAG(pctxt,tag_p) \
(((pctxt)->buffer.byteIndex < (pctxt)->buffer.size) ? \
((((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] & TM_B_IDCODE) != 31) ? \
(*(tag_p) = (((ASN1TAG)((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] & \
TM_CLASS_FORM) << ASN1TAG_LSHIFT) | \
((pctxt)->buffer.data[(pctxt)->buffer.byteIndex] & TM_B_IDCODE)), \
SET_ASN1CONSTAG((pctxt),*(tag_p)), (pctxt)->buffer.byteIndex++, 0) : \
xd_tag((pctxt), (tag_p))) : RTERR_ENDOFBUF)

/* This macro will check if the given amount of space is available      */
/* in the encode buffer and attempt tp expand the buffer if necessary   */

#define XE_CHKBUF(pctxt,len) \
if (len > (pctxt)->buffer.byteIndex) { \
int stat = xe_expandBuffer (pctxt, len); \
if (stat != 0) return stat; }

/* This macro will put one byte into the encode buffer.  It is assumed  */
/* that there is enough space available for the byte.                   */

#define XE_PUT1(pctxt,ch) \
(pctxt)->buffer.data[--(pctxt)->buffer.byteIndex] = ch;

/* This macro will put two bytes into the encode buffer.  It is assumed */
/* that there is enough space available for the bytes.                  */

#define XE_PUT2(pctxt,ch1,ch2) \
(pctxt)->buffer.byteIndex -= 2; \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex] = ch1; \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex+1] = ch2;

/*
 * This macro will put one byte into the encode buffer.  It will first
 * check to see if enough space is available for the byte and attempt
 * to expand the buffer if necessary.
*/
#define XE_SAFEPUT1(pctxt,ch) \
XE_CHKBUF(pctxt,1); (pctxt)->buffer.data[--(pctxt)->buffer.byteIndex] = ch;

#ifdef __cplusplus
}
#endif
/** @} berruntime */
#endif /* _ASN1BER_H_ */
