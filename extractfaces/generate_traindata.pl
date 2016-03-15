#!/usr/bin/perl

open(FILE, "../filelist.txt");
chomp(@filelist = (<FILE>));
close(FILE);

foreach $file (@filelist) {
    @filecomps = split("/", $file);
	$filename = $filecomps[-1];
	print $filename."\n";
	system("./extractfaces ../$file faces/$filename");
}
