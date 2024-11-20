#!/usr/bin/perl
use strict;
use warnings;

# Log file to analyze
my $log_file = "system_log.txt";

# Open the log file
open my $log_fh, '<', $log_file or die "Cannot open $log_file: $!";

# Initialize counters
my $total_matched = 0;
my $total_completed = 0;
my $cancelled_before_driver = 0;
my $cancelled_after_driver = 0;
my $cancelled_before_rider = 0;
my $cancelled_after_rider = 0;

# Parse the log file
while (my $line = <$log_fh>) {
    chomp $line;

    # Check for matched rides
    if ($line =~ /Matched RideRequest/) {
        $total_matched++;
    }

    # Check for completed rides
    if ($line =~ /Ride \w+ completed/) {
        $total_completed++;
    }

    # Check for rides canceled before starting by driver
    if ($line =~ /cancelled by Driver before starting/) {
        $cancelled_before_driver++;
    }

    # Check for rides canceled after starting by driver
    if ($line =~ /cancelled by Driver after starting/) {
        $cancelled_after_driver++;
    }

    # Check for rides canceled before starting by rider
    if ($line =~ /cancelled by Rider before starting/) {
        $cancelled_before_rider++;
    }

    # Check for rides canceled after starting by rider
    if ($line =~ /cancelled by Rider after starting/) {
        $cancelled_after_rider++;
    }
}

close $log_fh;

# Print summary
print "Ride Analysis Summary:\n";
print "----------------------\n";
print "Total Rides Matched: $total_matched\n";
print "Total Rides Completed: $total_completed\n";
print "Total Rides Cancelled Before Starting by Driver: $cancelled_before_driver\n";
print "Total Rides Cancelled After Starting by Driver: $cancelled_after_driver\n";
print "Total Rides Cancelled Before Starting by Rider: $cancelled_before_rider\n";
print "Total Rides Cancelled After Starting by Rider: $cancelled_after_rider\n";

