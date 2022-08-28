// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#include <Arduino.h>
#include "BF_AquesTalkPicoWire.h"

AquesTalkPicoWire::AquesTalkPicoWire()
{
}

AquesTalkPicoWire::~AquesTalkPicoWire()
{
}

int AquesTalkPicoWire::Begin(TwoWire &wire, int i2c_address)
{
  delay(80);  // 80ms: reset process of AquesTalk pico LSI
  m_wire = &wire;
  m_i2c_address = i2c_address;
  return 0;
}

int AquesTalkPicoWire::Send(const char* msg)
{
  m_wire->beginTransmission(m_i2c_address);
  m_wire->write(msg);
  int return_code = m_wire->endTransmission();
  if (return_code != 0)
    Serial.print("[AquesTalk Wire] ERROR Send\n");
  else
    Serial.printf("[AquesTalk Wire] Send:%s\n", msg);
  return return_code;
}

size_t AquesTalkPicoWire::Recv(char* res, size_t res_size)
{
  int i = 0;
  while (i < res_size - 1) {
    m_wire->requestFrom(m_i2c_address, 1);
    if (m_wire->available()) {
      char recv_data = m_wire->read();
      res[i++] = recv_data;
      if (recv_data == '>' || recv_data == '*')
        break;
    }
    else
      break;
  }
  res[i] = '\0';
  return strlen(res);
}

bool AquesTalkPicoWire::Busy()
{
  char res[10];
  int res_length = Recv(res, sizeof(res));
  if (res_length >= 2) {
    Serial.printf("[AquesTalk Wire] Receive:%s\n", res);
    return true;
  }
  if (res[0] != '>')
    return true;
  return false;
}
