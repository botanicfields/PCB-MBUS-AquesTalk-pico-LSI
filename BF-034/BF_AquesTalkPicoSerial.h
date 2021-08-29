// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#ifndef INCLUDED_BF_AQUESTALKPICOSERIAL_H
#define INCLUDED_BF_AQUESTALKPICOSERIAL_H

#include <Arduino.h>

class AquesTalkPicoSerial {
 public:
  AquesTalkPicoSerial();
  ~AquesTalkPicoSerial();

  int Begin(Stream &stream);
  int Send(const char* msg);
  size_t Recv(char* res, int res_size);

 private:
  Stream* m_stream;
  const int m_recv_size = 10;
  char* m_recv;
  int m_recv_count;
};

#endif  // INCLUDED_BF_AQUESTALKPICOSERIAL_H
