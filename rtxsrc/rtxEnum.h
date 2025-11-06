/**
 * Copyright (c) 1997-2025 by Objective Systems, Inc.
 * https://obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   https://obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
/**
 * @file rtxEnum.h
 * Common runtime types and functions for performing operations on
 * enumerated data items.
 */
#ifndef _RTXENUM_H_
#define _RTXENUM_H_

#include "rtxsrc/rtxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#ifdef __cplusplus
}
#endif

#endif
