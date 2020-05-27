#define CLK 2
#define DT 3
#define SW 4
#define LED_PIN 8
#define CHAR_BUFF_SIZE 10

#include "lcdgfx.h"
#include "GyverEncoder.h"

DisplaySSD1306_128x32_I2C display(-1);
Encoder enc1(CLK, DT, SW, TYPE2);

int lastSelectedValueSeconds = 0;
char charBuf[CHAR_BUFF_SIZE];

void setup() {
  
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  display.begin();
  display.clear();
  display.setFixedFont(comic_sans_font24x32_123);
  printNumber(0);
}

void loop() {
  enc1.tick();

  if (enc1.isTurn()) { 
    if (enc1.isRight()) {
      lastSelectedValueSeconds = lastSelectedValueSeconds + 10;
    } 
    else if (enc1.isLeft()) {
      lastSelectedValueSeconds = lastSelectedValueSeconds - 10;
    }
    printNumber(lastSelectedValueSeconds);
  }
  
}

static void printNumber(uint8_t number) {
    //display.setFixedFont(ssd1306xled_font6x8);
    // OK - display.setFixedFont(comic_sans_font24x32_123);
    display.clear();

    // clear buffer. Because strings of characters are terminated by a zero byte, 
    // only the first byte needs to be zeroed
    charBuf[0] = (char)0;
    String str = String(number) + ":00";
    str.toCharArray(charBuf, CHAR_BUFF_SIZE);
    display.printFixed(0,  0, charBuf, STYLE_NORMAL);
}
