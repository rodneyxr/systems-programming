#!/usr/bin/perl -w

$dir = shift;
open(FILES, "ls $dir |") or die;

while( <FILES> ) {
	chop;
	$dir .= '/' if $dir !~ /\/$/;
	print "$dir$_\n" if -f $dir.$_;
}
