// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#ifndef INCLUDED_BF_AQUESTALKPICOWIRE_H
#define INCLUDED_BF_AQUESTALKPICOWIRE_H

#include <Wire.h>

class AquesTalkPicoWire {
 public:
  AquesTalkPicoWire();
  ~AquesTalkPicoWire();

  // 0x2e: defaut/safe-mode i2c address of AquesTalk pico LSI
  int Begin(TwoWire &wire, int i2c_address = 0x2e);
  int Send(const char* msg);
  size_t Recv(char* res, size_t res_size);

 private:
  TwoWire* m_wire;
  int m_i2c_address;
};

#endif  // INCLUDED_BF_AQUESTALKPICOWIRE_H
