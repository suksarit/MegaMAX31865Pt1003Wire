#include <SPI.h>
#include <Adafruit_MAX31865.h>

// ===============================
// PIN CONFIG (Arduino Mega 2560)
// ===============================
#define MAX31865_CS 49  // Chip Select

// ===============================
// RTD CONFIG
// ===============================
#define RNOMINAL 100.0  // Pt100 = 100 ohm @ 0°C
#define RREF     430.0  // Reference resistor 430 ohm

// ===============================
// OBJECT
// ===============================
Adafruit_MAX31865 rtd = Adafruit_MAX31865(MAX31865_CS);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("MAX31865 Pt100 (3-Wire) Initializing...");

  // ใช้โหมด 3-wire
  rtd.begin(MAX31865_3WIRE);

  Serial.println("MAX31865 Ready");
}

void loop() {

  // อ่านอุณหภูมิ
  float temperature = rtd.temperature(RNOMINAL, RREF);

  // ตรวจ fault
  uint8_t fault = rtd.readFault();

  if (fault) {
    Serial.println("\nRTD Fault Detected:");

    if (fault & MAX31865_FAULT_HIGHTHRESH)
      Serial.println(" - RTD High Threshold");

    if (fault & MAX31865_FAULT_LOWTHRESH)
      Serial.println(" - RTD Low Threshold");

    if (fault & MAX31865_FAULT_REFINLOW)
      Serial.println(" - REFIN- > 0.85 x Bias (Short?)");

    if (fault & MAX31865_FAULT_REFINHIGH)
      Serial.println(" - REFIN- < 0.85 x Bias (FORCE- open)");

    if (fault & MAX31865_FAULT_RTDINLOW)
      Serial.println(" - RTDIN- < 0.85 x Bias (RTD open)");

    if (fault & MAX31865_FAULT_OVUV)
      Serial.println(" - Under/Over Voltage");

    // เคลียร์ fault หลังอ่าน
    rtd.clearFault();
  }
  else {
    // แสดงอุณหภูมิแบบไม่ขึ้นบรรทัดใหม่
    Serial.print("\rTemperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C      ");
  }

  delay(1000);  // อ่านทุก 1 วินาที
}
