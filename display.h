#ifndef DISPLAY_H
#define DISPLAY_H

#define DIGITS 2
#define SEGMENTS 7

#ifdef SIM
#include "simulator.h"
#else
#include <Arduino.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif
  void setupDisplay();

  void clearAll();
  void clear(int pos);

  void showBytes(int pos, byte val);
  void showInterval(int i);
  void showCount(unsigned long count);

#ifdef __cplusplus
}
#endif

#endif
