#ifndef SIM
//#define DEBUG 1
#endif

unsigned long offset;
unsigned long lastCheck;
unsigned long lastLoop;
unsigned long days;
unsigned long overflows;
unsigned long overflowTime;


#ifdef DEBUG
#define DAY    (1000ul * 1ul)
#define MINUTE (500ul)
#else
#define DAY    (1000ul * 60ul * 60ul * 24ul)
#define MINUTE (1000ul * 60ul)
#endif

// pinout display
// 11 4 5 7 6
// 9  3 x 8 10

byte codes[] = {
  0x3F, //00111111 // 0
  0x06, //00000110 // 1
  0x5B, //01011011 // 2
  0x4F, //01001111 // 3
  0x66, //01100110 // 4
  0x6D, //01101101 // 5
  0x7D, //01111101 // 6
  0x07, //00000111 // 7
  0x7F, //01111111 // 8
  0x6F, //01101111 // 9
};

#define DIGITS 2
#define SEGMENTS 7

int segmentSelector[DIGITS] = {10,11};

int segmentPins[SEGMENTS] = {3,4,5,6,7,8,9};

int resetPin = 2; // only 2 and 3 can be used w/ interrupt on mini

void reset() {

#ifdef DEBUG
  Serial.print("Reset at: ");
  Serial.println(millis());
#endif

  lastCheck = offset = millis();
  days = 0;
  overflows = 0;
  overflowTime = 0;
}

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

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

    for (int i = 0; i < SEGMENTS; i++) {
      digitalWrite(segmentPins[i], HIGH);
    }

    for (int j = 0; j < DIGITS; j++) {
      digitalWrite(segmentSelector[j], i == j ? HIGH : LOW);
    }

    for (int j = 0; j < SEGMENTS; j++) {
      digitalWrite(segmentPins[j], codes[digit] & index ? LOW : HIGH);
      index <<= 1;
    }
    day /= 10;
    delay(2);
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

#ifdef DEBUG
    Serial.print("Time: ");
    Serial.print(t);
    Serial.print(",\t Days: ");
    Serial.println(days);
#endif
  }

  lastLoop = t;
  setDays(days);
}
