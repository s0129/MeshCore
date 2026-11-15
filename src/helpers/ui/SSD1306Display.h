#pragma once

#include "DisplayDriver.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#define SSD1306_NO_SPLASH
#include <Adafruit_SSD1306.h>
#include <helpers/RefCountedDigitalPin.h>

#ifndef PIN_OLED_RESET
  #define PIN_OLED_RESET        21 // Reset pin # (or -1 if sharing Arduino reset pin)
#endif

#ifndef DISPLAY_ADDRESS
  #define DISPLAY_ADDRESS   0x3C
#endif

// Time allowed for a switched display supply to settle before the panel is
// initialised. SSD1306 needs ~100ms after VDD is applied.
#ifndef SSD1306_POWER_SETTLE_MS
  #define SSD1306_POWER_SETTLE_MS 300
#endif

class SSD1306Display : public DisplayDriver {
  Adafruit_SSD1306 display;
  bool _isOn;
  uint8_t _color;
  RefCountedDigitalPin* _peripher_power;

  bool i2c_probe(TwoWire& wire, uint8_t addr);
public:
  SSD1306Display(RefCountedDigitalPin* peripher_power=NULL, uint8_t width=128, uint8_t height=64) : DisplayDriver(width, height),
      display(width, height, &Wire, PIN_OLED_RESET),
      _peripher_power(peripher_power)
  {
    // Compact screens render single-line, ellipsized text. GFX wrapping would
    // also make getTextBounds() underestimate the width of long strings.
    if (height == 32) display.setTextWrap(false);
    _isOn = false;
  }
  bool begin();

  bool isOn() override { return _isOn; }
  void turnOn() override;
  void turnOff() override;
  void clear() override;
  void startFrame(ColorVal bkg = UIColor::window_bkg) override;
  void setTextSize(int sz) override;
  void setColor(ColorVal c) override;
  void setCursor(int x, int y) override;
  void print(const char* str) override;
  void fillRect(int x, int y, int w, int h) override;
  void drawRect(int x, int y, int w, int h) override;
  void drawXbm(int x, int y, const uint8_t* bits, int w, int h) override;
  uint16_t getTextWidth(const char* str) override;
  void endFrame() override;
};
