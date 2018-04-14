#!/usr/bin/perl -w

while(<>) {
	chop;
	open(FILEIN, $_) if -e;
	print <FILEIN>;	
}
