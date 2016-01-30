# HalfDuplexHardwareSerial
Arduino library containing class wrapper that adds half-duplex support to HardwareSerial objects.

Existing code can be modified by inserting setDirection() calls. However, this not the best way to use this class (see below):

```
#include "HalfDuplexHardwareSerial.h"

HalfDuplexHardwareSerial serial(Serial1);

// Send message
serial.setDirection(kTransmitOnly);
serial.write(msg,msgLength);
serial.setDirection(kReceiveOnly);

// Wait for response
while(!serial.available());
uint8_t b = serial.read();

```

The preferred method is to transmit the last byte using the writeLast() method. This method will
disable interrupts, wait for this last byte to exit the shift register, immediately disable the USART TX mode,
reconfigure the Tx pin to make it a floating input pin, enable the USART RX mode and then enable interrupts.
This method provides the quickest switch between Tx and Rx modes in order to reduce the chance that the Tx line
is still being driven to a high idle state when a receiver begins transmitting a response.

```

HalfDuplexHardwareSerial serial(Serial1);

// Send message
serial.setDirection(kTransmitOnly);
serial.write(msg,msgLength-1);
serial.writeLast(msg[msgLength-1]);

// Wait for response
while(!serial.available());
uint8_t b = serial.read();

```
