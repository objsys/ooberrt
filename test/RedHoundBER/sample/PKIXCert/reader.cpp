#include "ridgeback_ber.h"
#include "rtxsrc/rtxCommon.h"

#include <crtdbg.h>

#define DECLARE_MEMLEAK_DETECTOR0 { \
int LeakTmpFlag; \
_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); \
_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT); \
_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE); \
_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT); \
_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE); \
_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT); \
LeakTmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); \
LeakTmpFlag |= _CRTDBG_CHECK_ALWAYS_DF; \
LeakTmpFlag |= _CRTDBG_LEAK_CHECK_DF;   \
LeakTmpFlag |= _CRTDBG_DELAY_FREE_MEM_DF; \
_CrtSetDbgFlag(LeakTmpFlag); \
}

static class ___MemLeakDetector___ {
public:
   ___MemLeakDetector___ () DECLARE_MEMLEAK_DETECTOR0
} ___memleakdetector__;

using namespace rhsec_ber;

#include <stdio.h>
#include <stdlib.h>

#define MAXREADLEN 1024

int main (int argc, char** argv)
{
   OSBOOL      trace = TRUE, verbose = FALSE;
   const char* filename = "message.dat";
   int         i, stat;

   // Process command line arguments
   if (argc > 1) {
      for (i = 1; i < argc; i++) {
         if (!strcmp (argv[i], "-v")) verbose = TRUE;
         else if (!strcmp (argv[i], "-i")) filename = argv[++i];
         else if (!strcmp (argv[i], "-notrace")) trace = FALSE;
         else {
            printf ("usage: reader [-v] [-i <filename>] [-notrace]\n");
            printf ("   -v  verbose mode: print trace info\n");
            printf ("   -i <filename>  read encoded msg from <filename>\n");
            printf ("   -notrace  do not display trace info\n");
            return 1;
         }
      }
   }

   /* Read from input file */
   FILE* pFile = fopen (filename, "rb");
   if (0 == pFile) {
      printf ("Failed to open %s for read access\n", filename);
      return -1;
   }
   fseek (pFile , 0 , SEEK_END);
   size_t lSize = ftell (pFile);
   rewind (pFile);

   OSOCTET* buffer = new OSOCTET [lSize];
   if (0 == buffer) {
      printf ("Memory error\n");
      return -1;
   }

   size_t result = fread (buffer, 1, lSize, pFile);
   if (result != lSize) {
      printf ("Error reading file\n");
      return -1;
   }

   ASN1BERDecodeBuffer decodeBuffer (buffer, lSize);
   ASN1T_Certificate msgData;
   ASN1C_Certificate CertificatePDU (decodeBuffer, msgData);


   stat = CertificatePDU.Decode ();
   if (stat != 0) {
      printf ("decode failed\n");
      decodeBuffer.printErrorInfo ();
      return stat;
   }
   printf ("Decode of data was successful\n");
   printf ("Decoded record:\n");

   delete [] buffer;
   return 0;
}
