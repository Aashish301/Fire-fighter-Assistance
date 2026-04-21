#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>

// Vega board requires TwoWire instance
TwoWire Wire(0);

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensors
#define MQ135_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;  // I2C interface

// Smoothing buffers
#define SMOOTH_SIZE 5
float tempBuffer[SMOOTH_SIZE] = {0};
float humBuffer[SMOOTH_SIZE] = {0};
int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  // DHT11 init
  dht.begin();

  // BMP280 init
  if (!bmp.begin(0x76)) {  // Check your BMP280 I2C address
    Serial.println("BMP280 not found!");
    while (1);
  }

  delay(1000);
}

void loop() {
  // ---- MQ135 reading ----
  int gasValue = analogRead(MQ135_PIN);

  // ---- DHT11 reading ----
  float dhtTemp = dht.readTemperature();
  float dhtHum  = dht.readHumidity();

  if (!isnan(dhtTemp) && !isnan(dhtHum)) {
    tempBuffer[bufferIndex] = dhtTemp;
    humBuffer[bufferIndex] = dhtHum;
    bufferIndex = (bufferIndex + 1) % SMOOTH_SIZE;
  } else {
    Serial.println("Failed to read DHT11");
  }

  // Compute average to smooth fluctuations
  float avgTemp = 0, avgHum = 0;
  for (int i = 0; i < SMOOTH_SIZE; i++) {
    avgTemp += tempBuffer[i];
    avgHum  += humBuffer[i];
  }
  avgTemp /= SMOOTH_SIZE;
  avgHum  /= SMOOTH_SIZE;

  // ---- BMP280 reading ----
  float bmpTemp = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; // hPa
  float altitude = bmp.readAltitude(1013.25);  // meters

  // ---- Serial output ----
  Serial.print("MQ135: "); Serial.println(gasValue);
  Serial.print("DHT Temp: "); Serial.print(avgTemp); Serial.println(" C");
  Serial.print("DHT Hum: "); Serial.print(avgHum); Serial.println(" %");
  Serial.print("BMP Temp: "); Serial.print(bmpTemp); Serial.println(" C");
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
  Serial.println("-----------------------");

// ---- OLED display ----
display.clearDisplay();

display.setTextSize(1);
display.setCursor(0,0);
display.print("Gas: "); display.println(gasValue);

display.setCursor(0,10);
display.print("DHT H: "); display.print(avgHum); display.println(" %");

display.setCursor(0,20);
display.print("BMP T: "); display.print(bmpTemp); display.println(" C");

display.setCursor(0,30);
display.print("Pressure: "); display.print(pressure); display.println(" hPa");

display.setCursor(0,40);
display.print("Altitude: "); display.print(altitude); display.println(" m");

display.display();

  delay(2000);  // Delay between readings
}
