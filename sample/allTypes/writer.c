/* This test driver program encodes a data record and writes the        */
/* encoded result to an output file..                                   */

#include <stdio.h>
#include <stdlib.h>
#include "AllTypes.h"

#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxPrint.h"

#define MAXMSGLEN 102400

int setBit (OSOCTET* pBits, OSSIZE numbits, OSSIZE bitIndex)
{
   OSOCTET prev, mask = 0x80, *pbyte;

   if (bitIndex >= numbits)
      return RTERR_OUTOFBND;
   pbyte = pBits + bitIndex / 8;
   mask >>= bitIndex % 8;
   prev = *pbyte;
   *pbyte = (OSOCTET)(prev | mask);
   return prev & mask;
}

static void fill_Name (Name* pName, char* fn, char* mi, char* ln)
{
   pName->givenName = fn;
   pName->initial = mi;
   pName->familyName = ln;
}

/* Routine to fill the employee record with child test data */
static void fill_jSmith (OSCTXT* pctxt, SequenceType* pPR)
{
   ChildInformation* pChildInfo;

   fill_Name (&pPR->name, "John", "P", "Smith");

   rtxDListInit (&pPR->children);

   /* fill first child record */
   pChildInfo = rtxMemAllocType (pctxt, ChildInformation);
   fill_Name (&pChildInfo->name, "Ralph", "T", "Smith");
   pChildInfo->dateOfBirth = "19571111";
   rtxDListAppend (pctxt, &pPR->children, pChildInfo);

   /* fill second child record */
   pChildInfo = rtxMemAllocType (pctxt, ChildInformation);
   fill_Name (&pChildInfo->name, "Susan", "B", "Jones");
   pChildInfo->dateOfBirth = "19590717";
   rtxDListAppend (pctxt, &pPR->children, pChildInfo);
}


int main (int argc, char** argv)
{
   SequenceType seqType;
   OSCTXT       ctxt;
   OSOCTET      msgbuf[MAXMSGLEN], *msgptr, *pdata;
   int          i, j, len, lseq[1024];
   size_t       s;
   FILE*        fp;
   char*        filename = "message.dat";
   OSBOOL       trace = TRUE;
   OSOCTET      bs[10] = { 0xA1, 0xB2, 0xC3 };
   Asn116BitCharString bmp;
   OS32BITCHAR  uch[10] = { (unsigned char)'А', (unsigned char)'р',
                            (unsigned char)'т', (unsigned char)'е',
                            (unsigned char)'м', 0 };

   OSUNICHAR bmpChars[] = {
      0x00C0, 0x00F0, 0x00F2, 0x00E5, 0x00EC, 0x0020, 0x00C1, 0x00EE,
      0x00EB, 0x00E3, 0x00E0, 0x00F0 }; /* Артем Болгар */
   Asn132BitCharString ucs;
   Name         name;
   static OSOCTET otdata[] = { 0x9F, 0x32, 0x04, 0x06, 0x54, 0x32, 0x10 };
   ASN1OpenType ot;
   OSOCTET      ocs[2048];
   char         chs[2049];
   EnumType     melem[] = { married, single, divorced, children };
   OSBOOL       belem[] = { 0,1,1,0,1};
   SequenceType_iseqof_element ielem[] = { 0,1,2,3,4 };
   ChoiceType   choices[3];
   ASN1DynBitStr* pbitstr;
   SequenceType_bsseqof2_element* pseqofbitstr;
   ASN1DynOctStr* poctstr;
   static const char* seqofcsstr[] = { "Objective", "Systems", "Inc." };

   /* Process command line arguments */
   if (argc > 1) {
      for (i = 1; i < argc; i++) {
         if (!strcmp (argv[i], "-o")) filename = argv[++i];
         else if (!strcmp (argv[i], "-notrace")) trace = FALSE;
         else {
            printf ("usage: writer [ -v ] [ -o <filename>\n");
            printf ("   -o <filename>  write encoded msg to <filename>\n");
            printf ("   -notrace  do not display trace info\n");
            return 0;
         }
      }
   }

   if (rtInitContext (&ctxt) != 0) {
      printf ("context initialization failed\n");
      return -1;
   }

   /* Encode */

   xe_setp (&ctxt, msgbuf, sizeof(msgbuf));

   /* Populate structure of generated type */
   memset (&seqType, 0, sizeof (seqType));

   seqType.utc = "031231235538-0406";
   seqType.utc1 = "040229190538Z";
   seqType.gtc = "20030101020538.00+0430";
   seqType.gtc1 = "20040229190538.3141Z";

   seqType.bs0.numbits = 10*8 - 2;
   seqType.bs0.data = bs;
   for (s = 0; s < sizeof (bs); s++)
      bs[s] = (OSOCTET) (0xAA* (s + 1));

   seqType.bs1.numbits = 8;
   seqType.bs1.data[0] = 0;
   setBit (seqType.bs1.data, seqType.bs1.numbits, SequenceType_bs1_zero_1);
   setBit (seqType.bs1.data, seqType.bs1.numbits, SequenceType_bs1_a_1);
   setBit (seqType.bs1.data, seqType.bs1.numbits, SequenceType_bs1_b_1);
   setBit (seqType.bs1.data, seqType.bs1.numbits, SequenceType_bs1_c_1);

   seqType.bs2.numbits = 0;
   seqType.bs2.data = 0;

   seqType.bs3.numbits = 7;
   seqType.bs3.data[0] = 0;
   setBit (seqType.bs3.data, seqType.bs3.numbits, BS3_zero);
   setBit (seqType.bs3.data, seqType.bs3.numbits, BS3_a);
   setBit (seqType.bs3.data, seqType.bs3.numbits, BS3_b);
   setBit (seqType.bs3.data, seqType.bs3.numbits, BS3_c);

   for (s = 0; s < sizeof(chs); s++) {
      chs [s] = (char)(s%62 + 'A');
   }
   chs[sizeof(chs)-1] = '\0'; /* null-terminate string */
   seqType.cs = chs;

   ucs.data = uch;
   ucs.nchars = 5;
   seqType.unis = ucs;

   bmp.data = bmpChars;
   bmp.nchars = sizeof(bmpChars) / sizeof(OSUNICHAR);
   seqType.bmps = bmp;

   for (s = 0; s < sizeof (ocs);s++) {
      ocs [s] = (OSOCTET)s;
   }
   seqType.os.numocts = sizeof (ocs);
   seqType.os.data = ocs;

   seqType.val1 = 21;
   seqType.val2 = value2;
   seqType.enum_ = yes;
   seqType.boolean_ = 1;
   seqType.oid = oid;

   fill_jSmith (&ctxt, &seqType);
   fill_Name (&seqType.child.name, "AAA", "SSS", "BBB");
   seqType.child.dateOfBirth = "aaaaaaaaa";

   seqType.seqofEnum.n = 4;
   seqType.seqofEnum.elem = melem;

   rtxDListInit (&seqType.bsseqof1);
   for (i = 0; i < 4; i++) {
      pbitstr = rtxMemAllocType (&ctxt, ASN1DynBitStr);
      pbitstr->numbits = 5*8 - 1;
      pdata = (OSOCTET*) rtxMemAlloc (&ctxt, 5);
      for (j = 0; j < 5; j++) {
         pdata[j] = (OSOCTET) ((i + 1)* (j + 1) * 0xCC);
      }
      pbitstr->data = pdata;
      rtxDListAppend (&ctxt, &seqType.bsseqof1, (void*)pbitstr);
   }

   rtxDListInit (&seqType.bsseqof2);
   for (i = 0; i < 4; i++) {
      pseqofbitstr = rtxMemAllocType (&ctxt, SequenceType_bsseqof2_element);
      pseqofbitstr->numbits = 7;
      for (j = 0; j < 1; j++) {
         pseqofbitstr->data[j] = (OSOCTET) ((i + 1)* (j + 1) * 0xCC);
      }
      rtxDListAppend (&ctxt, &seqType.bsseqof2, (void*)pseqofbitstr);
   }

   rtxDListInit (&seqType.osseqof1);
   for (i = 0; i < 4; i++) {
      poctstr = rtxMemAllocType (&ctxt, ASN1DynOctStr);
      poctstr->numocts = 5;
      pdata = (OSOCTET*) rtxMemAlloc (&ctxt, 5);
      for (j = 0; j < 5; j++) {
         pdata[j] = (OSOCTET) ((i + 1)* (j + 1) * 0xCC);
      }
      poctstr->data = pdata;
      rtxDListAppend (&ctxt, &seqType.osseqof1, (void*)poctstr);
   }

   rtxDListInit (&seqType.csseqof1);
   for (i = 0; i < 3; i++) {
      rtxDListAppend (&ctxt, &seqType.csseqof1, (void*)seqofcsstr[i]);
   }

   seqType.bseqof.n = 5;
   seqType.bseqof.elem = belem;

   seqType.iseqof.n = 5;
   seqType.iseqof.elem = ielem;

   for (i = 0; i < 20; i++)
   {
      lseq[i] = i;
   }
   seqType.iseqof1.n = 20;
   seqType.iseqof1.elem = lseq;

   seqType.iseqof2.n = 5;
   seqType.iseqof2.elem[0] = 1;
   seqType.iseqof2.elem[1] = 2;
   seqType.iseqof2.elem[2] = 3;
   seqType.iseqof2.elem[3] = 4;
   seqType.iseqof2.elem[4] = 5;

   rtxDListInit (&seqType.seqofChoice);
   choices[0].t = T_ChoiceType_e1;
   choices[0].u.e1 = 1;
   rtxDListAppend (&ctxt, &seqType.seqofChoice, &choices[0]);
   choices[1].t = T_ChoiceType_e2;
   choices[1].u.e2 = "String data";
   rtxDListAppend (&ctxt, &seqType.seqofChoice, &choices[1]);
   choices[2].t = T_ChoiceType_e3;
   fill_Name (&name, "Art", "S", "Bolgar");
   choices[2].u.e3 = &name;
   rtxDListAppend (&ctxt, &seqType.seqofChoice, &choices[2]);

   ot.data = otdata;
   ot.numocts = sizeof (otdata);
   rtxDListAppend (&ctxt, &seqType.extElem1, &ot);

   xe_setp (&ctxt, msgbuf, sizeof(msgbuf));

   if ((len = asn1E_SequenceType (&ctxt, &seqType, ASN1EXPL)) >= 0)
   {
      msgptr = xe_getp (&ctxt);

      if (trace) {
         rtxPrintHexStr ("message", len, msgptr);
      }
   }
   else {
      rtxErrPrint (&ctxt);
      exit (-1);
   }

   /* Write the encoded message out to the output file */

   if ((fp = fopen (filename, "wb")) != NULL) {
      fwrite (msgptr, 1, len, fp);
      fclose (fp);
   }
   else {
      printf ("Error opening %s for write access\n", filename);
      return -1;
   }

   rtFreeContext (&ctxt);
   return 0;

}
