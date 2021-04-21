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
            switch(key_val) {
              case 0xAC:// Alt+$ -> ctrl+spaceに割り当て
                M5.Lcd.printf("ctrl+space ");
                bleKeyboard.press(KEY_LEFT_CTRL);
                bleKeyboard.press(0x20);
                delay(100);
                bleKeyboard.releaseAll();
                break;
              case 0x0D:
                M5.Lcd.printf("Return ");
                bleKeyboard.write(KEY_RETURN);
                break;
              case 0x08:
                M5.Lcd.printf("BackSpace ");
                bleKeyboard.write(KEY_BACKSPACE);
                break;
              case 0x7F:
                M5.Lcd.printf("Del ");
                bleKeyboard.write(KEY_DELETE);
                break;
              case 0xBF:
                M5.Lcd.printf("Left ");
                bleKeyboard.write(KEY_LEFT_ARROW);
                break;
              case 0xC0:
                M5.Lcd.printf("Down ");
                bleKeyboard.write(KEY_DOWN_ARROW);
                break;
              case 0xC1:
                M5.Lcd.printf("Right ");
                bleKeyboard.write(KEY_RIGHT_ARROW);
                break;
              case 0xB7:
                M5.Lcd.printf("Up ");
                bleKeyboard.write(KEY_UP_ARROW);
                break;
              case 0xBA:
                M5.Lcd.printf("Tab ");
                bleKeyboard.write(KEY_TAB);
                break;
              default:
                break;
            }
          }
        }
      }
    }
    //Serial.println("Sending Enter key...");
    //bleKeyboard.print("Hello world");

    //Serial.println("Sending Ctrl+Alt+Delete...");
    //bleKeyboard.press(KEY_LEFT_CTRL);
    //bleKeyboard.press(KEY_LEFT_ALT);
    //bleKeyboard.press(KEY_DELETE);
    //delay(100);
    //bleKeyboard.releaseAll();

  }
  delay(10);
}