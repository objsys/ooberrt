Objective Open BER Run-time (ooberrt)

The Objective Open BER Run-time is an open source C BER/DER run-time
library that may be used with source code generated from the
Objective Systems ASN1C compiler.  The library does not provide full
support for the X.680 standard, but provides support for the most
common items used in many ASN.1 specifications.  These include security
specifications such as PKIX, CMS and PKCS; a number of telephony call
detail record (CDR) formats such as TAP3, NRTRDE, and various vendor
switch specifications; and other simple specifications that make use
of the BER/DER encoding rules.

Particular ASN.1 types known to not be supported in this library include
REAL, EXTERNAL, EMBEDDED-PDV, Restricted Character String, and the
newer date/time and IRI types.  In addition, the libary is known to
be not as performant as our standard, commercial BER/DER/CER library.
Nonetheless, it should prove to be adequate for a variety of applications.

Building on Windows using Visual Studio

To build the library for Windows, unzip the package into the root directory
of an ASN1C installation and cd to the build subdirectory.  Execute the nmake
command to run the Visual Studio nmake utility to build the package. Visual
Studio C/C++ compilation flags are located in the platform.mk file in the root
directory.

Building on Linux

To build the library for Linux, unzip the package into the root directory of
an ASN1C installation.  Rename or delete the platform.mk file in the ooberrt
directory and then rename the platform.gnu file to platform.mk:

rm -f platform.mk
mv platform.gnu platform.mk

Then cd to the build subdirectory and execute make.

Note that source files in the package are stored in Windows (DOS) format.
It make be necessary to convert to UNIX format on some systems.
A utility program such as dos2unix can be used to convert the files
to UNIX format if necessary (Google dos2unix for details)

Using the compiled library

To use the library, it should be copied into a subdirectory under the
main ASN1C installation root directory.  Sample programs can be modified
to use the library by editing the associated makefile and changing the
path to the library file to reference the ooberrt_a.lib file (Windows)
or libooberrt.a file (Linux or Mac OSX).

License

The COPYING.txt file within this package contains information on the
open source license.  This license allows open source redistribution
of the source and binary files in the library; however, it does not
grant permission to redistribute applications containing code generated
with the ASN1C compiler.  For that, a license to the ASN1C SDK (any
configuration) is required.

