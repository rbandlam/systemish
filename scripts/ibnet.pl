#!/usr/bin/perl

use strict;
use Text::ParseWords;
use Data::Dumper;

my @discover = ();
if(@ARGV[0]) {
    @discover = `/usr/sbin/ibnetdiscover --node-name-map @ARGV[0]`;
} else {
    @discover = `/usr/sbin/ibnetdiscover`;
}
#my @switches = ();
#my @connections = ();

my $switch = '';

open (DOTFILE, '>/tmp/ibnet.dot');

print DOTFILE "digraph ibmap {\n";

for my $a (@discover) {
    my @line = shellwords($a);

    # Check to see if we are at the end of the usable data
    if (@line[0] eq 'Ca') {
	last;
    }

    if (@line[0] eq 'Switch') {
	# At this point, we know that we have a switch line. The switch line has the following:
	# Pos: Description
	# 1: Number of ports on switch
	# 2: Switch GUID
	# 4: Switch Name
	# 9: LID of the switch
#	my $reference = \@line;
#	push(@switches, $reference);
#	$switch = @line[4] . ' (' . @line[9] . ')';
	$switch = @line[4];
    }

    if (@line[0] =~ m/\[/) {
	# Line is a connection descriptor from the preceding server entry.
	# We also know that push above pushes the most recent switch onto the end of the @switches array
	if(@line[3] =~ m/HCA/) {
	    # Whee!
	} else {
#	    print DOTFILE "\"" . $switch . "\" -> \"" . @line[3] . " (" . @line[5] . ")\"\n";
	    print DOTFILE "\"" . $switch . "\" -> \"" . @line[3] . "\"\n";
	}
    }
}

print DOTFILE "}\n";

close (DOTFILE);
