// Copyright 2021 BotanicFields, Inc.
// BF-034 AquesTalk pico LSI Module for M5Stack
// example

#include <M5Stack.h>
#include "BF_AquesTalkPicoSerial.h"
#include "BF_M5AdcWave.h"

AquesTalkPicoSerial aqtp;

const char* preset_msg[] = {
// msssage must be less than 127 characters
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

// for loop control
const int loop_ms(100);  // 100ms
unsigned int loop_last_ms(0);  // loop control
int msg_select(0);

void setup()
{
  const bool lcd_enable(true);
  const bool sd_enable(true);
  const bool serial_enable(true);
  const bool i2c_enable(true);
  M5.begin(lcd_enable, sd_enable, serial_enable, i2c_enable);

  // Serial2 interface defined by Arduino-ESP32
  //  const int serial2_rx(16);  // GPIO16 as default
  //  const int serial2_tx(17);  // GPIO17 as default
  //  Serial2.begin(speed, SERIAL_8N1, serial2_rx, serial2_tx);
  // select serial speed
  Serial2.begin(  9600);  // safe mode
//  Serial2.begin( 38400);  // ATP3011 works, ATP3012 works
//  Serial2.begin( 76800);  // ATP3012 works, max of ATP3011 but does not work
//  Serial2.begin(115200);  // max of ATP3012 but does not work
  while (!Serial2) {
    Serial.println("[AquesTalk LSI] Waiting Serial2");
    delay(100);
  }
  aqtp.Begin(Serial2);

  // designate "true" if sleep pin is connected
  if (true /*false*/) {
    const int aqtp_sleep_pin(13);  // GPIO13 for sleep pin of m-bux module
    pinMode(aqtp_sleep_pin, OUTPUT);
    digitalWrite(aqtp_sleep_pin, HIGH);

    // designate "true" to set speed automatic for ATP3011 & not safe mode
    if (/*true*/ false) {
      digitalWrite(aqtp_sleep_pin, LOW);
      delay(500);
      digitalWrite(aqtp_sleep_pin, HIGH);
      delay(80);
      aqtp.Send("?");
      for (int i = 0; i < 10; ++i) {
        aqtp.ShowRes();
        delay(200);
      }
    }
  }

  // designate "true" to set serial-speed into EEPROM of ATP3012
  if (/*true*/ false) {
//    aqtp.WriteSerialSpeed(  9600);
    aqtp.WriteSerialSpeed( 38400);
//    aqtp.WriteSerialSpeed( 76800);
//    aqtp.WriteSerialSpeed(115200);
    for (int i = 0; i < 10; ++i) {
      aqtp.ShowRes();
      delay(200);
    }
  }

  // designate "true" to write preset message into EEPROM
  if (/*true*/ false)
    aqtp.WritePresetMsg(preset_msg, sizeof(preset_msg)/sizeof(preset_msg[0]));

  // designate "true" to dump EEPROM to the serial monitor
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

  AdcWaveInit();

  loop_last_ms = millis();
}

void loop()
{
  M5.update();

  int num_of_msg = sizeof(preset_msg)/sizeof(preset_msg[0]);
  if (M5.BtnA.wasReleased()) {
    if (--msg_select < 0)
      msg_select = num_of_msg - 1;
    aqtp.Send(preset_msg[msg_select]);
  }

  if (M5.BtnB.wasReleased()) {
    aqtp.Send(preset_msg[msg_select]);
  }

  if (M5.BtnC.wasReleased()) {
    if (++msg_select >= num_of_msg)
      msg_select = 0;
    aqtp.Send(preset_msg[msg_select]);
  }

  aqtp.ShowRes(2);
  AdcWave();

  delay(loop_ms + loop_last_ms - millis());
  loop_last_ms = millis();
}
