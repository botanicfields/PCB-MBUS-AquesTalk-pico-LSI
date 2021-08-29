// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#ifndef INCLUDED_BF_AQUESTALKPICOWIRE_H
#define INCLUDED_BF_AQUESTALKPICOWIRE_H

#include <Wire.h>

class AquesTalkPicoWire {
 public:
  AquesTalkPicoWire();
  ~AquesTalkPicoWire();

  int Begin(TwoWire &wire);
  int Send(const char* msg);
  size_t Recv(char* res, int res_size);

 private:
  TwoWire* m_wire;
  const int address_aquestalk_pico = 0x2e;
};

#endif  // INCLUDED_BF_AQUESTALKPICOWIRE_H
