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

#ifndef _OSSYSTYPES_H_
#define _OSSYSTYPES_H_

/* Ensure that the appropriate limits are active. */
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <stdlib.h>

#if !defined(_MSC_VER) && !defined(__BORLANDC__) && !defined(__WATCOMC__)
#include <inttypes.h> /* does not exist in windows; use builtins instead */
#endif /* not windows */

#include <limits.h>

#include <float.h>

#ifndef FALSE
#define FALSE   0
#define TRUE    1
#endif

#define NumberOf(x)	(sizeof(x)/sizeof(x[0]))

typedef void            OSVoid;
typedef void*           OSVoidPtr;
typedef unsigned char   OSBOOL;
typedef signed char     OSINT8;
typedef unsigned char   OSUINT8;
typedef short           OSINT16;
typedef unsigned short  OSUINT16;
typedef OSUINT8		OSOCTET;
typedef OSUINT8         OSUTF8CHAR;
typedef OSUINT16        OSUNICHAR;
typedef double          OSREAL;
typedef double          OSDOUBLE;
typedef float           OSFLOAT;
typedef int             OSINT32;
typedef unsigned int    OSUINT32;
#define OSINTCONST(val)  val
#define OSUINTCONST(val) val##u
typedef OSUINT16        OS16BITCHAR;
typedef OSUINT32        OS32BITCHAR;
typedef size_t          OSSIZE;

#define OSUINT32_MAX    ((OSUINT32)4294967295UL)
#define OSINT32_MAX     ((OSINT32)2147483647L)
#define OSINT32_MIN     ((OSINT32)(-OSINT32_MAX-1))

#define OSUINT16_MAX    ((OSUINT16)65535UL)
#define OSINT16_MAX     ((OSINT16)32767L)
#define OSINT16_MIN     ((OSINT16)(-OSINT16_MAX-1))

#define OSUINT8_MAX     ((OSUINT8)255U)
#define OSINT8_MAX      ((OSINT8)127L)
#define OSINT8_MIN      ((OSINT8)(-OSINT8_MAX-1))

#define OSREALMAX       ((OSREAL)DBL_MAX)
#define OSREALMIN       ((OSREAL)-DBL_MAX)

/*
 * Cast a string literal as a UTF-8 character string value
 */
#define OSUTF8(str) ((const OSUTF8CHAR*)str)

/**
 * @struct OSNumDateTime
 * Numeric date/time structure
 */
typedef struct OSNumDateTime {
   OSINT32      year;
   OSUINT8      mon;            /* 1 <=  mon    <= 12   */
   OSUINT8      day;            /* 1 <=  day    <= 31   */
   OSUINT8      hour;           /* 0 <=  hour   <= 23   */
   OSUINT8      min;            /* 0 <=  min    <= 59	*/
   OSREAL       sec;
   OSBOOL       tz_flag;        /* is tzo explicitely set? */
   OSINT32      tzo;            /* -1440 <= tzo <= 1440 */
} OSNumDateTime;

/* 64-bit long integer type */

#ifndef OSINT64
#if !defined(_NO_INT64_SUPPORT)
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__)
#define OSINT64 __int64
#define OSUINT64 unsigned __int64
#define OSINT64FMT  "%I64d"
#define OSUINT64FMT "%I64u"
#define OSINT64FMTX "%I64x"
#define OSINT64MAX  _I64_MAX
#define OSUINT64MAX _UI64_MAX
#define OSINT64MIN  _I64_MIN
#define OSI64CONST(val)  val##i64
#define OSUI64CONST(val) val##ui64

#elif defined(INT64_MAX) /* assume have ISO C99 standard types */
#define OSINT64  int64_t
#define OSUINT64 uint64_t
#define OSINT64FMT  "%lld"
#define OSUINT64FMT "%llu"
#define OSINT64FMTX "%llx"
#define OSINT64MAX  INT64_MAX
#define OSUINT64MAX UINT64_MAX
#define OSINT64MIN  INT64_MIN
#define OSI64CONST(val)  INT64_C(val)
#define OSUI64CONST(val) UINT64_C(val)

#else /* ?? __WORDSIZE != 64 */
#warning "Using long long for 64-bit integers."
#define OSINT64 long long
#define OSUINT64 unsigned long long
#define OSINT64FMT  "%lld"
#define OSUINT64FMT "%llu"
#define OSINT64FMTX "%llx"
#define OSI64CONST(val)  val##LL
#define OSUI64CONST(val) val##ULL

/* May throw an error if __STDC_LIMIT_MACROS is not defined */
#define OSINT64MAX  LLONG_MAX
#define OSUINT64MAX ULLONG_MAX
#define OSINT64MIN  LLONG_MIN

#endif /* _MSC_VER ... */
#endif /* !defined(_NO_INT64_SUPPORT) */

#ifndef OSINT64
/* if OSINT64 is still not defined - define it as long/unsigned long */
#define OSINT64 long
#define OSUINT64 unsigned long
#define OSINT64FMT  "%ld"
#define OSUINT64FMT "%lu"
#define OSINT64FMTX "%lx"
#define OSINT64MAX  LONG_MAX
#define OSUINT64MAX ULONG_MAX
#define OSINT64MIN  LONG_MIN
#define OSI64CONST(val)  val##L
#define OSUI64CONST(val) val##UL
#endif /* OSINT64 ... */

#endif /* OSINT64 */

/**
 * @struct OSDynOctStr rtxSysTypes.h
 * Dynamic binary string structure
 *
 * This structure is used in generated code for XSD hexBinary and
 * base64Binary types.
 */
typedef struct OSDynOctStr {
   OSUINT32     numocts;
   const OSOCTET* data;
} OSDynOctStr;

typedef OSDynOctStr OSOpenType;
typedef OSDynOctStr ASN1DynOctStr;

/**
 * @struct OSXSDAny
 * Structure to hold xsd:any data in binary and XML text form.
 */
typedef enum { OSXSDAny_binary, OSXSDAny_xmlText } OSXSDAnyAlt;
typedef struct OSXSDAny {
   OSXSDAnyAlt t;
   union {
      OSOpenType* binary;
      const OSUTF8CHAR* xmlText;
   } u;
} OSXSDAny;

/* UTF-8 name/value pair */

typedef struct OSUTF8NVP {
   const OSUTF8CHAR* name;
   const OSUTF8CHAR* value;
} OSUTF8NVP;

typedef OSUTF8NVP OSAnyAttr;
typedef OSUTF8NVP OSAnyElement;

/* ASN.1 useful type definitions */

typedef struct {  /* generic bit string structure (dynamic) */
   OSUINT32     numbits;
   const OSOCTET* data;
} ASN1DynBitStr;

typedef struct {  /* fixed-size bit string that can hold up to 32 bits */
   OSUINT32     numbits;
   OSOCTET      data[4];
} ASN1BitStr32;

#define ASN_K_MAXSUBIDS 128     /* maximum sub-id's in an object ID     */

typedef struct {        /* object identifier */
   OSUINT32     numids;
   OSUINT32     subid[ASN_K_MAXSUBIDS];
} ASN1OBJID;

typedef struct {
   OSUINT32       nchars;
   OSUNICHAR*     data;
} Asn116BitCharString;

typedef struct {
   OSUINT32       nchars;
   OS32BITCHAR*   data;
} Asn132BitCharString;

typedef Asn116BitCharString ASN1BMPString;
typedef Asn132BitCharString ASN1UniversalString;
typedef OSOpenType ASN1OpenType;

#endif
