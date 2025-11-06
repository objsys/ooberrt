# makefile to build generated files

include ../../../platform.mk

# include directories
OSROOTDIR = ../../..
RTLIBDIR  = $(OSROOTDIR)/lib
RTSRCDIR = $(OSROOTDIR)/rtsrc
RTXSRCDIR = $(OSROOTDIR)/rtxsrc
BERSRCDIR = $(OSROOTDIR)/rtbersrc
LIBDIR = ..\lib
SRCDIR = ..\src
HFILESDIR = ..\src
HFILES = $(RTXSRCDIR)/rtxCommon.h $(RTXSRCDIR)/OSRTContext.h \
$(RTXSRCDIR)/OSRTMsgBuf.h $(RTXSRCDIR)/OSRTMsgBufIF.h \
$(RTSRCDIR)/asn1CppTypes.h  $(RTSRCDIR)/asn1CSeqOfList.h \
$(RTSRCDIR)/asn1CBitStr.h $(BERSRCDIR)/asn1ber.h \
$(BERSRCDIR)/asn1BerCppTypes.h 
# compiler defs
CFLAGS = -c $(CVARS_) $(CFLAGS_) $(CBLDTYPE_)
CCFLAGS = $(CFLAGS)
IPATHS = -I$(SRCDIR) -I$(OSROOTDIR) -I$(HFILESDIR) -I$(RTSRCDIR) -I$(BERSRCDIR) $(IPATHS_)
LINKOPT = $(LINKOPT_) /Fm

# run-time libraries
RHDERLIB = $(LIBPFX)rhder_a.$(LIBEXT)

OBJECTS =  \
AlgorithmInformation-2009$(OBJ) \
AsymmetricKeyPackageModuleV1$(OBJ) \
PKIX-X400Address-2009$(OBJ) \
PKIX1Explicit-2009$(OBJ) \
PKIX1Implicit-2009$(OBJ) \
AttributeCertificateVersion1-2009$(OBJ) \
BinarySigningTimeModule-2010$(OBJ) \
CMS-AES-CCM-and-AES-GCM-2009$(OBJ) \
CMSAesRsaesOaep-2009$(OBJ) \
CMS-AuthEnvelopedData-2010$(OBJ) \
PKIXAlgs-2009$(OBJ) \
CryptographicMessageSyntaxAlgorithms-2009$(OBJ) \
CryptographicMessageSyntax-2010$(OBJ) \
CMSContentConstraintsCertExtn$(OBJ) \
CMSECCAlgs-2009-02$(OBJ) \
CompressedDataContent-2010$(OBJ) \
ContentCollectionModule-2010$(OBJ) \
CryptographicMessageSyntax-2010-shallow$(OBJ) \
DSSC$(OBJ) \
EncryptedKeyPackageModuleV1$(OBJ) \
PKIXCRMF-2009$(OBJ) \
EnrollmentMessageSyntax-2009$(OBJ) \
ExtendedSecurityServices-2009$(OBJ) \
MultipleSignatures-2010$(OBJ) \
OCSP-2009$(OBJ) \
PKCS-10$(OBJ) \
PKIX1-PSS-OAEP-Algorithms-2009$(OBJ) \
PKIXAttributeCertificate-2009$(OBJ) \
PKIXproxy88$(OBJ) \
PSKCAttributesModule$(OBJ) \
SCVP-2009$(OBJ) \
SecureMimeMessageV3dot1-2009$(OBJ) \
SymmetricKeyPackageModulev1$(OBJ) \
TAMP-Protocol-v2$(OBJ) \
TrustAnchorInfoModule$(OBJ)

ENCOBJECTS =  \
AlgorithmInformation-2009Enc$(OBJ) \
AsymmetricKeyPackageModuleV1Enc$(OBJ) \
PKIX-X400Address-2009Enc$(OBJ) \
PKIX1Explicit-2009Enc$(OBJ) \
PKIX1Implicit-2009Enc$(OBJ) \
AttributeCertificateVersion1-2009Enc$(OBJ) \
BinarySigningTimeModule-2010Enc$(OBJ) \
CMS-AES-CCM-and-AES-GCM-2009Enc$(OBJ) \
CMSAesRsaesOaep-2009Enc$(OBJ) \
CMS-AuthEnvelopedData-2010Enc$(OBJ) \
PKIXAlgs-2009Enc$(OBJ) \
CryptographicMessageSyntaxAlgorithms-2009Enc$(OBJ) \
CryptographicMessageSyntax-2010Enc$(OBJ) \
CMSContentConstraintsCertExtnEnc$(OBJ) \
CMSECCAlgs-2009-02Enc$(OBJ) \
CompressedDataContent-2010Enc$(OBJ) \
ContentCollectionModule-2010Enc$(OBJ) \
CryptographicMessageSyntax-2010-shallowEnc$(OBJ) \
DSSCEnc$(OBJ) \
EncryptedKeyPackageModuleV1Enc$(OBJ) \
PKIXCRMF-2009Enc$(OBJ) \
EnrollmentMessageSyntax-2009Enc$(OBJ) \
ExtendedSecurityServices-2009Enc$(OBJ) \
MultipleSignatures-2010Enc$(OBJ) \
OCSP-2009Enc$(OBJ) \
PKCS-10Enc$(OBJ) \
PKIX1-PSS-OAEP-Algorithms-2009Enc$(OBJ) \
PKIXAttributeCertificate-2009Enc$(OBJ) \
PKIXproxy88Enc$(OBJ) \
PSKCAttributesModuleEnc$(OBJ) \
SCVP-2009Enc$(OBJ) \
SecureMimeMessageV3dot1-2009Enc$(OBJ) \
SymmetricKeyPackageModulev1Enc$(OBJ) \
TAMP-Protocol-v2Enc$(OBJ) \
TrustAnchorInfoModuleEnc$(OBJ)

DECOBJECTS =  \
AlgorithmInformation-2009Dec$(OBJ) \
AsymmetricKeyPackageModuleV1Dec$(OBJ) \
PKIX-X400Address-2009Dec$(OBJ) \
PKIX1Explicit-2009Dec$(OBJ) \
PKIX1Implicit-2009Dec$(OBJ) \
AttributeCertificateVersion1-2009Dec$(OBJ) \
BinarySigningTimeModule-2010Dec$(OBJ) \
CMS-AES-CCM-and-AES-GCM-2009Dec$(OBJ) \
CMSAesRsaesOaep-2009Dec$(OBJ) \
CMS-AuthEnvelopedData-2010Dec$(OBJ) \
PKIXAlgs-2009Dec$(OBJ) \
CryptographicMessageSyntaxAlgorithms-2009Dec$(OBJ) \
CryptographicMessageSyntax-2010Dec$(OBJ) \
CMSContentConstraintsCertExtnDec$(OBJ) \
CMSECCAlgs-2009-02Dec$(OBJ) \
CompressedDataContent-2010Dec$(OBJ) \
ContentCollectionModule-2010Dec$(OBJ) \
CryptographicMessageSyntax-2010-shallowDec$(OBJ) \
DSSCDec$(OBJ) \
EncryptedKeyPackageModuleV1Dec$(OBJ) \
PKIXCRMF-2009Dec$(OBJ) \
EnrollmentMessageSyntax-2009Dec$(OBJ) \
ExtendedSecurityServices-2009Dec$(OBJ) \
MultipleSignatures-2010Dec$(OBJ) \
OCSP-2009Dec$(OBJ) \
PKCS-10Dec$(OBJ) \
PKIX1-PSS-OAEP-Algorithms-2009Dec$(OBJ) \
PKIXAttributeCertificate-2009Dec$(OBJ) \
PKIXproxy88Dec$(OBJ) \
PSKCAttributesModuleDec$(OBJ) \
SCVP-2009Dec$(OBJ) \
SecureMimeMessageV3dot1-2009Dec$(OBJ) \
SymmetricKeyPackageModulev1Dec$(OBJ) \
TAMP-Protocol-v2Dec$(OBJ) \
TrustAnchorInfoModuleDec$(OBJ)

all : $(LIBDIR)/$(RHDERLIB)

$(LIBDIR)/$(RHDERLIB) : $(OBJECTS) $(ENCOBJECTS) $(DECOBJECTS)
	$(LIBCMD) $(OBJECTS) $(ENCOBJECTS) $(DECOBJECTS)

# .cpp -> .obj rules
# common file rules
AlgorithmInformation-2009$(OBJ) : $(SRCDIR)/AlgorithmInformation-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AlgorithmInformation-2009.cpp

# decode file rules
AlgorithmInformation-2009Dec$(OBJ) : $(SRCDIR)/AlgorithmInformation-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AlgorithmInformation-2009Dec.cpp

# encode file rules
AlgorithmInformation-2009Enc$(OBJ) : $(SRCDIR)/AlgorithmInformation-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AlgorithmInformation-2009Enc.cpp

# common file rules
AsymmetricKeyPackageModuleV1$(OBJ) : $(SRCDIR)/AsymmetricKeyPackageModuleV1.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AsymmetricKeyPackageModuleV1.cpp

# decode file rules
AsymmetricKeyPackageModuleV1Dec$(OBJ) : $(SRCDIR)/AsymmetricKeyPackageModuleV1Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AsymmetricKeyPackageModuleV1Dec.cpp

# encode file rules
AsymmetricKeyPackageModuleV1Enc$(OBJ) : $(SRCDIR)/AsymmetricKeyPackageModuleV1Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AsymmetricKeyPackageModuleV1Enc.cpp

# common file rules
PKIX-X400Address-2009$(OBJ) : $(SRCDIR)/PKIX-X400Address-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX-X400Address-2009.cpp

# decode file rules
PKIX-X400Address-2009Dec$(OBJ) : $(SRCDIR)/PKIX-X400Address-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX-X400Address-2009Dec.cpp

# encode file rules
PKIX-X400Address-2009Enc$(OBJ) : $(SRCDIR)/PKIX-X400Address-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX-X400Address-2009Enc.cpp

# common file rules
PKIX1Explicit-2009$(OBJ) : $(SRCDIR)/PKIX1Explicit-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Explicit-2009.cpp

# decode file rules
PKIX1Explicit-2009Dec$(OBJ) : $(SRCDIR)/PKIX1Explicit-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Explicit-2009Dec.cpp

# encode file rules
PKIX1Explicit-2009Enc$(OBJ) : $(SRCDIR)/PKIX1Explicit-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Explicit-2009Enc.cpp

# common file rules
PKIX1Implicit-2009$(OBJ) : $(SRCDIR)/PKIX1Implicit-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Implicit-2009.cpp

# decode file rules
PKIX1Implicit-2009Dec$(OBJ) : $(SRCDIR)/PKIX1Implicit-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Implicit-2009Dec.cpp

# encode file rules
PKIX1Implicit-2009Enc$(OBJ) : $(SRCDIR)/PKIX1Implicit-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1Implicit-2009Enc.cpp

# common file rules
AttributeCertificateVersion1-2009$(OBJ) : $(SRCDIR)/AttributeCertificateVersion1-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AttributeCertificateVersion1-2009.cpp

# decode file rules
AttributeCertificateVersion1-2009Dec$(OBJ) : $(SRCDIR)/AttributeCertificateVersion1-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AttributeCertificateVersion1-2009Dec.cpp

# encode file rules
AttributeCertificateVersion1-2009Enc$(OBJ) : $(SRCDIR)/AttributeCertificateVersion1-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/AttributeCertificateVersion1-2009Enc.cpp

# common file rules
BinarySigningTimeModule-2010$(OBJ) : $(SRCDIR)/BinarySigningTimeModule-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/BinarySigningTimeModule-2010.cpp

# decode file rules
BinarySigningTimeModule-2010Dec$(OBJ) : $(SRCDIR)/BinarySigningTimeModule-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/BinarySigningTimeModule-2010Dec.cpp

# encode file rules
BinarySigningTimeModule-2010Enc$(OBJ) : $(SRCDIR)/BinarySigningTimeModule-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/BinarySigningTimeModule-2010Enc.cpp

# common file rules
CMS-AES-CCM-and-AES-GCM-2009$(OBJ) : $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009.cpp

# decode file rules
CMS-AES-CCM-and-AES-GCM-2009Dec$(OBJ) : $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009Dec.cpp

# encode file rules
CMS-AES-CCM-and-AES-GCM-2009Enc$(OBJ) : $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AES-CCM-and-AES-GCM-2009Enc.cpp

# common file rules
CMSAesRsaesOaep-2009$(OBJ) : $(SRCDIR)/CMSAesRsaesOaep-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSAesRsaesOaep-2009.cpp

# decode file rules
CMSAesRsaesOaep-2009Dec$(OBJ) : $(SRCDIR)/CMSAesRsaesOaep-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSAesRsaesOaep-2009Dec.cpp

# encode file rules
CMSAesRsaesOaep-2009Enc$(OBJ) : $(SRCDIR)/CMSAesRsaesOaep-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSAesRsaesOaep-2009Enc.cpp

# common file rules
CMS-AuthEnvelopedData-2010$(OBJ) : $(SRCDIR)/CMS-AuthEnvelopedData-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AuthEnvelopedData-2010.cpp

# decode file rules
CMS-AuthEnvelopedData-2010Dec$(OBJ) : $(SRCDIR)/CMS-AuthEnvelopedData-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AuthEnvelopedData-2010Dec.cpp

# encode file rules
CMS-AuthEnvelopedData-2010Enc$(OBJ) : $(SRCDIR)/CMS-AuthEnvelopedData-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMS-AuthEnvelopedData-2010Enc.cpp

# common file rules
PKIXAlgs-2009$(OBJ) : $(SRCDIR)/PKIXAlgs-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAlgs-2009.cpp

# decode file rules
PKIXAlgs-2009Dec$(OBJ) : $(SRCDIR)/PKIXAlgs-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAlgs-2009Dec.cpp

# encode file rules
PKIXAlgs-2009Enc$(OBJ) : $(SRCDIR)/PKIXAlgs-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAlgs-2009Enc.cpp

# common file rules
CryptographicMessageSyntaxAlgorithms-2009$(OBJ) : $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009.cpp

# decode file rules
CryptographicMessageSyntaxAlgorithms-2009Dec$(OBJ) : $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009Dec.cpp

# encode file rules
CryptographicMessageSyntaxAlgorithms-2009Enc$(OBJ) : $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntaxAlgorithms-2009Enc.cpp

# common file rules
CryptographicMessageSyntax-2010$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010.cpp

# decode file rules
CryptographicMessageSyntax-2010Dec$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010Dec.cpp

# encode file rules
CryptographicMessageSyntax-2010Enc$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010Enc.cpp

# common file rules
CMSContentConstraintsCertExtn$(OBJ) : $(SRCDIR)/CMSContentConstraintsCertExtn.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSContentConstraintsCertExtn.cpp

# decode file rules
CMSContentConstraintsCertExtnDec$(OBJ) : $(SRCDIR)/CMSContentConstraintsCertExtnDec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSContentConstraintsCertExtnDec.cpp

# encode file rules
CMSContentConstraintsCertExtnEnc$(OBJ) : $(SRCDIR)/CMSContentConstraintsCertExtnEnc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSContentConstraintsCertExtnEnc.cpp

# common file rules
CMSECCAlgs-2009-02$(OBJ) : $(SRCDIR)/CMSECCAlgs-2009-02.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSECCAlgs-2009-02.cpp

# decode file rules
CMSECCAlgs-2009-02Dec$(OBJ) : $(SRCDIR)/CMSECCAlgs-2009-02Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSECCAlgs-2009-02Dec.cpp

# encode file rules
CMSECCAlgs-2009-02Enc$(OBJ) : $(SRCDIR)/CMSECCAlgs-2009-02Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CMSECCAlgs-2009-02Enc.cpp

# common file rules
CompressedDataContent-2010$(OBJ) : $(SRCDIR)/CompressedDataContent-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CompressedDataContent-2010.cpp

# decode file rules
CompressedDataContent-2010Dec$(OBJ) : $(SRCDIR)/CompressedDataContent-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CompressedDataContent-2010Dec.cpp

# encode file rules
CompressedDataContent-2010Enc$(OBJ) : $(SRCDIR)/CompressedDataContent-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CompressedDataContent-2010Enc.cpp

# common file rules
ContentCollectionModule-2010$(OBJ) : $(SRCDIR)/ContentCollectionModule-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ContentCollectionModule-2010.cpp

# decode file rules
ContentCollectionModule-2010Dec$(OBJ) : $(SRCDIR)/ContentCollectionModule-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ContentCollectionModule-2010Dec.cpp

# encode file rules
ContentCollectionModule-2010Enc$(OBJ) : $(SRCDIR)/ContentCollectionModule-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ContentCollectionModule-2010Enc.cpp

# common file rules
CryptographicMessageSyntax-2010-shallow$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010-shallow.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010-shallow.cpp

# decode file rules
CryptographicMessageSyntax-2010-shallowDec$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010-shallowDec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010-shallowDec.cpp

# encode file rules
CryptographicMessageSyntax-2010-shallowEnc$(OBJ) : $(SRCDIR)/CryptographicMessageSyntax-2010-shallowEnc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/CryptographicMessageSyntax-2010-shallowEnc.cpp

# common file rules
DSSC$(OBJ) : $(SRCDIR)/DSSC.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/DSSC.cpp

# decode file rules
DSSCDec$(OBJ) : $(SRCDIR)/DSSCDec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/DSSCDec.cpp

# encode file rules
DSSCEnc$(OBJ) : $(SRCDIR)/DSSCEnc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/DSSCEnc.cpp

# common file rules
EncryptedKeyPackageModuleV1$(OBJ) : $(SRCDIR)/EncryptedKeyPackageModuleV1.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EncryptedKeyPackageModuleV1.cpp

# decode file rules
EncryptedKeyPackageModuleV1Dec$(OBJ) : $(SRCDIR)/EncryptedKeyPackageModuleV1Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EncryptedKeyPackageModuleV1Dec.cpp

# encode file rules
EncryptedKeyPackageModuleV1Enc$(OBJ) : $(SRCDIR)/EncryptedKeyPackageModuleV1Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EncryptedKeyPackageModuleV1Enc.cpp

# common file rules
PKIXCRMF-2009$(OBJ) : $(SRCDIR)/PKIXCRMF-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXCRMF-2009.cpp

# decode file rules
PKIXCRMF-2009Dec$(OBJ) : $(SRCDIR)/PKIXCRMF-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXCRMF-2009Dec.cpp

# encode file rules
PKIXCRMF-2009Enc$(OBJ) : $(SRCDIR)/PKIXCRMF-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXCRMF-2009Enc.cpp

# common file rules
EnrollmentMessageSyntax-2009$(OBJ) : $(SRCDIR)/EnrollmentMessageSyntax-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EnrollmentMessageSyntax-2009.cpp

# decode file rules
EnrollmentMessageSyntax-2009Dec$(OBJ) : $(SRCDIR)/EnrollmentMessageSyntax-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EnrollmentMessageSyntax-2009Dec.cpp

# encode file rules
EnrollmentMessageSyntax-2009Enc$(OBJ) : $(SRCDIR)/EnrollmentMessageSyntax-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/EnrollmentMessageSyntax-2009Enc.cpp

# common file rules
ExtendedSecurityServices-2009$(OBJ) : $(SRCDIR)/ExtendedSecurityServices-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ExtendedSecurityServices-2009.cpp

# decode file rules
ExtendedSecurityServices-2009Dec$(OBJ) : $(SRCDIR)/ExtendedSecurityServices-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ExtendedSecurityServices-2009Dec.cpp

# encode file rules
ExtendedSecurityServices-2009Enc$(OBJ) : $(SRCDIR)/ExtendedSecurityServices-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/ExtendedSecurityServices-2009Enc.cpp

# common file rules
MultipleSignatures-2010$(OBJ) : $(SRCDIR)/MultipleSignatures-2010.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/MultipleSignatures-2010.cpp

# decode file rules
MultipleSignatures-2010Dec$(OBJ) : $(SRCDIR)/MultipleSignatures-2010Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/MultipleSignatures-2010Dec.cpp

# encode file rules
MultipleSignatures-2010Enc$(OBJ) : $(SRCDIR)/MultipleSignatures-2010Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/MultipleSignatures-2010Enc.cpp

# common file rules
OCSP-2009$(OBJ) : $(SRCDIR)/OCSP-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/OCSP-2009.cpp

# decode file rules
OCSP-2009Dec$(OBJ) : $(SRCDIR)/OCSP-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/OCSP-2009Dec.cpp

# encode file rules
OCSP-2009Enc$(OBJ) : $(SRCDIR)/OCSP-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/OCSP-2009Enc.cpp

# common file rules
PKCS-10$(OBJ) : $(SRCDIR)/PKCS-10.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKCS-10.cpp

# decode file rules
PKCS-10Dec$(OBJ) : $(SRCDIR)/PKCS-10Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKCS-10Dec.cpp

# encode file rules
PKCS-10Enc$(OBJ) : $(SRCDIR)/PKCS-10Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKCS-10Enc.cpp

# common file rules
PKIX1-PSS-OAEP-Algorithms-2009$(OBJ) : $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009.cpp

# decode file rules
PKIX1-PSS-OAEP-Algorithms-2009Dec$(OBJ) : $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009Dec.cpp

# encode file rules
PKIX1-PSS-OAEP-Algorithms-2009Enc$(OBJ) : $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIX1-PSS-OAEP-Algorithms-2009Enc.cpp

# common file rules
PKIXAttributeCertificate-2009$(OBJ) : $(SRCDIR)/PKIXAttributeCertificate-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAttributeCertificate-2009.cpp

# decode file rules
PKIXAttributeCertificate-2009Dec$(OBJ) : $(SRCDIR)/PKIXAttributeCertificate-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAttributeCertificate-2009Dec.cpp

# encode file rules
PKIXAttributeCertificate-2009Enc$(OBJ) : $(SRCDIR)/PKIXAttributeCertificate-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXAttributeCertificate-2009Enc.cpp

# common file rules
PKIXproxy88$(OBJ) : $(SRCDIR)/PKIXproxy88.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXproxy88.cpp

# decode file rules
PKIXproxy88Dec$(OBJ) : $(SRCDIR)/PKIXproxy88Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXproxy88Dec.cpp

# encode file rules
PKIXproxy88Enc$(OBJ) : $(SRCDIR)/PKIXproxy88Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PKIXproxy88Enc.cpp

# common file rules
PSKCAttributesModule$(OBJ) : $(SRCDIR)/PSKCAttributesModule.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PSKCAttributesModule.cpp

# decode file rules
PSKCAttributesModuleDec$(OBJ) : $(SRCDIR)/PSKCAttributesModuleDec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PSKCAttributesModuleDec.cpp

# encode file rules
PSKCAttributesModuleEnc$(OBJ) : $(SRCDIR)/PSKCAttributesModuleEnc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/PSKCAttributesModuleEnc.cpp

# common file rules
SCVP-2009$(OBJ) : $(SRCDIR)/SCVP-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SCVP-2009.cpp

# decode file rules
SCVP-2009Dec$(OBJ) : $(SRCDIR)/SCVP-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SCVP-2009Dec.cpp

# encode file rules
SCVP-2009Enc$(OBJ) : $(SRCDIR)/SCVP-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SCVP-2009Enc.cpp

# common file rules
SecureMimeMessageV3dot1-2009$(OBJ) : $(SRCDIR)/SecureMimeMessageV3dot1-2009.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SecureMimeMessageV3dot1-2009.cpp

# decode file rules
SecureMimeMessageV3dot1-2009Dec$(OBJ) : $(SRCDIR)/SecureMimeMessageV3dot1-2009Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SecureMimeMessageV3dot1-2009Dec.cpp

# encode file rules
SecureMimeMessageV3dot1-2009Enc$(OBJ) : $(SRCDIR)/SecureMimeMessageV3dot1-2009Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SecureMimeMessageV3dot1-2009Enc.cpp

# common file rules
SymmetricKeyPackageModulev1$(OBJ) : $(SRCDIR)/SymmetricKeyPackageModulev1.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SymmetricKeyPackageModulev1.cpp

# decode file rules
SymmetricKeyPackageModulev1Dec$(OBJ) : $(SRCDIR)/SymmetricKeyPackageModulev1Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SymmetricKeyPackageModulev1Dec.cpp

# encode file rules
SymmetricKeyPackageModulev1Enc$(OBJ) : $(SRCDIR)/SymmetricKeyPackageModulev1Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/SymmetricKeyPackageModulev1Enc.cpp

# common file rules
TAMP-Protocol-v2$(OBJ) : $(SRCDIR)/TAMP-Protocol-v2.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TAMP-Protocol-v2.cpp

# decode file rules
TAMP-Protocol-v2Dec$(OBJ) : $(SRCDIR)/TAMP-Protocol-v2Dec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TAMP-Protocol-v2Dec.cpp

# encode file rules
TAMP-Protocol-v2Enc$(OBJ) : $(SRCDIR)/TAMP-Protocol-v2Enc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TAMP-Protocol-v2Enc.cpp

# common file rules
TrustAnchorInfoModule$(OBJ) : $(SRCDIR)/TrustAnchorInfoModule.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TrustAnchorInfoModule.cpp

# decode file rules
TrustAnchorInfoModuleDec$(OBJ) : $(SRCDIR)/TrustAnchorInfoModuleDec.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TrustAnchorInfoModuleDec.cpp

# encode file rules
TrustAnchorInfoModuleEnc$(OBJ) : $(SRCDIR)/TrustAnchorInfoModuleEnc.cpp \
$(HFILESDIR)/ridgeback_der.h $(HFILES)
	$(CCC) $(CCFLAGS) -c $(IPATHS) $(SRCDIR)/TrustAnchorInfoModuleEnc.cpp

clean:
	$(RM) $(SRCDIR)\*.cpp
	$(RM) $(HFILESDIR)\ridgeback_der.h 
	$(RM) *$(OBJ)
	$(RM) *~
	$(RM) *.pdb
