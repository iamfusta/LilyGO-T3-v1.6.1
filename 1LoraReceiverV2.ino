//alıcı için alınan mesaj counter eklendi,RSSI ve yeni gelen mesajlar ekranda kayarak yazdırılıyor. Biraz daha yakışıklı oldu :)

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
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

int messageCount = 0;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  while (!Serial);

  // Start OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is the I2C address for the OLED display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("LoRa Receiver - Msg: 0");
  display.display();

  Serial.println("Starting LoRa Receiver...");

  // Start SPI pins
  SPI.begin(SCK, MISO, MOSI, SS);
  
  // Start LoRa module
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) { // Using 433 MHz frequency
    Serial.println("LoRa Initialization Failed!");
    while (1);
  }
  Serial.println("LoRa Initialized.");
}

void loop() {
  // Check for incoming packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Increment message count
    messageCount++;

    // Read message
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    // Get RSSI of the received packet
    int rssi = LoRa.packetRssi();

    // Display message on serial port
    Serial.print("MSG: ");
    Serial.println(incoming);
    Serial.print("RSSI: ");
    Serial.println(rssi);

    // Update OLED display
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.printf("LoRa Rcvr Msg: %d", messageCount);

    // Display RSSI on the second line
    display.setCursor(0, 10);
    display.printf("RSSI: %d dBm", rssi);

    // Display messages starting from the third line
    display.setCursor(0, 20);

    static String allMessages = "";
    allMessages += incoming + "\n";

    // Ensure messages do not exceed display capacity
    int maxLines = SCREEN_HEIGHT / 8 - 2; // Height of one line is 8 pixels; first two lines are reserved
    int lineCount = 0;

    for (int i = allMessages.length() - 1; i >= 0 && lineCount < maxLines; --i) {
      if (allMessages[i] == '\n') {
        lineCount++;
      }
    }

    int startIdx = allMessages.indexOf('\n', allMessages.length() - lineCount * maxLines);
    if (startIdx == -1) {
      startIdx = 0;
    }

    display.print(allMessages.substring(startIdx));
    display.display();
  }
}
