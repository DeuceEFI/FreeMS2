#!/bin/sh
cd firmware
#hcs12mem --interface sm --port COM8 --baud 115200 --target mc9s12c128 --flash-address banked-ppage --flash-erase --flash-write FreeMS2-0.0.1-SNAPSHOT-Simple.s19 --reset
hcs12mem --interface sm --port /dev/ttyUSB0 --baud 115200 --target mc9s12c128 --flash-address banked-ppage --flash-erase --flash-write FreeMS2-0.0.1-SNAPSHOT-Simple.s19 --reset
cd ..