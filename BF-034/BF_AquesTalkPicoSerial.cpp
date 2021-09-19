// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#include <Arduino.h>
#include "BF_AquesTalkPicoSerial.h"

AquesTalkPicoSerial::AquesTalkPicoSerial()
{
  m_recv = new char[m_recv_size];
  for (int i = 0; i < m_recv_size; ++i)
    m_recv[i] = 0;
}

AquesTalkPicoSerial::~AquesTalkPicoSerial()
{
  delete[] m_recv;
}

int AquesTalkPicoSerial::Begin(Stream &stream)
{
  delay(80);  // 80ms: reset process of AquesTalk-Pico
  m_stream = &stream;
  m_recv_count = 0;
  return 0;
}

int AquesTalkPicoSerial::Send(const char* msg)
{
  m_stream->write(msg);
  Serial.printf("[AquesTalk Serial] Send:%s\n", msg);
  return 0;
}

size_t AquesTalkPicoSerial::Recv(char* res, size_t res_size)
{
  while (m_recv_count <  m_recv_size - 1) {
    if (m_stream->available()) {
      char recv_data = m_stream->read();
      m_recv[m_recv_count++] = recv_data;
      if (recv_data == '>' || recv_data == '*') break;
    }
    else
      return 0;
  }
  if (m_recv_count > res_size - 1)
    m_recv_count = res_size - 1;
  for (int i = 0; i < m_recv_count; ++i)
    res[i] = m_recv[i];
  res[m_recv_count] = '\0';
  m_recv_count = 0;
  return strlen(res);
}
