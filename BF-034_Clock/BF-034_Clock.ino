// Copyright 2021 BotanicFields, Inc.
// BF-031 PCF8563 RTC Module for M-BUS
// BF-034 AquesTalk pico LSI for M-BUS
// example
#include <M5Stack.h>
#include <Free_Fonts.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include "BF_Pcf8563.h"
#include "BF_RtcxNtp.h"
#include "BF_ResetReason.h"
#include "BF_AquesTalkPicoWire.h"

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// for NTP
const char* time_zone  = "JST-9";
const char* ntp_server = "pool.ntp.org";
bool localtime_valid(false);
struct tm local_tm;

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// for LCD
const int max_chars(26);
char str[max_chars];
TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

// put text into a sprite, push the sprite to LCD
void SprStr(TFT_eSprite *spr, const char *str, const GFXfont *font, int fore_color, int back_color, int pos_x, int pos_y, int width)
{
  spr->setFreeFont(font);
  spr->setTextSize(1);
  spr->createSprite(width, spr->fontHeight());
  spr->fillSprite(back_color);
  spr->setTextColor(fore_color, back_color);
  spr->setTextDatum(MC_DATUM);  // Middle-Center
  spr->drawString(str, width / 2, spr->fontHeight() / 2, GFXFF);
  spr->pushSprite(pos_x, pos_y);
  spr->deleteSprite();
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// for WiFi
const int wifi_config_portal_timeout_sec(60);
const unsigned int wifi_retry_interval_ms(60000);
      unsigned int wifi_retry_last_ms(0);
const int wifi_retry_max_times(3);
      int wifi_retry_times(0);

wl_status_t wifi_status(WL_NO_SHIELD);

const char* wl_status_str[] = {
  "WL_IDLE_STATUS",      // 0
  "WL_NO_SSID_AVAIL",    // 1
  "WL_SCAN_COMPLETED",   // 2
  "WL_CONNECTED",        // 3
  "WL_CONNECT_FAILED",   // 4
  "WL_CONNECTION_LOST",  // 5
  "WL_DISCONNECTED",     // 6
  "WL_NO_SHIELD",        // 7 <-- 255
  "wl_status invalid",   // 8
};

const char* WlStatus(wl_status_t wl_status)
{
  if (wl_status >= 0 && wl_status <= 6) {
    return wl_status_str[wl_status];
  }
  if (wl_status == 255) {
    return wl_status_str[7];
  }
  return wl_status_str[8];
}

void WifiCheck()
{
  wl_status_t wifi_status_new = WiFi.status();
  if (wifi_status != wifi_status_new) {
    wifi_status = wifi_status_new;
    Serial.printf("[WiFi]%s\n", WlStatus(wifi_status));
  }

  // retry interval
  if (millis() - wifi_retry_last_ms < wifi_retry_interval_ms) {
    return;
  }
  wifi_retry_last_ms = millis();

  // reboot if wifi connection fails
  if (wifi_status == WL_CONNECT_FAILED) {
    Serial.print("[WiFi]connect failed: rebooting..\n");
    ESP.restart();
    return;
  }

  // let the wifi process do if wifi is not disconnected
  if (wifi_status != WL_DISCONNECTED) {
    wifi_retry_times = 0;
    return;
  }

  // reboot if wifi is disconnected for a long time
  if (++wifi_retry_times > wifi_retry_max_times) {
    Serial.print("[WiFi]disconnect timeout: rebooting..\n");
    ESP.restart();
    return;
  }

  // reconnect, and reboot if reconnection fails
  Serial.printf("[WiFi]reconnect %d\n", wifi_retry_times);
  if (!WiFi.reconnect()) {
    Serial.print("[WiFi]reconnect failed: rebooting..\n");
    ESP.restart();
    return;
  };
}

void WifiConfigModeCallback(WiFiManager *wm)
{
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.println("WiFi config portal:");
  M5.Lcd.println(wm->getConfigPortalSSID().c_str());
  M5.Lcd.println(WiFi.softAPIP().toString().c_str());
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// to test RTCx
const bool enable_clock_out(true);
const bool enable_int(true);
const int  timer_10s(10);
const int  timer_60s(60);
const bool enable_interrupt(true);
const bool pulse_mode(true);  // generate short pulse
const bool keep_flag(true);   // every 60sec
bool clock_out_interrupt_enable(false);

// to monitor RTCx pins
const int pin_rtcx_clock_out(36);
const int pin_rtcx_interrupt35(35);
const int pin_rtcx_interrupt13(13);
bool rtcx_exist(false);
bool rtcx_interrupt35(false);
bool rtcx_interrupt13(false);
bool lcd_clock_out(false);
bool lcd_interrupt35(false);
bool lcd_interrupt13(false);
unsigned int lcd_interrupt35_ms(0);
unsigned int lcd_interrupt13_ms(0);

void RtcxInterrupt35()
{
  rtcx_interrupt35 = true;
}

void RtcxInterrupt13()
{
  rtcx_interrupt13 = true;
}

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// for TTS(Text To Speech)
AquesTalkPicoWire aqtp;
const int tts_max(64);
char tts[tts_max];
bool aqtp_exist(false);

//..:....1....:....2....:....3....:....4....:....5....:....6....:....7..
// for loop control
const unsigned int loop_period_ms(100);  // 100ms
      unsigned int loop_last_ms;

void setup()
{
  const bool lcd_enable(true);
  const bool sd_enable(true);
  const bool serial_enable(true);
  const bool i2c_enable(true);
  M5.begin(lcd_enable, sd_enable, serial_enable, i2c_enable);

  M5.Lcd.wakeup();
  M5.Lcd.setBrightness(100);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextDatum(TL_DATUM);

  // to check reset by INT-pin of RTCx PCF8563
  RESET_REASON reset_reason_0 = rtc_get_reset_reason(0);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);   M5.Lcd.print("CPU0:");
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);  M5.Lcd.println(ResetReason(reset_reason_0));
  Serial.printf("Reset Reason CPU0: %d=", reset_reason_0);
  Serial.println(ResetReasonVerbose(reset_reason_0));
  RESET_REASON reset_reason_1 = rtc_get_reset_reason(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);   M5.Lcd.print("CPU1:");
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);  M5.Lcd.println(ResetReason(reset_reason_1));
  Serial.printf("Reset Reason CPU1: %d=", reset_reason_1);
  Serial.println(ResetReasonVerbose(reset_reason_1));

  // RTCx PCF8563: connect and start
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.print("I2C, RTCx:");
  if (rtcx.Begin(Wire) == 0) {
    rtcx_exist = true;
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);  M5.Lcd.println("OK");
    if (SetTimeFromRtcx(time_zone)) {
      localtime_valid = true;
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.println("RTCx valid");
    }
  }
  else {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);  M5.Lcd.println("NG");
  }
  if (!localtime_valid) {
    Serial.print("RTC not valid: set the localtime temporarily\n");
    local_tm.tm_year = 117;  // 2017 > 2016, getLocalTime() returns true
    local_tm.tm_mon  = 0;    // January
    local_tm.tm_mday = 1;
    local_tm.tm_hour = 0;
    local_tm.tm_min  = 0;
    local_tm.tm_sec  = 0;
    struct timeval tv = { mktime(&local_tm), 0 };
    settimeofday(&tv, NULL);
  }
  getLocalTime(&local_tm);
  Serial.print(&local_tm, "localtime: %A, %B %d %Y %H:%M:%S\n");
  // print sample: must be < 64
  //....:....1....:....2....:....3....:....4....:....5....:....6....
  //localtime: Wednesday, September 11 2021 11:10:46

  // RTCx PCF8563: for adjusting trimmer capacitor
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.print("Clock Out:");
  if (rtcx.ClockOutForTrimmer(enable_clock_out) == 0) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);  M5.Lcd.println("OK");
  }
  else {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);  M5.Lcd.println("NG");
  }

  // RTCx PCF8563: timer
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.print("Set timer:");
  double timer_return = rtcx.SetTimer(timer_10s);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);  M5.Lcd.print(timer_return);
  if (timer_return != 0.0) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);  M5.Lcd.println("OK");
  }
  else {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);  M5.Lcd.println("NG");
  }

  // RTCx PCF8563: timer interrupt
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.print("Enable Interrupt:");
  if (rtcx.EnableTimerInterrupt(enable_interrupt, pulse_mode, keep_flag) == 0) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);  M5.Lcd.println("OK");
  }
  else {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);  M5.Lcd.println("NG");
  }

  // WiFi start
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.println("WiFi connecting.. ");
  WiFiManager wm;  // blocking mode only

  // erase SSID/Key to force rewrite
  if (digitalRead(BUTTON_A_PIN) == LOW) {
    wm.resetSettings();
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);  M5.Lcd.println("SSID/Key erased");
    delay(3000);
  }

  // WiFi connect
  wm.setConfigPortalTimeout(wifi_config_portal_timeout_sec);
  wm.setAPCallback(WifiConfigModeCallback);
  if (wm.autoConnect()) {
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);  M5.Lcd.println("OK");
  }
  else {
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);  M5.Lcd.println("NG");
  }
  WiFi.setSleep(false);  // https://macsbug.wordpress.com/2021/05/02/buttona-on-m5stack-does-not-work-properly/
  wifi_retry_last_ms = millis() - wifi_retry_interval_ms;

  // NTP start
  NtpBegin(time_zone, ntp_server);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);  M5.Lcd.println("NTP start");
  delay(3000);

  // wait button-C to proceed into loop
  M5.Lcd.setTextColor(TFT_CYAN, TFT_BLACK);  M5.Lcd.println("Button-C to continue:");
  while (!M5.BtnC.wasReleased()) {
    M5.update();
    WifiCheck();
    if (RtcxUpdate()) {
      localtime_valid = true;  // SNTP sync completed
    }
  }

  // RTCx PCF8563: disable CLKO and INT
  rtcx.ClockOutForTrimmer(!enable_clock_out);
  rtcx.DisableTimer();
  rtcx.DisableTimerInterrupt();
  clock_out_interrupt_enable = false;

  // for RTCx monitor
  if (rtcx_exist) {
    pinMode(pin_rtcx_clock_out, INPUT);
    pinMode(pin_rtcx_interrupt13, INPUT);
    attachInterrupt(pin_rtcx_interrupt13, RtcxInterrupt13, FALLING);
//    pinMode(pin_rtcx_interrupt35, INPUT);
//    attachInterrupt(pin_rtcx_interrupt35, RtcxInterrupt35, FALLING);
  }

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
  M5.Lcd.drawString("CLKO", M5.Lcd.width() * 21 / 100, M5.Lcd.height(), GFXFF);
  M5.Lcd.drawString("Date", M5.Lcd.width() * 50 / 100, M5.Lcd.height(), GFXFF);
  M5.Lcd.drawString("Time", M5.Lcd.width() * 79 / 100, M5.Lcd.height(), GFXFF);

  // start TTS
  aqtp.Begin(Wire);
  int return_code = aqtp.Send("$");
  Serial.printf("AquesTalk pico LSI return code = %d\n", return_code);
  if (return_code == 0) {
    aqtp_exist = true;
    aqtp.ShowRes();
    aqtp.WriteSpeed();  // set default
    aqtp.WritePause();  // set default
  }

  // loop control
  loop_last_ms = millis();
}

void loop()
{
  // WiFi recovery
  WifiCheck();

  // NTP sync
  if (RtcxUpdate()) {
    localtime_valid = true;  // SNTP sync completed
  }
  getLocalTime(&local_tm);

  // table clock
  int forecolor = TFT_WHITE;
  if (!localtime_valid) {
    forecolor = TFT_BLACK;
  }
  strftime(str, max_chars, "%H:%M:%S",  &local_tm);  SprStr(&spr, str, FSS24, forecolor, TFT_BLACK, 0,  30, 320);  // FreeSans24pt7b, height=56px
  strftime(str, max_chars, "%A",        &local_tm);  SprStr(&spr, str, FSS12, forecolor, TFT_BLACK, 0,  90, 320);  // FreeSans12pt7b, height=29px
  strftime(str, max_chars, "%B %d, %Y", &local_tm);  SprStr(&spr, str, FSS12, forecolor, TFT_BLACK, 0, 120, 320);  // FreeSans12pt7b, height=29px

  forecolor = TFT_RED;
  if (wifi_status == WL_CONNECTED) {
    forecolor = TFT_BLACK;
  }
  SprStr(&spr, WlStatus(wifi_status),             FSS9, forecolor,     TFT_BLACK, 60, 151, 260);  // FreeSans9pt7b, height=22px

  SprStr(&spr, WiFi.SSID().c_str(),               FSS9, TFT_LIGHTGREY, TFT_BLACK, 60, 173, 260);  // FreeSans9pt7b, height=22px
  SprStr(&spr, WiFi.localIP().toString().c_str(), FSS9, TFT_LIGHTGREY, TFT_BLACK, 60, 195, 260);  // FreeSans9pt7b, height=22px

  // monitor RTCx
  if (rtcx_exist) {
    lcd_clock_out = false;
    if (digitalRead(pin_rtcx_clock_out) == LOW) {
      lcd_clock_out = true;
    }
    if (rtcx_interrupt13) {
      rtcx_interrupt13 = false;
      lcd_interrupt13 = true;
      lcd_interrupt13_ms = millis();
    }
    if (rtcx_interrupt35) {
      rtcx_interrupt35 = false;
      lcd_interrupt35 = true;
      lcd_interrupt35_ms = millis();
    }
    forecolor = TFT_BLACK;
    if (lcd_clock_out) {
      forecolor = TFT_YELLOW;
    }
    SprStr(&spr, "CLKO", FSS9, forecolor, TFT_BLACK, 0, 195, 60);  // FreeSans9pt7b, height=22px

    forecolor = TFT_BLACK;
    if (lcd_interrupt13) {
      forecolor = TFT_YELLOW;
      if (millis() - lcd_interrupt13_ms > 1000) {
        lcd_interrupt13 = false;
      }
    }
    SprStr(&spr, "INT13", FSS9, forecolor, TFT_BLACK, 0, 151, 60);  // FreeSans9pt7b, height=22px

    forecolor = TFT_BLACK;
    if (lcd_interrupt35) {
      forecolor = TFT_YELLOW;
      if (millis() - lcd_interrupt35_ms > 1000) {
        lcd_interrupt35 = false;
      }
    }
    SprStr(&spr, "INT35", FSS9, forecolor, TFT_BLACK, 0, 173, 60);  // FreeSans9pt7b, height=22px
  }

  M5.update();
  // toggle monitoring RTCx
  if (M5.BtnA.wasReleased()) {
    if (rtcx_exist) {
      if (clock_out_interrupt_enable) {
        clock_out_interrupt_enable = false;
        rtcx.ClockOutForTrimmer(!enable_clock_out);
        rtcx.DisableTimer();
        rtcx.DisableTimerInterrupt();
      }
      else {
        clock_out_interrupt_enable = true;
        rtcx.ClockOutForTrimmer(enable_clock_out);
        rtcx.SetTimer(timer_60s);
        rtcx.EnableTimerInterrupt(enable_interrupt, pulse_mode, keep_flag);
      }
    }
  }
  // speech date
  if (M5.BtnB.wasReleased()) {
    if (localtime_valid && aqtp_exist) {
      strftime(tts, tts_max, "<NUMK VAL=%Y COUNTER=nenn>,",  &local_tm);  aqtp.Send(tts);
      strftime(tts, tts_max, "<NUMK VAL=%m COUNTER=gatu>,",  &local_tm);  aqtp.Send(tts);
      strftime(tts, tts_max, "<NUMK VAL=%d COUNTER=nichi>,", &local_tm);  aqtp.Send(tts);
      switch (local_tm.tm_wday) {
        case  0: strcpy(tts, "nichi/yo'ubide_su\r"); break;
        case  1: strcpy(tts, "getsu/yo'ubide_su\r"); break;
        case  2: strcpy(tts, "ka/yo'ubide_su\r"   ); break;
        case  3: strcpy(tts, "sui/yo'ubide_su\r"  ); break;
        case  4: strcpy(tts, "moku/yo'ubide_su\r" ); break;
        case  5: strcpy(tts, "kin/yo'ubide_su\r"  ); break;
        case  6: strcpy(tts, "do/yo'ubide_su\r"   ); break;
        default: strcpy(tts, "de_su\r"            ); break;
      }
      aqtp.Send(tts);
    }
  }
  // speech time
  if (M5.BtnC.wasReleased()) {
    if (localtime_valid && aqtp_exist) {
      strftime(tts, tts_max, "<NUMK VAL=%H COUNTER=ji>,<NUMK VAL=%M COUNTER=funn>de_su\r", &local_tm);  aqtp.Send(tts);
                            //....:....1....:....2....:....3....:....4....:....5....:....6...
    }
  }
  if (aqtp_exist) {
    aqtp.ShowRes(2);
  }

  // loop control
  unsigned int delay_ms(0);
  unsigned int elapse_ms = millis() - loop_last_ms;
  if (elapse_ms < loop_period_ms) {
    delay_ms = loop_period_ms - elapse_ms;
  }
  delay(delay_ms);
  loop_last_ms = millis();
//  Serial.printf("loop elapse = %dms\n", elapse_ms);  // monitor elapsed time
}
