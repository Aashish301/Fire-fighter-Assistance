#include "sensor_bmp280.h"
#include <Adafruit_BMP280.h>

static Adafruit_BMP280 bmp;
static bool initialised = false;

bool bmp280Init(uint8_t address) {
  initialised = bmp.begin(address);
  return initialised;
}

BMP280Data bmp280Read(float seaLevelHpa) {
  BMP280Data data;
  if (!initialised) {
    data.valid = false;
    return data;
  }
  data.temperature = bmp.readTemperature();
  data.pressure    = bmp.readPressure() / 100.0F;
  data.altitude    = bmp.readAltitude(seaLevelHpa);
  data.valid       = true;
  return data;
}
