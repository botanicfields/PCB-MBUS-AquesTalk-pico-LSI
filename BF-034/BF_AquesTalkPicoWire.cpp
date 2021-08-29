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

int AquesTalkPicoWire::Begin(TwoWire &wire)
{
  const int gpio_sleep(13);        // GPIO13
  pinMode(gpio_sleep, OUTPUT);
  digitalWrite(gpio_sleep, HIGH);  // wake up
  delay(80);  // 80ms: reset process of AquesTalk-Pico

  m_wire = &wire;
  return 0;
}

int AquesTalkPicoWire::Send(const char* msg)
{
  m_wire->beginTransmission(address_aquestalk_pico);
  m_wire->write(msg);
  int return_code = m_wire->endTransmission();
  if (return_code != 0)
    Serial.print("[AquesTalk Wire] ERROR Send\n");
  else
    Serial.printf("[AquesTalk Wire] Send:%s\n", msg);
  return return_code;
}

size_t AquesTalkPicoWire::Recv(char* res, int res_size)
{
  int i = 0;
  while (i < res_size - 1) {
    m_wire->requestFrom(address_aquestalk_pico, 1);
    if (m_wire->available()) {
      char recv_data = m_wire->read();
      res[i++] = recv_data;
      if (recv_data == '>' || recv_data == '*') break;
    }
    else
      break;
  }
  res[i] = '\0';
  return strlen(res);
}
