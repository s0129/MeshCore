#pragma once
// Arduino core pin definitions; MeshCore application sources live one level up.

#include "WVariant.h"

#define VARIANT_MCK (64000000ul)
#define USE_LFXO
#define NRF_APM

#define PINS_COUNT 48
#define NUM_DIGITAL_PINS 48
#define NUM_ANALOG_INPUTS 6
#define NUM_ANALOG_OUTPUTS 0

#define PIN_LED1 41
#define PIN_LED2 32
#define LED_BLUE PIN_LED1
#define LED_BUILTIN PIN_LED1
#define LED_CONN PIN_LED2
#define LED_STATE_ON LOW
#define BUTTON_PIN 25

#define HAS_WIRE 1
#define WIRE_INTERFACES_COUNT 2
#define PIN_WIRE_SDA 15
#define PIN_WIRE_SCL 17
#define PIN_WIRE1_SDA 31
#define PIN_WIRE1_SCL 30
#define PIN_OLED_EN 4
#define PIN_3V3_EN 5

#define PIN_SERIAL1_RX 21
#define PIN_SERIAL1_TX 20
#define PIN_SERIAL2_RX 22
#define PIN_SERIAL2_TX 24
// MeshCore names these pins from the GPS receiver's perspective.
#define PIN_GPS_TX PIN_SERIAL1_RX
#define PIN_GPS_RX PIN_SERIAL1_TX
#define PIN_GPS_EN 27
#define GPS_EN_ACTIVE LOW
#define GPS_BAUD_RATE 9600

#define SPI_INTERFACES_COUNT 1
#define PIN_SPI_MISO 47
#define PIN_SPI_MOSI 46
#define PIN_SPI_SCK 45
#define PIN_SPI_NSS 44
#define LORA_DIO_1 2
#define LORA_RESET 42
#define LORA_BUSY 43

// Common nRF52 shutdown code uses the P_LORA_* pin names.
#define P_LORA_NSS PIN_SPI_NSS
#define P_LORA_MISO PIN_SPI_MISO
#define P_LORA_MOSI PIN_SPI_MOSI
#define P_LORA_SCLK PIN_SPI_SCK
#define P_LORA_DIO_1 LORA_DIO_1
#define P_LORA_RESET LORA_RESET
#define P_LORA_BUSY LORA_BUSY
#define LR11X0_DIO3_TCXO_VOLTAGE 1.6f
