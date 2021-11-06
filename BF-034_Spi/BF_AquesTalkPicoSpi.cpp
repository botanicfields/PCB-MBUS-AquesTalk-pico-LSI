// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#include <Arduino.h>
#include "BF_AquesTalkPicoSpi.h"

AquesTalkPicoSpi::AquesTalkPicoSpi()
{
}

AquesTalkPicoSpi::~AquesTalkPicoSpi()
{
}

int AquesTalkPicoSpi::Begin(SPIClass &spi, int ss)
{
  delay(80);  // 80ms: reset process of AquesTalk-Pico
  m_spi = &spi;
  m_ss = ss;
  pinMode(m_ss, OUTPUT);
  digitalWrite(m_ss, HIGH);
  return 0;
}

int AquesTalkPicoSpi::Send(const char* msg)
{
  int i = 0;

  // default by Arduino-ESP32
  //   const m_frequency(1000000);
  //   m_spi->beginTransaction(SPISettings(m_frequency, MSBFIRST, SPI_MODE0));

  m_spi->beginTransaction(SPISettings());
  digitalWrite(m_ss, LOW);
  while (msg[i] != '\0') {
    m_spi->transfer(msg[i++]);
    delayMicroseconds(20);
  }
  digitalWrite(m_ss, HIGH);
  m_spi->endTransaction();
  Serial.printf("[AquesTalk Spi] Send:%s\n", msg);
  return 0;
}

size_t AquesTalkPicoSpi::Recv(char* res, size_t res_size)
{
  int i = 0;

  // default by Arduino-ESP32
  //   const m_frequency(1000000);
  //   m_spi->beginTransaction(SPISettings(m_frequency, MSBFIRST, SPI_MODE0));

  m_spi->beginTransaction(SPISettings());
  digitalWrite(m_ss, LOW);
  while (i < res_size - 1) {
    char recv_data = m_spi->transfer(0xff);
    delayMicroseconds(20);
    res[i++] = recv_data;
    if (recv_data == '>' || recv_data == '*')
      break;
  }
  digitalWrite(m_ss, HIGH);
  m_spi->endTransaction();
  res[i] = '\0';
  return strlen(res);
}
