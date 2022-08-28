// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack
// example

#include <M5Core2.h>
#include <Free_Fonts.h>
#include "BF_AquesTalkPicoWire.h"
#include "BF_M5AdcWave_Core2.h"

AquesTalkPicoWire aqtp;

const char* preset_msg[] = {
// message must be less than 127 characters
// ....:....1....:....2....:....3....:....4....:....5....:....6....:....7....:....8....:....9....:....A....:....8....:....C....:..
  "re'-jide_su.yo'-koso wata'ku_sitati,sho'kka-e\r",
  "iti'jide_su.ittai,do'-natte/iru'node_suka zo'rutaisa\r",
  "ni'jide_su.ma'da,deki'nainde_suka sinigamiha'kase\r",
  "sa'njide_su.sho'kka-no,se'kai,seifuku/ke'ikakuwa cha_ku'cha_kuto,_susun'deima_su\r",
  "yo'jide_su.omedeto-/goza'i/ma'_su jikken'wa,dai'seiko-de_su\r",
  "go'jide_su.sho'kka-no/tikara'wa,muge'n/na'node_su\r",
  "roku'jide_su.urotae'/nai'de,kudasa'i,jigo'kutai_si\r",
  "siti'jide_su.wata'_ku_sitati,sho'kka-no/shuku'teki kamen'raida-o/tao'_sunode_su\r",
  "hati'jide_su.sa'-/yukina'sai sho'kka-no/mono'tatiyo\r",
  "ku'jide_su.kiki'masho- ana'tano,niho'n,seifuku/ke'ikakuo\r",
  "jyu'-jide_su.oda'marinasai,jigo'kutai_si su'beteno/sippa'iwa,ana'tano/se'ide_su\r",
  "jyu-iti'jide_su.sho'kka-wa,su'beteo/haka'i_si,se'kaio/_si'hai_sima_su\r",
  "jyu-ni'jide_su.kaizo-/ni'ngenga,se'kaio/ugo'ka_si sono/kaizo-/ni'ngen+o,_si'hai/suru'noga,wata'ku_side_su\r",
  "ohayo-/gozaima'_su.ta'datini,saga_si/da'_sunode_su\r",
  "oyasumi/nasa'i.se'kaiwa,sho'kka-no/mono'de_su\r",
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
  const bool i2c_enable(true);  // // SDA=GPIO32. SCL=GPIO33, frequency = 100kHz
  M5.begin(lcd_enable, sd_enable, serial_enable, i2c_enable, kMBusModeOutput);  // supply 5V to M-BUS modules

  const int      wire1_sda(21);      // GPIO21
  const int      wire1_scl(22);      // GPIO22
  const uint32_t wire1_frq(100000);  // 100kHz
  Wire1.begin(wire1_sda, wire1_scl, wire1_frq);
  aqtp.Begin(Wire1);          // default or safe mode
//  aqtp.Begin(Wire1, 0xXX);  // i2c address customized

  // "true" if sleep pin is connected
  if (true /*false*/) {
    const int aqtp_sleep_pin(19);  // GPIO19 for sleep pin of M-BUS module
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
      play_command = play_current;
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
