// Copyright 2021 BotanicFields, Inc.
// M5Stack LCD Table Clock

#include <M5Stack.h>
#include <WiFi.h>
#include <Free_Fonts.h>
#include "BF_M5StackTableClock.h"

// for monitering RTC pins
const int pin_rtcx_clock_out(36);
const int pin_rtcx_interrupt35(35);
const int pin_rtcx_interrupt13(13);
bool rtcx_interrupt35(false);
bool rtcx_interrupt13(false);
int  rtcx_interrupt35_ms(0);
int  rtcx_interrupt13_ms(0);

void RtcxInterrupt35()
{
  rtcx_interrupt35 = true;
}

void RtcxInterrupt13()
{
  rtcx_interrupt13 = true;
}

// for LCD
const int lcd_size_x(320);
const int max_chars(26);
char str[max_chars + 1];  // +1 for '\0'
TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

// put text into a sprite, push the sprite to LCD
void SprStr(TFT_eSprite *spr, const char *str, const GFXfont *font, int fore_color, int back_color, int pos_x, int pos_y)
{
  spr->setFreeFont(font);
  spr->createSprite(lcd_size_x, spr->fontHeight());
  spr->fillSprite(back_color);
  spr->setTextSize(1);
  spr->setTextDatum(MC_DATUM);  // Middle-Center
  spr->setTextColor(fore_color, back_color);
  spr->drawString(str, lcd_size_x / 2, spr->fontHeight() / 2, GFXFF);
  spr->pushSprite(pos_x, pos_y);
  spr->deleteSprite();
}

void TableClockInit()
{
  pinMode(pin_rtcx_clock_out, INPUT);
  pinMode(pin_rtcx_interrupt35, INPUT);
  pinMode(pin_rtcx_interrupt13, INPUT);

  attachInterrupt(pin_rtcx_interrupt35, RtcxInterrupt35, FALLING);
  attachInterrupt(pin_rtcx_interrupt13, RtcxInterrupt13, FALLING);

  M5.Lcd.fillScreen(TFT_BLACK);
  SprStr(&spr, "BotanicFields, Inc.", FSS9, TFT_BLACK, TFT_OLIVE, 0, 0);  // FreeSans9pt7b, height=22px
}

void TableClock()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (tv.tv_usec / 100000 == 0) {  // every 0.1sec

    struct tm tm_now;
    getLocalTime(&tm_now);
    strftime(str, max_chars, "%H:%M:%S", &tm_now);
    SprStr(&spr, str, FSS24, TFT_WHITE, TFT_BLACK, 0,  30);  // FreeSans24pt7b, height=56px
    strftime(str, max_chars, "%A", &tm_now);
    SprStr(&spr, str, FSS12, TFT_WHITE, TFT_BLACK, 0,  90);  // FreeSans12pt7b, height=29px
    strftime(str, max_chars, "%B %d, %Y", &tm_now);
    SprStr(&spr, str, FSS12, TFT_WHITE, TFT_BLACK, 0, 120);  // FreeSans12pt7b, height=29px

    if (WiFi.status() == WL_CONNECTED) {
      SprStr(&spr, WiFi.SSID().c_str(),               FSS9, TFT_LIGHTGREY, TFT_BLACK, 0, 195);  // FreeSans9pt7b, height=22px
      SprStr(&spr, WiFi.localIP().toString().c_str(), FSS9, TFT_LIGHTGREY, TFT_BLACK, 0, 217);  // FreeSans9pt7b, height=22px
    }
    else {
      SprStr(&spr, "", FSS9, TFT_WHITE, TFT_BLACK, 0, 195);  // FreeSans9pt7b, height=22px
      SprStr(&spr, "", FSS9, TFT_WHITE, TFT_BLACK, 0, 217);  // FreeSans9pt7b, height=22px
    }
  }

  // monitering RTCx pins
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  if (digitalRead(pin_rtcx_clock_out) == LOW)
    M5.Lcd.drawString("CLKO", 0, 175);
  else
    M5.Lcd.drawString("    ", 0, 175);

  if (rtcx_interrupt35) {
    M5.Lcd.drawString("INT35", 60, 175);
    rtcx_interrupt35 = false;
    rtcx_interrupt35_ms = millis();
  }
  if (millis() - rtcx_interrupt35_ms > 1000) {
    M5.Lcd.drawString("     ", 60, 175);
    rtcx_interrupt35_ms = millis();
  }
  if (rtcx_interrupt13) {
    M5.Lcd.drawString("INT13", 132, 175);
    rtcx_interrupt13 = false;
    rtcx_interrupt13_ms = millis();
  }
  if (millis() - rtcx_interrupt13_ms > 1000) {
    M5.Lcd.drawString("     ", 132, 175);
    rtcx_interrupt13_ms = millis();
  }
}
