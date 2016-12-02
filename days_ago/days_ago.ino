unsigned long offset;
unsigned long lastCheck;
unsigned long days;
unsigned long overflows;
unsigned long overflowTime;

#define DAY    (1000ul * 60ul * 60ul * 24ul)
#define MINUTE (1000ul * 60ul)

byte codes[] = {
  0x3F, //00111111,
  0x06, //00000110,
  0x5B, //01011011,
  0x4F, //01001111,
  0x66, //01100110,
  0x6D, //01101101,
  0x7D, //01111101,
  0x07, //00000111,
  0x7F, //01111111,
  0x6F, //01101111
};

#define DIGITS 2
#define SEGMENTS 7

int segmentPins[DIGITS][SEGMENTS] = {
  {3,4,5,6,7,8,9},
  {10,11,12,13,14,15,16}
};

int resetPin = 2; // only 2 and 3 can be used w/ interrupt on mini

void reset() {
  lastCheck = offset = millis();
  days = -1;
  overflows = 0;
  overflowTime = 0;
}

void setup() {
  reset();

  for (int i = 0; i < DIGITS; i++) {
    for (int j = 0; j < SEGMENTS; j++) {
      pinMode(segmentPins[i][j], OUTPUT);
    }
  }

  pinMode(resetPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(resetPin), reset, RISING);
}

void setDays(unsigned long day) {
  int digit;
  byte index;

  for (int i = 0; i < DIGITS; i++) {
    digit = day % 10;
    index = 0x01;

    for (int j = 0; j < SEGMENTS; j++) {
      digitalWrite(segmentPins[i][j], codes[digit] & index ? HIGH : LOW);
      index <<= 1;
    }

    day /= 10;
  }
}

void loop() {
  delay(MINUTE);
  unsigned long t = millis();

  if (t < lastCheck) {
    overflows++;
    overflowTime = lastCheck;
  }

  unsigned long newDays = t > offset ?
    (overflowTime / DAY * overflows) + (t - offset) / DAY
    : (overflowTime / DAY * overflows) - (offset - t) / DAY;

  if (newDays != days) {
    setDays(newDays);
  }

  days = newDays;
  lastCheck = t;
}
