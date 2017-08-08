#!/usr/bin/perl

#
# Start waving: perl wave.pl start
# Stop waving:  perl wave.pl
#
# More details: https://github.com/Cyclenerd/iot-maneki-neko
#

use strict;
use Net::MQTT::Simple; # # http://search.cpan.org/perldoc?Net%3A%3AMQTT%3A%3ASimple


# Cat Name (CHANGE THIS!!!!11)
my $cat_name = "iotkatze";
# MQTT Server
my $mqtt_server = "iot.eclipse.org";


my $mqtt = Net::MQTT::Simple->new($mqtt_server); 

# Publish the message with the retain flag on.
# Use this for sensor values or anything else where the message indicates the current status of something.
# To discard a retained topic, provide an empty or undefined message.
if ($ARGV[0] eq "start") {
	print "[$cat_name] start waving\n";
	$mqtt->retain("winkekatze/$cat_name/command" => "wave");
} else {
	$mqtt->retain("winkekatze/$cat_name/command" => "");
	print "[$cat_name] stop waving\n";
}
