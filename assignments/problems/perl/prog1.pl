#!/usr/bin/perl -w

open(FILE1, "$ARGV[0]") or die "Unable to open ARGV[0]: $!\n";
open(FILE2, "$ARGV[1]") or die "Unable to open ARGV[1]: $!\n";
@file1 = <FILE1>;
@file2 = <FILE2>;

if (length(@file1) >= length(@file2)) {
	$len = @file1;
} else {
	$len = @file2;
}

for ($i = 0; $i < $len; $i++) {
	$l1 = shift @file1;
	$l2 = shift @file2;
	print $l1 if $l1;
	print $l2 if $l2;
}
