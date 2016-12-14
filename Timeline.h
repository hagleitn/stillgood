#ifndef TIMELINE_T
#define TIMELINE_T

struct _ts {
  unsigned long overflows;
  unsigned long t;
};

typedef _ts ts;

ts *now();
void updateTime();
void resetTime();
int pastInterval(volatile ts *t, unsigned long l);
int inInterval(volatile ts *t, unsigned long l);
unsigned long numberIntervals(volatile ts *t, unsigned long countInterval);
void assignTime(volatile ts*, volatile ts*);
unsigned long diff(volatile ts *, volatile ts*);

#endif
