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
#include <stdlib.h>
#include <string.h>

int rtInitContext (OSCTXT* pctxt)
{
   memset (pctxt, 0, sizeof(OSCTXT));
   return 0;
}

int rtInitContextUsingKey
(OSCTXT* pctxt, const OSOCTET* key, size_t keylen)
{
   memset (pctxt, 0, sizeof(OSCTXT));
   return 0;
}

int rtxInitContextBuffer
(OSCTXT* pctxt, const OSOCTET* bufaddr, size_t bufsiz)
{
   if (bufaddr == 0) {
      /* dynamic buffer */
      if (bufsiz == 0) bufsiz = ASN_K_ENCBUFSIZ;
      pctxt->buffer.data = (OSOCTET*) rtxMemAlloc (pctxt, bufsiz);
      if (!pctxt->buffer.data) return RTERR_NOMEM;
      pctxt->buffer.size = bufsiz;
      pctxt->buffer.dynamic = TRUE;
   }
   else {
      /* static buffer */
      pctxt->buffer.data = (OSOCTET*) bufaddr;
      pctxt->buffer.size = bufsiz;
      pctxt->buffer.dynamic = FALSE;
   }

   pctxt->buffer.byteIndex = 0;
   pctxt->buffer.bitOffset = 8;

   return 0;
}

void rtFreeContext (OSCTXT* pctxt)
{
   OSBOOL saveBuf = (pctxt->flags & ASN1SAVEBUF) != 0;

   if (pctxt->buffer.dynamic && pctxt->buffer.data) {
      if (!saveBuf) {
         rtxMemFreePtr (pctxt, pctxt->buffer.data);
      }
   }

   rtxErrFreeParms (pctxt);

   if (pctxt->pMemHeap != 0) {
      rtxMemFree (pctxt);
      free (pctxt->pMemHeap);
      pctxt->pMemHeap = 0;
   }
}

void rtxCopyContext (OSCTXT* pdest, OSCTXT* psrc)
{
   memcpy (&pdest->buffer, &psrc->buffer, sizeof(ASN1BUFFER));
   pdest->flags = psrc->flags;
}

void rtxCtxtSetFlag (OSCTXT* pctxt, OSUINT16 mask)
{
   pctxt->flags |= mask;
}

void rtxCtxtClearFlag (OSCTXT* pctxt, OSUINT16 mask)
{
   pctxt->flags &= ~mask;
}
