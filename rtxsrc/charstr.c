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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxCharStr.h"

char* rtxStrcat (char* dest, size_t bufsiz, const char* src)
{
   if (0 == dest || 0 == bufsiz || 0 == src)
      return (char*)0;
   else
      return rtxStrncat (dest, bufsiz, src, strlen(src));
}

char* rtxStrncat(char* dest, size_t bufsiz, const char* src, size_t nchars)
{
   size_t i;

   if (0 == dest || bufsiz <= nchars || 0 == src || 0 == nchars)
      return (char*)0;

   i = OSCRTLSTRLEN (dest);

   if ((bufsiz - i) <= nchars || i >= bufsiz) /*  check for underflow */
      return (char*)0;

   return rtxStrncpy (&dest[i], bufsiz - i, src, nchars);
}

char* rtxStrcpy (char* dest, size_t bufsiz, const char* src)
{
   if (0 == dest || 0 == bufsiz || 0 == src)
      return (char*)0;
   else
      return rtxStrncpy (dest, bufsiz, src, strlen(src));
}

char* rtxStrncpy(char* dest, size_t bufsiz, const char* src, size_t nchars)
{
   size_t i;

   if (0 == dest || bufsiz < nchars || 0 == src || bufsiz == 0) {
      return (char *)0;
   }

   for (i = 0; i < nchars; i++) {
      if (src[i] == '\0') break;
      else dest[i] = src[i];
   }

   if (i < bufsiz) {
      dest[i] = '\0';
      return dest;
   }
   else
   {
      dest[i-1] = '\0'; /* always terminate */
      return (char *)0;
   }
}

char* rtxStrdup (OSCTXT* pctxt, const char* src)
{
   size_t bufsize = strlen(src) + 1;
   char* tmpbuf = (char*) rtxMemAlloc (pctxt, bufsize);
   if (tmpbuf != 0) {
      rtxStrncpy (tmpbuf, bufsize, src, bufsize - 1);
   }
   return tmpbuf;
}

const char* rtxStrJoin
(char* dest, size_t bufsiz, const char* str1, const char* str2,
 const char* str3, const char* str4, const char* str5)
{
   size_t partLen, idx = 0;
   const char* parts[5];
   OSUINT32 i;

   parts[0] = str1; parts[1] = str2; parts[2] = str3;
   parts[3] = str4, parts[4] = str5;

   for (i = 0; i < 5; i++) {
      if (0 != parts[i]) {
         partLen = strlen (parts[i]);
         if ((idx + partLen) < bufsiz) {
            rtxStrcpy (&dest[idx], bufsiz-idx, parts[i]);
            idx += partLen;
         }
         else break;
      }
      else break;
   }

   dest[idx] = '\0';

   return dest;
}

int rtxUIntToCharStr (OSUINT32 value, char* dest, size_t bufsiz, char padchar)
{
#if defined(_MSC_VER)
   _snprintf_s (dest, bufsiz, _TRUNCATE, "%u", value);
#else
   snprintf (dest, bufsiz, "%u", value);
#endif
   return 0;
}
