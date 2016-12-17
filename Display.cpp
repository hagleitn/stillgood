#include "Display.h"

// pinout display
// 11 4 5 7 6
// 9  x 3 8 10

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

byte intervals[][DIGITS] = {
  {0x00, 0x5E}, // 01011110 // d
  {0x00, 0x74}, // 01110100 // h
  {0x54, 0x54}, // 01010100 // m
  {0x00, 0x6D}  // 01101101 // 5
};

int segmentSelector[DIGITS] = {10,11};

int segmentPins[SEGMENTS] = {3,4,5,6,7,8,9};

void setupDisplay() {
  for (int i = 0; i < DIGITS; i++) {
    pinMode(segmentSelector[i], OUTPUT);
  }

  for (int i = 0; i < SEGMENTS; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  clearAll();
}

void clearAll() {
  for (int i = 0; i < DIGITS; ++i) {
    clear(i);
  }
}

void clear(int pos) {
  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }
}

void showBytes(int pos, byte val) {
  byte index = 0x01;

  clear(pos);

  for (int i = 0; i < DIGITS; i++) {
    digitalWrite(segmentSelector[i], i == pos ? HIGH : LOW);
  }

  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(segmentPins[i], val & index ? LOW : HIGH);
    index <<= 1;
  }
  delay(2);
}

void showInterval(int i) {
  showBytes(0, intervals[i][1]);
  showBytes(1, intervals[i][0]);
}

void showCount(unsigned long count) {
  int digit;

  for (int i = 0; i < DIGITS; i++) {
    digit = count % 10;
    showBytes(i, codes[digit]);
    count /= 10;
  }
}
