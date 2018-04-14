#!/usr/bin/perl -w

$regex = shift;
$dir = shift;
$filelist = `ls $dir`;
chop $filelist;
@files = split(" ", $filelist);
while($filename = shift @files) {
	next unless -f "$dir/$filename" && -T "$dir/$filename";
	open(FILE, "$dir/$filename") or die;
	while(<FILE>) {
		chop;
		print "$dir/$filename:$_\n" if /$regex/;
	}
}
