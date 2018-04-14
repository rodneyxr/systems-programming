#!/usr/bin/perl -w

while( <> ) {
	push(@lines, $_);
}

@sorted = sort @lines;

foreach $line (@sorted) {
	split /:/, $line;
	print "$_[0]		$_[4]\n";
}
