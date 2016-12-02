#ifdef SIM

/**
   This file is just a simple wrapper around the actual arduino file
   to run w/o a board. It mocks a bunch of arduino functions to be
   able to compile the sketch and print out the results of running the
   program.
*/

#include <stdio.h>

#define RISING 0
#define HIGH 1
#define LOW 0

#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

typedef char byte;

unsigned long _millis = 0;

unsigned long millis() {
  // wrap around every 50 days
  return _millis % (1000ul*60ul*60ul*24ul*50ul);
}

int digitalPinToInterrupt(int p) {
  return 0;
}

void (*_a)(); // reset callback

void attachInterrupt(int p, void (*a)(), int m) {
  _a = a;
}

void pinMode(int p, int m) {
  char *mode;

  if (m == 0) mode = "OUTPUT";
  if (m == 1) mode = "INPUT";
  if (m == 2) mode = "INPUT_PULLUP";
  printf("Pin %i: %s\n", p, mode);
}

int cnt = 0;
int cnt2 = 0;
int last = 0;
byte b = 0x01 << 6;
byte codes[];

/**
   These writes are used for the 7 segment display only. So every 7
   writes look up what was printed.
*/
void digitalWrite(int p, int v) {
  b = (b>>1) | (v<<6);
  if (++cnt == 7) {
    cnt = 0;
    for (int i = 0; i < 10; i++) {
      if (codes[i] == b) {
        if (++cnt2 == 2) {
          printf("%i%i\n",i,last);
          cnt2 = 0;
        }
        last = i;
      }
    }
    b = 0x00;
  }
}

void delay(int p) {
  _millis += p;
}

void setup();
void loop();
void reset();

int main(int argc, char **argv) {

  setup();

  for (int i = 1; i < 3; i++) {

    // simulate 200 days of runtime
    while(_millis < i*200ul*1000*60*60*24) {
      _millis += 1;
      loop();
    }

    // simulate pressing the reset.
    _a();
  }
  return 0;
}
#include "days_ago.ino"
#endif
