// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#pragma once
#include "BF_AquesTalkPico.h"
#include <SPI.h>

class AquesTalkPicoSpi : public AquesTalkPico {
 public:
  AquesTalkPicoSpi();
  ~AquesTalkPicoSpi();
  int    Begin(SPIClass &spi, int ss);
  int    Send(const char* msg);
  size_t Recv(char* res, size_t res_size);
  bool   Busy();

 private:
  SPIClass* m_spi;
  int       m_ss;
};
