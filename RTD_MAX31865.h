#ifndef RTD_MAX31865_H
#define RTD_MAX31865_H

#include <Arduino.h>
#include <Adafruit_MAX31865.h>

class RTD_MAX31865 {
public:
  RTD_MAX31865(uint8_t csPin,
               float rNominal = 100.0,
               float rRef = 430.0);

  void begin(uint8_t wireMode);
  void update(unsigned long now);
  void setReadInterval(uint16_t intervalMs);

  float   getTemperature() const;
  uint8_t getFault() const;
  bool    hasFault() const;
  bool    isDataValid() const;

private:
  Adafruit_MAX31865 _rtd;
  float _rNominal;
  float _rRef;

  float _temperature;
  uint8_t _fault;

  unsigned long _lastReadTime;
  uint16_t _readInterval;
};

#endif
