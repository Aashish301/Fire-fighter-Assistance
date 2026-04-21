#include "sensor_dht.h"
#include <DHT.h>

#define DHTPIN  2
#define DHTTYPE DHT11

static DHT dht(DHTPIN, DHTTYPE);

#define SMOOTH_SIZE 5
static float tempBuffer[SMOOTH_SIZE] = {0};
static float humBuffer[SMOOTH_SIZE]  = {0};
static int   bufferIndex      = 0;
static int   samplesCollected = 0;

void dhtInit() {
  dht.begin();
}

DHTData dhtRead() {
  DHTData data;
  data.temperature = dht.readTemperature();
  data.humidity    = dht.readHumidity();
  data.valid       = !isnan(data.temperature) && !isnan(data.humidity);
  return data;
}

DHTData dhtReadSmooth() {
  DHTData raw = dhtRead();

  if (raw.valid) {
    tempBuffer[bufferIndex] = raw.temperature;
    humBuffer[bufferIndex]  = raw.humidity;
    bufferIndex = (bufferIndex + 1) % SMOOTH_SIZE;
    if (samplesCollected < SMOOTH_SIZE) samplesCollected++;
  }

  int count = (samplesCollected > 0) ? samplesCollected : 1;
  float avgTemp = 0, avgHum = 0;
  for (int i = 0; i < count; i++) {
    avgTemp += tempBuffer[i];
    avgHum  += humBuffer[i];
  }

  DHTData result;
  result.temperature = avgTemp / count;
  result.humidity    = avgHum  / count;
  result.valid       = (samplesCollected > 0);
  return result;
}
