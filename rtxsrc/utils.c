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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxEnum.h"

/* Compare two OID's for equality */

OSBOOL rtOIDsEqual (const ASN1OBJID* pOID1, const ASN1OBJID* pOID2)
{
   if (pOID1->numids == pOID2->numids) {
      OSUINT32 ui;
      for (ui = 0; ui < pOID1->numids; ui++) {
         if (pOID1->subid[ui] != pOID2->subid[ui]) {
            return FALSE;
         }
      }
      return TRUE;
   }

   return FALSE;
}

OSBOOL rtCopyOID (const ASN1OBJID* srcOID, ASN1OBJID* dstOID)
{
   if (srcOID == NULL || dstOID == NULL) return FALSE;
   if (srcOID->numids > 0) {
      dstOID->numids = srcOID->numids;
      memcpy (dstOID->subid, srcOID->subid,
              dstOID->numids * sizeof (dstOID->subid[0]));
   }
   else {
      dstOID->numids = 0;
   }
   return TRUE;
}

OSINT32 rtxLookupEnum
(const OSUTF8CHAR* strValue, size_t strValueSize,
 const OSEnumItem enumTable[], OSUINT16 enumTableSize)
{
   size_t lower = 0;
   size_t upper = enumTableSize - 1;
   size_t middle;
   int    cmpRes;

   if (strValueSize == (size_t)-1) {
      strValueSize = rtxUTF8LenBytes (strValue);
   }

   while (lower < upper && upper != (size_t)-1) {
      middle = (lower + upper)/2;

      cmpRes = strncmp
         ((const char*)enumTable[middle].name, (const char*)strValue,
          strValueSize);

      if (cmpRes == 0)
         cmpRes = (int)enumTable[middle].namelen - (int)strValueSize;

      if (cmpRes == 0) { /* equal */
         return (int)middle;
      }
      if (cmpRes < 0)
         lower = middle+1;
      else
         upper = middle-1;
   }

   if (lower == upper && (size_t)enumTable[lower].namelen == strValueSize &&
       strncmp ((const char*)enumTable[lower].name, (const char*)strValue,
                strValueSize) == 0) {
      return (int)lower;
   }

   return RTERR_INVENUM;
}

OSINT32 rtxLookupEnumByValue
(OSINT32 value, const OSEnumItem enumTable[], size_t enumTableSize)
{
   size_t i;
   for (i = 0; i < enumTableSize; i++) {
      if (enumTable[i].value == value) return (int)i;
   }
   return RTERR_INVENUM;
}
