#include <SPI.h>
#include <LoRa.h>

// Pin tanımlamaları
#define SCK 5      // GPIO5 - SCK
#define MISO 19    // GPIO19 - MISO
#define MOSI 27    // GPIO27 - MOSI
#define SS 18      // GPIO18 - CS
#define RST 14     // GPIO14 - RESET
#define DI0 26     // GPIO26 - IRQ

void setup() {
  // Seri haberleşmeyi başlat
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Başlatılıyor...");

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
  // LoRa mesajını gönder
  Serial.println("Mesaj gönderiliyor: merhaba");
  LoRa.beginPacket();
  LoRa.print("merhaba");
  LoRa.endPacket();

  // Bir süre bekle
  delay(100);
}
