/* This test driver program reads an encoded record from a file         */
/* and decodes it and displays the results..                            */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AllTypes.h"
#include "rtxsrc/rtxCommon.h"

int main (int argc, char** argv)
{
   SequenceType pdu;
   OSCTXT       ctxt;
   OSOCTET      msgbuf[10256];
   ASN1TAG      tag;
   int          i, len, stat;
   FILE*        fp;
   const char*  filename = "message.dat";
   OSBOOL       trace = TRUE;

   /* Process command line arguments */

   if (argc > 1) {
      for (i = 1; i < argc; i++) {
         if (!strcmp (argv[i], "-i")) filename = argv[++i];
         else if (!strcmp (argv[i], "-notrace")) trace = FALSE;
         else {
            printf ("usage: reader [-i <filename>][-notrace]\n");
            printf ("   -i <filename>  read encoded msg from <filename>\n");
            printf ("   -notrace  do not display trace info\n");
            return 0;
         }
      }
   }

   /* Init context structure */

   if (rtInitContext (&ctxt) != 0) {
      printf ("Error initializing context\n");
      return -1;
   }

   /* Read input file into a memory buffer */

   if ((fp = fopen (filename, "rb")) != 0) {
      len = (int) fread (msgbuf, 1, sizeof(msgbuf), fp);
   }
   else {
      printf ("Error opening %s for read access\n", filename);
      return -1;
   }

   stat = xd_setp (&ctxt, msgbuf, len, &tag, &len);
   if (0 != stat) {
      rtxErrPrint (&ctxt);
      return stat;
   }

   asn1Init_SequenceType (&pdu);

   /* Decode */

   if (tag == TV_SequenceType) {

      /* Call compiler generated decode function */

      stat = asn1D_SequenceType (&ctxt, &pdu, ASN1EXPL, 0);
      if (stat == 0) {
         if (trace) {
            printf ("Decode of SequenceType was successful\n");
            printf ("Decoded record:\n");
            asn1Print_SequenceType ("pdu", &pdu);
         }
      }
      else {
         printf ("decode of SequenceType failed\n");
         rtxErrPrint (&ctxt);
         return -1;
      }
   }
   else {
      printf ("unexpected tag %hx received\n", tag);
   }

   rtFreeContext (&ctxt);

   return 0;
}
