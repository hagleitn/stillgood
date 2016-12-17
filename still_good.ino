#ifndef SIM
//#define DEBUG 1
#endif

#include "ClickHandler.h"
#include "Display.h"
#include "Timeline.h"

#ifndef SIM
#include "LowPower.h"
#endif

ts lastCheck;

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
volatile ts lastStateChange;

volatile int interval;

void setState(state_t s) {
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
  assignTime(&lastStateChange,now());
}

void reset() {

  resetTime();

  assignTime(&lastCheck,now());
  count = 0;
  manual = 0;
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
    setState(BASE);
    break;
  case SHOW_INTERVAL:
    // ignore
    break;
  case MANUAL_ENTRY:
    count++;
    manual++;
    setState(MANUAL_ENTRY);
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
    setState(SHOW_INTERVAL);
    break;
  case POWERSAVE:
    setState(BASE);
    break;
  case SHOW_INTERVAL:
    changeInterval();
    setState(SHOW_INTERVAL);
    break;
  case MANUAL_ENTRY:
    count += 2;
    manual += 2;
    setState(MANUAL_ENTRY);
    break;
  }
}

void longClick() {
#ifdef DEBUG
  Serial.println("long click");
#endif

  switch(state) {
  case BASE:
    setState(MANUAL_ENTRY);
    break;
  case POWERSAVE:
    setState(BASE);
    break;
  case SHOW_INTERVAL:
    // ignore
    break;
  case MANUAL_ENTRY:
    count++;
    manual++;
    setState(MANUAL_ENTRY);
    break;
  }
}

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

  setInterval(0);
  reset();

  setState(BASE);

  setupDisplay();

  setupClicks(resetPin, &click, &doubleClick, &longClick);
}

void loop() {
  updateTime();

  // handle user input
  updateClicks();

  if (pastInterval(&lastCheck,checkInterval)) {
    count = numberIntervals(now(), countInterval);
    count += manual;
    assignTime(&lastCheck,now());

#ifdef DEBUG
    Serial.print("Time: ");
    Serial.print(now()->t);
    Serial.print(",\t Passed: ");
    Serial.println(count);
#endif
  }

  // time transitions
  switch(state) {
  case BASE:
#ifndef SIM
    if (pastInterval(&lastStateChange, 2 * MINUTE)) {
      setState(POWERSAVE);
      clearAll();
    }
#endif
    break;
  case MANUAL_ENTRY:
    if (pastInterval(&lastStateChange, 5 * SECOND)) {
      setState(BASE);
    }
    break;
  case SHOW_INTERVAL:
    if (pastInterval(&lastStateChange, 2 * SECOND)) {
      setState(BASE);
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
    if (((diff(now(),&lastStateChange)) / 300) % 2 == 0) {
      showCount(count);
    } else {
      showBytes(0, 0x00);
      showBytes(1, 0x00);
    }
    break;
  }
}
