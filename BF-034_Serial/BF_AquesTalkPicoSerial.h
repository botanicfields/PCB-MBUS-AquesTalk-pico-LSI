// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#pragma once
#include "BF_AquesTalkPico.h"

class AquesTalkPicoSerial : public AquesTalkPico {
 public:
  AquesTalkPicoSerial();
  ~AquesTalkPicoSerial();
  int    Begin(Stream &stream);
  int    Send(const char* msg);
  size_t Recv(char* res, size_t res_size);
  bool   Busy();

 private:
  Stream*   m_stream;
  const int m_recv_size = 10;
  char*     m_recv;
  int       m_recv_count;
};
