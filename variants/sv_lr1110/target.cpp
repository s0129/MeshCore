#include "target.h"
#include <helpers/sensors/MicroNMEALocationProvider.h>

SVLR1110Board board;
RADIO_CLASS radio = new Module(PIN_SPI_NSS, LORA_DIO_1, LORA_RESET, LORA_BUSY, SPI);
WRAPPER_CLASS radio_driver(radio, board);
VolatileRTCClock rtc_clock;
MicroNMEALocationProvider nmea(Serial1, &rtc_clock);
EnvironmentSensorManager sensors(nmea);
#ifdef DISPLAY_CLASS
  RefCountedDigitalPin oled_power(PIN_OLED_EN, LOW);
  DISPLAY_CLASS display(&oled_power, 128, 32);
#endif

static const uint32_t rfswitch_dios[Module::RFSWITCH_MAX_PINS] = {
  RADIOLIB_LR11X0_DIO5, RADIOLIB_LR11X0_DIO6,
  RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC
};

static const Module::RfSwitchMode_t rfswitch_table[] = {
  { LR11x0::MODE_STBY,  { LOW,  LOW  } },
  { LR11x0::MODE_RX,    { HIGH, LOW  } },
  { LR11x0::MODE_TX,    { HIGH, HIGH } },
  { LR11x0::MODE_TX_HP, { LOW,  HIGH } },
  { LR11x0::MODE_TX_HF, { LOW,  LOW  } },
  { LR11x0::MODE_GNSS,  { LOW,  LOW  } },
  { LR11x0::MODE_WIFI,  { LOW,  LOW  } },
  END_OF_MODE_TABLE
};

#ifndef LORA_CR
  #define LORA_CR 5
#endif

bool radio_init() {
  SPI.setPins(PIN_SPI_MISO, PIN_SPI_SCK, PIN_SPI_MOSI);
  SPI.begin();
  radio.setRfSwitchTable(rfswitch_dios, rfswitch_table);

  int status = radio.begin(LORA_FREQ, LORA_BW, LORA_SF, LORA_CR,
      RADIOLIB_LR11X0_LORA_SYNC_WORD_PRIVATE, LORA_TX_POWER, 16, LR11X0_DIO3_TCXO_VOLTAGE);
  if (status == RADIOLIB_ERR_NONE) status = radio.setCRC(2);
  if (status == RADIOLIB_ERR_NONE) status = radio.explicitHeader();
  if (status == RADIOLIB_ERR_NONE) status = radio.setRxBoostedGainMode(true);
  if (status != RADIOLIB_ERR_NONE) {
    Serial.print("ERROR: radio init failed: ");
    Serial.println(status);
    return false;
  }
  return true;
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng);
}
