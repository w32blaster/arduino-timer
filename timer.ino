#include "lcdgfx.h"
#include <Wire.h>
DisplaySSD1306_128x32_I2C display(-1);

const int LED_PIN = 8;

void setup() {
  
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  display.begin();
  display.clear();
  
}

void loop() {
  
  display.setFixedFont(ssd1306xled_font6x8);
  display.printFixed (0,  0, "Dobroe utro, Altana!", STYLE_NORMAL);
}
