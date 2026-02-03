#include "RTD_MAX31865.h"

RTD_MAX31865 rtd(49);

void setup() {
  Serial.begin(115200);
  rtd.begin(MAX31865_3WIRE);
  rtd.setReadInterval(150);
}

void loop() {
  unsigned long now = millis();

  rtd.update(now);

  static unsigned long lastPrint = 0;
  if (now - lastPrint >= 500) {
    lastPrint = now;

    if (rtd.hasFault()) {
      Serial.println("RTD FAULT");
    } else {
      Serial.print("Temp = ");
      Serial.println(rtd.getTemperature());
    }
  }
}
