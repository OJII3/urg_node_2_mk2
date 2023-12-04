#!/bin/bash
# Change udev-rules to allow access to the device

grep -F "SUBSYSTEM=="usb", MODE="0666"" /etc/udev/rules.d/usb.rules ||
sudo echo "SUBSYSTEM=="usb", MODE="0666" >> /etc/udev/rules.d/usb.rules

