// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#include <Arduino.h>
#include "BF_AquesTalkPico.h"

AquesTalkPico::AquesTalkPico()
{
}

AquesTalkPico::~AquesTalkPico()
{
}

int AquesTalkPico::ShowRes(int res_length_to_show)
{
  char res[10];
  int res_length = Recv(res, sizeof(res));
  if (res_length >= res_length_to_show)
    Serial.printf("[AquesTalk LSI] Receive:%s\n", res);
  return res_length;
}

int AquesTalkPico::DumpEeprom()
{
  int data[16];

  for (int address = 0; address <= 0x3ff; ++address) {
    char msg[] = "#R3FF\r";
    msg[2] = HexChar(address / 256     );
    msg[3] = HexChar(address /  16 % 16);
    msg[4] = HexChar(address       % 16);
    if (Send(msg) != 0)
      return 1;
    delay(10);

    while (true) {
      char res[10];
      int res_length = Recv(res, sizeof(res));
      delay(10);
      if (res_length == 0)
        continue;
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
        return 2;
      }
    }
    if ((address + 1) % 16 == 0) {
      Serial.printf("[EEPROM] %03x:", address - 0x00f);
      for (int i = 0; i < 16; ++i)
        Serial.printf(" %02x", data[i]);
      Serial.println();
    }
  }
  return 0;
}

int AquesTalkPico::WriteEeprom(int address, int data)
{
  char msg[] = "#W3FFHH\r";
  msg[2] = HexChar(address / 256 % 16);  // 0xabcd --> 'b'
  msg[3] = HexChar(address /  16 % 16);  // 0xabcd --> 'c'
  msg[4] = HexChar(address       % 16);  // 0xabcd --> 'd'
  msg[5] = HexChar(data    /  16 % 16);  // 0xabcd --> 'c'
  msg[6] = HexChar(data          % 16);  // 0xabcd --> 'd'
  if (Send(msg) != 0)
    return 1;

  while (true) {
    delay(10);
    char res[10];
    int res_length = Recv(res, sizeof(res));
    if (res_length > 1) {
      Serial.printf("[AquesTalk LSI] Write EEPROM Error:%s\n", res);
      return 2;
    }
    else if (res[0] == '>')
      return 0;
  }
}

int AquesTalkPico::WritePresetMsg(const char* msg[], int num_of_msg)
{
  int address = 0x040;
  for (int i = 0; i < num_of_msg; ++i) {
    int j = 0;
    while (true) {
      char data = msg[i][j++];
      if (WriteEeprom(address++, data) != 0)
        return 1;
      if (address > 0x3ff)
        return 2;
      if (data == '\0')
        break;
    }
    Serial.printf("[AquesTalk LSI] Message No.%d, Address = %X\n", i, address);
  }
  return 0;
}

int AquesTalkPico::WriteSpeed(int speed)
{
  if (speed < 50)
    speed = 50;
  if (speed > 300)
    speed = 300;
  if (WriteEeprom(2, speed) != 0)
    return 2;
  if (WriteEeprom(3, speed / 256) != 0)
    return 3;
  return 0;
}

int AquesTalkPico::WritePause(int pause)
{
  if (pause < 256)
    pause = 256;
  if (pause > 65535)
    pause = 65535;
  if (WriteEeprom(4, pause) != 0)
    return 2;
  if (WriteEeprom(5, pause / 256) != 0)
    return 3;
  return 0;
}

int AquesTalkPico::WriteSerialSpeed(int serial_speed)
{
  int data(0);
  switch (serial_speed) {
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
  default: return 1;
  }
  if (WriteEeprom(0x000, data) != 0)
    return 2;
  if (WriteEeprom(0x001, data / 256) != 0)
    return 3;
  return 0;
}

int AquesTalkPico::WriteI2cAddress(int i2c_address)
{
  return WriteEeprom(0x006, i2c_address);
}

char AquesTalkPico::HexChar(int n) {
  if (n <  0) return 'x';
  if (n < 10) return '0' + n;
  if (n < 16) return 'A' + n - 10;
  return 'x';
}
