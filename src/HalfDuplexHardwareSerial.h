#ifndef _HALFDUPLEXHARDWARESERIAL_H_
#define _HALFDUPLEXHARDWARESERIAL_H_

#include "HardwareSerial.h"

enum Direction { kTransmitOnly, kReceiveOnly };

/**
 * Wrapper for Arduino HardwareSerial objects that adds half-duplex operation via
 * a new setDirection() method. The half-duplex modes allow the RX and TX pins to be tied
 * together when a one-wire serial bus is required. Use of this class eliminates the
 * need for external components.
 */
class HalfDuplexHardwareSerial {
public:

  HalfDuplexHardwareSerial(HardwareSerial& serial) : mDirection(kReceiveOnly), mSerial(serial) {}
  void begin(unsigned long baud);
  void begin(unsigned long baud, uint8_t config);
  void end() { mSerial.end(); }
  int available(void) { return mSerial.available(); }
  int peek(void) { return mSerial.peek(); }
  int read(void) { return mSerial.read(); }
  int availableForWrite(void) { return mSerial.availableForWrite(); }
  void flush(void) { mSerial.flush(); }
  size_t write(uint8_t b) { return mSerial.write(b); }
  size_t write(unsigned long n) { return mSerial.write(n); }
  size_t write(long n) { return mSerial.write(n); }
  size_t write(unsigned int n) { return mSerial.write(n); }
  size_t write(int n) { return mSerial.write(n); }
  size_t write(const char *str) { return mSerial.write(str); }
  size_t write(const uint8_t *buffer, size_t size) { return mSerial.write(buffer,size); }
  operator bool() { return (bool)mSerial; }

  size_t writeLast(uint8_t b);

  void setDirection(Direction direction);

protected:

  void enableRx();
  void enableTx();
  void disableRx();
  void disableTx();

  Direction mDirection;
  HardwareSerial& mSerial;
};

#endif

