# Create distribution kit for C only

use strict;

sub copyRtSrcFiles {
    my $subdir = shift;
    mkdir ("./ooberrt/$subdir");
    `cp ./$subdir/*.c ./ooberrt/$subdir`;
    `cp ./$subdir/objects.mk ./ooberrt/$subdir`;
    `cp ./$subdir/rules.mk ./ooberrt/$subdir`;
}


my $clean = 1;
my $numargs = @ARGV;

if ($numargs > 0 && $ARGV[0] eq '-noclean') {
    $clean = 0;
}
`rm -rf ooberrt` if (-e 'ooberrt');

chdir ('./rtbersrc');
`perl mkdepend.pl`;
chdir ('../rtxsrc');
`perl mkdepend.pl`;
chdir ('..');

mkdir ("./ooberrt");
mkdir ("./ooberrt/build");
mkdir ("./ooberrt/lib");
mkdir ("./ooberrt/rtsrc");
mkdir ("./ooberrt/sample");
mkdir ("./ooberrt/sample/employee");

&copyRtSrcFiles ("rtbersrc");
&copyRtSrcFiles ("rtxsrc");
`cp ./rtbersrc/asn1ber.h ./ooberrt/rtbersrc`;
`cp ./rtsrc/rtBCD.h ./ooberrt/rtsrc`;
`cp ./rtsrc/rtPrint.h ./ooberrt/rtsrc`;
`cp ./rtxsrc/rtxCharStr.h ./ooberrt/rtxsrc`;
`cp ./rtxsrc/rtxCommon.h ./ooberrt/rtxsrc`;
`cp ./rtxsrc/rtxEnum.h ./ooberrt/rtxsrc`;
`cp ./rtxsrc/rtxPrint.h ./ooberrt/rtxsrc`;
`cp ./rtxsrc/systypes.h ./ooberrt/rtxsrc`;
`cp ./build/makefile ./ooberrt/build`;
`cp ./sample/employee/* ./ooberrt/sample/employee`;
`cp platform.* ./ooberrt`;
`cp COPYING.txt ./ooberrt`;
`cp README.txt ./ooberrt`;

`rm -f ooberrt.zip`;
`zip -r ooberrt.zip ooberrt`;
`rm -rf ooberrt` if ($clean);

