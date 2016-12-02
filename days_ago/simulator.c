#ifdef SIM

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
  return _millis % (1000ul*60ul*60ul*24ul*50ul);
}

int digitalPinToInterrupt(int p) {
  return 0;
}

void (*_a)();

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
    while(_millis < i*200ul*1000*60*60*24) {
      _millis += 1;
      loop();
    }
    _a();
  }
  return 0;
}
#include "days_ago.ino"
#endif
