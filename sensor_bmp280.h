#pragma once

struct BMP280Data {
  float temperature; // °C
  float pressure;    // hPa
  float altitude;    // metres
  bool  valid;
};

bool       bmp280Init(uint8_t address = 0x76);
BMP280Data bmp280Read(float seaLevelHpa = 1013.25);
