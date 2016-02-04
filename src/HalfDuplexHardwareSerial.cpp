
#include <avr/interrupt.h>
#include "HalfDuplexHardwareSerial.h"

#define USART_PORT PORTD
#define USART_DDR DDRD
#define USART_PIN PIND
#define USART_RX_PIN PD0
#define USART_TX_PIN PD1

#define USART0_PORT PORTD
#define USART0_DDR DDRD
#define USART0_PIN PIND
#define USART0_RX_PIN PD0
#define USART0_TX_PIN PD1

#define USART1_PORT PORTD
#define USART1_DDR DDRD
#define USART1_PIN PIND
#define USART1_RX_PIN PD2
#define USART1_TX_PIN PD3

#define USART_N_PORT(usart) USART ## usart ## _PORT  
#define USART_N_DDR(usart) USART ## usart ## _DDR  
#define USART_N_PIN(usart) USART ## usart ## _PIN
#define USART_N_RX_PIN(usart) USART## usart ## _RX_PIN
#define USART_N_TX_PIN(usart) USART## usart ## _TX_PIN

#define USART_ENABLE_RX() \
    UCSRA &= ~(_BV(FE) | _BV(DOR) | _BV(UPE); \
    UCSRB |= _BV(RXEN);

#define USART_N_ENABLE_RX(usart) \
    UCSR ## usart ## A &= ~(_BV(FE ## usart) | _BV(DOR ## usart) | _BV(UPE ## usart)); \
    UCSR ## usart ## B |= _BV(RXEN ## usart);

#define USART_DISABLE_RX() \
    UCSRB &= ~_BV(RXEN); \
    USART_DDR &= ~_BV(USART_RX_PIN()); \
    USART_PORT &= ~_BV(USART_RX_PIN());

#define USART_N_DISABLE_RX(usart) \
    UCSR ## usart ## B &= ~_BV(RXEN ## usart); \
    USART ## usart ## _DDR &= ~_BV(USART ## usart ## _RX_PIN); \
    USART ## usart ## _PORT &= ~_BV(USART ## usart ## _RX_PIN);

#define USART_ENABLE_TX() \
    USART_DDR |= _BV(USART_TX_PIN); \
    USART_PORT |= _BV(USART_TX_PIN); \
    UCSRB |= _BV(TXEN);

#define USART_N_ENABLE_TX(usart) \
    USART ## usart ## _DDR |= _BV(USART ## usart ## _TX_PIN); \
    USART ## usart ## _PORT |= _BV(USART ## usart ## _TX_PIN); \
    UCSR ## usart ## B |= _BV(TXEN ## usart);

#define USART_DISABLE_TX(usart) \
    cli(); \
    while ((UCSRA & _BV(TXC)) == 0); \
    UCSRA &= ~(_BV(FE) | _BV(DOR) | _BV(UPE); \
    UCSRB &= ~_BV(TXEN); \
    USART_DDR &= ~_BV(USART_TX_PIN); \
    USART_PORT &= ~_BV(USART_TX_PIN); \
    sei();

#define USART_N_DISABLE_TX(usart) \
    cli(); \
    while ((UCSR ## usart ## A & _BV(TXC ## usart)) == 0); \
    UCSR ## usart ## A &= ~(_BV(FE ## usart) | _BV(DOR ## usart) | _BV(UPE ## usart)); \
    UCSR ## usart ## B &= ~_BV(TXEN ## usart); \
    USART_N_DDR(usart) &= ~_BV(USART_N_TX_PIN(usart)); \
    USART_N_PORT(usart) &= ~_BV(USART_N_TX_PIN(usart)); \
    sei();

void HalfDuplexHardwareSerial::begin(unsigned long baud) { 
  mSerial.begin(baud);
  setDirection(kReceiveOnly);
}

void HalfDuplexHardwareSerial::begin(unsigned long baud, uint8_t config) {
  mSerial.begin(baud,config);
  setDirection(kReceiveOnly);
}

size_t HalfDuplexHardwareSerial::writeLast(uint8_t b) {
  size_t sent;
  cli();
  sent = mSerial.write(b);
  setDirection(kReceiveOnly);
  sei();
}

void HalfDuplexHardwareSerial::setDirection(Direction direction) {
  if (direction == mDirection) {
    return;
  }
  else if (direction == kTransmitOnly) {
    disableRx();
    enableTx();
  }
  else {
    disableTx();
    enableRx();
  }
  mDirection = direction;
}

void HalfDuplexHardwareSerial::enableRx() {
  if (false) {}
#ifdef HAVE_HWSERIAL0
  else if (&mSerial == &Serial) {
#if defined(UBRRH) && defined(UBRRL)
    USART_ENABLE_RX()
#else
    USART_N_ENABLE_RX(0)
#endif
  }
#endif
#ifdef HAVE_HWSERIAL1
  else if (&mSerial == &Serial1) {
    USART_N_ENABLE_RX(1)
    
  }
#endif
#ifdef HAVE_HWSERIAL2
  else if (&mSerial == &Serial2) {
    USART_N_ENABLE_RX(2)
    
  }
#endif
#ifdef HAVE_HWSERIAL3
  else if (&mSerial == &Serial3) {
    USART_N_ENABLE_RX(3)
  }
#endif
}

void HalfDuplexHardwareSerial::disableRx() {
  if (false) {}
#ifdef HAVE_HWSERIAL0
  else if (&mSerial == &Serial) {
#if defined(UBRRH) && defined(UBRRL)
    USART_DISABLE_RX()
#else
    USART_N_DISABLE_RX(0)
#endif
  }
#endif
#ifdef HAVE_HWSERIAL1
  else if (&mSerial == &Serial1) {
    USART_N_DISABLE_RX(1)
  }
#endif
#ifdef HAVE_HWSERIAL2
  else if (&mSerial == &Serial2) {
    USART_N_DISABLE_RX(2)
  }
#endif
#ifdef HAVE_HWSERIAL3
  else if (&mSerial == &Serial3) {
    USART_N_DISABLE_RX(3)
  }
#endif
}

void HalfDuplexHardwareSerial::enableTx() {
  if (false) {}
#ifdef HAVE_HWSERIAL0
  else if (&mSerial == &Serial) {
#if defined(UBRRH) && defined(UBRRL)
    USART_ENABLE_TX()
#else
    USART_N_ENABLE_TX(0)
#endif
  }
#endif
#ifdef HAVE_HWSERIAL1
  else if (&mSerial == &Serial1) {
    USART_N_ENABLE_TX(1)
    
  }
#endif
#ifdef HAVE_HWSERIAL2
  else if (&mSerial == &Serial2) {
    USART_N_ENABLE_TX(2)
    
  }
#endif
#ifdef HAVE_HWSERIAL3
  else if (&mSerial == &Serial3) {
    USART_N_ENABLE_TX(3)
  }
#endif
}

void HalfDuplexHardwareSerial::disableTx() {
  if (false) {}
#ifdef HAVE_HWSERIAL0
  else if (&mSerial == &Serial) {
#if defined(UBRRH) && defined(UBRRL)
    USART_DISABLE_TX()
#else
    USART_N_DISABLE_TX(0)
#endif
  }
#endif
#ifdef HAVE_HWSERIAL1
  else if (&mSerial == &Serial1) {
    USART_N_DISABLE_TX(1)
  }
#endif
#ifdef HAVE_HWSERIAL2
  else if (&mSerial == &Serial2) {
    USART_N_DISABLE_TX(2)
  }
#endif
#ifdef HAVE_HWSERIAL3
  else if (&mSerial == &Serial3) {
    USART_N_DISABLE_TX(3)
  }
#endif
}


