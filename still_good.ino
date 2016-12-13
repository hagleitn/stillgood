#ifndef SIM
//#define DEBUG 1
#endif

#include "ClickHandler.h"
#include "Display.h"

#ifndef SIM
#include "LowPower.h"
#endif

unsigned long offset;
unsigned long lastCheck;
unsigned long lastLoop;
unsigned long overflows;
unsigned long overflowTime;

volatile unsigned long count;
volatile unsigned long manual;

#define DAY    (HOUR * 24ul)
#define HOUR   (MINUTE * 60ul)
#define MINUTE (SECOND * 60ul)
#define SECOND (1000ul)

int resetPin = 2; // only 2 and 3 can be used w/ interrupt on mini

unsigned long countInterval;
unsigned long checkInterval;

unsigned long countIntervals[] = {DAY, HOUR, MINUTE, SECOND};
unsigned long checkIntervals[] = {MINUTE, MINUTE, SECOND, SECOND / 10};


enum _state_t {
  BASE,
  POWERSAVE,
  SHOW_INTERVAL,
  MANUAL_ENTRY
};

typedef _state_t state_t;

volatile state_t state;
volatile unsigned long lastStateChange;

volatile int interval;

void setState(state_t s, unsigned long t) {
#ifdef DEBUG
  switch(s) {
  case BASE:
    Serial.println("BASE");
    break;
  case POWERSAVE:
    Serial.println("POWERSAVE");
    break;
  case MANUAL_ENTRY:
    Serial.println("MANUAL_ENTRY");
    break;
  case SHOW_INTERVAL:
    Serial.println("SHOW_INTERVAL");
  }
#endif
  state = s;
  lastStateChange = t;
}

void reset() {

  unsigned long t = lastLoop;

  lastCheck = offset = t;
  count = 0;
  manual = 0;
  overflows = 0;
  overflowTime = 0;
}

void setInterval(int i) {
  interval = i;
  checkInterval = checkIntervals[i];
  countInterval = countIntervals[i];
}

void changeInterval() {
  reset();
  setInterval((interval + 1) % 4);
}

void click() {
#ifdef DEBUG
  Serial.println("click");
#endif

  switch(state) {
  case BASE:
    reset();
    break;
  case POWERSAVE:
    setState(BASE, lastLoop);
    break;
  case SHOW_INTERVAL:
    // ignore
    break;
  case MANUAL_ENTRY:
    count++;
    manual++;
    setState(MANUAL_ENTRY, lastLoop);
    break;
  }
}

void doubleClick() {
#ifdef DEBUG
  Serial.println("double click");
#endif

  switch(state) {
  case BASE:
    changeInterval();
    setState(SHOW_INTERVAL, lastLoop);
    break;
  case POWERSAVE:
    setState(BASE, lastLoop);
    break;
  case SHOW_INTERVAL:
    changeInterval();
    setState(SHOW_INTERVAL, lastLoop);
    break;
  case MANUAL_ENTRY:
    count += 2;
    manual += 2;
    setState(MANUAL_ENTRY, lastLoop);
    break;
  }
}

void longClick() {
#ifdef DEBUG
  Serial.println("long click");
#endif

  switch(state) {
  case BASE:
    setState(MANUAL_ENTRY, lastLoop);
    break;
  case POWERSAVE:
    setState(BASE, lastLoop);
    break;
  case SHOW_INTERVAL:
    // ignore
    break;
  case MANUAL_ENTRY:
    count++;
    manual++;
    setState(MANUAL_ENTRY, lastLoop);
    break;
  }
}

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

  unsigned long t = millis();

  lastLoop = t;
  setInterval(0);
  reset();

  setState(BASE, t);

  setupDisplay();

  setupClicks(resetPin, &click, &doubleClick, &longClick);
}

void loop() {
  unsigned long t = millis();

  // handle user input
  updateClicks(t);

  // update time passed, etc
  if (t < lastCheck) {
    overflows++;
    overflowTime = lastLoop;
    lastCheck = t;
  } else if (t - lastCheck >= checkInterval) {
    count = t >= offset ?
      (overflowTime / countInterval * overflows) + (t - offset) / countInterval
      : (overflowTime / countInterval * overflows) - (offset - t) / countInterval;
    count += manual;
    lastCheck = t;

#ifdef DEBUG
    Serial.print("Time: ");
    Serial.print(t);
    Serial.print(",\t Passed: ");
    Serial.println(count);
#endif
  }

  // time transitions
  switch(state) {
  case BASE:
#ifndef SIM
    if (t >= lastStateChange && t - lastStateChange > 2 * MINUTE) {
      setState(POWERSAVE, t);
      clearAll();
    }
#endif
    break;
  case MANUAL_ENTRY:
    if (t >= lastStateChange && t - lastStateChange > 5 * SECOND) {
      setState(BASE, t);
    }
    break;
  case SHOW_INTERVAL:
    if (t >= lastStateChange && t - lastStateChange > 2 * SECOND) {
      setState(BASE, t);
    }
    break;
  default:
    // ignore
    break;
  }


  // update display
  switch(state) {
  case BASE:
    showCount(count);
    break;
  case POWERSAVE:
#ifndef SIM
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON,
                  SPI_OFF, USART0_OFF, TWI_OFF);
#else
    delay(8 * SECOND);
#endif
    break;
  case SHOW_INTERVAL:
    showInterval(interval);
    break;
  case MANUAL_ENTRY:
    if (((t - lastStateChange) / 300) % 2 == 0) {
      showCount(count);
    } else {
      showBytes(0, 0x00);
      showBytes(1, 0x00);
    }
    break;
  }

  lastLoop = t;
}
