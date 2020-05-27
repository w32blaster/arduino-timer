#define CLK 2
#define DT 3
#define SW 4
#define LED_PIN 8
#define CHAR_BUFF_SIZE 10

#include "lcdgfx.h"
#include "GyverEncoder.h"

DisplaySSD1306_128x32_I2C display(-1);
Encoder enc1(CLK, DT, SW, TYPE2);

uint16_t lastSelectedValueSeconds = 0;
char charBuf[CHAR_BUFF_SIZE];
bool isRunning = false;

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

  if (isRunning) {
    if (enc1.isClick()) {
       // stop running timer
       isRunning = false;
       printNumber(0);
    } else {
      processRunningTimer();   
    }
    
  } else {
    processTimerSetting();
  }
  
}

// Run timer every second
static void processRunningTimer() {
  lastSelectedValueSeconds = lastSelectedValueSeconds - 1;
  if (lastSelectedValueSeconds == 0) {
   printNumber(lastSelectedValueSeconds);
   isRunning = false; 
  } else {
   printNumber(lastSelectedValueSeconds);
   delay(1000); 
  }
}

// Set timer using encoder, when it is not running
static void processTimerSetting() {
  if (enc1.isTurn()) { 
    if (enc1.isRight()) {
      lastSelectedValueSeconds = lastSelectedValueSeconds + 10;
    } 
    else if (enc1.isLeft()) {
      lastSelectedValueSeconds = lastSelectedValueSeconds - 10;
    }
    printNumber(lastSelectedValueSeconds);
  }

  if (enc1.isClick()) {
    // Run the timer!
    isRunning = true;
  } 
}

static char* formatSeconds(uint16_t seconds) {
  // clear buffer. Because strings of characters are terminated by a zero byte, 
  // only the first byte needs to be zeroed
  charBuf[0] = (char)0;
  String str;
    
  if (seconds < 0) {
    str = "NaN";
  } else {
    int mins = seconds / 60;
    int secs = seconds % 60;
    str = (mins < 10 ? "0" : "") + String(mins) + ":" + (secs < 10 ? "0" : "") + String(secs); 
  }

  str.toCharArray(charBuf, CHAR_BUFF_SIZE);
  return charBuf;
}

static void printNumber(uint8_t number) {
    display.clear();
    display.printFixed(0,  0, formatSeconds(number), STYLE_NORMAL);
}
