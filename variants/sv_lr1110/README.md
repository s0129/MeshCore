# SV LR1110

Custom SV LoRa board using the Elecrow nRFLR1110 module (nRF52840 + LR1110),
a 128×32 SSD1306 OLED, UART GPS and a BME688 environmental sensor. Pin assignments
and the LR1110 RF-switch table match `meshtastic/firmware/variants/sv_lr1110`.

## Build

Run from the MeshCore project directory:

```sh
pio run -e sv_lr1110_companion_radio_ble -t create_uf2
```

Available environments:

| Environment | Function |
| --- | --- |
| `sv_lr1110_companion_radio_ble` | BLE companion with OLED and button |
| `sv_lr1110_companion_radio_usb` | USB companion with OLED and button |
| `sv_lr1110_repeater` | Headless repeater |
| `sv_lr1110_room_server` | Headless room server |

The UF2 is written to `.pio/build/<environment>/firmware.uf2`. Copy it to the
board's UF2 bootloader drive, or use `pio run -e <environment> -t upload` for
serial DFU with an existing compatible bootloader.

### Flashing over serial DFU

This bootloader exposes a CDC DFU port (`0x239A:0x002A`) but no UF2 mass-storage
drive. Enter it with a 1200-baud touch on the application port, then flash a
package generated with `sd-req 0xFFFE` so it is not rejected when the
bootloader's SoftDevice ID differs from the declared `0x00B6`:

```sh
# touch: open and close the application COM port at 1200 baud
adafruit-nrfutil dfu genpkg --dev-type 0x0052 --sd-req 0xFFFE \
  --application .pio/build/<env>/firmware.hex .pio/build/<env>/dfu.zip
adafruit-nrfutil dfu serial -pkg .pio/build/<env>/dfu.zip -p COM<n> -b 115200 -sb
```

`-sb` selects the single-bank path, which skips the dual-bank activation delay.
A double-tap reset also enters the bootloader. The application USB descriptor
reports `SV LR1110`, which is a quick way to confirm the right image is running.

These builds require **S140 6.1.1** and a compatible Adafruit nRF52 bootloader.
The application starts at `0x26000` and ends before `0xD4000`; MeshCore reserves
`0xD4000`–`0xECFFF` for ExtraFS. A Meshtastic installation may have a different
SoftDevice or filesystem layout. Check the installed bootloader/SoftDevice
before flashing; back up settings first. An application UF2 does not install
or upgrade the SoftDevice.

For BLE, the default pairing PIN is randomized at boot and shown on the OLED
unless a PIN has been saved. If no display is detected, the fallback is `123456`.
Repeater and room-server default admin passwords are `password`; the default
room password is `hello`. Change these before deployment. Radio defaults are
inherited from the root `platformio.ini`; configure frequency, bandwidth and
power for your region before use.

## Hardware

The Arduino pin variant lives in `arduino/`; the MeshCore integration lives in
`target.cpp` and `SVLR1110Board.h`. Keep application sources outside `arduino/`
so PlatformIO does not compile them as part of the Arduino core library.

Pin numbers use the direct nRF mapping: P0.xx = xx, P1.xx = 32 + xx.

| Function | nRF52840 pins |
| --- | --- |
| LR1110 NSS / SCK / MOSI / MISO | P1.12 / P1.13 / P1.14 / P1.15 |
| LR1110 reset / busy / IRQ | P1.10 / P1.11 / P0.02 |
| OLED SDA / SCL / power enable | P0.15 / P0.17 / P0.04 |
| BME688 SDA / SCL / power enable | P0.31 / P0.30 / P0.05 |
| GPS MCU RX / MCU TX / power enable | P0.21 / P0.20 / P0.27 |
| Button | P0.25, active-low |
| Status / connection LEDs | P1.09 / P1.00, active-low |

All three peripheral power switches are active-low. The OLED and sensor rails are
switched on in `initVariant()` so they settle before the display and BME688 are
probed; `SSD1306Display` additionally waits `SSD1306_POWER_SETTLE_MS` after
claiming a switched supply (the SSD1306 needs ~100 ms after VDD is applied, and
initialising too early leaves the panel blank even though it ACKs on I2C).
The GPS uses 9600 baud;
the BME688 uses the existing BME680 driver on `Wire1`. The radio uses a 1.6 V
TCXO and LR1110 DIO5/DIO6 RF switching. There is no battery ADC connected on
this board revision, so battery voltage is reported as zero (unavailable).

## Hardware checks after flashing

- Verify radio initialization and bidirectional messages with another MeshCore
  node using identical radio settings.
- Check OLED boot text, BLE PIN, long node/message text, button wake and display
  power cycling.
- Verify GPS fixes/time sync and BME688 telemetry; check that GPS enable follows
  its configured on/off state.
- Verify shutdown turns off the OLED, GPS, sensor rail, LEDs and radio.

### Observed on the reference unit

- OLED (SSD1306 128×32) at `0x3C` on `Wire`; it initialises at boot and wakes
  from the auto-off on a button press.
- BME688 at `0x77` on `Wire1`; plausible temperature, pressure, humidity and a
  gas resistance that rises from ~6 kΩ to ~60 kΩ as the heater stabilises.
- GPS (ATGM336H) at 9600 baud reports a fix after a couple of minutes with a
  cold start and sky view: 7 satellites, HDOP ~4, valid position and time.
  It emits `GGA`, `RMC`, `GSA` and `GSV` (the last two only once tracking).
- With no sky view the receiver still emits checksum-valid `GGA`/`RMC` with
  quality 0 and zero satellites, so "sentences but no fix" means signal, not
  wiring.
- The nRF52 UART RX buffer is 64 bytes. Blocking for more than ~60 ms at
  9600 baud overruns it and corrupts sentences; MeshCore's provider drains the
  port in a tight loop and reports `ok`/`bad` checksum counts through the `req:`
  GPS diagnostics, which is the quickest way to spot an overrun.

Compilation does not validate RF performance, wiring or the installed bootloader.
