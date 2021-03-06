/* This test driver program reads an encoded record from a file         */
/* and decodes it and displays the results..                            */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Employee.h"
#include "rtxsrc/rtxCommon.h"

int main (int argc, char** argv)
{
   PersonnelRecord employee;
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

   asn1Init_PersonnelRecord (&employee);

   /* Decode */

   if (tag == TV_PersonnelRecord) {

      /* Call compiler generated decode function */

      stat = asn1D_PersonnelRecord (&ctxt, &employee, ASN1EXPL, 0);
      if (stat == 0) {
         if (trace) {
            printf ("Decode of PersonnelRecord was successful\n");
            printf ("Decoded record:\n");
            asn1Print_PersonnelRecord ("Employee", &employee);
         }
      }
      else {
         printf ("decode of PersonnelRecord failed\n");
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
