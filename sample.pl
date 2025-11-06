#!/usr/local/bin/perl
# Execute all sample programs.

if ($^O =~ /Win/) {
    $ENV{"PATH"} = $ENV{"PATH"} . ';.;..\..\..\bin';
}
else {
    $ENV{"PATH"} = $ENV{"PATH"} . ':.:../../../bin';

    # set platform.mk file
    `cp -f platform.gnu platform.mk`;
}

select STDERR; $| = 1;
select STDOUT; $| = 1;

my $makeprog = 'make';
if (defined ($ENV{'MAKEPROG'})) {
    $makeprog = $ENV{'MAKEPROG'};
}
elsif (defined ($ENV{'MSVCDIR'}) || defined ($ENV{'VCINSTALLDIR'})) {
    $makeprog = "nmake -nologo";
}

my $function = shift @ARGV;
opendir (SAMPLEDIR, "./sample") ||
    die ("unable to open sample directory");

while (my $filename = readdir(SAMPLEDIR)) {
    if (-d "./sample/$filename" && $filename ne '.' && $filename ne '..') {
        print("$filename\n");
        chdir("./sample/$filename");
        if ($function eq "clean") {
            print("$makeprog clean\n");
            print(`$makeprog clean`);
        }
        else { # assume test
            print("$makeprog\n");
            print(`$makeprog`);

            if (-e "writer.exe" || -e "writer") {
                print ("executing writer ..\n");
                `writer > writer.log`;
                if (-e "./good/writer.log") {
                    print "diff writer.log ./good/writer.log\n";
                    print `diff writer.log ./good/writer.log`;
                }
                print ("writer complete\n");
            }

            if (-e "reader.exe" || -e "reader") {
                print ("executing reader ..\n");
                `reader > reader.log`;
                if (-e "./good/reader.log") {
                    print "diff reader.log ./good/reader.log\n";
                    print `diff reader.log ./good/reader.log`;
                }
                print ("reader complete\n");
            }
        }
        chdir("../..");
    }
}

closedir(SAMPLEDIR);

