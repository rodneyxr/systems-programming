#!/usr/bin/perl -w

while($dir = shift) {
	open(FILES, "find $dir -name core |") or die;
	while($file = <FILES>) {
		print "rm $file";
	}
}