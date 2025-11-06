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
 * @file rtPrint.h  Functions to print ASN.1 data in various formats.
 */
#ifndef _RTPRINT_H_
#define _RTPRINT_H_
#include <stdio.h>
#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxPrint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup valsToStdout Print Values to Standard Output
 * @ingroup cruntime
 * @{
 * These functions print the output in a "name=value" format. The
 * value format is obtained by calling one of the ToString functions with the
 * given value.
 */
#define rtPrintBoolean rtxPrintBoolean
#define rtPrintInteger rtxPrintInteger
#define rtPrintInt64   rtxPrintInt64
#define rtPrintUnsigned rtxPrintUnsigned
#define rtPrintUInt64  rtxPrintUInt64
#define rtPrintReal    rtxPrintReal
#define rtPrintCharStr rtxPrintCharStr
#define rtPrintHexStr  rtxPrintHexStr

/**
 * This function prints the value of a bit string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bitsto be printed.
 * @param data         A pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintBitStr (const char* name, OSSIZE numbits,
                              const OSOCTET* data, const char* conn);

#define rtPrintBitStrNamed(pctxt,numbits,data,conn,nb,pos,bn) \
rtPrintBitStr(name,numbits,data,conn)

/**
 * This function prints the value of a bit string to stdout in brace text
 * format.
 *
 * @param name         The name of the variable to print.
 * @param numbits      The number of bits to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtPrintBitStrBraceText
(const char* name, OSSIZE numbits, const OSOCTET* data);

/**
 * This function prints the value of an octet string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintOctStr (const char* name, OSSIZE numocts,
                              const OSOCTET* data, const char* conn);

/**
 * This function prints the value of a 16-bit character string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrint16BitCharStr (const char* name,
                                   const Asn116BitCharString* bstring);


/**
 * This function prints the value of a 32-bit character string to stdout.
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrint32BitCharStr (const char* name,
                                   const Asn132BitCharString* bstring,
                                   const char* conn);

/**
 * This function prints a Universal string to standard output.  Characters
 * in the string that are within the normal Ascii range are printed as
 * single characters.  Characters outside the Ascii range are printed
 * as 8-byte hex codes (0xnnnnnnnn).
 *
 * @param name         The name of the variable to print.
 * @param bstring      A pointer to the bit string to be printed.
 */
EXTERNRT void rtPrintUnivCharStr (const char* name,
                                  const Asn132BitCharString* bstring);

/**
 * This function prints the value of an object identifier to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOID (const char* name, const ASN1OBJID* pOID);

/**
 * This function prints the value of an object identifier
 * to stdout.  Only the value is printed, not the name.
 *
 * @param pOID         A pointer to the OID to be printed.
 */
EXTERNRT void rtPrintOIDValue (const ASN1OBJID* pOID);

/**
 * This function prints the value of an object identifier to stdout.
 * This version allows the number of sub-identifiers and array of
 * subidentifiers to be passed in separately allowing it to be used
 * for both static and dynamic OID structures.
 *
 * @param name         The name of the variable to print.
 * @param numids       Number of sub-identifiers
 * @param subidArray   Pointer to array of sub-identifiers.
 */
EXTERNRT void rtPrintOID2
(const char* name, OSSIZE numids, const OSUINT32* subidArray);

/**
 * This function prints the value of an object identifier
 * to stdout.  Only the value is printed, not the name.
 * This version allows the number of sub-identifiers and array of
 * subidentifiers to be passed in separately allowing it to be used
 * for both static and dynamic OID structures.
 *
 * @param numids       Number of sub-identifiers
 * @param subidArray   Pointer to array of sub-identifiers.
 */
EXTERNRT void rtPrintOIDValue2 (OSSIZE numids, const OSUINT32* subidArray);

/**
 * This function prints the value of an open type to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         The pointer to the data to be printed.
 * @param conn         A pointer to the connector between the name and the
 *                       value. This points to either -> or .
 */
EXTERNRT void rtPrintOpenType (const char* name, OSSIZE numocts,
                               const OSOCTET* data, const char* conn);

/**
 * This function prints the value of an open type extension to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pElemList    A pointer to an element of a list.
 */
EXTERNRT void rtPrintOpenTypeExt (const char* name, const OSRTDList* pElemList);

/**
 * This function prints the value of an open type extension in brace
 * text format to stdout.
 *
 * @param name         The name of the variable to print.
 * @param pElemList    A pointer to an element of a list.
 */
EXTERNRT void rtPrintOpenTypeExtBraceText
(const char* name, const OSRTDList* pElemList);

#define rtPrintIndent rtxPrintIndent
#define rtPrintIncrIndent rtxPrintIncrIndent
#define rtPrintDecrIndent rtxPrintDecrIndent
#define rtPrintCloseBrace rtxPrintCloseBrace
#define rtPrintOpenBrace  rtxPrintOpenBrace

/**
 * @} valsToStdout
 */
#ifdef __cplusplus
}
#endif
#endif
