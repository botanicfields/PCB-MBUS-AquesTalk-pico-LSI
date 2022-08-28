// Copyright 2021 BotanicFields, Inc.
// M5Stack LCD Sound Wave

#include <M5Stack.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"   // for esp_adc_cal_characteristics_t
#include "BF_M5AdcWave.h"

// finite impulse response filter
const int fir_frame_size(150);
const int fir_tap_size(20);
const double fir_coef[]
  = { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, };  // moving average
int fir_frame[fir_frame_size];
int ring_buf[fir_tap_size];
int ring_buf_head(0);

// analog to digital converter
esp_adc_cal_characteristics_t adc_chars;

// LCD display
const int wave_x_size = fir_frame_size;
const int wave_y_size(100);
const int wave_x(160);
const int wave_y(100);
const int face_x_size(120);
const int face_y_size(120);
const int face_x(25);
const int face_y(25);
const int dead_zone(200);  // to detect inactivity of AquesTalk pico LSI
TFT_eSprite wave = TFT_eSprite(&M5.Lcd);
TFT_eSprite face = TFT_eSprite(&M5.Lcd);

void AdcWaveInit()
{
  // ADC set up
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);  // GPIO35, attenuation 11dB
  adc1_config_width(ADC_WIDTH_BIT_12);                         // width 12bit
  const int default_vref(1100);
  esp_adc_cal_value_t cal_value = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, default_vref, &adc_chars);

  // Check type of calibration value to characterize ADC
  String msg = "[AdcWaveInit] cal_value = ";
  switch (cal_value) {
    case ESP_ADC_CAL_VAL_EFUSE_VREF: msg += "eFuse Vref";  break;
    case ESP_ADC_CAL_VAL_EFUSE_TP:   msg += "Two Point";   break;
    default:                         msg += "Default";     break;
  }
  Serial.println(msg);

  // ring buffer for FIR
  ring_buf_head = 0;

  // LCD
  wave.createSprite(wave_x_size, wave_y_size);
  face.createSprite(face_x_size, face_y_size);
}

void AdcWave()
{
  unsigned int start_ms = millis();

  // read ADC for one frame of LCD
  for (int i = - fir_tap_size; i < fir_frame_size; ++i) {

    // read ADC into ring buffer
    if(++ring_buf_head >= fir_tap_size)
      ring_buf_head = 0;
    ring_buf[ring_buf_head] = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_7), &adc_chars);

    // FIR filtering process on the ring buffer
    if (i >= 0) {
      double fir_sum = 0.0;
      for (int j = 0; j < fir_tap_size; ++j) {
        int k = ring_buf_head - j;
        if (k < 0)
          k += fir_tap_size;
        fir_sum += fir_coef[j] * (double)ring_buf[k];
      }
      fir_frame[i] = (int)fir_sum;
    }
  }
  unsigned int elapsed_ms = millis() - start_ms;

  // statistics
  int sum = fir_frame[0];
  int min = fir_frame[0];
  int max = fir_frame[0];
  for (int i = 1; i < fir_frame_size; ++i) {
    sum += fir_frame[i];
    min = min > fir_frame[i] ? fir_frame[i] : min;
    max = max < fir_frame[i] ? fir_frame[i] : max;
  }
  int ave = sum / fir_frame_size;
  int p2p = max - min;
  Serial.printf("[AdcWave] %dms, min %d, ave %d, max %d, p2p %d\n", elapsed_ms, min, ave, max, p2p);


  // wave on LCD
  wave.fillSprite(TFT_DARKGREY);
  wave.drawRect(0, 0, wave_x_size, wave_y_size, TFT_LIGHTGREY);
  if (min < dead_zone) {
    for (int i = 0; i < fir_frame_size; ++i)
      wave.drawPixel(i, wave_y_size / 2, TFT_WHITE);
  } else {
    for (int i = 0; i < fir_frame_size; ++i)
      wave.drawPixel(i, wave_y_size - fir_frame[i] * wave_y_size / 2 / ave, TFT_WHITE);
  }
  wave.pushSprite(wave_x, wave_y);

  // ellipse on LCD
  int eye_size = face_x_size / 24;
  int eye_r_x  = face_x_size * 1 / 4;
  int eye_l_x  = face_x_size * 3 / 4;
  int eyes_y   = face_y_size * 2 / 5;
  int mouth_size  = face_x_size / 4;
  int mouth_x     = face_x_size / 2;
  int mouth_y     = face_y_size * 3 / 4;
  const int mouse_close = 2;
  face.fillSprite(TFT_BLACK);
  if (min < dead_zone) {
    face.fillEllipse(eye_r_x, eyes_y, eye_size, eye_size, TFT_CYAN);
    face.fillEllipse(eye_l_x, eyes_y, eye_size, eye_size, TFT_CYAN);
    face.drawEllipse(mouth_x, mouth_y, mouth_size, mouse_close, TFT_CYAN);
  } else {
    face.fillEllipse(eye_r_x, eyes_y, eye_size, eye_size - p2p * eye_size / 2 / ave, TFT_CYAN);
    face.fillEllipse(eye_l_x, eyes_y, eye_size, eye_size - p2p * eye_size / 2 / ave, TFT_CYAN);
    face.drawEllipse(mouth_x, mouth_y, mouth_size, mouse_close + p2p * mouth_size / 2 / ave, TFT_CYAN);
  }
  face.pushSprite(face_x, face_y);
}
