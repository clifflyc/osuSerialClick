# osuSerialClick

For people who want to build a custum keypad for osu!
This program makes up for the Arduino(uno)'s lack of keyboard abilities.

It is a very basic program that connects to a serial port, reads chars from the ports and then sends (when appropriate) keyboard input signals to press/release the z/x keys.

The config file ("click config.txt") should be in the same directory as build.

The Arduino code is included, simply wire (two) buttons up, connecting either pin 11 or 12 to GND for each button.

