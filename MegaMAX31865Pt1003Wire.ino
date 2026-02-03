#include <SPI.h>
#include <Adafruit_MAX31865.h>

// ===============================
// PIN CONFIG
// ===============================
#define MAX31865_CS 49

// ===============================
// RTD CONFIG
// ===============================
#define RNOMINAL 100.0
#define RREF     430.0

// ===============================
// TIMING CONFIG
// ===============================
#define TEMP_READ_INTERVAL   1000   // อ่านอุณหภูมิทุก 1 วินาที
#define TEMP_PRINT_INTERVAL  1000   // แสดงผลทุก 1 วินาที

// ===============================
// OBJECT
// ===============================
Adafruit_MAX31865 rtd = Adafruit_MAX31865(MAX31865_CS);

// ===============================
// GLOBAL STATE
// ===============================
float currentTemperature = NAN;
uint8_t lastFault = 0;

unsigned long lastTempReadTime  = 0;
unsigned long lastTempPrintTime = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  rtd.begin(MAX31865_3WIRE);

  Serial.println("MAX31865 Pt100 Non-blocking Ready");
}

void loop() {
  unsigned long now = millis();

  // ==================================================
  // 1) อ่านอุณหภูมิ "เท่าที่จำเป็น"
  // ==================================================
  if (now - lastTempReadTime >= TEMP_READ_INTERVAL) {
    lastTempReadTime = now;

    currentTemperature = rtd.temperature(RNOMINAL, RREF);
    lastFault = rtd.readFault();

    if (lastFault) {
      rtd.clearFault();
    }
  }

  // ==================================================
  // 2) แสดงผล (แยกจากการอ่าน)
  // ==================================================
  if (now - lastTempPrintTime >= TEMP_PRINT_INTERVAL) {
    lastTempPrintTime = now;

    if (lastFault) {
      Serial.println("\nRTD Fault Detected:");

      if (lastFault & MAX31865_FAULT_HIGHTHRESH)
        Serial.println(" - RTD High Threshold");

      if (lastFault & MAX31865_FAULT_LOWTHRESH)
        Serial.println(" - RTD Low Threshold");

      if (lastFault & MAX31865_FAULT_REFINLOW)
        Serial.println(" - REFIN- > 0.85 x Bias");

      if (lastFault & MAX31865_FAULT_REFINHIGH)
        Serial.println(" - REFIN- < 0.85 x Bias");

      if (lastFault & MAX31865_FAULT_RTDINLOW)
        Serial.println(" - RTDIN- open");

      if (lastFault & MAX31865_FAULT_OVUV)
        Serial.println(" - Under/Over Voltage");
    }
    else {
      Serial.print("\rTemperature: ");
      Serial.print(currentTemperature, 2);
      Serial.print(" °C      ");
    }
  }

  // ==================================================
  // 3) ตรงนี้คือที่ของ logic TN Mower อื่น ๆ
  // ==================================================
  // motorControl();
  // safetyCheck();
  // communication();
}
