// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack
// example

#include <M5Stack.h>

//#define AQUESTALKPICOSERIAL
#define AQUESTALKPICOWIRE
//#define AQUESTALKPICOSPI

//const bool aqtp_safe_mode(true);  // PMOD(1,0) = (HIGH, LOW ) for safe mode
const bool aqtp_safe_mode(false);  // PMOD(1,0) = (HIGH, HIGH) for command mode

//const bool aqtp_atp3011(true);  // ATP3011 .. send "?" to set speed
const bool aqtp_atp3011(false);  // ATP3012

//const bool aqtp_write_serial_speed(true);  // for ATP3012 only, safe-mode recommended
const bool aqtp_write_serial_speed(false);

//const bool aqtp_write_i2c_address(true);  // safe-mode recommended
const bool aqtp_write_i2c_address(false);

//const bool aqtp_write_preset_messages(true);
const bool aqtp_write_preset_messages(false);  // skip writing

const bool aqtp_dump_eeprom(true);
//const bool aqtp_dump_eeprom(false);  // skip dumping

const char* preset_msg[] = {
  "re'-jidesu.yo'-koso wata'kushitachi,sho'kka-e.\r",
  "ichi'jidesu.itta'i,do'-natte/iru'node_suka?zo'rutaisa,\r",
  "ni'jidesu.ma'da,deki'nainnde_suka?shinigamiha'kase,\r",
  "sa'njidesu.sho'kka-no,se'kai/seifuku/ke'ikakuwa chaku'chakuto,susunn'deimasu.\r",
  "yo'jidesu.omedeto-/goza'imasu jikken'wa,dai'seiko-desu.\r",
  "go'jidesu.sho'kka-no/chikara'wa,muge'nnnanodesu.\r",
  "roku'jidesu.uro'taenaide/kuda'sai,jigo'kutaishi,\r",
  "shichi'jidesu.wata'kushitachi,sho'kka-no/shuku'teki kamenn'raida-o/tao'sunodesu.\r",
  "hachi'jidesu.sa'-/yuki'nasai sho'kka-no/mono'tachiyo.\r",
  "ku'jidesu.kiki'masho- ana'tano/niho'nn/seifuku/ke'ikakuo.\r",
  "jyu'-jidesu.oda'marinasai,jigo'kutaishi su'beteno/shippaiwa,ana'tano/se'idesu.\r",
  "jyu-ichi'jidesu.sho'kka-wa,su'beteo/haka'ishi,se'kaio/shi'haishimasu.\r",
  "jyu-ni'jidesu.kaizo-ni'ngennga/se'kaio/ugo'kashi sono/kaizo-ni'nngenn+o/shi'hai/suru'noga,wata'kushidesu.\r",
  "ohayo-/goza'imasu.ta'dachini,sagashida'_sunodesu.\r",
  "oyasuminasa'i.se'kaiwa,sho'kka-no/mono'desu.\r",
};

#ifdef AQUESTALKPICOSERIAL
#include "BF_AquesTalkPicoSerial.h"
AquesTalkPicoSerial aqtp;
const int aqtp_serial_speed_safe = 9600;  // safe mode
const int aqtp_serial_speed = 76800;      // max of ATP3011, also works on ATP3012
//const int aqtp_serial_speed = 115200;   // max of ATP3012, but does not work
const int aqtp_sleep_pin = 5;  // GPIO5 for sleep control
#endif

#ifdef AQUESTALKPICOWIRE
#include "BF_AquesTalkPicoWire.h"
AquesTalkPicoWire aqtp;
const int aqtp_i2c_address = 0x2e;  // for customize
#endif

#ifdef AQUESTALKPICOSPI
#include "BF_AquesTalkPicoSpi.h"
AquesTalkPicoSpi aqtp;
SPIClass vspi(VSPI);
const int vspi_ss = 5;
#endif

// sub functions
void DumpEeprom();
int WriteEeprom(int address, int data);
void WriteSerialSpeed(int aqtp_serial_speed);
void WritePresetMsg(const char* msg[], int num_of_msg);
void ReadRes();

// for loop control
const int loop_ms(100);  // 100ms
int loop_last_ms(0);     // loop control
int msg_select(0);

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
//  Serial2.begin(9600);
  if (aqtp_safe_mode)
    Serial2.begin(aqtp_serial_speed_safe);
  else
    Serial2.begin(aqtp_serial_speed);
  while (!Serial2) {
    Serial.println("[AquesTalk LSI] Waiting Serial2");
    delay(100);
  }
  aqtp.Begin(Serial2);

  // for setting serial-speed of ATP3011
  if (aqtp_atp3011) {
    pinMode(aqtp_sleep_pin, OUTPUT);
    digitalWrite(aqtp_sleep_pin, LOW);
    delay(500);
    digitalWrite(aqtp_sleep_pin, HIGH);
    delay(80);
    aqtp.Send("?");
    for (int i = 0; i < 10; ++i) {
      ReadRes();
      delay(200);
    }
  }

  if (aqtp_write_serial_speed) {
    WriteSerialSpeed(aqtp_serial_speed);
    for (int i = 0; i < 10; ++i) {
      ReadRes();
      delay(200);
    }
  }
#endif

#ifdef AQUESTALKPICOWIRE
// defined by M5Stack
//  const int wire_sda(21);  // GPIO21 as default
//  const int wire_scl(22);  // GPIO22 as default
//  const int wire_frequency(100000);  // 100kHz as default
//  Wire.begin(wire_sda, wire_scl, wire_frequency);
//  aqtp.Begin(Wire);  // for default
  if (aqtp_safe_mode)
    aqtp.Begin(Wire);  // default/safe mode
  else
    aqtp.Begin(Wire, aqtp_i2c_address);

  if (aqtp_write_i2c_address) {
    WriteI2cAddress(aqtp_i2c_address);
    for (int i = 0; i < 10; ++i) {
      ReadRes();
      delay(200);
    }
  }
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

  if (aqtp_write_preset_messages)
    WritePresetMsg(preset_msg, sizeof(preset_msg)/sizeof(preset_msg[0]));

  if (aqtp_dump_eeprom)
    DumpEeprom();

  aqtp.Send("#V\r");
  for (int i = 0; i < 10; ++i) {
    ReadRes();
    delay(200);
  }

  aqtp.Send("#J\r");
  for (int i = 0; i < 10; ++i) {
    ReadRes();
    delay(200);
  }
  aqtp.Send("#K\r");
  for (int i = 0; i < 10; ++i) {
    ReadRes();
    delay(200);
  }

  loop_last_ms = millis();
}

void loop()
{
  M5.update();
  ReadRes();

  if (M5.BtnA.wasReleased()) {
    if (--msg_select < 0)
      msg_select = 14;
    aqtp.Send(preset_msg[msg_select]);
  }

  if (M5.BtnB.wasReleased()) {
    aqtp.Send(preset_msg[msg_select]);
  }

  if (M5.BtnC.wasReleased()) {
    if (++msg_select > 14)
      msg_select = 0;
    aqtp.Send(preset_msg[msg_select]);
  }

  delay(loop_ms + loop_last_ms - millis());
  loop_last_ms = millis();
}

// sub functions
//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
const char hex_char[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
void DumpEeprom()
{
  int data[16];

  for (int address = 0; address <= 0x3ff; ++address) {
    char msg[] = "#R3FF\r";
    msg[2] = hex_char[address / 256     ];
    msg[3] = hex_char[address /  16 % 16];
    msg[4] = hex_char[address       % 16];
    aqtp.Send(msg);
    delay(10);

    while (true) {
      char res[10];
      int res_length = aqtp.Recv(res, sizeof(res));
      delay(10);
      if (res_length == 0) {
        continue;
      }
      else if (res_length == 1 && res[0] == '*') {
        Serial.printf("%s\n", res);
        continue;
      }
      else if (res_length == 3 && res[2] == '>') {
        data[address % 16] = (res[0] >= 'A' ? res[0] - 'A' + 10 : res[0] - '0') * 16;
        data[address % 16] += res[1] >= 'A' ? res[1] - 'A' + 10 : res[1] - '0';
        break;
      }
      else {
        Serial.printf("[AquesTalk LSI] Dump EEPROM Error:%s\n", res);
        break;
      }
    }
    if ((address + 1) % 16 == 0) {
      Serial.printf("[EEPROM] %03x:", address - 0x00f);
      for (int i = 0; i < 16; ++i)
        Serial.printf(" %02x", data[i]);
      Serial.println();
    }
  }
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
int WriteEeprom(int address, int data)
{
  char msg[] = "#W3FFCC\r";
  msg[2] = hex_char[address / 256 % 16];  // 0xabcd --> 'b'
  msg[3] = hex_char[address /  16 % 16];  // 0xabcd --> 'c'
  msg[4] = hex_char[address       % 16];  // 0xabcd --> 'd'
  msg[5] = hex_char[data    /  16 % 16];  // 0xabcd --> 'c'
  msg[6] = hex_char[data          % 16];  // 0xabcd --> 'd'
  aqtp.Send(msg);

  while (true) {
    delay(10);
    char res[10];
    int res_length = aqtp.Recv(res, sizeof(res));
    if (res_length > 1) {
      Serial.printf("[AquesTalk LSI] Write EEPROM Error:%s\n", res);
      return 1;
    }
    else if (res[0] == '>')
      return 0;
  }
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
void WriteSerialSpeed(int aqtp_serial_speed)
{
  int data(0);
  switch (aqtp_serial_speed) {
  case   2400: data = 0x0340;  break;
  case   4800: data = 0x01a0;  break;
  case   9600: data = 0x00cf;  break;
  case  14400: data = 0x008a;  break;
  case  19200: data = 0x0067;  break;
  case  28800: data = 0x0044;  break;
  case  38400: data = 0x0033;  break;
  case  57600: data = 0x0022;  break;
  case  76800: data = 0x0019;  break;
  case 115200: data = 0x0010;  break;
  default:     data = 0x00cf;  break;
  }
  WriteEeprom(0x000, data);
  WriteEeprom(0x001, data / 256);
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
void WriteI2cAddress(int aqtp_i2c_address)
{
  WriteEeprom(0x006, aqtp_i2c_address);
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
void WritePresetMsg(const char* msg[], int num_of_msg)
{
  int address = 0x040;
  for (int i = 0; i < num_of_msg; ++i) {
    int j = 0;
    while (true) {
      char data = msg[i][j++];
      WriteEeprom(address++, data);
      if (address > 0x3ff)
        break;
      if (data == '\0')
        break;
    }
    Serial.printf("[AquesTalk LSI] Message No.%d, Address = %X\n", i, address);
  }
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8
void ReadRes()
{
  char res[10];
  if (aqtp.Recv(res, sizeof(res)) > 1)
    Serial.printf("[AquesTalk LSI] Receive:%s\n", res);
}
