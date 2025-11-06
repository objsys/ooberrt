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

#include <string.h>
#include "asn1ber.h"

static int xe_identifier (OSCTXT *pctxt, unsigned ident);
static int xe_len (OSCTXT *pctxt, int length);
static int xe_tag (OSCTXT *pctxt, ASN1TAG tag);
static int xe_expandBuffer (OSCTXT *pctxt, size_t length);

int xe_bigint
(OSCTXT* pctxt, const char* pvalue, ASN1TagType tagging)
{
   int aal, stat;
   size_t len;

   if (0 == pvalue || *pvalue == 0)
      return LOG_RTERR (pctxt, RTERR_BADVALUE);

   /* Verify 1st two chars of string are "0x" or "0b" */

   len = strlen (pvalue);
   if ((len == 1 && pvalue[0] == '0') ||
      (len > 2 && pvalue[0] == '0' &&
      (pvalue[1] == 'x' || pvalue[1] == 'b')))
   {
      /* do fast encoding for radices 2 and 16 */

      OSOCTET ub, val = 0, lastVal = 0, maxDig;
      size_t i, shift = 0, shiftShift, perByte, end = 2, numDig;
      OSBOOL positive = FALSE;

      if (len > 2) {

         /* set some variables according to radix */

         if (pvalue[1] == 'b') { /* binary */
            shiftShift = 1;
            perByte = 8;
            maxDig = 0x1;
         }
         else {                    /* hexadecimal */
            shiftShift = 4;
            perByte = 2;
            maxDig = 0xF;
         }

         /* determine is the number positive or negative */

         if (((len - 2) & (perByte - 1)) != 0)
            positive = TRUE;
         else {
            HEXCHARTONIBBLE (pvalue[2], ub);
            if (!(ub & (OSINTCONST(1) << (shiftShift - 1))))
               positive = TRUE;
         }

         /* skip all leading zeros in positive number */

         if (positive) {
            for (; end < len && pvalue[end] == '0'; end++)
               ;
         }
         else { /* negative case */

            /* skip all 0xFFs (or '11111111') */

            while (end < len) {
               for (i = 0; i < perByte; i++) {
                  HEXCHARTONIBBLE (pvalue[end + i], ub);
                  if (ub != maxDig)
                     break;
               }
               if (i != perByte)
                  break;
               end += i;
            }
         }

         /* perform encoding of significant part */

         for (i = len - 1, aal = 0, numDig = 0; i >= end; i--) {
            HEXCHARTONIBBLE (pvalue[i], ub);
            if (ub > maxDig)
               return LOG_RTERR (pctxt, RTERR_BADVALUE);

            val |= (ub << shift);
            shift += shiftShift;

            numDig++;
            if ((numDig & (perByte - 1)) == 0) {
               if (pctxt->buffer.byteIndex == 0)
                  if ((stat = xe_expandBuffer(pctxt, 1)) != 0)
                     return LOG_RTERR (pctxt, stat);

               pctxt->buffer.byteIndex--;

               ASN1BUFCUR(pctxt) = val;
               lastVal = val;
               val = 0;
               shift = 0;
               aal++;
            }
         }
      }
      else { /* special zero case */
         positive = TRUE;
         numDig = aal = 0;
         perByte = 1;
      }

      /* if the last patial value should be stored */

      if ((numDig & (perByte - 1)) != 0) {
         if (pctxt->buffer.byteIndex == 0)
            if ((stat = xe_expandBuffer(pctxt, 1)) != 0)
               return LOG_RTERR (pctxt, stat);
         pctxt->buffer.byteIndex--;

         ASN1BUFCUR(pctxt) = val;
         aal++;
      }
      else {

         /* encode prefix (0 or 0xFF) if necessary */

         do {
            if (positive && (numDig == 0 || (lastVal & 0x80))) {
               val = 0;
            }
            else if (!positive && (numDig == 0 || !(lastVal & 0x80))) {
               val = 0xFF;
            }
            else break;

            if (pctxt->buffer.byteIndex == 0)
               if ((stat = xe_expandBuffer(pctxt, 1)) != 0)
                  return LOG_RTERR (pctxt, stat);
            pctxt->buffer.byteIndex--;

            ASN1BUFCUR(pctxt) = val;
            aal++;
         } while (0);
      }
   }
   else {
      rtxErrAddStrParm (pctxt, "big integer in base 10 format");
      return LOG_RTERR (pctxt, RTERR_NOTSUPP);
   }

   if (tagging == ASN1EXPL)
      aal = xe_tag_len (pctxt, TM_UNIV|TM_PRIM|ASN_ID_INT, aal);

   return (int)(aal);
}

int xe_bitstr
(OSCTXT* pctxt, const OSOCTET* pvalue, OSUINT32 numbits,
 ASN1TagType tagging)
{
   register int	aal, ll, numbyts = numbits/8;
   OSOCTET	b;

   if (0 == numbits) {
      /* special case: to be compliant with DER, must encode 1 zero byte */
      b = 0;
      aal = xe_memcpy (pctxt, &b, 1);
   }
   else {
      if (pvalue == NULL) return (0);

      if ((b = (OSOCTET) (numbits % 8)) != 0) {
         b = (OSOCTET) (8 - b);
         numbyts++;
      }

      aal = xe_memcpy (pctxt, pvalue, numbyts);

      if (aal > 0)
         aal  = ((ll = xe_memcpy (pctxt, &b, 1)) > 0) ? aal + ll : ll;
   }

   if (tagging == ASN1EXPL && aal > 0)
      aal = xe_tag_len (pctxt, TM_UNIV|TM_PRIM|ASN_ID_BITSTR, aal);

   return (aal);
}

int xe_OpenType
(OSCTXT* pctxt, const OSOCTET* pvalue, OSUINT32 numocts)
{
   OSCTXT lctxt;
   int aal = 0, already_encoded, len, stat;
   ASN1TAG tag;

   if (numocts > 0) {
      if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

      if ((stat = rtInitContext (&lctxt)) != 0)
         return LOG_RTERR(pctxt, stat);

      stat = xd_setp (&lctxt, pvalue, numocts, &tag, &len);
      rtFreeContext (&lctxt);

      if (stat != 0) return LOG_RTERR (pctxt, stat);

      if (tag == 0 && len == 0) /* EOC is not allowed as OpenType */
         return LOG_RTERR(pctxt, RTERR_BADVALUE);

      /* An any field is considered to be already encoded if the given	*/
      /* pointer is equal to the current encode pointer.  This will be	*/
      /* the case when an extra field is being prepended onto an	*/
      /* existing ASN.1 message (for example, the X.410 PDU)..		*/

      already_encoded = (pvalue == OSRTBUFPTR(pctxt));

      /* For an indefinite length message, need to get the actual 	*/
      /* length by parsing tags until the end of the message is 	*/
      /* reached..							*/

      if (len == ASN_K_INDEFLEN) {  /* indefinite length message */
         return RTERR_NOTSUPP;
      }

      /* If not already copied, copy message component to encode buffer	*/

      aal = (already_encoded) ?
	 len : xe_memcpy (pctxt, pvalue, len);
   }
   else {
      /* Open type must have > 0 octets */
      return LOG_RTERR (pctxt, RTERR_BADVALUE);
   }

   return (aal);
}

int xe_OpenTypeExt (OSCTXT* pctxt, OSRTDList* pElemList)
{
   OSRTDListNode* pnode;
   ASN1OpenType* pOpenType;
   int aal = 0, ll;

   if (0 != pElemList) {
      pnode = pElemList->tail;
      while (0 != pnode) {
         if (0 != pnode->data) {
            pOpenType = (ASN1OpenType*)pnode->data;
            ll = xe_OpenType (pctxt, pOpenType->data, pOpenType->numocts);
            if (ll >= 0) aal += ll; else return LOG_RTERR (pctxt, ll);
         }
         pnode = pnode->prev;
      }
   }

   return (aal);
}

int xe_boolean
(OSCTXT* pctxt, OSBOOL *pvalue, ASN1TagType tagging)
{
   OSOCTET ub;
   int aal = 1;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   ub = (*pvalue == 0) ? 0 : 0xff;

   XE_CHKBUF (pctxt, 1);
   XE_PUT1 (pctxt, ub);

   if (tagging == ASN1EXPL) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_BOOL, 1L);
      aal += 2;
   }

   return aal;
}

int xe_charstr (OSCTXT* pctxt, const char* pvalue,
                   ASN1TagType tagging, ASN1TAG tag)
{
   int ll, ll0 = 0;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   ll = xe_octstr
      (pctxt, (OSOCTET*)pvalue, (int)strlen(pvalue), ASN1IMPL);
   if (ll >= 0) ll0 += ll; else return (ll);

   if (tagging == ASN1EXPL)
      ll0 = xe_tag_len (pctxt, tag, ll0);

   return (ll0);
}

int xe_16BitCharStr
(OSCTXT* pctxt, Asn116BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag)
{
   OSOCTET lbuf[2];
   int i, ll, ll0 = 0;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   for (i = pvalue->nchars - 1; i >= 0; i--) {
      lbuf[0] = (OSOCTET) (pvalue->data[i] / 256);
      lbuf[1] = (OSOCTET) (pvalue->data[i] % 256);

      ll = xe_memcpy (pctxt, lbuf, 2);
      if (ll >= 0) ll0 += ll; else return (ll);
   }

   if (tagging == ASN1EXPL)
      ll0 = xe_tag_len (pctxt, tag, ll0);

   return (ll0);
}

int xe_32BitCharStr
(OSCTXT* pctxt, Asn132BitCharString* pvalue,
 ASN1TagType tagging, ASN1TAG tag)
{
   OSOCTET lbuf[4];
   int i, ll, ll0 = 0;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   for (i = pvalue->nchars - 1; i >= 0; i--) {
      OS32BITCHAR sym = pvalue->data[i];
      lbuf[0] = (OSOCTET) (sym / 256 / 256 / 256);
      lbuf[1] = (OSOCTET) (sym / 256 / 256 % 256);
      lbuf[2] = (OSOCTET) (sym / 256 % 256 );
      lbuf[3] = (OSOCTET) (sym % 256);

      ll = xe_memcpy (pctxt, lbuf, 4);
      if (ll >= 0) ll0 += ll; else return (ll);
   }

   if (tagging == ASN1EXPL)
      ll0 = xe_tag_len (pctxt, tag, ll0);

   return (ll0);
}

static int xe_identifier (OSCTXT *pctxt, unsigned ident)
{
   register int aal = 0, ll;
   OSOCTET b, first_byte = TRUE;

   do
   {
      b = (OSOCTET) ((first_byte) ?
             (ident % 128) : ((ident % 128) | 0x80));

      aal = ((ll = xe_memcpy (pctxt, &b, 1)) >= 0) ? aal + ll : ll;
      if (aal < 0) break;

      ident /= 128; first_byte = FALSE;
   }
   while (ident > 0);

   return (aal);
}

static int xe_tag (OSCTXT *pctxt, ASN1TAG tag)
{
   int          aal = 0, ll, rshift;
   ASN1TAG      ltag;
   OSUINT32	id_code = tag & TM_IDCODE;
   OSOCTET	b, class_form;

   /* Split tag into class/form and ID code components */

   rshift = (sizeof(ltag) * 8) - 3;
   ltag = tag >> rshift;
   class_form = (OSOCTET)(ltag << 5);

   /* Encode components */

   if (id_code < 31) {
      b = (OSOCTET) (class_form + id_code);
      aal = ((ll = xe_memcpy (pctxt, &b, 1)) >= 0) ? aal + ll : ll;
   }
   else {
      aal = ((ll = xe_identifier (pctxt, id_code)) >= 0) ? aal + ll : ll;
      if (aal > 0) {
         b = (OSOCTET) (class_form | TM_B_IDCODE);
         aal = ((ll = xe_memcpy (pctxt, &b, 1)) >= 0) ? aal + ll : ll;
      }
   }

   return aal;
}

int xe_len (OSCTXT *pctxt, int length)
{
   int		aal, ll, xl;
   OSOCTET	i, b;

   aal = xl = 0;
   i = 0;

   if (length >= 0)
   {
      xl  = (length > 127);
      do {
         b   = (OSOCTET) (length % 256);	/* shave off LSB of length */
         aal = ((ll = xe_memcpy (pctxt, &b, 1)) >= 0) ? aal + ll : ll;
         length /= 256; i++;
      }
      while (length > 0 && aal >= 0);
   }
   else
      xl = (length == ASN_K_INDEFLEN);  /* indefinite length */

   if (xl && aal >= 0)
   {
      i |= 0x80;
      aal = ((ll = xe_memcpy (pctxt, &i, 1)) >= 0) ? aal + ll : ll;
   }

   return (aal);
}

int xe_tag_len (OSCTXT *pctxt, ASN1TAG tag, int length)
{
   int aal, ll;

   if (length < 0 && length != ASN_K_INDEFLEN) return (length);

   aal = ((ll = xe_len (pctxt, length)) >= 0) ?
      (length == ASN_K_INDEFLEN) ? ll : length + ll : ll;

   if (aal > 0) {
      ll = xe_tag (pctxt, tag);
      aal = (ll >= 0) ? aal + ll : ll;
   }

   return (aal);
}

int xe_derCanonicalSort (OSCTXT* pctxt, OSRTSList* pList)
{
   OSBOOL swap = TRUE, swapped = FALSE;
   OSRTSListNode *pNode1, *pNode2;
   Asn1BufLocDescr *pComponent1, *pComponent2;
   OSOCTET *ptmpbuf, *pelem1, *pelem2;
   OSOCTET *pEOB = pctxt->buffer.data + (pctxt->buffer.size - 1);
   int cmplen, offset, result, totalSize;
   void* ptmp;

   /* Sort elements */

   while (swap) {
      swap = FALSE;
      pNode1 = 0;
      pNode2 = pList->head;

      while (pNode2) {
         if (pNode1) {
            pComponent1 = (Asn1BufLocDescr*) pNode1->data;
            pComponent2 = (Asn1BufLocDescr*) pNode2->data;

            cmplen = ASN1MIN (pComponent1->numocts, pComponent2->numocts);

            /* Calculate encode buffer location pointers */
            pelem1 = pEOB - pComponent1->offset;
            pelem2 = pEOB - pComponent2->offset;

            result = memcmp (pelem1, pelem2, cmplen);
            if (result < 0 || (result == 0 &&
                               pComponent1->numocts < pComponent2->numocts)) {
               /* swap the data, not node */
               ptmp = pNode1->data;
               pNode1->data = pNode2->data;
               pNode2->data = ptmp;
               swap = swapped = TRUE;
            }
         }
         pNode1 = pNode2;
         pNode2 = pNode2->next;
      }
   }

   /* Compute total size of all sorted elements */

   pNode1 = pList->head;
   totalSize = 0;

   while (pNode1) {
      pComponent1 = (Asn1BufLocDescr*) pNode1->data;
      totalSize += pComponent1->numocts;
      pNode1 = pNode1->next;
   }

   /* If at least one element was swapped.. */

   if (swapped) {

      /* Copy sorted elements to temp buffer */

      ptmpbuf = (OSOCTET*) rtxMemAlloc (pctxt, totalSize);
      if (ptmpbuf != 0) {
         pNode1 = pList->head;
         offset = totalSize;

         while (pNode1) {
            pComponent1 = (Asn1BufLocDescr*) pNode1->data;
            pelem1 = pEOB - pComponent1->offset;

            offset -= pComponent1->numocts;
            memcpy (&ptmpbuf[offset], pelem1, pComponent1->numocts);

            rtxMemFreePtr (pctxt, pComponent1);
            pNode2 = pNode1;
            pNode1 = pNode2->next;
            rtxMemFreePtr (pctxt, pNode2);
         }

         /* Now copy the temp buffer back into the encode buffer */

         memcpy (OSRTBUFPTR(pctxt), ptmpbuf, totalSize);
         rtxMemFreePtr (pctxt, ptmpbuf);
      }
      else
         return LOG_RTERR (pctxt, RTERR_NOMEM);
   }
   else {
      /* Clean up memory */
      pNode1 = pList->head;
      while (pNode1) {
         pComponent1 = (Asn1BufLocDescr*) pNode1->data;
         rtxMemFreePtr (pctxt, pComponent1);
         pNode2 = pNode1;
         pNode1 = pNode2->next;
         rtxMemFreePtr (pctxt, pNode2);
      }
   }

   return totalSize;
}

int xe_enum (OSCTXT* pctxt, OSINT32 *pvalue, ASN1TagType tagging)
{
   int aal;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   aal = xe_integer (pctxt, pvalue, ASN1IMPL);

   if (tagging == ASN1EXPL && aal >= 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_ENUM, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_enumInt8 (OSCTXT* pctxt, OSINT8 *object_p, ASN1TagType tagging)
{
   if (object_p) {
      OSINT32 tmp = *object_p;
      return xe_enum (pctxt, &tmp, tagging);
   }
   return LOG_RTERR(pctxt, RTERR_BADVALUE);
}

int xe_enumInt16 (OSCTXT* pctxt, OSINT16 *object_p, ASN1TagType tagging)
{
   if (object_p) {
      OSINT32 tmp = *object_p;
      return xe_enum (pctxt, &tmp, tagging);
   }
   return LOG_RTERR(pctxt, RTERR_BADVALUE);
}

int xe_enumUnsigned (OSCTXT* pctxt, OSUINT32 *object_p, ASN1TagType tagging)
{
   int aal;

   if (0 == object_p) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   aal = xe_uint (pctxt, object_p, ASN1IMPL);

   if (tagging == ASN1EXPL && aal >= 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_ENUM, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_enumUInt8 (OSCTXT* pctxt, OSUINT8 *object_p, ASN1TagType tagging)
{
   if (object_p) {
      OSUINT32 tmp = *object_p;
      return xe_enumUnsigned (pctxt, &tmp, tagging);
   }
   return LOG_RTERR(pctxt, RTERR_BADVALUE);
}

int xe_enumUInt16 (OSCTXT* pctxt, OSUINT16 *object_p, ASN1TagType tagging)
{
   if (object_p) {
      OSUINT32 tmp = *object_p;
      return xe_enumUnsigned (pctxt, &tmp, tagging);
   }
   return LOG_RTERR(pctxt, RTERR_BADVALUE);
}

void xe_free (OSCTXT* pctxt)
{
   if (pctxt->buffer.dynamic && pctxt->buffer.data) {
      rtxMemFreePtr (pctxt, pctxt->buffer.data);
      pctxt->buffer.data = 0;
      pctxt->buffer.dynamic = FALSE;
   }
}

int xe_int16 (OSCTXT* pctxt, OSINT16 *pvalue, ASN1TagType tagging)
{
   register OSOCTET lb;
   register int aal = 1;
   register OSINT32 temp;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *pvalue;

   lb = (OSOCTET) (temp % 256);
   XE_SAFEPUT1 (pctxt, lb);

   temp /= 256;
   if (temp < 0 && lb != 0) temp--; /* two's complement adjustment */

   if (*pvalue > 0 && temp == 0) {
      if (lb & 0x80) {
         XE_SAFEPUT1 (pctxt, 0L);
         aal++;
      }
   }
   else if (*pvalue < 0 && temp == -1) {
      if (!(lb & 0x80)) {
         XE_SAFEPUT1 (pctxt, 0xFF);
         aal++;
      }
   }
   else if (temp != 0) {
      XE_SAFEPUT1 (pctxt, (OSOCTET)temp);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_int8 (OSCTXT* pctxt, OSINT8 *pvalue, ASN1TagType tagging)
{
   int aal = 1;
   OSOCTET lb;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   lb = (OSOCTET) *pvalue;
   XE_SAFEPUT1 (pctxt, lb);

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, 1L);
      aal += 2;
   }

   return (aal);
}

int xe_integer (OSCTXT* pctxt, OSINT32 *pvalue, ASN1TagType tagging)
{
   OSOCTET lb;
   int  aal = 0;
   OSINT32 temp;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *pvalue;

   do {
      lb = (OSOCTET) (temp % 256);
      temp /= 256;
      if (temp < 0 && lb != 0) temp--; /* two's complement adjustment */
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   } while (temp != 0 && temp != -1 && aal >= 0);

   /* If the value is positive and bit 8 of the leading byte is set,    */
   /* copy a zero byte to the contents to signal a positive number..    */

   if (*pvalue > 0 && ((lb & 0x80) == 0x80))
   {
      lb  = 0;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   /* If the value is negative and bit 8 of the leading byte is clear,  */
   /* copy a -1 byte (0xFF) to the contents to signal a negative        */
   /* number..                                                          */

   else if (*pvalue < 0 && ((lb & 0x80) == 0))
   {
      lb  = 0xFF;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_int64 (OSCTXT* pctxt, OSINT64 *object_p, ASN1TagType tagging)
{
   OSOCTET lb;
   register int aal = 0;
   OSINT64 temp;

   if (0 == object_p) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *object_p;

   do {
      lb = (OSOCTET) (temp % 256);
      temp /= 256;
      if (temp < 0 && lb != 0) temp--; /* two's complement adjustment */
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   } while (temp != 0 && temp != -1 && aal >= 0);

   /* If the value is positive and bit 8 of the leading byte is set,	*/
   /* copy a zero byte to the contents to signal a positive number..	*/

   if (*object_p > 0 && ((lb & 0x80) == 0x80))
   {
      lb  = 0;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   /* If the value is negative and bit 8 of the leading byte is clear,	*/
   /* copy a -1 byte (0xFF) to the contents to signal a negative 	*/
   /* number..								*/

   else if (*object_p < 0 && ((lb & 0x80) == 0))
   {
      lb  = 0xFF;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_uint64 (OSCTXT* pctxt, OSUINT64 *object_p, ASN1TagType tagging)
{
   OSOCTET  lb;
   int        aal = 0;
   OSUINT64 temp;

   if (0 == object_p) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *object_p;

   do {
      lb = (OSOCTET) (temp % 256);
      temp /= 256;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   } while (temp != 0);

   /* If bit 8 of the leading byte is set, copy a zero byte to the      */
   /* contents to signal a positive number..                            */

   if (lb & 0x80) {
      lb = 0;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

static int xe_expandBuffer (OSCTXT *pctxt, size_t length)
{
   OSOCTET* newBuf_p, *msg_p;
   size_t usedBytes;

   if (pctxt->buffer.dynamic)
   {
      /* If dynamic encoding is enabled, expand the current buffer to	*/
      /* allow encoding to continue.                                    */

      size_t extent = ASN1MAX (ASN_K_ENCBUFSIZ, length);
      size_t newSize = pctxt->buffer.size + extent;

      newBuf_p = (OSOCTET*) rtxMemAlloc (pctxt, newSize);
      if (!newBuf_p) return LOG_RTERR (pctxt, RTERR_NOMEM);

      /* Set message pointer position in new buffer and copy existing	*/
      /* message to it..						*/

      usedBytes = pctxt->buffer.size - pctxt->buffer.byteIndex;
      msg_p = (newBuf_p + newSize) - usedBytes;
      memcpy (msg_p, OSRTBUFPTR(pctxt), usedBytes);

      /* Free old buffer and set context parameters */

      rtxMemFreePtr (pctxt, pctxt->buffer.data);
      pctxt->buffer.data = newBuf_p;
      pctxt->buffer.size = newSize;
      pctxt->buffer.byteIndex += extent;

      return (0);
   }
   else
      return LOG_RTERR (pctxt, RTERR_BUFOVFLW);
}

int xe_memcpy
(OSCTXT *pctxt, const OSOCTET* pvalue, size_t length)
{
   if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_BADVALUE);

   if (length > pctxt->buffer.byteIndex) {
      int stat = xe_expandBuffer (pctxt, length);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }

   if (length == 1) {
      pctxt->buffer.byteIndex --;
      *OSRTBUFPTR(pctxt) = *pvalue;
   }
   else {
      pctxt->buffer.byteIndex -= length;
      memcpy (OSRTBUFPTR(pctxt), pvalue, length);
   }
   return (int)length;
}

void xe_getBufLocDescr (OSCTXT *pctxt, OSUINT32 length,
                        Asn1BufLocDescr* pDescr)
{
   size_t startIndex = pctxt->buffer.size - 1;
   pDescr->offset = (OSUINT32)(startIndex - pctxt->buffer.byteIndex);
   pDescr->numocts = length;
}

int xe_null (OSCTXT* pctxt, ASN1TagType tagging)
{
   if (tagging == ASN1EXPL) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_NULL, 0L);
      return 2;
   }
   else return 0;
}

int xe_objid (OSCTXT* pctxt, ASN1OBJID *pvalue, ASN1TagType tagging)
{
   unsigned	subid;
   register int	aal, ll, i;

   /* Validate given object ID by applying ASN.1 rules */

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_INVOBJID);
   if (pvalue->numids < 2) return LOG_RTERR(pctxt, RTERR_INVOBJID);
   if (pvalue->subid[0] > 2) return LOG_RTERR(pctxt, RTERR_INVOBJID);
   if (pvalue->subid[0] != 2 && pvalue->subid[1] > 39)
      return LOG_RTERR(pctxt, RTERR_INVOBJID);

   /* Passed checks, encode object identifier */

   for (aal = 0, i = pvalue->numids - 1; i >= 1 && aal >= 0; i--)
   {
      subid = (i == 1) ?
	 (pvalue->subid[0] * 40) + pvalue->subid[1] : pvalue->subid[i];

      aal = ((ll = xe_identifier (pctxt, subid)) >= 0) ? aal + ll : ll;
   }

   if (tagging == ASN1EXPL && aal > 0)
      aal = xe_tag_len (pctxt, TM_UNIV|TM_PRIM|ASN_ID_OBJID, aal);

   return (aal);
}

int encodeRelOID (OSCTXT* pctxt, ASN1OBJID *pvalue, ASN1TagType tagging)
{
   register int	aal, ll, i;

   /* Validate given object ID by applying ASN.1 rules */

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_INVOBJID);
   if (pvalue->numids < 1) return LOG_RTERR(pctxt, RTERR_INVOBJID);

   /* Passed checks, encode object identifier */

   for (aal = 0, i = pvalue->numids - 1; i >= 0 && aal >= 0; i--)
   {
      aal = ((ll = xe_identifier (pctxt, pvalue->subid[i])) >= 0) ? aal + ll : ll;
   }

   if (tagging == ASN1EXPL && aal > 0)
      aal = xe_tag_len (pctxt, TM_UNIV|TM_PRIM|ASN_ID_RELOID, aal);

   return (aal);
}

int xe_octstr
(OSCTXT* pctxt, const OSOCTET* pvalue,
 OSUINT32 numocts, ASN1TagType tagging)
{
   int aal = (0 == pvalue) ? 0 : xe_memcpy (pctxt, pvalue, numocts);

   if (tagging == ASN1EXPL && aal >= 0)
      aal = xe_tag_len (pctxt, TM_UNIV|TM_PRIM|ASN_ID_OCTSTR, aal);

   return (aal);
}

int xe_setp (OSCTXT* pctxt, OSOCTET *buf_p, int bufsiz)
{
   int stat = rtxInitContextBuffer (pctxt, buf_p, bufsiz);

   if (stat == 0)
      pctxt->buffer.byteIndex = pctxt->buffer.size - 1;

   return (stat != 0) ? LOG_RTERR (pctxt, stat) : 0;
}

OSOCTET* xe_getp (OSCTXT* pctxt)
{
   return OSRTBUFPTR (pctxt);
}

int xe_uint16 (OSCTXT* pctxt, OSUINT16 *pvalue, ASN1TagType tagging)
{
   register OSOCTET lb;
   register int aal = 1;
   register OSUINT32 temp;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *pvalue;

   lb = (OSOCTET) (temp % 256);
   XE_SAFEPUT1 (pctxt, lb);

   temp /= 256;
   if (temp != 0) {
      lb = (OSOCTET) temp;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   /* If bit 8 of the leading byte is set, copy a zero byte to the      */
   /* contents to signal a positive number..                            */

   if (lb & 0x80) {
      XE_SAFEPUT1 (pctxt, 0L);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_uint8 (OSCTXT* pctxt, OSUINT8 *pvalue, ASN1TagType tagging)
{
   int aal = 1;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   XE_SAFEPUT1 (pctxt, *pvalue);

   /* add leading zero byte if necessary */

   if (*pvalue & 0x80) {
      XE_SAFEPUT1 (pctxt, 0L);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

int xe_uint (OSCTXT* pctxt, OSUINT32 *pvalue, ASN1TagType tagging)
{
   OSOCTET lb;
   int	aal = 0;
   OSUINT32 temp;

   if (0 == pvalue) return LOG_RTERR(pctxt, RTERR_BADVALUE);

   temp = *pvalue;

   do {
      lb = (OSOCTET) (temp % 256);
      temp /= 256;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   } while (temp != 0 && aal >= 0);

   /* If bit 8 of the leading byte is set, copy a zero byte to the      */
   /* contents to signal a positive number..                            */

   if ((lb & 0x80) == 0x80) {
      lb  = 0;
      XE_SAFEPUT1 (pctxt, lb);
      aal++;
   }

   if (tagging == ASN1EXPL && aal > 0) {
      XE_CHKBUF (pctxt, 2);
      XE_PUT2 (pctxt, ASN_ID_INT, (OSOCTET)aal);
      aal += 2;
   }

   return (aal);
}

static OSBOOL testBit
(const OSOCTET* pBits, OSSIZE numbits, OSSIZE bitIndex)
{
   return (OSBOOL)((bitIndex >= numbits) ? FALSE :
      (pBits [bitIndex / 8] & (0x80 >> bitIndex % 8)));
}

int derEncBitString
(OSCTXT* pctxt, const OSOCTET* pvalue, OSUINT32 numbits, ASN1TagType tagging)
{
   /* Adjust bit count to remove trailing zero bits */
   while (numbits > 0) {
      if (!testBit (pvalue, numbits, numbits - 1))
         numbits--;
      else
         break;
   }
   return xe_bitstr (pctxt, pvalue, numbits, tagging);
}
