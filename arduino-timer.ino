#define CLK 2
#define DT 3
#define SW 4
#define LED_RED_PIN 9
#define LED_GREEN_PIN 8
#define CHAR_BUFF_SIZE 10

// the screen can show only two numbers plus ":" plus two numbers, thus the maxumum time that can be displayed is "59:59"
#define MAX_TIME_SEC (59 * 60) + 59

#include "lcdgfx.h"
#include "GyverEncoder.h"
#include "TimerOne.h"

DisplaySSD1306_128x32_I2C display(-1);
Encoder enc1(CLK, DT, SW, TYPE2);

unsigned int currentSecond = 0;
char charBuf[CHAR_BUFF_SIZE];
bool isRunning = false;

void setup() {

  // put your setup code here, to run once:
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  
  turnGreen();

  display.begin();
  display.clear();
  display.setFixedFont(comic_sans_font24x32_123);
  printNumber(currentSecond);

  // we use Timer in order to catch button click ("cancel") white delay
  Timer1.initialize(1000); // 1ms
  Timer1.attachInterrupt(timerIsr);
}

void timerIsr() {
  enc1.tick(); 
}

void loop() {
 
  if (isRunning) {
    if (enc1.isClick()) {
      // cancel running timer
      isRunning = false;
      printNumber(0);
      turnGreen();
    } else {
      processRunningTimer();
    }

  } else {
    processTimerSetting();
  }

}

// Run timer every second
static void processRunningTimer() {
  currentSecond = currentSecond - 1;
  if (currentSecond == 0) {
    
    // time is run out, stop the timer
    printNumber(currentSecond);
    isRunning = false;
    turnGreen();
  } else {

    // continue ticking
    printNumber(currentSecond);
    delay(1000);
  }
}

// Set timer using encoder, when it is not running
static void processTimerSetting() {
  
  if (enc1.isTurn()) {

    // increase/decrease value
    if (enc1.isFastR()) currentSecond = currentSecond + 60; 
    else if (enc1.isRight()) currentSecond = currentSecond + 1;
    else if (enc1.isFastL()) currentSecond = currentSecond - 60;
    else if (enc1.isLeft()) currentSecond = currentSecond - 1;
    
    if (currentSecond > MAX_TIME_SEC) {
      currentSecond = 0;
    }

    printNumber(currentSecond);
  }

  if (enc1.isClick()) {
    // Run the timer!
    isRunning = true;
    turnRed();
  }
  
}

static char* formatSeconds(unsigned int seconds) {
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

static void printNumber(unsigned int number) {
  display.clear();
  display.printFixed(0,  0, formatSeconds(number), STYLE_NORMAL);
}

// we use LED with common anode, that means LOW is when light turned on and HIGH is off
static void turnGreen() {
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
}
static void turnRed() {
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
}
