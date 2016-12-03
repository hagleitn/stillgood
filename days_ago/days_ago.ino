unsigned long offset;
unsigned long lastCheck;
unsigned long lastLoop;
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

int segmentSelector[DIGITS] = {10,11};

int segmentPins[SEGMENTS] = {3,4,5,6,7,8,9};

int resetPin = 2; // only 2 and 3 can be used w/ interrupt on mini

void reset() {
  lastCheck = offset = millis();
  days = 0;
  overflows = 0;
  overflowTime = 0;
}

void setup() {
  reset();

  for (int i = 0; i < DIGITS; i++) {
    pinMode(segmentSelector[i], OUTPUT);
  }

  for (int i = 0; i < SEGMENTS; i++) {
    pinMode(segmentPins[i], OUTPUT);
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

    for (int j = 0; j < DIGITS; j++) {
      digitalWrite(segmentSelector[j], i == j ? HIGH : LOW);
    }

    for (int j = 0; j < SEGMENTS; j++) {
      digitalWrite(segmentPins[j], codes[digit] & index ? LOW : HIGH);
      index <<= 1;
    }

    delay(2);
    day /= 10;
  }
}

void loop() {
  unsigned long t = millis();

  if (t < lastCheck) {
    overflows++;
    overflowTime = lastLoop;
    lastCheck = t;
  } else if (t - lastCheck > MINUTE) {
    days = t > offset ?
      (overflowTime / DAY * overflows) + (t - offset) / DAY
      : (overflowTime / DAY * overflows) - (offset - t) / DAY;
    lastCheck = t;
  }

  lastLoop = t;
  setDays(days);
}
