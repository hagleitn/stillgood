#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>

#define RISING 0
#define CHANGE 1
#define HIGH 1
#define LOW 0

#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

#define DIGITS 2

typedef char byte;

class Serial_ {
 public:
  void begin(int i) {}
  void println(long l) {}
  void print(long l) {}
  void println(char *c) {}
  void print(char *c) {}
};

static Serial_ Serial;

#ifdef __cplusplus
extern "C"{
#endif
  void pinMode(int p, int m);
  void digitalWrite(int p, int v);
  int digitalRead(int p);
  void delay(int p);
  unsigned long millis();

  int digitalPinToInterrupt(int p);

  void attachInterrupt(int p, void (*a)(), int m);
#ifdef __cplusplus
}
#endif

#endif
