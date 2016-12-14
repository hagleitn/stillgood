#ifndef CLICK_HANDLER_H
#define CLICK_HANDLER_H

#ifdef SIM
#include "Simulator.h"
#else
#include <Arduino.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif

  void setupClicks(int pin, void (*click)(), void (*doubleClick)(), void (*longClick)());
  void updateClicks();

#ifdef __cplusplus
}
#endif

#endif
