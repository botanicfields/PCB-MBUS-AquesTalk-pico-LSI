// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#pragma once
#include "BF_AquesTalkPico.h"
#include <Wire.h>

class AquesTalkPicoWire : public AquesTalkPico {
 public:
  AquesTalkPicoWire();
  ~AquesTalkPicoWire();

  // 0x2e: defaut/safe-mode i2c address of AquesTalk pico LSI
  int    Begin(TwoWire &wire, int i2c_address = 0x2e);
  int    Send(const char* msg);
  size_t Recv(char* res, size_t res_size);
  bool   Busy();

 private:
  TwoWire* m_wire;
  int      m_i2c_address;
};
