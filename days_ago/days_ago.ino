#ifndef SIM
#define DEBUG 1
#endif

unsigned long offset;
unsigned long lastCheck;
unsigned long lastLoop;
unsigned long count;
unsigned long overflows;
unsigned long overflowTime;


#define DAY    (HOUR * 24ul)
#define HOUR   (MINUTE * 60ul)
#define MINUTE (SECOND * 60ul)
#define SECOND (1000ul)

#define DIGITS 2
#define SEGMENTS 7

// pinout display
// 11 4 5 7 6
// 9  3 x 8 10

// digits
//    1
// 6     2
//    7 
// 5     3
//    4

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

byte modes[][DIGITS] = {
  {0x00, 0x5E}, // 01011110 // d
  {0x00, 0x74}, // 01110100 // h
  {0x54, 0x54}, // 01010100 // m
  {0x00, 0x6D}  // 01101101 // 5  
};

int segmentSelector[DIGITS] = {10,11};

int segmentPins[SEGMENTS] = {3,4,5,6,7,8,9};

int resetPin = 2; // only 2 and 3 can be used w/ interrupt on mini

unsigned long countInterval;
unsigned long checkInterval;

unsigned long countIntervals[] = {DAY, HOUR, MINUTE, SECOND};
unsigned long checkIntervals[] = {MINUTE, MINUTE, SECOND, SECOND / 10};

int state = -1;
unsigned long lastStateChange;
boolean mode = false;
boolean toggle = false;

void changeState() {
  state = (state + 1) % 4;
  checkInterval = checkIntervals[state];
  countInterval = countIntervals[state];
  lastStateChange = millis();
}

void reset() {

  if (toggle && millis() - offset < SECOND / 2 && millis() - offset > 50) {
    changeState();
    toggle = false;
#ifdef DEBUG
    Serial.print("State change to: ");
    Serial.println(state);
#endif
  } else { 
    toggle = true;  
#ifdef DEBUG
    Serial.print("Reset at: ");
    Serial.println(millis());
#endif
  }
   
  lastCheck = offset = millis();
  count = 0;
  overflows = 0;
  overflowTime = 0;
}

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#endif

  changeState();
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

void showBytes(int pos, byte val) {
    byte index = 0x01;
    
    for (int i = 0; i < SEGMENTS; i++) {
      digitalWrite(segmentPins[i], HIGH);
    }

    for (int i = 0; i < DIGITS; i++) {
      digitalWrite(segmentSelector[i], i == pos ? HIGH : LOW);
    }

    for (int i = 0; i < SEGMENTS; i++) {
      digitalWrite(segmentPins[i], val & index ? LOW : HIGH);
      index <<= 1;
    }
    delay(2);
}

void setMode() {
  showBytes(0, modes[state][1]);
  showBytes(1, modes[state][0]);
}

void setCount(unsigned long count) {
  int digit;

  for (int i = 0; i < DIGITS; i++) {
    digit = count % 10;
    showBytes(i, codes[digit]);
    count /= 10;
  }
}

void loop() {
  unsigned long t = millis();

  if (t < lastCheck) {
    overflows++;
    overflowTime = lastLoop;
    lastCheck = t;
  } else if (t - lastCheck > checkInterval) {
    count = t > offset ?
      (overflowTime / countInterval * overflows) + (t - offset) / countInterval
      : (overflowTime / countInterval * overflows) - (offset - t) / countInterval;
    lastCheck = t;

#ifdef DEBUG
    Serial.print("Time: ");
    Serial.print(t);
    Serial.print(",\t Passed: ");
    Serial.println(count);
#endif
  }

  lastLoop = t;
  if (t > lastStateChange && t - lastStateChange < SECOND) {
    setMode();
    mode = true;
  } else {
    if (mode) {
      reset();
      mode = false;
    }
    setCount(count);
  }
}
