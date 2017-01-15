// angel-cube

#include "pitches.h"
// winning music
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


const int buttons[] = {
  2, 4, 8,
};
const int buttonCount = sizeof(buttons) / sizeof(int);
const int minCount = 1;

//////add 9 buttons

// led pins
const int led0diff = 13; //// change it to a new led, not the one on the board
const int led1diff = 6;
const int led2diff = 7;
const int speaker = 12;

// game states
// 0 not started
// 1 started
// 2 almost finished
// 3 finished
int gameState;
int target; // number of buttons to win

int cycles;
const int interval = 100;
unsigned long previousMillis;

void setup() {
  // for debugging
  Serial.begin(9600);
  
  // initialize the led and speaker pins as an outputs
  pinMode(led0diff, OUTPUT);
  pinMode(led1diff, OUTPUT);
  pinMode(led2diff, OUTPUT);
  pinMode(speaker, OUTPUT);

  // initialize the button pins as an inputs
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttons[i], INPUT);
  }    

  // init random
  randomSeed(analogRead(0));
  gameState = 0;
}

void initGame() {
  target = random(minCount, buttonCount + 1);
  gameState = 1;
}

void ledOn(int i) {
  switch(i) {
  case 0:
    digitalWrite(led0diff, HIGH);
    break;
  case 1:
    digitalWrite(led1diff, HIGH);
    break;
  case 2:
    digitalWrite(led2diff, HIGH);
    break;
  }
}

void ledOnAll() {
  ledOn(0);
  ledOn(1);
  ledOn(2);
}

void ledOff(int i) {
  switch(i) {
  case 0:
    digitalWrite(led0diff, LOW);
    break;
  case 1:
    digitalWrite(led1diff, LOW);
    break;
  case 2:
    digitalWrite(led2diff, LOW);
    break;
  }
}

void ledOffAll() {
  ledOff(0);
  ledOff(1);
  ledOff(2);
}

void playGame(bool almost) {
  // count pressed buttons
  int pressedButtonCount = 0;
  for (int i = 0; i < buttonCount; i++) {
    if (digitalRead(buttons[i]) == HIGH) {
      pressedButtonCount++;
    }
  }
  int diff = abs(pressedButtonCount - target);

  // if the correct number of buttons are pressed, we are almost finished
  gameState = (diff == 0 ? 2 : 1);
  if (!almost) {
    cycles = -1;
  }
  if (almost && diff == 0) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      cycles++;
      previousMillis = currentMillis;
    }
  }
  if (cycles > 4) {
    gameState = 3;
  }

  ledOffAll();
  switch(diff) {
  case 0:
    ledOn(0);
  case 1:
    ledOn(1);
  case 2:
    ledOn(2);
  }
}

void winningLightShow() {
  for (int i = 0; i < 9; i++) {
    ledOnAll();
    delay(100);
    ledOffAll();
    delay(100);
    if (i > 0 && i % 3 == 0) {
      delay(250);
    }
  }
}

void winningLightShowWithMusic() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    if (thisNote % 2 == 0) {
      ledOffAll();
    } else {
      ledOnAll();
    }
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speaker, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.3;
    delay(pauseBetweenNotes);
    noTone(speaker);
  }
}

void finishGame() {
  winningLightShowWithMusic();
  winningLightShow();
  gameState = 0;
}

void loop() {
  switch(gameState) {
  case 0: // not started
    initGame();
    break;
  case 1: // started
    playGame(false);
    break;
  case 2: // almost finished
    playGame(true);
    break;
  case 3: // finished
    finishGame();
    break;
  }
}

