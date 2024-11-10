#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin tanımlamaları
#define SCK 5      // GPIO5 - SCK
#define MISO 19    // GPIO19 - MISO
#define MOSI 27    // GPIO27 - MOSI
#define SS 18      // GPIO18 - CS
#define RST 14     // GPIO14 - RESET
#define DI0 26     // GPIO26 - IRQ

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Seri haberleşmeyi başlat
  Serial.begin(115200);
  while (!Serial);

  // OLED ekranı başlat
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C OLED ekranın I2C adresidir
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("LoRa Alici");
  display.display();

  Serial.println("LoRa Alici Baslatiliyor...");

  // SPI pinlerini başlat
  SPI.begin(SCK, MISO, MOSI, SS);
  
  // LoRa modülünü başlat
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) { // 433 MHz frekansı kullanılıyor
    Serial.println("LoRa Başlatılamadı!");
    while (1);
  }
  Serial.println("LoRa Başlatıldı.");
}

void loop() {
  // Gelen paketi kontrol et
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Mesajı oku
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    // Mesajı seri port ve OLED ekrana yaz
    Serial.print("Mesaj alındı: ");
    Serial.println(incoming);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Mesaj alindi: ");
    display.print(incoming);
    display.display();
  }
}
