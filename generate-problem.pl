#!/usr/local/bin/perl
#
# Quick and dirty script to create input data for a bin packing problem.

use warnings;
use strict;

my $num_items = $ARGV[0];	# n
my $bin_size  = $ARGV[1];	# K

print 	$num_items . "\n" .
	$bin_size  . "\n";	

for(my $i = 0; $i < $num_items; $i++)
{
	print int(rand($bin_size)+1) . "\n";
}
