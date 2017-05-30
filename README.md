This little Arduino libary allows you to access the signals on a Spektrum DSM serial receiver using an Arduino.
It currently uses Serial1, and so will work only on Mega, Due, Teensy 3.X, and
other boards that have multiple serial ports.  It uses interrupts, rather than
polling. It is based on [this
code](https://github.com/dmcgriffy/DroneControlSystem/blob/master/DCS/RX.cpp).
The library provides two C++ classes: SpektrumDSM1024, for older receivers
using seven channels and 10-bit precision; and SpektrumDSM2048, for modern
receivers (like the Lemon RX) using eight channels and 11-bit precision.  I
have tested only the SpektrumDSM2048 class.
