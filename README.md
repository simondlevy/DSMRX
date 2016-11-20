This little Arduino libary allows you to access the signals on a Spektrum DSM serial receiver using an Arduino.
It currently uses Serial1, and so will work only on Due, Teensy 3.X, and other boards that have multiple
serial ports.  It uses interrupts, rather than polling.
