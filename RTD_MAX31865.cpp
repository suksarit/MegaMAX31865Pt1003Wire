#include "RTD_MAX31865.h"

RTD_MAX31865::RTD_MAX31865(uint8_t csPin,
                           float rNominal,
                           float rRef)
  : _rtd(csPin),
    _rNominal(rNominal),
    _rRef(rRef),
    _temperature(NAN),
    _fault(0),
    _lastReadTime(0),
    _readInterval(200)
{
}

void RTD_MAX31865::begin(uint8_t wireMode) {
  _rtd.begin(wireMode);
}

void RTD_MAX31865::setReadInterval(uint16_t intervalMs) {
  _readInterval = intervalMs;
}

void RTD_MAX31865::update(unsigned long now) {
  if (now - _lastReadTime < _readInterval) return;
  _lastReadTime = now;

  _temperature = _rtd.temperature(_rNominal, _rRef);
  _fault = _rtd.readFault();

  if (_fault) {
    _rtd.clearFault();
  }
}

float RTD_MAX31865::getTemperature() const {
  return _temperature;
}

uint8_t RTD_MAX31865::getFault() const {
  return _fault;
}

bool RTD_MAX31865::hasFault() const {
  return _fault != 0;
}

bool RTD_MAX31865::isDataValid() const {
  return (!isnan(_temperature) && _fault == 0);
}
