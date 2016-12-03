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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asn1ber.h"

static int errTag1NotMatched (OSCTXT *pctxt, OSOCTET expectedTag);
static int xd_MovePastEOC (OSCTXT* pctxt);
static void saveBufferState (OSCTXT* pCtxt, ASN1BUFSAVE* pSavedInfo);
static void restoreBufferState (OSCTXT* pCtxt, ASN1BUFSAVE* pSavedInfo);

int xd_bigint
(OSCTXT *pctxt, const char** pvalue, ASN1TagType tagging, int length)
{
   int bufsiz, i, ub, off = 2;
   char* tmpstr;
   OSBOOL leadingZeros = FALSE;

   if (tagging == ASN1EXPL) {
      int status;

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }

      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   /* check is the first byte zero */

   if (length > 0 && ASN1BUFCUR(pctxt) == 0) {
      leadingZeros = TRUE;
      XD_BUMPIDX(pctxt, 1);         /* skip.., */
      length--;
   }

   /* skip all remained leading zeros */

   while (length > 0 && ASN1BUFCUR(pctxt) == 0) {
      XD_BUMPIDX(pctxt, 1);
      length--;
   }

   bufsiz = (length * 2) + 4;
   tmpstr = (char*) rtxMemAlloc (pctxt, bufsiz);
   if (tmpstr != 0) {
      tmpstr[0] = '0';
      tmpstr[1] = 'x';

      if (length == 0 || (leadingZeros && (ASN1BUFCUR(pctxt) & 0x80)))
         tmpstr [off++] = '0';

      for (i = 0; i < length; i++) {
         OSOCTET oct = ASN1BUFCUR(pctxt);

         ub = (oct >> 4) & 0x0f;
         NIBBLETOHEXCHAR (ub, tmpstr[off++]);

         ub = (oct & 0x0f);
         NIBBLETOHEXCHAR (ub, tmpstr[off++]);

         XD_BUMPIDX(pctxt, 1);
      }

      tmpstr[off] = '\0';
      *pvalue = tmpstr;
   }
   else
      return LOG_RTERR (pctxt, RTERR_NOMEM);

   return 0;
}

int xd_bitstr
(OSCTXT* pctxt, const OSOCTET** pvalue2, OSUINT32* numbits_p,
 ASN1TagType tagging, int length)
{
   OSOCTET* pbitstr = 0;
   int stat = 0;
   int size, ll = 0;

   if (tagging == ASN1EXPL) {
      if ((stat = xd_match1 (pctxt, ASN_ID_BITSTR, &length)) < 0)
         return errTag1NotMatched (pctxt, ASN_ID_BITSTR);
   }

   /* For an indefinite length message, need to get the actual 	*/
   /* length by parsing tags until the end of the message is 	*/
   /* reached..							*/
   if (length == ASN_K_INDEFLEN) {  /* indefinite length message */
      return RTERR_NOTSUPP;
   }
   else ll = size = length;

   if (ll > 1) {
      pbitstr = (OSOCTET*) rtxMemAlloc (pctxt, ll - 1);
      if (0 == pbitstr)
         return LOG_RTERR (pctxt, RTERR_NOMEM);
      else {
         *numbits_p = (ll - 1) * 8; /* set max # bits */
         stat = xd_bitstr_s (pctxt, pbitstr, numbits_p, ASN1IMPL, size);
      }
   }
   else {
      *numbits_p = 0;
      if (ll == 1) {
         if (ASN1BUFCUR (pctxt) != 0)
            stat = RTERR_INVLEN;
         else
            XD_BUMPIDX (pctxt, 1);
      }
   }
   if (stat != 0) {
      rtxMemFreePtr (pctxt, pbitstr);
      return LOG_RTERR (pctxt, stat);
   }
   *pvalue2 = pbitstr;

   return 0;
}

int xd_bitstr_s
(OSCTXT* pctxt, OSOCTET* pvalue, OSUINT32* numbits_p,
 ASN1TagType tagging, int length)
{
   int bufsiz = (*numbits_p - 1) / 8 + 1;
   int stat = 0;
   OSOCTET b = 8;

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_BITSTR)) {
         return errTag1NotMatched (pctxt, ASN_ID_BITSTR);
      }
      stat = XD_LEN (pctxt, &length);
      if (stat != 0) LOG_RTERR (pctxt, stat);
   }

   if (length > 0) {
      if ((stat = XD_MEMCPY1 (pctxt, &b)) == 0) {
         length--;  /* adjust by 1; first byte is unused bit count */
         if (length == 0) {
            if (0 == b)
               *numbits_p = 0;
            else
               stat = RTERR_INVLEN;
         }
         if (stat == 0) {
            if (b <= 7) { /* initial octet should be 0..7 */
               if (length <= bufsiz) {
                  stat = xd_memcpy (pctxt, pvalue, length);
                  *numbits_p = (length * 8) - b;
               }
               else
                  stat = RTERR_STROVFLW;
            }
            else
               stat = RTERR_BADVALUE;
         }
      }
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }
   else if (length == 0)
      *numbits_p = 0;
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   return 0;
}

int xd_boolean
(OSCTXT *pctxt, OSBOOL *pvalue, ASN1TagType tagging, int length)
{
   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_BOOL)) {
         return errTag1NotMatched (pctxt, ASN_ID_BOOL);
      }
      length = XD_FETCH1 (pctxt);
   }

   if (length == 1)
      *pvalue = XD_FETCH1 (pctxt);
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   return 0;
}

int xd_charstr
(OSCTXT* pctxt, const char** pvalue,
 ASN1TagType tagging, ASN1TAG tag, int length)
{
   int stat = 0;
   int size, ll = 0;

   if (tagging == ASN1EXPL) {
      if ((stat = xd_match1 (pctxt, ASN1TAG2BYTE(tag), &length)) < 0)
         return errTag1NotMatched (pctxt, ASN1TAG2BYTE(tag));
   }

   /* For an indefinite length message, need to get the actual 	*/
   /* length by parsing tags until the end of the message is 	*/
   /* reached..							*/
   if (length == ASN_K_INDEFLEN) {  /* indefinite length message */
      return RTERR_NOTSUPP;
   }
   else ll = size = length;

   if (ll > 0) {
      char* tmpstr = (char*) rtxMemAlloc (pctxt, ll + 1);
      if (tmpstr) {
         ll = size;
         stat = xd_octstr_s (pctxt, (OSOCTET*)tmpstr,
                             (OSUINT32*) &ll, ASN1IMPL, size);
         if (stat != 0) {
            rtxMemFreePtr (pctxt, tmpstr);
            return LOG_RTERR (pctxt, stat);
         }
         tmpstr[ll] = '\0';
         *pvalue = tmpstr;
      }
      else
         return LOG_RTERR (pctxt, RTERR_NOMEM);
   }
   else {
      char* tmpstr = (char*) rtxMemAlloc (pctxt, 1);
      if (tmpstr) {
         tmpstr[0] = '\0';
         *pvalue = tmpstr;
      }
      else
         return LOG_RTERR (pctxt, RTERR_NOMEM);
   }

   return 0;
}

int xd_chkend (OSCTXT* pctxt, ASN1CCB* ccb_p)
{
   int		len;
   ASN1TAG	tag;
   register int	eoc, stat, usedBytes;

   if (ccb_p->len == ASN_K_INDEFLEN)
   {
      stat = xd_tag_len (pctxt, &tag, &len, 0);

      if (stat == RTERR_ENDOFBUF) {
         rtxErrReset (pctxt);
         eoc = TRUE;
      }
      else if (tag == 0 && len == 0)
      {
         /* Advance cursor past EOC only if enveloping context  */
         /* parsed the indefinite length marker..               */
         /*
         if (ccb_p->tagging == ASN1EXPL) {
            xd_tag_len (pctxt, &tag, &len, XM_ADVANCE);
         }
         */
         eoc = TRUE;
      }
      else
         eoc = FALSE;
   }
   else
   {
      usedBytes = (int)(OSRTBUFPTR(pctxt) - ccb_p->ptr);
      eoc = ((usedBytes >= ccb_p->len) ||
         (pctxt->buffer.byteIndex >= pctxt->buffer.size));
   }

   return (eoc);
}

int xd_count (OSCTXT *pctxt, int length, int *count_p)
{
   ASN1BUFFER	saved_ctxt;
   ASN1CCB	ccb;
   int		stat;

   /* Save current decoding context */

   memcpy (&saved_ctxt, &pctxt->buffer, sizeof(ASN1BUFFER));

   /* Count elements */

   *count_p = 0;

   ccb.len = length;
   ccb.ptr = OSRTBUFPTR (pctxt);

   while (!XD_CHKEND (pctxt, &ccb)) {
      if ((stat = xd_NextElement (pctxt)) == 0) {
         (*count_p)++;
      }
      else
         return LOG_RTERR (pctxt, stat);
   }

   /* Restore original message pointer and length */

   memcpy (&pctxt->buffer, &saved_ctxt, sizeof(ASN1BUFFER));

   return 0;
}

int xd_enum
(OSCTXT *pctxt, OSINT32 *pvalue, ASN1TagType tagging, int length)
{
   register int status;

   if (tagging == ASN1EXPL) {
      if (ASN1BUFCUR (pctxt) != ASN_ID_ENUM) {
         return errTag1NotMatched (pctxt, ASN_ID_ENUM);
      }
      else
         XD_BUMPIDX (pctxt, 1);

      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   status = xd_integer (pctxt, pvalue, ASN1IMPL, length);
   if (status != 0) return LOG_RTERR (pctxt, status);

   return 0;
}

int xd_enumUnsigned
(OSCTXT *pctxt, OSUINT32 *object_p, ASN1TagType tagging, int length)
{
   register int status;

   if (tagging == ASN1EXPL) {
      if (ASN1BUFCUR (pctxt) != ASN_ID_ENUM) {
         return errTag1NotMatched (pctxt, ASN_ID_ENUM);
      }
      else
         XD_BUMPIDX (pctxt, 1);

      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   status = xd_unsigned (pctxt, object_p, ASN1IMPL, length);
   if (status != 0) return LOG_RTERR (pctxt, status);

   return 0;
}

int xd_int16
(OSCTXT *pctxt, OSINT16 *pvalue, ASN1TagType tagging, int length)
{
   register int status = 0;
   signed char	b;	/* signed   */
   OSOCTET	ub;	/* unsigned */

   if (tagging == ASN1EXPL) {
      if ((status = XD_CHKDEFLEN (pctxt, 2)) != 0) /* tag + len */
         return LOG_RTERR (pctxt, status);

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      length = (int) XD_FETCH1 (pctxt);
   }

   /* Make sure integer will fit in target variable */

   if (length > (int)(sizeof(OSINT16))) {
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   }
   else if (length <= 0) {
      /* note: indef len not allowed */
      return LOG_RTERR (pctxt, RTERR_INVLEN);
   }
   status = XD_CHKDEFLEN (pctxt, length);
   if (status != 0) return LOG_RTERR (pctxt, status);

   /* Copy first byte into a signed char variable and assign it to	*/
   /* object.  This should handle sign extension in the case of a	*/
   /* negative number..                                                 */

   b = XD_FETCH1 (pctxt);

   if (length > 1) {
      ub = XD_FETCH1 (pctxt);
      *pvalue = (((OSINT16)b) * 256) + ub;
   }
   else
      *pvalue = b;

   return 0;
}

int xd_int8
(OSCTXT *pctxt, OSINT8 *pvalue, ASN1TagType tagging, int length)
{
   register int status = 0;

   if (tagging == ASN1EXPL) {
      if ((status = XD_CHKDEFLEN (pctxt, 3)) != 0) /* tag + len + val */
         return LOG_RTERR (pctxt, status);

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      length = (int) XD_FETCH1 (pctxt);
   }

   /* Make sure integer will fit in target variable */

   if (length != sizeof(OSINT8)) { /* note: only 1 byte len is allowed */
      return LOG_RTERR (pctxt, RTERR_INVLEN);
   }

   *pvalue = (OSINT8) XD_FETCH1 (pctxt);

   return 0;
}

int xd_integer
(OSCTXT *pctxt, OSINT32 *pvalue, ASN1TagType tagging, int length)
{
   register int status = 0;
   OSOCTET	ub;	/* unsigned */

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   /* Make sure integer will fit in target variable */

   if (length > (int)(sizeof(OSINT32))) {
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   }
   else if (length > 0) {
      status = XD_CHKDEFLEN (pctxt, length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   /* Copy first byte into a signed char variable and assign it to */
   /* object.  This should handle sign extension in the case of a  */
   /* negative number..                                            */

   if (length > 0) {
      *pvalue = (signed char) XD_FETCH1 (pctxt);
      length--;
   }

   /* Now use unsigned bytes to add in the rest of the integer */

   while (length > 0) {
      ub = XD_FETCH1 (pctxt);
      *pvalue = (*pvalue * 256) + ub;
      length--;
   }

   return 0;
}

int xd_int64 (OSCTXT *pctxt, OSINT64 *object_p,
	      ASN1TagType tagging, int length)
{
   OSOCTET ub; /* unsigned */

   if (tagging == ASN1EXPL) {
      int status;

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   /* Make sure integer will fit in target variable */

   if (length > (int)(sizeof(OSINT64))) {
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   }
   else if (length > 0) {
      int stat = XD_CHKDEFLEN (pctxt, length);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN); /* note: indef len not allowed */

   /* Copy first byte into a signed char variable and assign it to	*/
   /* object.  This should handle sign extension in the case of a	*/
   /* negative number..							*/

   if (length > 0) {
      *object_p = (signed char)XD_FETCH1 (pctxt);;
      length--;
   }

   /* Now use unsigned bytes to add in the rest of the integer..	*/

   while (length > 0) {
      ub = XD_FETCH1 (pctxt);
      *object_p = (*object_p * 256) + ub;
      length--;
   }

   return (0);
}

int xd_len (OSCTXT *pctxt, int *len_p)
{
   OSOCTET b = XD_FETCH1 (pctxt);
   register int	i;

   *len_p = 0;

   if (b > 0x80) {
      i = b & 0x7F;

      if (i > 4)
         return LOG_RTERR (pctxt, RTERR_INVLEN);

      for (*len_p = 0; i > 0; i--) {
         b = XD_FETCH1 (pctxt);
         *len_p = (*len_p * 256) + b;
      }
   }
   else if (b == 0x80) *len_p = ASN_K_INDEFLEN;
   else *len_p = b;

   /* Length is not indefinite length */
   if (*len_p != ASN_K_INDEFLEN) {
      if (*len_p < 0 ||
          *len_p > (int)(pctxt->buffer.size - pctxt->buffer.byteIndex))
         return LOG_RTERR (pctxt, RTERR_INVLEN);
   }

   return (0);
}

int xd_match (OSCTXT *pctxt, ASN1TAG tag, int *len_p, OSOCTET flags)
{
   int		parsed_len;
   ASN1TAG	parsed_tag;
   ASN1BUFSAVE  savedBufferInfo;
   int          found = FALSE, status;
   OSBOOL       constructed;

   tag &= ~TM_CONS;

   /* If skip flag set, advance decode pointer to next field */

   if (flags & XM_SKIP)
   {
      status = xd_tag_len (pctxt, &parsed_tag, &parsed_len, XM_ADVANCE);

      if (status == 0)
      {
         if (!(parsed_tag & TM_CONS)) {
            pctxt->buffer.byteIndex += parsed_len;
         }
      }
      else {
         ASN1BUF_RESTORE (pctxt);
         if (status == RTERR_ENDOFBUF) {
            rtxErrReset (pctxt);

            /* If element is optional, do not set error params */
            if (!(flags & XM_OPTIONAL)) {
               berErrAddTagParm (pctxt, tag);      /* expected tag */
               berErrAddTagParm (pctxt, parsed_tag); /* parsed tag */
            }

            /* Only return status, do not log error.  Error will be     */
            /* logged by caller..                                       */
            return RTERR_IDNOTFOU;
         }
         else
            return LOG_RTERR(pctxt, status);
      }
   }

   /* Save the context here in the event a seek is to be performed.	*/
   /* If element being seeked is not found, this will be needed to 	*/
   /* return to the original starting point..				*/

   saveBufferState (pctxt, &savedBufferInfo);

   do
   {
      status = xd_tag_len (pctxt, &parsed_tag, &parsed_len, XM_ADVANCE);

      if (status == 0) {
         constructed = (OSBOOL)(0 != (parsed_tag & TM_CONS));
         parsed_tag &= ~TM_CONS;
         if (tag == parsed_tag) {
            found = TRUE;
            if (len_p) *len_p = parsed_len;
            if (!(flags & XM_ADVANCE)) {
               if (len_p) {
                  if (pctxt->buffer.byteIndex > savedBufferInfo.byteIndex) {
                     OSSIZE tmpsize = 
                        pctxt->buffer.byteIndex - savedBufferInfo.byteIndex;
                     if (tmpsize <= OSINT32_MAX) {
                        (*len_p) += (int)tmpsize;
                        if (*len_p < (int)tmpsize) return RTERR_TOOBIG;
                     }
                     else return RTERR_TOOBIG;
                  }
                  else return RTERR_BADVALUE;
               }
               ASN1BUF_RESTORE (pctxt);
            }
         }
         else if ((flags & XM_SEEK) && (!constructed)) {
            /* advance decode pointer */
            pctxt->buffer.byteIndex += parsed_len;
         }
      }
   } while (!found  && (flags & XM_SEEK) && status == 0);

   if (!found)
   {
      restoreBufferState (pctxt, &savedBufferInfo);

      if (status == RTERR_ENDOFBUF || status == 0)
      {
         rtxErrReset (pctxt);
         berErrAddTagParm (pctxt, tag);          /* expected tag */
         berErrAddTagParm (pctxt, parsed_tag);   /* parsed tag   */
         return RTERR_IDNOTFOU; /* it will be logged later, by the generated
            code, or reset by rtxErrReset (for optional seq elements). */
      }
   }
   if (status != 0) return LOG_RTERR(pctxt, status);
   return 0;
}

int xd_match1 (OSCTXT *pctxt, OSOCTET tag, int *len_p)
{
   int stat, len;
   ASN1BUFFER *pbuffer = &pctxt->buffer;
   register OSOCTET rtag = pbuffer->data[pbuffer->byteIndex];

   if ((rtag & (~TM_FORM)) != (tag & (~TM_FORM))) {
      return errTag1NotMatched (pctxt, tag);
   }

   /* Advance the cursor */

   pbuffer->byteIndex++;

   SET_ASN1CONSTAG_BYTE(pctxt, rtag);

   if ((stat = XD_LEN (pctxt, &len)) != 0)
      return LOG_RTERR (pctxt, stat);

   if (len_p && len == ASN_K_INDEFLEN) {

      /* Verify form of tag is constructed.  If not, indefinite   */
      /* length is invalid.                                       */

      if (0 == (pctxt->flags & ASN1CONSTAG))
         return LOG_RTERR (pctxt, RTERR_INVLEN);

   }
   if (len_p) *len_p = len;

   return 0;
}

int xd_memcpy (OSCTXT* pctxt, OSOCTET *pvalue, int length)
{
   OSBOOL do_copy = (OSBOOL) ((ASN1BUF_INDEFLEN(pctxt)) ?
      TRUE : ((pctxt->buffer.byteIndex + length) <= pctxt->buffer.size));

   if (do_copy) {
      memcpy (pvalue, OSRTBUFPTR(pctxt), length);
      pctxt->buffer.byteIndex += length;
      return (0);
   }
   else return LOG_RTERR (pctxt, RTERR_ENDOFBUF);
}

int xd_MovePastEOC (OSCTXT* pctxt)
{
   ASN1TAG tag;
   int ilcnt = 1, len, stat = 0;

   while (ilcnt > 0) {
      stat = xd_tag_len (pctxt, &tag, &len, XM_ADVANCE);
      if (stat != 0) break;

      if (len > 0) pctxt->buffer.byteIndex += len;
      else if (len == ASN_K_INDEFLEN) ilcnt++;
      else if (tag == 0 && len == 0) ilcnt--;
   }

   if (stat != 0) return LOG_RTERR (pctxt, stat);
   return 0;
}

int xd_NextElement (OSCTXT* pctxt)
{
   ASN1TAG tag;
   int len, stat;

   stat = xd_tag_len (pctxt, &tag, &len, XM_ADVANCE);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   if (len == ASN_K_INDEFLEN) {
      stat = xd_MovePastEOC (pctxt);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }
   else {
      pctxt->buffer.byteIndex += len;
   }

   return 0;
}

int xd_null (OSCTXT *pctxt, ASN1TagType tagging)
{
   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_NULL)) {
         return errTag1NotMatched (pctxt, ASN_ID_NULL);
      }

      /* Check length to make sure it is zero */
      if (XD_FETCH1 (pctxt) != 0) {
         return LOG_RTERR (pctxt, RTERR_INVLEN);
      }
   }

   return (0);
}

int xd_objid
(OSCTXT *pctxt, ASN1OBJID *pvalue, ASN1TagType tagging, int length)
{
   register int	status, j;
   unsigned	subid;
   OSOCTET	b;

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_OBJID)) {
         return errTag1NotMatched (pctxt, ASN_ID_OBJID);
      }

      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   /* Check length */

   if (length > 0) {
      status = XD_CHKDEFLEN (pctxt, length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   /* Parse OID contents */

   j = 0;
   while (length > 0)
   {
      if (j < ASN_K_MAXSUBIDS) {

         /* Parse a subidentifier out of the contents field */

         pvalue->subid[j] = 0;
         do {
            b = XD_FETCH1 (pctxt);
            pvalue->subid[j] = (pvalue->subid[j] * 128) + (b & 0x7F);
            length--;
         } while (b & 0x80 && length > 0);

         /* Handle the first subidentifier special case: the first two 	*/
         /* sub-id's are encoded into one using the formula (x * 40) + y */

         if (j == 0)
         {
            subid = pvalue->subid[0];
            pvalue->subid[0] = ((subid / 40) >= 2) ? 2 : subid / 40;
            pvalue->subid[1] = (pvalue->subid[0] == 2) ?
               subid - 80 : subid % 40;
            j = 2;
         }
         else j++;
      }
      else
         return LOG_RTERR (pctxt, RTERR_INVOBJID);
   }

   if (length != 0)
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   pvalue->numids = j;
   return 0;
}

int decodeRelOID
(OSCTXT *pctxt, ASN1OBJID *pvalue, ASN1TagType tagging, int length)
{
   register int status, j;
   OSOCTET b;

   status = 0;

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_RELOID)) {
         return errTag1NotMatched (pctxt, ASN_ID_OBJID);
      }

      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   /* Check length */

   if (length > 0) {
      status = XD_CHKDEFLEN (pctxt, length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }
   else
      return LOG_RTERR (pctxt, RTERR_INVLEN);

   j = 0;
   while (length > 0)
   {
      if (j < ASN_K_MAXSUBIDS) {

         /* Parse a subidentifier out of the contents field */

         pvalue->subid[j] = 0;
         do {
            b = XD_FETCH1 (pctxt);
            pvalue->subid[j] = (pvalue->subid[j] * 128) + (b & 0x7F);
            length--;
         } while (b & 0x80);

         j++;
      }
      else
         return LOG_RTERR (pctxt, RTERR_INVOBJID);
   }

   if (length != 0) return LOG_RTERR (pctxt, RTERR_INVLEN);

   pvalue->numids = j;
   return 0;
}

int xd_octstr
(OSCTXT* pctxt, const OSOCTET** pvalue2, OSUINT32* numocts_p,
 ASN1TagType tagging, int length)
{
   OSOCTET* poctstr = 0;
   int stat = 0;

   if (tagging == ASN1EXPL) {
      if ((stat = xd_match1 (pctxt, ASN_ID_OCTSTR, &length)) < 0)
         return errTag1NotMatched (pctxt, ASN_ID_OCTSTR);
   }

   if (stat == 0) {
      int size, ll = 0;

      /* For an indefinite length message, need to get the actual 	*/
      /* length by parsing tags until the end of the message is 	*/
      /* reached..							*/
      if (length == ASN_K_INDEFLEN) {  /* indefinite length message */
         return RTERR_NOTSUPP;
      }
      else ll = size = length;

      if (ll > 0) {
         poctstr = (OSOCTET*) rtxMemAlloc (pctxt, ll);
         if (0 != poctstr) {
            *numocts_p = size;
            stat = xd_octstr_s
               (pctxt, poctstr, numocts_p, ASN1IMPL, size);
         }
         else
            return LOG_RTERR (pctxt, RTERR_NOMEM);
      }
      else {
         *numocts_p = 0;
      }
   }

   if (stat != 0) {
      rtxMemFreePtr (pctxt, poctstr);
      return LOG_RTERR (pctxt, stat);
   }

   *pvalue2 = poctstr;

   return 0;
}

int xd_octstr_s
(OSCTXT* pctxt, OSOCTET* pvalue, OSUINT32* numocts_p,
 ASN1TagType tagging, int length)
{
   int stat = 0;

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_OCTSTR)) {
         return errTag1NotMatched (pctxt, ASN_ID_OCTSTR);
      }
      stat = XD_LEN (pctxt, &length);
      if (stat != 0) LOG_RTERR (pctxt, stat);
   }

   /* Check length */
   if (length < 0)
      return LOG_RTERR (pctxt, RTERR_INVLEN);
   else if ((OSUINT32)length > *numocts_p)
      return LOG_RTERR (pctxt, RTERR_STROVFLW);

   if (pctxt->flags & ASN1CONSTAG) {
      ASN1CCB ccb;
      OSUINT32 idx = 0, len = (OSUINT32) length;

      ccb.len = length;
      ccb.ptr = OSRTBUFPTR(pctxt);

      while (!XD_CHKEND (pctxt, &ccb)) {
         len = length;
         stat = xd_octstr_s
            (pctxt, pvalue + idx, &len, ASN1EXPL, length);

         if (stat != 0) return LOG_RTERR (pctxt, stat);
         else idx += len;
      }

      /* If not indefinite length, make sure we consumed exactly as  */
      /* many bytes as specified in the constructed length field..   */

      if (length != ASN_K_INDEFLEN) {
         size_t usedBytes = OSRTBUFPTR(pctxt) - ccb.ptr;
         if (usedBytes != (size_t)length)
            return LOG_RTERR (pctxt, RTERR_INVLEN);
      }

      *numocts_p = idx;
   }
   else if (length > 0) {
      stat = xd_memcpy (pctxt, pvalue, length);
      *numocts_p = length;
   }
   else {
      *numocts_p = 0;
   }

   return (0);
}

int xd_OpenType
(OSCTXT *pctxt, const OSOCTET** pvalue2, OSUINT32* numocts_p)
{
   int	length, status;
   size_t savedByteIndex;
   OSOCTET* pvalue;
   ASN1TAG tag;

   /* Store pointer to data in object */

   pvalue = OSRTBUFPTR (pctxt);
   savedByteIndex = pctxt->buffer.byteIndex;

   /* Advance decode pointer to the next field */

   if ((status = xd_tag_len (pctxt, &tag, &length, XM_ADVANCE)) == 0)
   {
      if (length > 0) {              /* fixed length object  */
         pctxt->buffer.byteIndex += length;
      }
      else if (length == ASN_K_INDEFLEN) {  /* indefinite length */
         status = xd_MovePastEOC (pctxt);      /* move past EOC marker */
      }
      else if (tag == 0 && length == 0) /* EOC is not allowed as OpenType */
         return LOG_RTERR (pctxt, RTERR_BADVALUE);

      *numocts_p = (int)(pctxt->buffer.byteIndex - savedByteIndex);
   }

   /* It is possible to encouter an EOB condition if the ANY field was	*/
   /* defined as an implicit null field and is the last field in the	*/
   /* buffer (this is common in X.410 messages).  Assume this is the	*/
   /* case if an RTERR_ENDOFBUF status is returned from xd_tag_len.*/

   if (status == RTERR_ENDOFBUF) {
      rtxErrReset (pctxt);
      status = 0;
   }
   else
      if (status != 0) return LOG_RTERR (pctxt, status);

   *pvalue2 = (const OSOCTET*) rtxMemAlloc (pctxt, *numocts_p);
   if (*pvalue2 != 0)
      memcpy ((void*)*pvalue2, pvalue, *numocts_p);
   else
      return LOG_RTERR (pctxt, RTERR_NOMEM);

   return 0;
}

static int openTypeAppend (OSCTXT *pctxt, OSRTDList *pElemList)
{
   int stat;

   if (0 != pElemList) {
      ASN1OpenType* pOpenType = rtxMemAllocType (pctxt, ASN1OpenType);
      if (pOpenType == NULL) return LOG_RTERR (pctxt, RTERR_NOMEM);

      stat = xd_OpenType (pctxt, &pOpenType->data, &pOpenType->numocts);

      if (stat != 0) {
         rtxMemFreePtr (pctxt, pOpenType);
         return LOG_RTERR (pctxt, stat);
      }
      else
         rtxDListAppend (pctxt, pElemList, pOpenType);
   }
   else {
      stat = xd_NextElement (pctxt);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }

   return 0;
}

int xd_OpenTypeExt
(OSCTXT* pctxt, ASN1CCB* ccb_p, ASN1TAG tag, OSRTDList *pElemList)
{
   int length, stat;

   if (tag == ASN_K_NOTAG) {

      /* Loop through remaining elements in SEQUENCE */

      while (!XD_CHKEND (pctxt, ccb_p)) {
         stat = openTypeAppend (pctxt, pElemList);
         if (stat != 0) return LOG_RTERR (pctxt, stat);
      }
   }
   else {

      /* Loop through elements until matching tag found or some other   */
      /* error occurs..                                                 */

      while ((stat = xd_match (pctxt, tag, &length, 0)) == RTERR_IDNOTFOU) {

         rtxErrReset (pctxt);

         if (XD_CHKEND (pctxt, ccb_p)) {
            return LOG_RTERR (pctxt, RTERR_ENDOFBUF);
         }
         else {
            stat = openTypeAppend (pctxt, pElemList);
            if (stat != 0) return LOG_RTERR (pctxt, stat);
         }
      }
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }

   return 0;
}

int xd_setp
(OSCTXT *pctxt, const OSOCTET* msg_p,
 int msglen, ASN1TAG *tag_p, int *len_p)
{
   int	stat, ll;
   ASN1TAG ltag;

   /* Initialize the context for decoding */

   stat = rtxInitContextBuffer (pctxt, msg_p, (msglen > 0) ? msglen : INT_MAX);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   /* Reset BER indefinite length and last EOC flags */

   pctxt->flags &= (~(ASN1INDEFLEN | ASN1LASTEOC));

   /* If message is fixed length, add length of ID and length fields 	*/
   /* onto contents field length to get total message length; else set	*/
   /* global message length to indefinite length indicator value...	*/

   stat = xd_tag_len (pctxt, &ltag, &ll, XM_ADVANCE);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   if (ll == ASN_K_INDEFLEN) {
      pctxt->flags |= ASN1INDEFLEN;
      if (len_p) *len_p = ASN_K_INDEFLEN;
   }
   else {
      pctxt->buffer.size = (size_t)ll + pctxt->buffer.byteIndex;
      if (len_p) *len_p = (int)pctxt->buffer.size;
      if (msglen > 0 && pctxt->buffer.size > (OSUINT32)msglen) {
         stat = RTERR_INVLEN;
      }
   }

   pctxt->buffer.byteIndex = 0;

   if (tag_p) *tag_p = ltag;

   if (stat != 0) return LOG_RTERR (pctxt, stat);
   return 0;
}

int xd_Tag1AndLen (OSCTXT *pctxt, int *len_p)
{
   int stat;
   OSUINT16 mask = ASN1INDEFLEN | ASN1LASTEOC;
   OSOCTET tagbyte;

   /* Check for attempt to read past EOB */

   if ((pctxt->flags & mask) == mask)
      return LOG_RTERR(pctxt, RTERR_ENDOFBUF);

   /* Set constructed tag bit in context flags based on tag value */

   tagbyte = XD_FETCH1 (pctxt);

   SET_ASN1CONSTAG_BYTE(pctxt, tagbyte);

   /* Decode length */

   stat = XD_LEN (pctxt, len_p);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}

int xd_tag (OSCTXT* pctxt, ASN1TAG *tag_p)
{
   ASN1TAG      class_form, id_code;
   int          lcnt = 0;
   OSOCTET    b = XD_FETCH1 (pctxt);

   *tag_p = 0;

   class_form = (ASN1TAG)(b & TM_CLASS_FORM);
   class_form <<= ASN1TAG_LSHIFT;

   if ((id_code = (b & TM_B_IDCODE)) == 31) {
      id_code = 0;
      do {
         b = XD_FETCH1 (pctxt);
         id_code = (id_code * 128) + (b & 0x7F);
         if (id_code > TM_IDCODE || lcnt++ > 8)
            return LOG_RTERR(pctxt, RTERR_BADTAG);
      } while (b & 0x80);
   }

   *tag_p = class_form | id_code;

   /* Set constructed tag bit in context flags based on parsed value */
   SET_ASN1CONSTAG(pctxt, class_form);

   return (0);
}

int xd_tag_len
(OSCTXT *pctxt, ASN1TAG *tag_p, int *len_p, OSOCTET flags)
{
   int stat;
   OSUINT16 mask = ASN1INDEFLEN | ASN1LASTEOC;

   /* Check for attempt to read past EOB */

   if (0 != (pctxt->flags & ASN1INDEFLEN)) {
      if ((pctxt->flags & mask) == mask)
         return LOG_RTERR(pctxt, RTERR_ENDOFBUF);
   }
   else {
      register size_t usedBytes =
         OSRTBUFPTR(pctxt) - pctxt->buffer.data;
      if (usedBytes >= pctxt->buffer.size)
         return LOG_RTERR(pctxt, RTERR_ENDOFBUF);
   }

   /* Save context prior to parsing this tag and length.  It may be	*/
   /* used by generated decode functions to restore the decode point if	*/
   /* an operation is to be retried..					*/

   ASN1BUF_SAVE (pctxt);

   if ((stat = XD_TAG (pctxt, tag_p)) == 0)
   {
      stat = XD_LEN (pctxt, len_p);

      /* Indefinite length message check.. if indefinite length parsed,	*/
      /* bump indefinite length section counter; if EOC ID parsed, 	*/
      /* decrement count.  If overall message is of indefinite length 	*/
      /* and count goes to zero, signal end of message.			*/

      if (stat == 0)
      {
         if (*len_p == ASN_K_INDEFLEN) {

            /* Verify form of tag is constructed.  If not, indefinite   */
            /* length is invalid.                                       */

            if (0 == (pctxt->flags & ASN1CONSTAG))
               return LOG_RTERR(pctxt, RTERR_INVLEN);
         }
      }
   }

   if (!(flags & XM_ADVANCE)) ASN1BUF_RESTORE (pctxt);

   if (stat != 0) return LOG_RTERR(pctxt, stat);
   return 0;
}

int xd_uint16
(OSCTXT *pctxt, OSUINT16 *pvalue, ASN1TagType tagging, int length)
{
   register int status = 0;
   OSOCTET    ub;	/* unsigned */

   if (tagging == ASN1EXPL) {
      /* tag + len + 1b val */
      if ((status = XD_CHKDEFLEN (pctxt, 3)) != 0)
         return LOG_RTERR (pctxt, status);

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      length = (int) XD_FETCH1 (pctxt);
   }

   /* Make sure integer will fit in target variable */
   if (length > (int)(sizeof(OSUINT16) + 1))
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   else if (length == (sizeof(OSUINT16) + 1)) {
      /* first byte must be zero */
      if (0 != ASN1BUFCUR(pctxt))
         return LOG_RTERR (pctxt, RTERR_TOOBIG);

      XD_BUMPIDX (pctxt, 1); /* skip it */

      if ((status = XD_CHKDEFLEN (pctxt, 1)) != 0)
         return LOG_RTERR (pctxt, status);
      length--;
   }

   /* Use unsigned bytes to decode unsigned integer.. */

   ub = XD_FETCH1 (pctxt);

   if (length > 1) {
      *pvalue = (OSUINT16) ((ub * 256) + XD_FETCH1 (pctxt));
   }
   else
      *pvalue = ub;

   return 0;
}

int xd_uint8
(OSCTXT *pctxt, OSUINT8 *pvalue, ASN1TagType tagging, int length)
{
   register int status = 0;

   if (tagging == ASN1EXPL) {
      if ((status = XD_CHKDEFLEN (pctxt, 3)) != 0) /* tag + len + val */
         return LOG_RTERR (pctxt, status);

      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      length = (int) XD_FETCH1 (pctxt);
   }

   /* Make sure integer will fit in target variable */
   if (length > (int)(sizeof(OSOCTET) + 1))
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   else if (length == (sizeof(OSOCTET) + 1)) {
      /* first byte must be zero */
      if (0 != ASN1BUFCUR(pctxt))
         return LOG_RTERR (pctxt, RTERR_TOOBIG);

      XD_BUMPIDX (pctxt, 1); /* skip it */

      if ((status = XD_CHKDEFLEN (pctxt, 1)) != 0)
         return LOG_RTERR (pctxt, status);
   }

   /* Use unsigned bytes to decode unsigned integer.. */

   *pvalue = XD_FETCH1 (pctxt);

   return 0;
}

int xd_uint
(OSCTXT *pctxt, OSUINT32 *pvalue, ASN1TagType tagging, int length)
{
   register int	status = 0;
   OSOCTET	ub;	/* unsigned */

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      status = XD_LEN (pctxt, &length);
      if (status != 0) return LOG_RTERR (pctxt, status);
   }

   if (status == 0) {
      /* Make sure integer will fit in target variable (ED, 4/22/02) */
      if (length > (int)(sizeof(OSUINT32) + 1))
         return LOG_RTERR (pctxt, RTERR_TOOBIG);
      else if (length == (sizeof(OSUINT32) + 1)) {
         /* first byte must be zero */
         if (0 != ASN1BUFCUR(pctxt))
            return LOG_RTERR (pctxt, RTERR_TOOBIG);
      }

      /* Use unsigned bytes to decode unsigned integer.. */

      *pvalue = 0;

      while (length > 0 && status == 0) {
         if ((status = XD_MEMCPY1 (pctxt, &ub)) == 0) {
            *pvalue = (*pvalue * 256) + ub;
            length--;
         }
      }
   }

   if (status != 0) return LOG_RTERR (pctxt, status);
   return 0;
}

int xd_uint64 (OSCTXT *pctxt, OSUINT64 *object_p,
               ASN1TagType tagging, int length)
{
   OSOCTET      ub;     /* unsigned */
   int          stat;
   OSBOOL       negative;

   if (tagging == ASN1EXPL) {
      if (!XD_MATCH1 (pctxt, ASN_ID_INT)) {
         return errTag1NotMatched (pctxt, ASN_ID_INT);
      }
      stat = XD_LEN (pctxt, &length);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }

   /* if the length is zero, return 0 */
   if (length == 0) {
      *object_p = 0;
      return 0;
   }

   /* Verify that encoded value is not negative */

   negative = ((ASN1BUFCUR(pctxt) & 0x80) != 0);

   /* Make sure integer will fit in target variable */

   if (length > (int)(sizeof(OSUINT64) + 1))
      return LOG_RTERR (pctxt, RTERR_TOOBIG);
   else if (length == (int)(sizeof(OSUINT64) + 1)) {

      /* first byte must be zero */

      if (0 != ASN1BUFCUR(pctxt))
         return LOG_RTERR (pctxt, RTERR_TOOBIG);

      XD_BUMPIDX (pctxt, 1); /* skip it */

      if ((stat = XD_CHKDEFLEN (pctxt, 1)) != 0)
         return LOG_RTERR (pctxt, stat);

      length--;
   }

   /* Use unsigned bytes to decode unsigned integer.. */

   *object_p = 0;

   while (length > 0) {
      ub = XD_FETCH1 (pctxt);
      *object_p = (*object_p * 256) + ub;
      length--;
   }

   if (negative) {
      OSINT64 signedValue = (OSINT64) *object_p;
      if (signedValue >= 0) signedValue = -signedValue;
      rtxErrAddStrParm (pctxt, "_uint64");
      rtxErrAddInt64Parm (pctxt, signedValue);
      return LOG_RTERR (pctxt, RTERR_CONSVIO);
   }

   return 0;
}

int xd_16BitCharStr (OSCTXT* pctxt, Asn116BitCharString* pvalue,
                     ASN1TagType tagging, ASN1TAG tag, int length)
{
   OSUINT32   i;
   OSINT32    stat = 0, isConstructedTag;
   OSOCTET* data = 0;
   OSUINT32   nchars = 0;
   OS16BITCHAR* data16 = 0;

   if (tagging == ASN1EXPL) {
      if ((stat = xd_match1 (pctxt, ASN1TAG2BYTE(tag), &length)) < 0)
         /* RTERR_IDNOTFOU will be logged later, by the generated code,
            or reset by rtxErrReset (for optional seq elements). */
         return (stat == RTERR_IDNOTFOU) ? stat : LOG_RTERR (pctxt, stat);
   }
   isConstructedTag = (int)((pctxt->flags & ASN1CONSTAG) != 0);

   if (0 == length) {
      nchars = 0;
      data = 0;
   }
   else {
      int size, ll = 0;

      /* For an indefinite length message, need to get the actual 	*/
      /* length by parsing tags until the end of the message is 	*/
      /* reached..							*/
      if (length == ASN_K_INDEFLEN) {  /* indefinite length message */
         return RTERR_NOTSUPP;
      }
      else ll = size = length;

      data = (OSOCTET*) rtxMemAlloc (pctxt, ll);
      data16 = (OS16BITCHAR*) data;

      if (0 != data) {
         ll = size;
         if ((stat = xd_octstr_s
              (pctxt, data, (OSUINT32*) &ll, ASN1IMPL, size)) != 0) {
            rtxMemFreePtr (pctxt, data);
            return LOG_RTERR (pctxt, stat);
         }
         nchars = ll / 2;
         for (i = 0; i < nchars; i++, data += 2) {
            data16[i] = (OS16BITCHAR) ((data[0] * 256) + data[1]);
         }
      }
      else
         return LOG_RTERR (pctxt, RTERR_NOMEM);
   }

   if (isConstructedTag && length == ASN_K_INDEFLEN) {
      if (XD_MATCHEOC (pctxt)) XD_BUMPIDX (pctxt, 2);
      else {
         rtxMemFreePtr (pctxt, data);
         return LOG_RTERR (pctxt, RTERR_INVLEN);
      }
   }
   pvalue->data = data16;
   pvalue->nchars = nchars;
   return 0;
}

int xd_32BitCharStr (OSCTXT* pctxt, Asn132BitCharString* pvalue,
                     ASN1TagType tagging, ASN1TAG tag, int length)
{
   OSUINT32   i;
   OSINT32    stat = 0, isConstructedTag;
   OSOCTET* data = 0;
   OSUINT32   nchars = 0;
   OS32BITCHAR* data32 = 0;

   if (tagging == ASN1EXPL) {
      if ((stat = xd_match1 (pctxt, ASN1TAG2BYTE(tag), &length)) < 0)
         /* RTERR_IDNOTFOU will be logged later, by the generated code,
            or reset by rtxErrReset (for optional seq elements). */
         return (stat == RTERR_IDNOTFOU) ? stat : LOG_RTERR (pctxt, stat);
   }
   isConstructedTag = (int)((pctxt->flags & ASN1CONSTAG) != 0);

   if (0 == length) {
      nchars = 0;
      data32 = 0;
   }
   else {
      int size, ll = 0;

      /* For an indefinite length message, need to get the actual 	*/
      /* length by parsing tags until the end of the message is 	*/
      /* reached..							*/
      if (length == ASN_K_INDEFLEN) {  /* indefinite length message */
         return RTERR_NOTSUPP;
      }
      else ll = size = length;

      data = (OSOCTET*) rtxMemAlloc (pctxt, ll);
      data32 = (OS32BITCHAR*)data;

      if (0 != data) {
         ll = size;
         if ((stat = xd_octstr_s
              (pctxt, data, (OSUINT32*) &ll, ASN1IMPL, size)) != 0) {
            rtxMemFreePtr (pctxt, data);
            return LOG_RTERR (pctxt, stat);
         }
         nchars = ll / 4;
         for (i = 0; i < nchars; i++, data += 4) {
            data32[i] = (OS32BITCHAR) (((data[0] * 256) + data[1]) *
               65536UL + (((data[2] * 256) + data[3])));
         }
      }
      else
         return LOG_RTERR (pctxt, RTERR_NOMEM);
   }

   if (isConstructedTag && length == ASN_K_INDEFLEN) {
      if (XD_MATCHEOC (pctxt)) XD_BUMPIDX (pctxt, 2);
      else {
         rtxMemFreePtr (pctxt, data);
         return LOG_RTERR (pctxt, RTERR_INVLEN);
      }
   }
   pvalue->data = data32;
   pvalue->nchars = nchars;
   return 0;
}

/**
 * Single-byte tag not matched error
 */
static int errTag1NotMatched (OSCTXT *pctxt, OSOCTET expectedTag)
{
   /* expected tag */
   berErrAddTagParm (pctxt, _GENASN1TAG(expectedTag));

   /* parsed tag */
   berErrAddTagParm (pctxt, _GENASN1TAG(ASN1BUFCUR(pctxt)));

   return RTERR_IDNOTFOU;
}

static void saveBufferState (OSCTXT* pCtxt, ASN1BUFSAVE* pSavedInfo)
{
   if (!pSavedInfo) pSavedInfo = &pCtxt->savedInfo;
   pSavedInfo->byteIndex = pCtxt->buffer.byteIndex;
   pSavedInfo->bitOffset = pCtxt->buffer.bitOffset;
   pSavedInfo->flags = pCtxt->flags;
}

static void restoreBufferState (OSCTXT* pCtxt, ASN1BUFSAVE* pSavedInfo)
{
   if (!pSavedInfo) pSavedInfo = &pCtxt->savedInfo;
   pCtxt->buffer.byteIndex = pSavedInfo->byteIndex;
   pCtxt->buffer.bitOffset = pSavedInfo->bitOffset;
   pCtxt->flags = (pSavedInfo->flags & (~ASN1LASTEOC));
}

/* Add an ASN.1 tag parameter to an error message */

static const char* rtTagToString (ASN1TAG tag, char* buffer, size_t bufsiz)
{
   OSUINT32 idmask = 0xFFFFFFFF >> (((sizeof(ASN1TAG) - 2) * 8) + 3);
   OSUINT32 idcode = ((OSUINT32)tag) & idmask;
   ASN1TAG  tclass = tag & TM_PRIV;
   char lbuf[32];

   switch (tclass) {
   case TM_UNIV: sprintf (lbuf, "[UNIVERSAL %d]", idcode); break;
   case TM_APPL: sprintf (lbuf, "[APPLICATION %d]", idcode); break;
   case TM_CTXT: sprintf (lbuf, "[%d]", idcode); break;
   case TM_PRIV: sprintf (lbuf, "[PRIVATE %d]", idcode); break;
   default:      sprintf (lbuf, "[??? %d]", idcode); break;
   }

   bufsiz = (sizeof (lbuf) < bufsiz) ? sizeof (lbuf) : bufsiz;
   strncpy (buffer, lbuf, bufsiz);
   buffer[bufsiz-1] = '\0';

   return buffer;
}

int berErrAddTagParm (OSCTXT* pctxt, ASN1TAG errParm)
{
   char lbuf[32];
   rtTagToString (errParm, lbuf, sizeof(lbuf));
   return rtxErrAddStrParm (pctxt, lbuf);
}

