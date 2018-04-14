#!/usr/bin/perl -w

$dir = shift;
open(FILES, "ls $dir |") or die "Unable to open $dir: $!\n";
while( <FILES> ) {
	chop;
	print "$dir\/$_\n" if -d $dir.'/'.$_;
}
