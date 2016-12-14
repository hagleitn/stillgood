#include "Timeline.h"
#include <limits.h>

#ifndef SIM
#include <Arduino.h>
#else
#include "Simulator.h"
#endif

unsigned long _offset;

ts _now = { 0, 0 };

ts *now() {
  return &_now;
}

void resetTime() {
  _now.t = millis();
  _offset = _now.t;
  _now.overflows = 0;
}

void assignTime(volatile ts *t1, volatile ts *t2) {
  t1->overflows = t2->overflows;
  t1->t = t2->t;
}

unsigned long diff(volatile ts *t1, volatile ts *t2) {
  return (t1->overflows == t2->overflows) ? t1->t - t2->t :
    (t1->overflows - t2->overflows ) * ULONG_MAX - t2->t + t1->t;
}

unsigned long numberIntervals(volatile ts *t, unsigned long countInterval) {
  return t->t >= _offset ?
    (ULONG_MAX / countInterval * t->overflows) + (t->t - _offset) / countInterval
    : (ULONG_MAX / countInterval * t->overflows) - (_offset - t->t) / countInterval;
}

int inInterval(volatile ts *t, unsigned long l) {
  if (t->overflows == _now.overflows) {
    if (t->t + l > t->t) {
      return t->t <= _now.t && t->t + l > _now.t;
    } else {
      return t->t <= _now.t;
    }
  } else {
    if (_now.overflows != t->overflows + 1) {
      return false;
    } else {
      return l > _now.t + (ULONG_MAX - t->t);
    }
  }
}

int pastInterval(volatile ts *t, unsigned long l) {
  if (t->overflows == _now.overflows) {
    if (t->t + l > t->t) {
      return t->t + l < _now.t;
    } else {
      return false;
    }
  } else {
    if (_now.overflows < t->overflows) {
      return false;
    } else {
      if (_now.overflows != t->overflows + 1) {
        return true;
      } else {
        return l < _now.t + (ULONG_MAX - t->t);
      }
    }
  }
}

void updateTime() {
  unsigned long ms = millis();

  if (ms < _now.t) {
    _now.overflows++;
  }

  _now.t = ms;
}
