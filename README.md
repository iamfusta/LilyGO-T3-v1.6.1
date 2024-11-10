# LilyGO-T3-v1.6.1
LilyGO T3 v1.6.1 basic transmitter receiver

# LoRa 433 MHz Transmitter and Receiver Project (LilyGO T3 v1.6.1)

This project provides a simple LoRa transmitter and receiver application using LilyGO T3 v1.6.1 LoRa boards operating at 433 MHz frequency. The transmitter module periodically broadcasts a "hello" message, and the receiver module displays this message on an OLED screen. All connections are made through the PCB, and no additional wiring is required.

## 💡 Project Summary

The goal of this project is to demonstrate wireless data transmission using LoRa technology. The transmitter and receiver codes provide a simple but effective example of data transmission. The received messages are displayed on both the serial monitor and the OLED screen.

## 🚀 Requirements

- **LilyGO T3 v1.6.1**: Boards with 433 MHz LoRa module.
- **Arduino IDE**: Required software to compile and upload the code.
- **Libraries**:
  - [LoRa.h](https://github.com/sandeepmistry/arduino-LoRa): Required for the LoRa module.
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library): Required for graphical drawing.
  - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306): Required to control the OLED screen.

## 📋 Setup

1. Open the Arduino IDE and add the necessary libraries via the "Library Manager."
2. Upload the transmitter and receiver codes to the respective boards.
3. When you power the boards, the transmitter broadcasts the "hello" message, and the receiver displays it on the OLED screen.

## 🗒️ Code Explanation

- **Transmitter Code**: Periodically sends the "hello" message over the serial port.
- **Receiver Code**: Listens for incoming messages and displays them on both the OLED screen and the serial monitor.

### Transmitter Code Summary
The transmitter controls the LoRa module via SPI pins and sends the "hello" message every 100 milliseconds.

### Receiver Code Summary
The receiver reads incoming packets using the LoRa module and writes the message to the OLED screen. With OLED integration, you can monitor the messages locally.

## 🌍 Usage

The project is very simple to use. Once both boards are powered on, the transmitter board broadcasts the message, and the receiver board displays the message on the OLED screen. No additional wiring or connections are needed by the user; all lines are connected via the PCB.

## 🚪 License

This project is licensed under the [GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007](https://www.gnu.org/licenses/gpl-3.0.en.html). Under this license, users are encouraged to share the project with their own code as open source. You are free to use, distribute, and make modifications. Please remember to share your contributions as open source as well.

## 🤖 Development Suggestions

- You can adjust the data transmission frequency to perform longer-range tests.
- Consider adding specific actions (such as lighting an LED or controlling a motor) based on the data received by the LoRa receiver.

If you have any questions or feedback, feel free to reach out via the "Issues" section on GitHub. Happy coding! 🛠️
![counter alıcı](https://github.com/user-attachments/assets/fb31c46f-9fa1-42b9-af26-cf11c5c7b88a)
![lora alici dtandby](https://github.com/user-attachments/assets/c5498736-fccb-48d1-a0c5-a271d1d96fab)
![lora almış](https://github.com/user-attachments/assets/8ea09bf7-b7db-4bd7-bf1c-511b9618e297)
