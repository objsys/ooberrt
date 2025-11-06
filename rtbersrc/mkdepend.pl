# generate dependency rules file (rules.mk)
# usage: mkdepend [-v]

$g_debug = 0;
$g_cfile = 0;

$numargs = @ARGV;

if ($numargs >= 1 && $ARGV[$numargs - 1] eq "-v") {
    $g_debug = 1;
}

if (-e "rules.mk") {
    `rm -f rules.mk~`;
    `mv rules.mk rules.mk~`;
}

open (OUTFILE, ">rules.mk") ||
    die ("could not open rules.mk for output");

print OUTFILE "# C/C++ to object file compilation rules\n";

# open objects.mk file and use to get list of target objects

open (INFILE, "objects.mk") || die ("could not open objects.mk: $!");

while (<INFILE>) {
    if (/\$\((\w+)\)\$\(PS\)(\w+)\$\(OBJ\)/) {
        $objdir = $1;
        $filebase = $2;
        # print "$objdir $filebase\n";

        # determine source file
        if ( -e "../rtbersrc/$filebase.c" ) {
            &addRules ("../rtbersrc/$filebase.c", $objdir);
        }
        elsif ( -e "../rtbersrc/$filebase.cpp" ) {
            &addRules ("../rtbersrc/$filebase.cpp", $objdir);
        }
        else {
            print "source file for $filebase not found\n";
        }
    }
}

close (INFILE);
close (OUTFILE);

sub addRules {
    local $sourcefile = shift;
    local $objdir = shift;
    local $cppfile = ($sourcefile =~ /\.cpp$/);
    local @rules = (`gcc $sourcefile -E -MM -I. -I..`);

    foreach $line (@rules) {
        # if line starts with a non-whitespace character, it is the
        # start of a new rule ..
        if ($line =~ /^\w/) {
            # change .o to $(OBJ)
            $line =~ s/\.o/\$(OBJ)/;
            $line = '$(' . $objdir . ')$(PS)' . $line;

            # add a newline
            print OUTFILE "\n";

            # get C source file from rule
            # $line =~ /^\w+\$\(OBJ\)\:\s+([\w\.\/]+)/;
            $g_cfile = $sourcefile;
            $g_cfile =~ s/\.\.\/rtbersrc\//\$\(RTBERSRCDIR\)\$\(PS\)/g;
        }

        # change source file paths to use macro names
        $line =~ s/\.\.\/rtsrc\//\$\(RTSRCDIR\)\$\(PS\)/g;
        $line =~ s/\.\.\/rtxsrc\//\$\(RTXSRCDIR\)\$\(PS\)/g;
        $line =~ s/\.\.\/rtbersrc\//\$\(RTBERSRCDIR\)\$\(PS\)/g;

        print OUTFILE $line;
    }

    # add compile command
    if ($g_cfile) {
        if ($cppfile) {
            print OUTFILE
                "\t\$(CCC) \$(CCFLAGS) -c \$(IPATHS) \$(OBJOUT) $g_cfile\n";
        }
        else {
            print OUTFILE
                "\t\$(CC) \$(CFLAGS) -c \$(IPATHS) \$(OBJOUT) $g_cfile\n";
        }
    }
}

