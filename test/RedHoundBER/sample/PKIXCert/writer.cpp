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

class MyRDNSequenceCC : public ASN1C_RDNSequence {
public:
   MyRDNSequenceCC (ASN1CType& ccobj, ASN1T_RDNSequence& data) :
      ASN1C_RDNSequence (ccobj, data) {}

   void appendCommonName (const char* name) {
      ASN1T_RelativeDistinguishedName* pRDN = AppendNewElement();
      ASN1C_RelativeDistinguishedName rdnC (*this, *pRDN);
      ASN1T_RelativeDistinguishedName_element* pRDNElem;

      pRDNElem = rdnC.AppendNewElement ();
      pRDNElem->type = id_at_commonName;
      pRDNElem->value.t = 
         PKIX1Explicit_2009_SupportedAttributes::T_at_x520CommonName;
      pRDNElem->value.u.at_x520CommonName = new_ASN1T_X520CommonName (*this);
      pRDNElem->value.u.at_x520CommonName->t = T_X520CommonName_printableString;
      pRDNElem->value.u.at_x520CommonName->u.printableString = name;
   }

   void appendDomainComponent (const char* name) {
      ASN1T_RelativeDistinguishedName* pRDN = AppendNewElement();
      ASN1C_RelativeDistinguishedName rdnC (*this, *pRDN);
      ASN1T_RelativeDistinguishedName_element* pRDNElem;

      pRDNElem = rdnC.AppendNewElement ();
      pRDNElem->type = id_domainComponent;
      pRDNElem->value.t = 
         PKIX1Explicit_2009_SupportedAttributes::T_at_domainComponent;
      pRDNElem->value.u.at_domainComponent = name;
   }
} ;

int main (int argc, char** argv)
{
   const OSOCTET* msgptr;
   OSBOOL       dump = FALSE;
   OSBOOL       trace = TRUE;
   OSBOOL       verbose = FALSE;
   FILE*        fp;
   const char*  filename = "message.dat";
   int          i, len;

   /* Process Command line arguments */
   if (argc > 1) {
      for (i = 1; i < argc; i++) {
         if (!strcmp (argv[i], "-v")) verbose = TRUE;
         else if (!strcmp (argv[i], "-o")) filename = argv[++i];
         else if (!strcmp (argv[i], "-notrace")) trace = FALSE;
         else if (!strcmp (argv[i], "-dump")) dump = TRUE;
         else {
            printf ("usage: writer [ -v ] [ -o <filename>\n");
            printf ("   -v  verbose mode: print trace info\n");
            printf ("   -o <filename>  write encoded msg to <filename>\n");
            printf ("   -notrace  do not display trace info\n");
            printf ("   -dump     generate a hex dump file\n");
            return 1;
         }
      }
   }

   /* Create an instance of the compiler generated class.
      This example uses a dynamic message buffer..*/
   ASN1BEREncodeBuffer encodeBuffer (0,0);
   ASN1T_Certificate msgData;
   ASN1T_Certificate* pvalue = &msgData;
   ASN1C_Certificate CertificatePDU (encodeBuffer, msgData);

   // Populate structure of generated type
   pvalue->toBeSigned.version = PKIX1Explicit_2009_Version::v3;
   pvalue->toBeSigned.serialNumber = "0x1234567890abcdef";

   pvalue->toBeSigned.signature_.algorithm = sha1WithRSAEncryption;
   pvalue->toBeSigned.signature_.m.parametersPresent = 1;
   pvalue->toBeSigned.signature_.parameters.t = 
      PKIX1Explicit_2009_SignatureAlgorithms::T_sa_rsaWithSHA1;

   // Issuer
   ASN1T_RDNSequence issuerRDNSeqValue;
   pvalue->toBeSigned.issuer.t = T_Name_rdnSequence;
   pvalue->toBeSigned.issuer.u.rdnSequence = &issuerRDNSeqValue;

   MyRDNSequenceCC issuerRDNSeq 
      (CertificatePDU, *pvalue->toBeSigned.issuer.u.rdnSequence);

   issuerRDNSeq.appendDomainComponent ("com");
   issuerRDNSeq.appendDomainComponent ("example");
   issuerRDNSeq.appendCommonName ("Example CA");

   // Validity
   pvalue->toBeSigned.validity.notBefore.t = T_Time_utcTime;
   pvalue->toBeSigned.validity.notBefore.u.utcTime = "040430142534Z";

   pvalue->toBeSigned.validity.notAfter.t = T_Time_utcTime;
   pvalue->toBeSigned.validity.notAfter.u.utcTime = "050430142534Z";

   // Subject
   ASN1T_RDNSequence subjectRDNSeqValue;
   pvalue->toBeSigned.subject.t = T_Name_rdnSequence;
   pvalue->toBeSigned.subject.u.rdnSequence = &subjectRDNSeqValue;

   MyRDNSequenceCC subjectRDNSeq 
      (CertificatePDU, *pvalue->toBeSigned.subject.u.rdnSequence);

   subjectRDNSeq.appendDomainComponent ("com");
   subjectRDNSeq.appendDomainComponent ("example");
   subjectRDNSeq.appendCommonName ("Example CA");

   // SubjectPublicKeyInfo
   ASN1T_SubjectPublicKeyInfo* pKeyInfo = 
      &pvalue->toBeSigned.subjectPublicKeyInfo;

   pKeyInfo->algorithm.algorithm = rsaEncryption;

   pKeyInfo->algorithm.m.parametersPresent = 1;
   pKeyInfo->algorithm.parameters.t = 
      PKIX1Explicit_2009_PublicKeyAlgorithms::T_pk_rsa;

   static const OSOCTET key[] = {
      0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xC2,
      0xD7, 0x97, 0x6D, 0x28, 0x70, 0xAA, 0x5B, 0xCF,
      0x23, 0x2E, 0x80, 0x70, 0x39, 0xEE, 0xDB, 0x6F,
      0xD5, 0x2D, 0xD5, 0x6A, 0x4F, 0x7A, 0x34, 0x2D,
      0xF9, 0x22, 0x72, 0x47, 0x70, 0x1D, 0xEF, 0x80,
      0xE9, 0xCA, 0x30, 0x8C, 0x00, 0xC4, 0x9A, 0x6E,
      0x5B, 0x45, 0xB4, 0x6E, 0xA5, 0xE6, 0x6C, 0x94,
      0x0D, 0xFA, 0x91, 0xE9, 0x40, 0xFC, 0x25, 0x9D,
      0xC7, 0xB7, 0x68, 0x19, 0x56, 0x8F, 0x11, 0x70,
      0x6A, 0xD7, 0xF1, 0xC9, 0x11, 0x4F, 0x3A, 0x7E,
      0x3F, 0x99, 0x8D, 0x6E, 0x76, 0xA5, 0x74, 0x5F,
      0x5E, 0xA4, 0x55, 0x53, 0xE5, 0xC7, 0x68, 0x36,
      0x53, 0xC7, 0x1D, 0x3B, 0x12, 0xA6, 0x85, 0xFE,
      0xBD, 0x6E, 0xA1, 0xCA, 0xDF, 0x35, 0x50, 0xAC,
      0x08, 0xD7, 0xB9, 0xB4, 0x7E, 0x5C, 0xFE, 0xE2,
      0xA3, 0x2C, 0xD1, 0x23, 0x84, 0xAA, 0x98, 0xC0,
      0x9B, 0x66, 0x18, 0x9A, 0x68, 0x47, 0xE9, 0x02,
      0x03, 0x01, 0x00, 0x01
   };
   pKeyInfo->subjectPublicKey.numbits = sizeof(key) * 8;
   pKeyInfo->subjectPublicKey.data = (OSOCTET*) key;

   // Extensions
   pvalue->toBeSigned.m._v3ExtPresent = 1;
   pvalue->toBeSigned.m.extensionsPresent = 1;

   ASN1C_TBSCertificate_extensions extHelper 
      (CertificatePDU, pvalue->toBeSigned.extensions);

   ASN1T_TBSCertificate_extensions_element* 
      pExtElem = extHelper.AppendNewElement ();

   static const OSOCTET ext1ValueData[] = {
      0x08, 0x68, 0xaf, 0x85, 0x33, 0xc8, 0x39, 0x4a, 
      0x7a, 0xf8, 0x82, 0x93, 0x8e, 0x70, 0x6a, 0x4a, 
      0x20, 0x84, 0x2c, 0x32
   } ;
   ASN1TDynOctStr ext1Value;
   ext1Value.numocts = sizeof(ext1ValueData);
   ext1Value.data = (OSOCTET*)ext1ValueData;
   pExtElem->extnID = id_ce_subjectKeyIdentifier;
   pExtElem->critical = FALSE;
   pExtElem->extnValue.t = CertExtensions::T_ext_SubjectKeyIdentifier;
   pExtElem->extnValue.u.ext_SubjectKeyIdentifier = &ext1Value;

   pExtElem = extHelper.AppendNewElement ();
   ASN1T_KeyUsage keyUsage;
   ASN1C_KeyUsage keyUsageHelper (keyUsage);
   keyUsageHelper.set (ASN1C_KeyUsage::keyCertSign);
   keyUsageHelper.set (ASN1C_KeyUsage::cRLSign);
   pExtElem->extnID = id_ce_keyUsage;
   pExtElem->critical = TRUE;
   pExtElem->extnValue.t = CertExtensions::T_ext_KeyUsage;
   pExtElem->extnValue.u.ext_KeyUsage = &keyUsage;

   pExtElem = extHelper.AppendNewElement ();
   ASN1T_BasicConstraints basicConstraints;
   basicConstraints.cA = TRUE;
   pExtElem->extnID = id_ce_basicConstraints;
   pExtElem->critical = TRUE;
   pExtElem->extnValue.t = CertExtensions::T_ext_BasicConstraints;
   pExtElem->extnValue.u.ext_BasicConstraints = &basicConstraints;

   pvalue->algorithm.m.parametersPresent = TRUE;
   pvalue->algorithm.algorithm = sha1WithRSAEncryption;
   pvalue->algorithm.parameters.t =
      PKIX1Explicit_2009_SignatureAlgorithms::T_sa_rsaWithSHA1;

   static const OSOCTET mySignature[] = {
      0x6C, 0xF8, 0x02, 0x74, 0xA6, 0x61, 0xE2, 0x64, 
      0x04, 0xA6, 0x54, 0x0C, 0x6C, 0x72, 0x13, 0xAD,
      0x3C, 0x47, 0xFB, 0xF6, 0x65, 0x13, 0xA9, 0x85, 
      0x90, 0x33, 0xEA, 0x76, 0xA3, 0x26, 0xD9, 0xFC,
      0xD1, 0x0E, 0x15, 0x5F, 0x28, 0xB7, 0xEF, 0x93, 
      0xBF, 0x3C, 0xF3, 0xE2, 0x3E, 0x7C, 0xB9, 0x52,
      0xFC, 0x16, 0x6E, 0x29, 0xAA, 0xE1, 0xF4, 0x7A, 
      0x6F, 0xD5, 0x7F, 0xEF, 0xB3, 0x95, 0xCA, 0xF3,
      0x66, 0x88, 0x83, 0x4E, 0xA1, 0x35, 0x45, 0x84, 
      0xCB, 0xBC, 0x9B, 0xB8, 0xC8, 0xAD, 0xC5, 0x5E,
      0x46, 0xD9, 0x0B, 0x0E, 0x8D, 0x80, 0xE1, 0x33, 
      0x2B, 0xDC, 0xBE, 0x2B, 0x92, 0x7E, 0x4A, 0x43,
      0xA9, 0x6A, 0xEF, 0x8A, 0x63, 0x61, 0xB3, 0x6E, 
      0x47, 0x38, 0xBE, 0xE8, 0x0D, 0xA3, 0x67, 0x5D,
      0xF3, 0xFA, 0x91, 0x81, 0x3C, 0x92, 0xBB, 0xC5, 
      0x5F, 0x25, 0x25, 0xEB, 0x7C, 0xE7, 0xD8, 0xA1
   } ;
   pvalue->signature_.numbits = sizeof(mySignature) * 8;
   pvalue->signature_.data = mySignature;

   /* Encode */
   if ((len = CertificatePDU.Encode ()) >= 0) {
      printf ("len = %d\n", len);
      msgptr = encodeBuffer.getMsgPtr ();
      if (trace) {
         printf ("Encoding was successful\n");
         printf ("Hex dump of encoded record:\n");
         for (i = 0; i < len; i++) {
            printf ("%02x ", msgptr[i]);
            if (i > 0 && 0 == i % 16) {
               printf ("\n");
            }
         }
         printf ("\n");
      }
   }
   else {
      printf ("Encoding failed\n");
      encodeBuffer.printErrorInfo ();
      return (-1);
   }

   /* Write the encoded message out to the output file */
   if ((fp = fopen (filename, "wb"))) {
      fwrite (msgptr, 1, len, fp);
      fclose (fp);
   }

   return 0;
}
