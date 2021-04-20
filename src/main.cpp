/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <M5Stack.h>
#include <BleKeyboard.h>

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

BleKeyboard bleKeyboard("esp-key");

void setup() {
  M5.begin();
  M5.Power.begin();
  Wire.begin();

  M5.Lcd.setTextFont(2);
  M5.Lcd.println("FACES Keyboard I2C Read Example");
  pinMode(KEYBOARD_INT, INPUT_PULLUP);

  M5.Lcd.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {

  if(bleKeyboard.isConnected()) {
    if(digitalRead(KEYBOARD_INT) == LOW) {
      Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
      while (Wire.available()) { 
        uint8_t key_val = Wire.read();                  // receive a byte as character
        if(key_val != 0) {
          if(key_val >= 0x20 && key_val < 0x7F) { // ASCII String
            bleKeyboard.print((char)key_val);
            M5.Lcd.print((char)key_val);
          } else {
            M5.Lcd.printf("0x%02X ",key_val);
          }
        }
      }
    }
    //Serial.println("Sending Enter key...");
    //bleKeyboard.print("Hello world");
    //bleKeyboard.write(KEY_RETURN);

    //Serial.println("Sending Ctrl+Alt+Delete...");
    //bleKeyboard.press(KEY_LEFT_CTRL);
    //bleKeyboard.press(KEY_LEFT_ALT);
    //bleKeyboard.press(KEY_DELETE);
    //delay(100);
    //bleKeyboard.releaseAll();

  }
  delay(10);
}