// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#ifndef INCLUDED_BF_AQUESTALKPICOSPI_H
#define INCLUDED_BF_AQUESTALKPICOSPI_H

#include <SPI.h>

class AquesTalkPicoSpi {
 public:
  AquesTalkPicoSpi();
  ~AquesTalkPicoSpi();

  int Begin(SPIClass &spi, int ss);
  int Send(const char* msg);
  size_t Recv(char* res, int res_size);

 private:
  SPIClass* m_spi;
  int m_ss;
};

#endif  // INCLUDED_BF_AQUESTALKPICOSPI_H
