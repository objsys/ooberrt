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

/* Run-time print utility functions */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "rtsrc/rtPrint.h"

#ifndef BITSTR_BYTES_IN_LINE
#define BITSTR_BYTES_IN_LINE 16
#endif

void rtxPrintBoolean (const char* name, OSBOOL value)
{
   printf ("%s = %s\n", name, value ? "true" : "false");
}

void rtxPrintInteger (const char* name, OSINT32 value)
{
   printf ("%s = %d\n", name, value);
}

void rtxPrintUnsigned (const char* name, OSUINT32 value)
{
   printf ("%s = %u\n", name, value);
}

void rtxPrintCharStr (const char* name, const char* cstring)
{
   printf ("%s = '%s'\n", name,
      (cstring == 0) ? "(null)" : cstring);
}

void rtxPrintUTF8CharStr (const char* name, const OSUTF8CHAR* cstring)
{
   printf ("%s = '%s'\n", name,
      (cstring == 0) ? "(null)" : (const char*)cstring);
}

void rtxPrintUnicodeCharStr
(const char* name, const OSUNICHAR* str, int nchars)
{
   int i;
   if (0 == str) return;
   if (nchars < 0) {
      nchars = 0;
      while (str[nchars] != 0) nchars++;
   }
   printf ("%s = '", name);
   for (i = 0; i < nchars; i++) {
      if (isprint (str[i]))
         putc (str[i], stdout);
      else
         printf ("0x%04x", str[i]);
   }
   printf ("'\n");
}

static char* strTrim (char *s)
{
   int i = (int)OSCRTLSTRLEN(s) - 1;
   while (i >= 0 && s[i] == ' ') i--;
   s[i+1] = '\0';
   return (s);
}

static int rtxByteToHexChar (OSOCTET byte, char* buf, OSSIZE bufsize)
{
   OSUINT32 tmpval = byte;
   if (bufsize < 3) return RTERR_BUFOVFLW;
   tmpval >>= 4;
   buf[0] = (char)((tmpval <= 9) ? tmpval + '0' : tmpval - 10 + 'a');
   tmpval = byte & 0x0F;  /* shave off MS nibble */
   buf[1] = (char)((tmpval <= 9) ? tmpval + '0' : tmpval - 10 + 'a');
   buf[2] = '\0';
   return 0;
}

static void rtxHexDumpToFileEx
(FILE* fp, const OSOCTET* data, OSSIZE numocts, int bytesPerUnit)
{
   OSSIZE   i;
   OSUINT32 val;
   OSOCTET  b, dumpstr = 1;
   char     hexstr[49], ascstr[17], buf[20], *pbuf;
   int      k, unitsPerLine = 16 / bytesPerUnit, ai, shift;

   memset (hexstr, ' ', 48); hexstr[48] = '\0';
   memset (ascstr, ' ', 16); ascstr[16] = '\0';
   ai = 0;

   if (bytesPerUnit > 4) bytesPerUnit = 4;

   for (i = 0; i < numocts / bytesPerUnit; i++)
   {
      pbuf = (char*)buf;
      buf [bytesPerUnit * 2] = 0;
      if (bytesPerUnit == 1) {
         val = *data++;
         shift = 0;
      }
      else if (bytesPerUnit == 2) {
         val = *data++;
         val = (val * 256) + *data++;
         shift = 8;
      }
      else { /* greater than 2 */
         OSSIZE ii;
         for (ii = 0, val = 0; ii < sizeof (OSUINT32); ii++) {
            val = (val * 256) + *data++;
         }
         shift = (sizeof(OSUINT32) - 1) * 8;
      }
      for (k = 0; k < bytesPerUnit; k++, pbuf += 2, shift -= 8) {
         b = (OSOCTET)((val >> shift) & 0xFF);
         rtxByteToHexChar (b, pbuf, 20);
         ascstr[ai++] = (char) (isprint(b) ? b : '.');
      }
      *pbuf = 0;

      k = i % unitsPerLine * (bytesPerUnit * 2 + 1);
      memcpy (&hexstr[k], buf, bytesPerUnit * 2);

      if ((dumpstr = (OSOCTET) ((i + 1) % unitsPerLine == 0)) != 0)
      {
         fprintf (fp, "%48s %16s\n", hexstr, ascstr);
         if (i < (numocts - 1)) {
            OSCRTLMEMSET (hexstr, ' ', 48);
            OSCRTLMEMSET (ascstr, ' ', 16);
            ai = 0;
         }
      }
   }

   if (!dumpstr) fprintf (fp, "%48s %s\n", hexstr, strTrim(ascstr));
}

static void rtxHexDumpToFile
(FILE* fp, const OSOCTET* data, OSSIZE numocts)
{
   rtxHexDumpToFileEx (fp, data, numocts, 1);
}

static void rtxHexDump (const OSOCTET* data, OSSIZE numocts)
{
   rtxHexDumpToFile (stdout, data, numocts);
}

void rtxPrintHexStr
(const char* name, size_t numocts, const OSOCTET* data)
{
   if (numocts <= 32) {
      OSUINT32 i;
      printf ("%s = 0x", name);
      for (i = 0; i < numocts; i++) {
         printf ("%02x", data[i]);
      }
      printf ("\n");
   }
   else {
      printf ("%s =\n", name);
      rtxHexDump (data, numocts);
   }
}

void rtxPrintNVP (const char* name, const OSUTF8NVP* pnvp)
{
   printf ("%s.name  = '%s'\n", name,
      (pnvp->name == 0) ? "(null)" : (const char*)pnvp->name);
   printf ("%s.value = '%s'\n", name,
      (pnvp->value == 0) ? "(null)" : (const char*)pnvp->value);
}

void rtxPrintHexBinary
(const char* name, size_t numocts, const OSOCTET* data)
{
   printf ("%s.numocts = %u\n", name, numocts);
   printf ("%s.data = \n", name);
   rtxHexDump (data, numocts);
}

void rtxPrintNull (const char* name)
{
   printf ("%s = <null>\n", name);
}

/* Indentation for brace text printing */

static OSUINT32 g_indent = 0;

void rtxPrintIndent ()
{
   OSUINT32 i;
   for (i = 0; i < g_indent; i++) printf (" ");
}

void rtxPrintIncrIndent ()
{
   g_indent += OSRTINDENTSPACES;
}

void rtxPrintDecrIndent ()
{
   if (g_indent > 0)
      g_indent -= OSRTINDENTSPACES;
}

void rtxPrintCloseBrace ()
{
   rtxPrintDecrIndent ();
   rtxPrintIndent ();
   printf ("}\n");
}

void rtxPrintOpenBrace (const char* name)
{
   rtxPrintIndent ();
   printf ("%s {\n", name);
   rtxPrintIncrIndent ();
}

static void rtBitStringDump (OSSIZE numbits, const OSOCTET* data)
{
   OSSIZE i, numocts = numbits / 8;
   char buff[9];

   if (numbits == 0 || 0 == data) return;

   if (numocts > 8)
      printf ("\n");

   for (i = 0; i < numocts; i++) {
      if ((i != 0) && (i % BITSTR_BYTES_IN_LINE == 0)) {
         printf ("\n");
      }
      else if (i % BITSTR_BYTES_IN_LINE != 0) {
         printf (" ");
      }

      printf ("0x%02X", data[i]);
   }

   if (i * 8 != numbits) {
      OSOCTET tm = data[i];
      OSSIZE nmBits = numbits % 8;
      OSSIZE j;

      for (j = 0; j < nmBits; j++, tm<<=1)
         buff[j] = ((tm >> 7) & 1) + '0';
      for (; j < 8; j++)
         buff[j] = 'x';

      if ((i % BITSTR_BYTES_IN_LINE) == (BITSTR_BYTES_IN_LINE - 1))
         printf ("\n%.8s", buff);
      else if (i > 0)
         printf (" %.8s", buff);
      else
         printf ("%.8s", buff);
   }
}

void rtPrintBitStr (const char* name, OSSIZE numbits,
                                const OSOCTET* data, const char* conn)
{
   printf ("%s%snumbits = %u\n", name, conn, numbits);
   printf ("%s%sdata = ", name, conn);
   rtBitStringDump (numbits, data);
   printf ("\n");
}

void rtPrintBitStrBraceText
(const char* name, OSSIZE numbits, const OSOCTET* data)
{
   printf ("%s = { %u, ", name, numbits);
   rtBitStringDump (numbits, data);
   printf (" }\n");
}

void rtPrintOctStr (const char* name, OSSIZE numocts,
                                const OSOCTET* data, const char* conn)
{
   printf ("%s%snumocts = %u\n", name, conn, numocts);
   printf ("%s%sdata = \n", name, conn);
   rtxHexDump (data, numocts);
}

void rtPrint16BitCharStr
(const char* name, const Asn116BitCharString* bstring)
{
   rtxPrintUnicodeCharStr (name, bstring->data, bstring->nchars);
}

void rtPrint32BitCharStr
(const char* name, const Asn132BitCharString* bstring, const char* conn)
{
   printf ("%s%snchars = %u\n", name, conn, bstring->nchars);
   printf ("%s%sdata = \n", name, conn);
   rtxHexDumpToFileEx (stdout, (OSOCTET*)bstring->data,
                       bstring->nchars * sizeof (OSUINT32), 4);
}

void rtPrintOID (const char* name, const ASN1OBJID* pOID)
{
   printf ("%s = ", name);
   rtPrintOIDValue (pOID);
}

void rtPrintOIDValue (const ASN1OBJID* pOID)
{
   OSUINT32 ui;
   printf ("{ ");
   for (ui = 0; ui < pOID->numids; ui++) {
      printf ("%d ", pOID->subid[ui]);
   }
   printf ("}\n");
}

void rtPrintOpenType (const char* name, OSSIZE numocts,
                      const OSOCTET* data, const char*  conn)
{
   printf ("%s%snumocts = %u\n", name, conn, numocts);
   printf ("%s%sdata = \n", name, conn);
   rtxHexDump (data, numocts);
}

void rtPrintOpenTypeExt (const char* name, const OSRTDList* pElemList)
{
   ASN1OpenType* pOpenType;
   if (0 != pElemList) {
      OSRTDListNode* pnode = pElemList->head;
      while (0 != pnode) {
         if (0 != pnode->data) {
            pOpenType = (ASN1OpenType*) pnode->data;
            rtPrintOpenType (name, pOpenType->numocts, pOpenType->data, ".");
         }
         pnode = pnode->next;
      }
   }
}

void rtPrintUnivCharStr (const char* name, const Asn132BitCharString* bstring)
{
   OSUINT32 i;
   printf ("%s = '", name);
   for (i = 0; i < bstring->nchars; i++) {
      if (isprint (bstring->data[i]))
         putc (bstring->data[i], stdout);
      else
         printf ("0x%08x", bstring->data[i]);
   }
   printf ("'\n");
}

void rtPrintOpenTypeExtBraceText
(const char* name, const OSRTDList* pElemList)
{
   ASN1OpenType* pOpenType;
   if (0 != pElemList) {
      OSRTDListNode* pnode = pElemList->head;
      while (0 != pnode) {
         if (0 != pnode->data) {
            pOpenType = (ASN1OpenType*) pnode->data;
            rtxPrintIndent ();
            rtxPrintHexStr (name, pOpenType->numocts, pOpenType->data);
         }
         pnode = pnode->next;
      }
   }
}

