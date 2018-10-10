#!/usr/bin/perl

open(my $f, ">:raw", "t/test.def") or die $!;
@ARGV = <t/*.cpp>;
while (<>) {
    /^\s*void\s+(test_\w*)\s*\(\s*\)/ and print $f "DIAG(\"$1\"); void $1(); $1();\n";
}
