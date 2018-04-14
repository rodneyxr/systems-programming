#!/usr/bin/perl -w

($oldext, $newext, $dir) = @ARGV;

open(FILES, "/bin/ls $dir |") or die "Unable to open $dir: $!\n";

while($file = <FILES>) {
	chop $file;
	next if $file !~ /(.+\.)$oldext$/ or !-f "$dir/$file";
	print "mv $dir/$file $dir/$1$newext\n";	
}
