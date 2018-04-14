#!/usr/bin/perl -w
open(FILES, "find $ARGV[0] |") or die "";
@files = <FILES>;
shift @files;
print sort @files;
