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

#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxCommon.h"

int rtxUTF8Len (const OSUTF8CHAR* inbuf)
{
   size_t inlen = rtxUTF8LenBytes (inbuf);
   size_t inbufx = 0;
   int nbytes, nchars = 0;
   OSOCTET byte;

   while (inbufx < inlen) {

      /* Process the first byte of the UTF-8 sequence */

      byte = inbuf[inbufx++];
      if (byte < 0x80) {
         nbytes = 0;
      }
      else if ((byte & 0xe0) == 0xc0) {
         nbytes = 1;
      }
      else if ((byte & 0xf0) == 0xe0) {
         nbytes = 2;
      }
      else if ((byte & 0xf8) == 0xf0) {
         nbytes = 3;
      }
      else if ((byte & 0xfc) == 0xf8) {
         nbytes = 4;
      }
      else if ((byte & 0xfe) == 0xfc) {
         nbytes = 5;
      }
      else {
         return RTERR_INVUTF8; /* invalid encoding error */
      }

      inbufx += nbytes;
      nchars ++;
   }

   return (nchars);
}

size_t rtxUTF8LenBytes (const OSUTF8CHAR* inbuf)
{
   size_t len = 0;

   if (0 != inbuf) {
      while (0 != inbuf[len]) len++;
   }

   return len;
}

const char* rtBMPToCString
(ASN1BMPString* pBMPString, char* cstring, OSSIZE cstrsize)
{
   OSUINT32 i, j;
   for (i = 0, j = 0; i < pBMPString->nchars; i++) {
      if (pBMPString->data[i] < 256) {
         cstring[j++] = (char)pBMPString->data[i];
         if (j >= cstrsize) return cstring;
      }
   }
   cstring[j] = '\0';
   return cstring;
}

const char* rtUCSToCString
(ASN1UniversalString* pUCSString, char* cstring, OSSIZE cstrsize)
{
   OSUINT32 i, j;
   for (i = 0, j = 0; i < pUCSString->nchars; i++) {
      if (pUCSString->data[i] < 256) {
         cstring[j++] = (char)pUCSString->data[i];
         if (j >= cstrsize) return cstring;
      }
   }
   cstring[j] = '\0';
   return cstring;
}

int rtValidateStr (OSUINT32 tag, const char *pdata)
{
   /* not supported in OO version */
   return 0;
}

