#!/usr/bin/perl

use strict;
use Net::MQTT::Simple;

# MQTT Server
my $mqtt_server = "iot.eclipse.org";
my $mqtt = Net::MQTT::Simple->new($mqtt_server);

# Subscribes to the given topic(s) and registers the callbacks.
# Note that only the first matching handler will be called for every message, even if filter patterns overlap.
$mqtt->run(
	# all winkekatze commands
	"winkekatze/+/command" => sub {
		my ($topic, $message) = @_;
		print "[$topic] $message\n";
	},
	# all topics
	#"#" => sub {
	#	my ($topic, $message) = @_;
	#	print "[$topic] $message\n";
	#},
);