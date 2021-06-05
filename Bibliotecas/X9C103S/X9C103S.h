#ifndef X9C103S_h
#define X9C103S_h

#include "Arduino.h"

#define X9C103S_UP          HIGH
#define X9C103S_DOWN        LOW
#define X9C103S_MAX_AMOUNT  99
#define X9C103S_UNKNOWN     255

class X9C103S
{
 public:
  X9C103S(uint8_t incPin, uint8_t udPin, uint8_t csPin);
  void increase(uint8_t amount);
  void decrease(uint8_t amount);
  void change(uint8_t direction, uint8_t amount);
  void set(uint8_t value);
  uint8_t get();
  void reset();

 private:
  uint8_t _incPin;
  uint8_t _udPin;
  uint8_t _csPin;
  uint8_t _currentValue;
};

#endif
