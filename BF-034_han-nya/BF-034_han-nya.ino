// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack
// example

#include <M5Stack.h>
#include <Free_Fonts.h>
#include "BF_AquesTalkPicoWire.h"
#include "BF_M5AdcWave.h"

AquesTalkPicoWire aqtp;

// Han-nya Shingyo
const char* preset_msg[] = {
// message must be less than 127 characters
// ....:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8....:....9....:....A....:....8....:....C....:..
  "maka-/hannya-haramita-/si'n-gyo-.\r",
  "kanji-zaibo-sa-gyo-jin/hannya-ha-ra-mi-ta-ji-/sho-kengo-onkaiku-do-/issaiku-yaku\r",
  "sha-ri-si-/sikifu-i-ku-/ku-fu-i-siki/sikisokuze-ku-/ku-sokuze-siki/jyu-so-gyo-shikiyaku/bu-nyo-ze-\r",
  "sha-ri-si-ze-/sho-ho-ku-so-/fu-sho-fu-metu/fu-ku-fu-jyo-/fu-zo-fu-gen\r",
  "ze-ko-ku-chu-/mu-siki/mu-jyu-so-gyo-shiki/mu-genni-bi-zessinni-/mu-sikisho-ko-mi-sokuho-/mu-genkainaisi-/mu-i-sikikai\r",
  "mu-mu-myo-yaku/mu-mu-myo-jinnaisi-/mu-ro-si-yaku/mu-ro-si-jin/mu-ku-shu-metudo-/mu-ti-yaku/mu-toku\r",
  "i-mu-sho-tokuko-/bo-daisatta-e-/hannya-ha-ra-mi-ta-ko-\r",
  "sinmu-ke-ge-/mu-ke-ge-ko-/mu-u-ku-fu-/onri-issai/tendo-mu-so-/ku-gyo-ne-han/sanze-sho-butue-/hannya-ha-ra-mi-ta-ko-\r",
  "tokua-nokuta-ra-/sanmyakusanbo-daiko-ti-/hannya-ha-ra-mi-ta-\r",
  "ze-daijinshu-/ze-daimyo-shu-/ze-mu-jyo-shu-/ze-mu-to-do-shu-/no-jo-issaiku-/sinjitufu-ko-\r",
  "ko-setuhannya-ha-ra-mi-ta-shu-/sokusesshu-wa'tu\r",
  "gya-te-/gya-te-/ha-ra-gya-te-/haraso-gya-te-/bo-ji-sowaka-/hannya-sin-gyo-.\r",
};

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// main
enum play_command_t {
  play_stop,
  play_current,
  play_next,
  play_previous,
  play_continuous,  // play continuously from current
  play_forward,     // play continuously from next
  play_backward,    // play continuously from previous
};
play_command_t play_command(play_stop);
int msg_selected(0);
const unsigned int msg_space(500);
const unsigned int loop_period_ms(100);
      unsigned int loop_last_ms;

void setup()
{
  const bool lcd_enable(true);
  const bool sd_enable(true);
  const bool serial_enable(true);
  const bool i2c_enable(true);  // SCL = GPIO22, SDA = GPIO21, frequency = 100kHz
  M5.begin(lcd_enable, sd_enable, serial_enable, i2c_enable);
  aqtp.Begin(Wire);             // default or safe mode

  // "true" if sleep pin is connected
  if (true /*false*/) {
    const int aqtp_sleep_pin(13);  // GPIO13 for sleep pin of M-BUS module
    pinMode(aqtp_sleep_pin, OUTPUT);
    digitalWrite(aqtp_sleep_pin, HIGH);
  }

  // "true" to write i2c address into EEPROM
  if (/*true*/ false)
    aqtp.WriteI2cAddress(0x2E);  // change i2c address to customize

  // "true" to write preset message into EEPROM
  if (/*true*/ false)
    aqtp.WritePresetMsg(preset_msg, sizeof(preset_msg)/sizeof(preset_msg[0]));

  // "true" to dump EEPROM to the serial monitor
  if (/*true*/ false)
    aqtp.DumpEeprom();

  aqtp.Send("#V\r");  // read version
  for (int i = 0; i < 10; ++i) {
    aqtp.ShowRes();
    delay(200);
  }
  aqtp.Send("#J\r");  // chime sound J
  for (int i = 0; i < 10; ++i) {
    aqtp.ShowRes();
    delay(200);
  }
  aqtp.Send("#K\r");  // chime sound K
  for (int i = 0; i < 10; ++i) {
    aqtp.ShowRes();
    delay(200);
  }

  // for Han-nya Shingyo
  aqtp.WriteSpeed(70);
  aqtp.WritePause();

  // play control
  play_command = play_stop;
  msg_selected = 0;

  // LCD control
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FSS9);  // FreeSans9pt7b, height=22px
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_OLIVE);
  M5.Lcd.setTextDatum(TC_DATUM);  // Align Middle-Center
  M5.Lcd.setTextPadding(M5.Lcd.width());
  M5.Lcd.drawString("BotanicFields, Inc.", M5.Lcd.width() / 2, 0, GFXFF);
  M5.Lcd.setTextDatum(BC_DATUM);  // Align Bottom-Center
  M5.Lcd.setTextPadding(M5.Lcd.width() / 6);
  M5.Lcd.drawString("Prev", M5.Lcd.width() * 21 / 100, M5.Lcd.height(), GFXFF);
  M5.Lcd.drawString("Play", M5.Lcd.width() * 50 / 100, M5.Lcd.height(), GFXFF);
  M5.Lcd.drawString("Next", M5.Lcd.width() * 79 / 100, M5.Lcd.height(), GFXFF);
  AdcWaveInit();

  // loop control
  loop_last_ms = millis();
}

void loop()
{
  // buttons
  M5.update();
  if (M5.BtnA.wasReleased()) {
    if (aqtp.Busy()) {
      aqtp.Send("$");  // Abort
    }
    play_command = play_previous;
  }
  if (M5.BtnB.wasReleased()) {
    if (aqtp.Busy()) {
      aqtp.Send("$");  // Abort
      play_command = play_stop;
    }
    else {
      msg_selected = 0;
      play_command = play_continuous;
    }
  }
  if (M5.BtnC.wasReleased()) {
    if (aqtp.Busy()) {
      aqtp.Send("$");  // Abort
    }
    play_command = play_forward;
  }

  // play messages
  int num_of_msg = sizeof(preset_msg)/sizeof(preset_msg[0]);
  switch (play_command) {
    case play_current:
      if (!aqtp.Busy()) {
        aqtp.Send(preset_msg[msg_selected]);
        play_command = play_stop;
      }
      break;
    case play_next:
      if (!aqtp.Busy()) {
        if (++msg_selected >= num_of_msg)
          msg_selected = 0;
        aqtp.Send(preset_msg[msg_selected]);
        play_command = play_stop;
      }
      break;
    case play_previous:
      if (!aqtp.Busy()) {
        if (--msg_selected < 0)
          msg_selected = num_of_msg - 1;
        aqtp.Send(preset_msg[msg_selected]);
        play_command = play_stop;
      }
      break;
    case play_continuous:
      if (!aqtp.Busy()) {
        delay(msg_space);
        aqtp.Send(preset_msg[msg_selected]);
        if (++msg_selected >= num_of_msg)
          msg_selected = 0;
      }
      break;
    case play_forward:
      if (!aqtp.Busy()) {
        if (++msg_selected >= num_of_msg)
          msg_selected = 0;
        delay(msg_space);
        aqtp.Send(preset_msg[msg_selected]);
      }
      break;
    case play_backward:
      if (!aqtp.Busy()) {
        if (--msg_selected < 0)
          msg_selected = num_of_msg - 1;
        delay(msg_space);
        aqtp.Send(preset_msg[msg_selected]);
      }
      break;
    default:
        play_command = play_stop;
      break;
  }
  aqtp.ShowRes(2);
  AdcWave();

  // loop control
  unsigned int delay_ms(0);
  unsigned int elapse_ms = millis() - loop_last_ms;
  if (elapse_ms < loop_period_ms) {
    delay_ms = loop_period_ms - elapse_ms;
  }
  delay(delay_ms);
  loop_last_ms = millis();
//  Serial.printf("loop elapse = %dms\n", elapse_ms);  // for monitoring elapsed time
}
