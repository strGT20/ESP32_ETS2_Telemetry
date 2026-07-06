#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// SH1106/SH1107 OLED 128x64 di I2C, address default 0x3C
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);

String lineBuf = "";
float speed = 0.0;
int rpm = 0;

void setup() {
  Serial.begin(115200);
  delay(200);

  // Init I2C OLED
  if (!display.begin(0x3C, true)) { // alamat default 0x3C
    Serial.println("OLED tidak terdeteksi!");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Waiting data...");
  display.display();
}

void loop() {
  // Baca serial sampai newline
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      parseLine(lineBuf);
      lineBuf = "";
      updateDisplay();
    } else if (c != '\r') {
      lineBuf += c;
      if (lineBuf.length() > 64) lineBuf = ""; // reset jika overflow
    }
  }
}

void parseLine(const String &s) {
  int comma = s.indexOf(',');
  if (comma <= 0) return;
  String sp = s.substring(0, comma);
  String rp = s.substring(comma + 1);
  speed = sp.toFloat();
  rpm = rp.toInt();
}

void updateDisplay() {
  display.clearDisplay();

  // Speed besar di tengah
  display.setTextSize(3); 
  display.setCursor(0, 0);
  display.print((int)speed);
  display.setTextSize(1);
  display.print(" km/h");

  // RPM di bawah
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("RPM ");
  display.print(rpm);

  display.display();
}
