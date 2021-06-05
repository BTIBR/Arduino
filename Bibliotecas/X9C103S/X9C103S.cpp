#include "Arduino.h"
#include "X9C103S.h"

X9C103S::X9C103S(uint8_t incPin, uint8_t udPin, uint8_t csPin) {
  _incPin = incPin;
  _udPin = udPin;
  _csPin = csPin;  
  _currentValue = X9C103S_UNKNOWN;

  pinMode(_incPin, OUTPUT);
  pinMode(_udPin, OUTPUT);
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);

}

void X9C103S::reset() {
  // change down maximum number of times to ensure the value is 0
  decrease(X9C103S_MAX_AMOUNT);
  _currentValue = 0;
}

void X9C103S::set(uint8_t value) {
  value = constrain(value, 0, X9C103S_MAX_AMOUNT);
  if (_currentValue == X9C103S_UNKNOWN) reset();
  if (_currentValue > value) {
    change(X9C103S_DOWN, _currentValue-value);
  } else if (_currentValue < value) {
    change(X9C103S_UP, value-_currentValue);
  }
}

uint8_t X9C103S::get() {
  return _currentValue;
}

void X9C103S::increase(uint8_t amount) {
  amount = constrain(amount, 0, X9C103S_MAX_AMOUNT);
  change(X9C103S_UP, amount);
}

void X9C103S::decrease(uint8_t amount) {
  amount = constrain(amount, 0, X9C103S_MAX_AMOUNT);
  change(X9C103S_DOWN, amount);
}

void X9C103S::change(uint8_t direction, uint8_t amount) {
  amount = constrain(amount, 0, X9C103S_MAX_AMOUNT);
  digitalWrite(_udPin, direction);
  digitalWrite(_incPin, HIGH);
  digitalWrite(_csPin, LOW);

  for (uint8_t i=0; i<amount; i++) {
    digitalWrite(_incPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_incPin, HIGH);
    delayMicroseconds(2);
    if (_currentValue != X9C103S_UNKNOWN) {
      _currentValue += (direction == X9C103S_UP ? 1 : -1);
      _currentValue = constrain(_currentValue, 0, X9C103S_MAX_AMOUNT);
    }
    
  }
  digitalWrite(_csPin, HIGH);
}

