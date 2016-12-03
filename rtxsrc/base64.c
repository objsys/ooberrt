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

#include "rtxsrc/rtxCommon.h"

static char getBase64Char (int idx)
{
   if (idx >= 0 && idx <= 25) return (char)('A' + (char)idx);
   else if (idx > 25 && idx <= 51) return (char)('a' + (char)idx - 26);
   else if (idx > 51 && idx <= 61) return (char)('0' + (char)idx - 52);
   else if (idx == 62) return '+';
   else if (idx == 63) return '/';
   else return -1;
}

long rtxBase64EncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   size_t i;
   OSOCTET* pDstData;
   size_t numFullGroups = srcDataSize/3;
   size_t numBytesInPartialGroup = srcDataSize - 3*numFullGroups;
   size_t resultLen = 4*((srcDataSize + 2)/3);

   pDstData = *ppDstData = (OSOCTET*) rtxMemAlloc (pctxt, resultLen + 1);
   if (pDstData == 0) return LOG_RTERR (pctxt, RTERR_NOMEM);

   /* Translate all full groups from byte array elements to Base64 */
   for (i = 0; i < numFullGroups; i++) {
       int byte0 = *pSrcData++ & 0xff;
       int byte1 = *pSrcData++ & 0xff;
       int byte2 = *pSrcData++ & 0xff;
       *pDstData++ = getBase64Char (byte0 >> 2);
       *pDstData++ = getBase64Char (((byte0 << 4) & 0x3f) | (byte1 >> 4));
       *pDstData++ = getBase64Char (((byte1 << 2) & 0x3f) | (byte2 >> 6));
       *pDstData++ = getBase64Char (byte2 & 0x3f);
   }

   /* Translate partial group if present */
   if (numBytesInPartialGroup != 0) {
       int byte0 = *pSrcData++ & 0xff;
       *pDstData++ = getBase64Char (byte0 >> 2);
       if (numBytesInPartialGroup == 1) {
          *pDstData++ = getBase64Char ((byte0 << 4) & 0x3f);
          *pDstData++ = '=';
          *pDstData++ = '=';
       } else {
          /* assert numBytesInPartialGroup == 2; */
          int byte1 = *pSrcData++ & 0xff;
          *pDstData++ = getBase64Char (((byte0 << 4) & 0x3f) | (byte1 >> 4));
          *pDstData++ = getBase64Char ((byte1 << 2) & 0x3f);
          *pDstData++ = '=';
       }
   }
   return (long)resultLen;
}

/* Decode */

/*
 * This array is a lookup table that translates characters
 * drawn from the "Base64 Alphabet" (as specified in Table 1 of RFC 2045)
 * into their 6-bit positive integer equivalents. Characters that
 * are not in the Base64 alphabet but fall within the bounds of the
 * array are translated to -1. Note, first 40 values are omitted, because
 * all of them are -1. Use offset -40 to fetch values from this table.
 */
static const signed char decodeTable[] = {
   /*
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, */
   -1, -1, -1, 62, -1, -1, -1, 63,
   52, 53, 54, 55, 56, 57, 58, 59,
   60, 61, -1, -1, -1, -1, -1, -1,
   -1,  0,  1,  2,  3,  4,  5,  6,
   7,   8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, -1, -1, -1, -1, -1,
   -1, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48,
   49, 50, 51, -1, -1, -1, -1, -1
};

#define BASE64TOINT(c) ((((OSOCTET)c) < 128) ? decodeTable [(c) - 40] : -1)

static long base64ToBin
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET* pvalue)
{
   size_t i, j = 0;
   int ch[4]; /* group buffer */
   OSOCTET* bufp = pvalue;

   for (i = 0; i < srcDataSize; ) {
      if (OS_ISSPACE (pSrcData[i])) {
         i++;
      }
      else if (pSrcData[i] != '=') {
         ch[j] = BASE64TOINT (pSrcData[i]);
         if (ch[j] < 0) {
            return (0 != pctxt) ?
               LOG_RTERR (pctxt, RTERR_INVBASE64) : RTERR_INVBASE64;
         }
         else if (++j == 4) {
            /* Group is complete */
            *pvalue++ = (OSOCTET) ((ch[0] << 2) | (ch[1] >> 4));
            *pvalue++ = (OSOCTET) ((ch[1] << 4) | (ch[2] >> 2));
            *pvalue++ = (OSOCTET) ((ch[2] << 6) |  ch[3]);
            j = 0;
         }
         i++;
      }
      else { /* partial group */
         if (j < 2 || j > 3) {
            return (0 != pctxt) ?
               LOG_RTERR (pctxt, RTERR_INVBASE64) : RTERR_INVBASE64;
         }
         *pvalue++ = (OSOCTET) ((ch[0] << 2) | (ch[1] >> 4));
         if (j == 3) {
            *pvalue++ = (OSOCTET) ((ch[1] << 4) | (ch[2] >> 2));
         }
         while (pSrcData[i] == '=') i++;
         j = 0;
      }
   }

   return (long)(pvalue - bufp);
}

static long getBinDataLen (const char* pSrcData, size_t srcDataSize)
{
   size_t i, j = 0;
   long nbytes = 0;
   for (i = 0; i < srcDataSize; ) {
      if (OS_ISSPACE (pSrcData[i])) {
         i++;
      }
      else if (pSrcData[i] != '=') {
         if (++j == 4) {
            nbytes += 3;
            j = 0;
         }
         i++;
      }
      else { /* partial group */
         if (j == 2) nbytes += 1;
         else if (j == 3) nbytes += 2;
         else return RTERR_INVBASE64;
         while (pSrcData[i] == '=') i++;
         j = 0;
      }
   }
   return nbytes;
}

long rtxBase64DecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   size_t bufsiz;
   OSOCTET* pvalue;
   long stat = getBinDataLen (pSrcData, srcDataSize);

   if (stat < 0) return LOG_RTERR (pctxt, (int)stat);
   else bufsiz = (size_t) stat;

   pvalue = *ppDstData = (OSOCTET*) rtxMemAlloc (pctxt, bufsiz);
   if (pvalue == 0) return LOG_RTERR (pctxt, RTERR_NOMEM);

   return base64ToBin (pctxt, pSrcData, srcDataSize, pvalue);
}
