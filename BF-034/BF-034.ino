// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack
// example

#include <M5Stack.h>

//#define AQUESTALKPICOSERIAL
//#define AQUESTALKPICOWIRE
#define AQUESTALKPICOSPI

#ifdef AQUESTALKPICOSERIAL
#include "BF_AquesTalkPicoSerial.h"
AquesTalkPicoSerial aqtp;
#endif

#ifdef AQUESTALKPICOWIRE
#include "BF_AquesTalkPicoWire.h"
AquesTalkPicoWire aqtp;
#endif

#ifdef AQUESTALKPICOSPI
#include "BF_AquesTalkPicoSpi.h"
AquesTalkPicoSpi aqtp;
SPIClass vspi(VSPI);
const int vspi_ss = 5;
#endif

// for loop control
const int loop_ms(100);  // 100ms
int loop_last_ms(0);     // loop control

void setup()
{
  const bool lcd_enable(true);
  const bool sd_enable(true);
  const bool serial_enable(true);
  const bool i2c_enable(true);
  M5.begin(lcd_enable, sd_enable, serial_enable, i2c_enable);

#ifdef AQUESTALKPICOSERIAL
// defined by Arduino-ESP32
//  const int serial2_rx(16);  // GPIO16 as default
//  const int serial2_tx(17);  // GPIO17 as default
//  Serial2.begin(9600, SERIAL_8N1, serial2_rx, serial2_tx);
  Serial2.begin(9600);
  aqtp.Begin(Serial2);
#endif

#ifdef AQUESTALKPICOWIRE
// defined by M5Stack
//  const int wire_sda(21);  // GPIO21 as default
//  const int wire_scl(22);  // GPIO22 as default
//  const int wire_frequency(100000);  // 100kHz as default
//  Wire.begin(wire_sda, wire_scl, wire_frequency);
  aqtp.Begin(Wire);
#endif

#ifdef AQUESTALKPICOSPI
// defined by Arduino-ESP32
//  const int vspi_sclk(18);  // SCLK = GPIO18 as default
//  const int vspi_miso(19);  // MISO = GPIO19 as default
//  const int vspi_mosi(23);  // MOSI = GPIO23 as default
//  const int vspi_ss(5);     // SS   = GPIO5  as default
//  vspi->begin(vspi_sclk, vspi_miso, vspi_mosi, vspi_ss);
  vspi.begin();
  aqtp.Begin(vspi, vspi_ss);
#endif

  loop_last_ms = millis();
}

void loop()
{
  M5.update();

  char res[10];
  if (aqtp.Recv(res, 10) > 0)
    Serial.printf("[AquesTalk LSI] Receive:%s\n", res);

  if (M5.BtnA.wasReleased())
    aqtp.Send("#V\r");

  if (M5.BtnB.wasReleased())
    aqtp.Send("#J\r");

  if (M5.BtnC.wasReleased())
    aqtp.Send("oyasuminasai\r");

  delay(loop_ms + loop_last_ms - millis());
  loop_last_ms = millis();
}
