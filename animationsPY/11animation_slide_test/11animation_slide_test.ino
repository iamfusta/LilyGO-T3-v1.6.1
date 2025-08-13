/*
  LILYGO TTGO T3 v1.6.1 - SD Karttan Animasyon Slayt Gösterisi

  Bu kod, SD karttaki '/anim_bin/animation_list.txt' dosyasını okur,
  içindeki animasyon klasörlerini tespit eder ve bu animasyonları
  sürekli bir döngüde, slayt gösterisi gibi oynatır.
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Donanım Pin Ayarları ---
#define LORA_CS  18
#define SD_CS    13
#define SPI_MOSI 15
#define SPI_MISO 2
#define SPI_SCK  14

// --- OLED Ayarları ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RESET -1 

// --- Animasyon Ayarları ---
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64
#define FRAME_DELAY 42 // Kareler arası bekleme (ms)
#define MAX_ANIMATIONS 20 // SD karttan okunacak maksimum animasyon sayısı

// Nesneler
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SPIClass sdSPI(HSPI);

// Global Değişkenler
String animationList[MAX_ANIMATIONS];
int animationCount = 0;
int currentAnimationIndex = 0;
int currentFrameIndex = 0;
unsigned long nextFrameTime = 0;
byte frameBuffer[512]; // Bir kareyi RAM'de tutmak için

// ---------- OLED Mesaj Fonksiyonu ----------
void oledMsg(const String &l1, const String &l2 = "", const String &l3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(l1);
  display.println(l2);
  display.println(l3);
  display.display();
}

// ---------- Ana Kurulum Fonksiyonu ----------
void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED baslatilamadi"));
    for (;;);
  }
  oledMsg("Sistem Basliyor...");

  // LoRa'yı sustur, SPI'yı özel pinlerle başlat
  pinMode(LORA_CS, OUTPUT);
  digitalWrite(LORA_CS, HIGH);
  sdSPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

  if (!SD.begin(SD_CS, sdSPI, 20000000)) { // Hızı 20MHz'e çıkararak dene
    oledMsg("HATA:", "SD Kart", "Bulunamadi!");
    while(true);
  }
  oledMsg("SD Kart Hazir", "Liste okunuyor...");
  
  // Animasyon listesini SD karttan oku
  File listFile = SD.open("/anim_bin/animation_list.txt");
  if (listFile) {
    while (listFile.available() && animationCount < MAX_ANIMATIONS) {
      String animName = listFile.readStringUntil('\n');
      animName.trim(); // Satır sonu karakterlerini temizle
      if (animName.length() > 0) {
        animationList[animationCount] = animName;
        Serial.println("Bulunan animasyon: " + animName);
        animationCount++;
      }
    }
    listFile.close();
  } else {
    oledMsg("HATA:", "animation_list.txt", "bulunamadi!");
    while(true);
  }

  if (animationCount == 0) {
    oledMsg("UYARI:", "Listede animasyon", "bulunmuyor!");
    while(true);
  }
  
  oledMsg(String(animationCount) + " animasyon bulundu.", "Slayt gosterisi", "basliyor...");
  delay(2000);
}

// ---------- Ana Döngü ----------
void loop() {
  if (millis() < nextFrameTime) {
    return; // Henüz yeni kare zamanı gelmedi
  }
  nextFrameTime = millis() + FRAME_DELAY;

  // Mevcut animasyon ve kare için dosya yolunu oluştur
  String currentAnimationName = animationList[currentAnimationIndex];
  String framePath = "/anim_bin/" + currentAnimationName + "/anim_" + String(currentFrameIndex) + ".bin";

  File frameFile = SD.open(framePath, FILE_READ);
  if (frameFile) {
    // Dosya bulundu, oku ve ekrana çiz
    frameFile.read(frameBuffer, 512);
    frameFile.close();

    display.clearDisplay();
    display.drawBitmap((SCREEN_WIDTH - FRAME_WIDTH) / 2, (SCREEN_HEIGHT - FRAME_HEIGHT) / 2, frameBuffer, FRAME_WIDTH, FRAME_HEIGHT, 1);
    
    // İsteğe bağlı: Animasyon adını ekrana yazdır
    display.setTextSize(1);
    display.setCursor(2, 2);
    display.print(currentAnimationName);

    display.display();

    currentFrameIndex++; // Bir sonraki kareye geç
  } else {
    // Dosya bulunamadı, bu animasyonun sonu demektir
    Serial.println(currentAnimationName + " animasyonu bitti.");
    
    // Bir sonraki animasyona geç
    currentAnimationIndex++;
    if (currentAnimationIndex >= animationCount) {
      currentAnimationIndex = 0; // Listenin sonuna gelindiyse başa dön
    }
    currentFrameIndex = 0; // Yeni animasyon için kare sayacını sıfırla
    
    Serial.println("Siradaki animasyon: " + animationList[currentAnimationIndex]);
    delay(1000); // Animasyonlar arası kısa bir bekleme
  }
}
