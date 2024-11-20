#!/usr/bin/perl
use strict;
use warnings;
use List::Util qw(shuffle);

# Parameters for data generation
my $num_drivers = 5;   # Number of drivers
my $num_riders = 6;    # Number of riders
my $num_rides = 50;     # Number of rides

# Random coordinate range
my $min_coord = 0;
my $max_coord = 20;

# Generate drivers
open my $drivers_fh, '>', 'drivers.txt' or die "Cannot open drivers.txt: $!";
for my $i (1 .. $num_drivers) {
    my $driver_id = sprintf("D%03d", $i);
    my $name = "Driver_$i";
    my $x = int(rand($max_coord - $min_coord)) + $min_coord;
    my $y = int(rand($max_coord - $min_coord)) + $min_coord;
    my $availability = int(rand(2)); # 0 or 1
    print $drivers_fh "$driver_id,$name,$x,$y,$availability\n";
}
close $drivers_fh;
print "Generated drivers.txt\n";

# Generate riders
open my $riders_fh, '>', 'riders.txt' or die "Cannot open riders.txt: $!";
for my $i (1 .. $num_riders) {
    my $rider_id = sprintf("R%03d", $i);
    my $name = "Rider_$i";
    print $riders_fh "$rider_id,$name\n";
}
close $riders_fh;
print "Generated riders.txt\n";

# Generate rides
open my $rides_fh, '>', 'rides.txt' or die "Cannot open rides.txt: $!";
my @rider_ids = map { sprintf("R%03d", $_) } 1 .. $num_riders;
my @shuffled_rider_ids = shuffle(@rider_ids);

for my $i (1 .. $num_rides) {
    my $request_id = sprintf("REQ%03d", $i);
    my $rider_id = $shuffled_rider_ids[($i - 1) % @shuffled_rider_ids];
    my $pickup_x = int(rand($max_coord - $min_coord)) + $min_coord;
    my $pickup_y = int(rand($max_coord - $min_coord)) + $min_coord;
    my $dropoff_x = int(rand($max_coord - $min_coord)) + $min_coord;
    my $dropoff_y = int(rand($max_coord - $min_coord)) + $min_coord;
    print $rides_fh "$request_id,$rider_id,$pickup_x,$pickup_y,$dropoff_x,$dropoff_y\n";
}
close $rides_fh;
print "Generated rides.txt\n";

print "Test data generation complete.\n";

