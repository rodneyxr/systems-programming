#!/usr/bin/perl -w

while($dir = <>) {
	chop $dir;
	open(CORES, "find $dir -name core |") or die;
	while(<CORES>) {
		print "rm $_";
	}
}
