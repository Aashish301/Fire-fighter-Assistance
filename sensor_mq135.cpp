#include "sensor_mq135.h"
#include <Arduino.h>

#define MQ135_PIN A0

void mq135Init() {
  pinMode(MQ135_PIN, INPUT);
}

int mq135Read() {
  return analogRead(MQ135_PIN);
}

bool mq135IsHigh(int value, int threshold) {
  return value > threshold;
}
