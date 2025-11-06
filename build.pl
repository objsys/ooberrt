sub cd {
    local $dir = shift;
    chdir ($dir) || die ("could not chdir to $dir");
}

my $makeprog = 'nmake';
if ($^O !~ /Win/) {
    # assume Linux, set platform.mk file
    `cp -f platform.gnu platform.mk`;
    $makeprog = 'make';
}

mkdir ('./lib') if (! -e './lib');

&cd ('rtbersrc');
`perl mkdepend.pl`;
&cd ('../rtxsrc');
`perl mkdepend.pl`;

&cd ('../build');
`$makeprog`;
