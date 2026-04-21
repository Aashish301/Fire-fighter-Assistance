#pragma once

struct DHTData {
  float temperature;
  float humidity;
  bool  valid;
};

void    dhtInit();
DHTData dhtRead();       // raw read
DHTData dhtReadSmooth(); // averaged over buffer
