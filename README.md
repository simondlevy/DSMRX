<img src="lemonrx.png" width=300>

This libary allows you to access the signals on a Spektrum DSM
serial receiver using an Arduino or other microcontroller.  It uses interrupts,
rather than polling. The library provides two C++ classes: SpektrumDSM1024, for
older receivers using seven channels and 10-bit precision; and SpektrumDSM2048,
for modern receivers (like the Lemon RX) using eight channels and 11-bit
precision.  There is also a standalone
[sketch](https://github.com/simondlevy/SpektrumDSM/tree/master/examples/BindSpektrum)
allowing you to bind your receiver to your transmitter using an Arduino Uno,
Mega, or other five-volt Arduino board.  

For maximum flexibility, the [SpektrumDSM]() class does not contain any Arduino-specific code.
Instead, as shown in this [example](), you write your own interrupt handler, which calls
the [SpektrumDSM::handleSerialEvent]().  In turn, this method
calls the <b>serialAvailable</b> and <b>serialRead</b> methods that you've written to
supply the serial data.

I have tested this library only on the SpektrumDSM2048 class.

This library borrows heavily from David McGriffy's [Drone Control
System](https://github.com/dmcgriffy/DroneControlSystem/blob/master/DCS/RX.cpp).

