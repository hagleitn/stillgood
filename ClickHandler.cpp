#include "ClickHandler.h"
#include "Timeline.h"

#ifndef SIM
//#define DEBUG
//#include "hardwareSerial.h"
#endif

void (*click)();
void (*longClick)();
void (*doubleClick)();

#define LONG_CLICK_INTERVAL 1000
#define DOUBLE_CLICK_INTERVAL 250
#define MIN_INTERVAL 50

enum _click_state_t {
  BASE,
  HALF_CLICK,
  CLICK,
  CLICK_AND_HALF
};

typedef enum _click_state_t click_state_t;

int pin;

volatile click_state_t click_state;
volatile ts lastClickStateChange;

volatile int clicks;
volatile int longClicks;
volatile int doubleClicks;

void setState(click_state_t s) {
#ifdef DEBUG
  switch(s) {
  case BASE:
    Serial.println("BASE");
    break;
  case HALF_CLICK:
    Serial.println("HALF_CLICK");
    break;
  case CLICK:
    Serial.println("CLICK");
    break;
  case CLICK_AND_HALF:
    Serial.println("CLICK_AND_HALF");
  }
#endif

  click_state = s;
  assignTime(&lastClickStateChange, now());
}

void rising() {
#ifdef DEBUG
  Serial.println("rising");
#endif

  switch(click_state) {
  case BASE:
    // ignore
    break;
  case HALF_CLICK:
    if (inInterval(&lastClickStateChange, LONG_CLICK_INTERVAL)) {
      setState(CLICK);
    }
    if (pastInterval(&lastClickStateChange, LONG_CLICK_INTERVAL)) {
      longClicks++;
      setState(BASE);
    }
    break;
  case CLICK:
    // ignore
    break;
  case CLICK_AND_HALF:
    if (inInterval(&lastClickStateChange, LONG_CLICK_INTERVAL)) {
      doubleClicks++;
      setState(BASE);
    }
    if (pastInterval(&lastClickStateChange, LONG_CLICK_INTERVAL)) {
      clicks++;
      longClicks++;
      setState(BASE);
    }
    break;
  }
}

void falling() {
#ifdef DEBUG
  Serial.println("falling");
#endif

  switch(click_state) {
  case BASE:
    setState(HALF_CLICK);
    break;
  case HALF_CLICK:
    // ignore
    break;
  case CLICK:
    if (!inInterval(&lastClickStateChange, MIN_INTERVAL)) {
      setState(CLICK_AND_HALF);
    }
    break;
  case CLICK_AND_HALF:
    // ignore
    break;
  }
}

void change() {
  int val = digitalRead(pin);

  if (click_state == CLICK
      && pastInterval(&lastClickStateChange, DOUBLE_CLICK_INTERVAL)) {
    setState(BASE);
    clicks++;
  }

  if (val == HIGH) {
    rising();
  } else {
    falling();
  }
}

void setupClicks(int _pin, void (*_click)(), void (*_doubleClick)(), void (*_longClick)()) {
  pin = _pin;
  click = _click;
  doubleClick = _doubleClick;
  longClick = _longClick;
  setState(BASE);

  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), &change, CHANGE);
}

void updateClicks() {

  if (click_state == CLICK
      && pastInterval(&lastClickStateChange, DOUBLE_CLICK_INTERVAL)) {
    setState(BASE);
    clicks++;
  }

  while(clicks > 0) {
    click();
    clicks--;
  }

  while(doubleClicks > 0) {
    doubleClick();
    doubleClicks--;
  }

  while(longClicks > 0) {
    longClick();
    longClicks--;
  }
}
