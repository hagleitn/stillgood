#ifndef CLICK_HANDLER_H
#define CLICK_HANDLER_H

#ifdef SIM
#include "simulator.h"
#else
#include <Arduino.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif

  void setupClicks(int pin, void (*click)(), void (*doubleClick)(), void (*longClick)());
  void updateClicks(unsigned long t);

#ifdef __cplusplus
}
#endif

#endif
