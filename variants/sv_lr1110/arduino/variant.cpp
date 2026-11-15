#include "variant.h"
// This file is built by PlatformIO's Arduino variant library.
#include "wiring_constants.h"
#include "wiring_digital.h"

const uint32_t g_ADigitalPinMap[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
};

void initVariant() {
  digitalWrite(PIN_LED1, !LED_STATE_ON);
  pinMode(PIN_LED1, OUTPUT);
  digitalWrite(PIN_LED2, !LED_STATE_ON);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // The three AO3401A high-side power switches are active-low. Bring the OLED
  // and sensor rails up immediately so they have settled before any driver
  // (display, BME688) talks to them.
  digitalWrite(PIN_3V3_EN, LOW);
  pinMode(PIN_3V3_EN, OUTPUT);
  digitalWrite(PIN_OLED_EN, LOW);
  pinMode(PIN_OLED_EN, OUTPUT);

  // GPS power stays off until the GPS location provider enables it.
  digitalWrite(PIN_GPS_EN, HIGH);
  pinMode(PIN_GPS_EN, OUTPUT);
}
