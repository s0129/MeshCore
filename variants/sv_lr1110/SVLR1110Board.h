#pragma once

#include <helpers/NRF52Board.h>
#include <SPI.h>
#include <Wire.h>

class SVLR1110Board : public NRF52Board {
public:
  SVLR1110Board() : NRF52Board("SV_LR1110_OTA") {}

  void begin() override {
    NRF52Board::begin();
    digitalWrite(PIN_3V3_EN, LOW);
    delay(100);
    Wire.setPins(PIN_WIRE_SDA, PIN_WIRE_SCL);
    Wire.begin();
  }

  uint16_t getBattMilliVolts() override {
    return 0;  // No battery ADC is connected on this revision.
  }

  const char* getManufacturerName() const override {
    return "SV LR1110";
  }

  void shutdownPeripherals() override {
    NRF52Board::shutdownPeripherals();
    SPI.end();
    Wire.end();
    Wire1.end();
    Serial1.end();
    digitalWrite(PIN_OLED_EN, HIGH);
    digitalWrite(PIN_GPS_EN, HIGH);
    digitalWrite(PIN_3V3_EN, HIGH);
    digitalWrite(PIN_LED1, !LED_STATE_ON);
    digitalWrite(PIN_LED2, !LED_STATE_ON);
  }
};
