unsigned long lastCheck;
unsigned long days;
unsigned long overflows;
unsigned long overflowTime;

#define DAY    (1000ul * 60ul * 60ul * 24ul)
#define MINUTE (1000ul * 60ul)

byte codes[] = {
  B00111111,
  B00000110,
  B01011011,
  B01001111,
  B01100110,
  B01101101,
  B01111101,
  B00000111,
  B01111111,
  B01101111
};

#define DIGITS 2
#define SEGMENTS 7

int segmentPins[DIGITS][SEGMENTS] = {
  {3,4,5,6,7,8,9},
  {10,11,12,13,14,15,16}
};

int resetPin = 2;

void reset() {
  lastCheck = millis();
  days = 0;
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
  for (int i = 0; i < DIGITS; i++) {
    int digit = days % 10;
    byte index = 0x01;
  
    for (int j = 0; j < SEGMENTS; j++) {
      digitalWrite(segmentPins[i][j], codes[digit] | index ? HIGH : LOW);
      index <<= 1;
    }

    days /= 10;
  }
}

void loop() {
  delay(MINUTE);
  unsigned long t = millis();
  
  if (t < lastCheck) {
    overflows++;
    overflowTime = lastCheck;
  }
  
  unsigned long newDays = (overflowTime / DAY) * overflows + t / DAY;

  if (newDays != days) {
    setDays(newDays);
  }

  days = newDays;
  lastCheck = t;
}

