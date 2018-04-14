#!/usr/bin/perl -w

$regex = shift;

while(<>) {
	print "$ARGV:$_" if m/$regex/;
}
