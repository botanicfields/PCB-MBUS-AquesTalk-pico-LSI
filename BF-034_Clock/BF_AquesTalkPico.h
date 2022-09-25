// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack

#pragma once

class AquesTalkPico {
 public:
  AquesTalkPico();
  ~AquesTalkPico();

  virtual int    Send(const char* msg) = 0;
  virtual size_t Recv(char* res, size_t res_size) = 0;
  virtual bool   Busy() = 0;

  int ShowRes(int res_length_to_show = 1);
  int DumpEeprom();
  int WriteEeprom(int address, int data);
  int WritePresetMsg(const char* msg[], int num_of_msg);
  int WriteSpeed(int speed = 100);
  int WritePause(int pause = 65535);
  int WriteSerialSpeed(int serial_speed = 9600);
  int WriteI2cAddress(int i2c_address = 0x2e);

 private:
  char HexChar(int n);
};
