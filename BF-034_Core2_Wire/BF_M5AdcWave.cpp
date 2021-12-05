// Copyright 2021 BotanicFields, Inc.
// M5Stack LCD Sound Wave

#include <M5Core2.h>
#include <Free_Fonts.h>
// #include "driver/adc.h" // included by "esp_adc_cal.h"
#include "esp_adc_cal.h"   // for esp_adc_cal_characteristics_t
#include "BF_M5AdcWave.h"

const int    fir_size(20);
const double fir_coef[] =
 { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, };  // moving average
int ring_buf[fir_size];
int ring_buf_head(0);
const int fir_out_size(150);
int       fir_out[fir_out_size];

esp_adc_cal_characteristics_t adc_chars;

const int lcd_x_size = fir_out_size;
const int lcd_y_size(100);
TFT_eSprite spr1 = TFT_eSprite(&M5.Lcd);
TFT_eSprite spr2 = TFT_eSprite(&M5.Lcd);

void AdcWaveInit()
{
  adc_power_on();
  adc_gpio_init(ADC_UNIT_1, ADC_CHANNEL_7);                    // ADC1 Channel 7 = GPIO35
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);  // attenuation 11dB
  adc1_config_width(ADC_WIDTH_BIT_12);                         // width 12bit

  //Characterize ADC at particular ATTEN
  const int default_vref(1100);
  esp_adc_cal_value_t cal_value = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, default_vref, &adc_chars);

  //Check type of calibration value used to characterize ADC
  String msg = "[AdcWaveInit] cal_value = ";
  switch (cal_value) {
  case ESP_ADC_CAL_VAL_EFUSE_VREF: msg += "eFuse Vref"; break;
  case ESP_ADC_CAL_VAL_EFUSE_TP:   msg += "Two Point";  break;
  default:                         msg += "Default";    break;
  }
  Serial.println(msg);

  // ring buffer and FIR
  ring_buf_head = 0;

  // LCD
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FSS9);  // FreeSans9pt7b, height=22px
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextDatum(MC_DATUM);  // Align Middle-Center
  M5.Lcd.setTextColor(TFT_BLACK, TFT_OLIVE);
  M5.Lcd.fillRect(  0,   0, 320, 22, TFT_OLIVE);  M5.Lcd.drawString("BotanicFields, Inc.", 160, 11, GFXFF);
  M5.Lcd.fillRect( 40, 218,  50, 22, TFT_OLIVE);  M5.Lcd.drawString("Prev",  65, 228, GFXFF);
  M5.Lcd.fillRect(135, 218,  50, 22, TFT_OLIVE);  M5.Lcd.drawString("Play", 160, 228, GFXFF);
  M5.Lcd.fillRect(230, 218,  50, 22, TFT_OLIVE);  M5.Lcd.drawString("Next", 255, 228, GFXFF);
  spr1.createSprite(lcd_x_size, lcd_y_size);
  spr2.createSprite(120, 120);
}

void AdcWave()
{
  int start_ms = millis();

  // read ADC for one frame of LCD
  for (int i = -100; i < fir_out_size; ++i) {

    // read ADC into ring buffer
    if(++ring_buf_head >= fir_size)
      ring_buf_head = 0;
    ring_buf[ring_buf_head] = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_7), &adc_chars);

    // FIR filter on the ring buffer
    double fir_sum = 0.0;
    for (int j = 0; j < fir_size; ++j) {
      int k = ring_buf_head - j;
      if (k < 0)
        k += fir_size;
      fir_sum += fir_coef[j] * (double)ring_buf[k];
    }

    // output of FIR filter
    if (i >= 0)
      fir_out[i] = (int)fir_sum;

    delayMicroseconds(10);
  }

  // statistics
  int sum = fir_out[0];
  int min = fir_out[0];
  int max = fir_out[0];
  for (int i = 1; i < fir_out_size; ++i) {
    sum += fir_out[i];
    min = min > fir_out[i] ? fir_out[i] : min;
    max = max < fir_out[i] ? fir_out[i] : max;
  }
  int ave = sum / fir_out_size;
  int pp = max - min;
  int elapsed_ms = millis() - start_ms;
  Serial.printf("[AdcWave] %dms, min %d, ave %d, max %d, pp %d\n", elapsed_ms, min, ave, max, pp);

  // wave on LCD
  spr1.fillSprite(TFT_DARKGREY);
  spr1.drawRect(0, 0, lcd_x_size, lcd_y_size, TFT_LIGHTGREY);
  for (int i = 0; i < fir_out_size; ++i)
    spr1.drawPixel(i, lcd_y_size / 2 - (fir_out[i] - ave) / 20, TFT_WHITE);
  spr1.pushSprite(160, 100);

  // ellipse on LCD
  spr2.fillSprite(TFT_BLACK);
  spr2.fillEllipse(30, 50,  5, 5 - pp / 300, TFT_CYAN);
  spr2.fillEllipse(90, 50,  5, 5 - pp / 400, TFT_CYAN);
  spr2.drawEllipse(60, 90, 30, 1 + pp /  50, TFT_CYAN);
  spr2.pushSprite(25, 25);
}
