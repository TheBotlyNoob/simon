#include <Arduino.h>
#include <Tone.h>

typedef enum E_LIGHT {
  L_RED = 0,
  L_BLUE = 1,
  L_YELLOW = 2,
  L_GREEN = 3,
} E_LIGHT;

#define RED_LED 2
#define BLUE_LED 7
#define YELLOW_LED 4
#define GREEN_LED 12

#define WEIRD_ROW 13

#define RED_BTN 8
#define BLUE_BTN 9
#define YELLOW_BTN 10
#define GREEN_BTN 11

#define START_STOP_BTN 3

#define BUZZER_1 5
#define BUZZER_2 6

Tone BUZZERS[2];

typedef struct sound {
  int one;
  int two;
} sound;

#define C_WIN sound{NOTE_C4, NOTE_E4}
#define C_LOSE sound{NOTE_C4, NOTE_D4}

#define C_RED sound{NOTE_A4, NOTE_A4}
#define C_BLUE sound{NOTE_E4, NOTE_E4}
#define C_YELLOW sound{NOTE_CS4, NOTE_CS4}
#define C_GREEN sound{NOTE_E3, NOTE_E3};

#define NUM_LEVELS 15

int l_delay = 500;
#define MIN_DELAY 200
#define DELAY_MOD 50

int current_level = 0;
bool playing = false;

E_LIGHT levels[NUM_LEVELS];

void buzzer_play(sound s) {
  BUZZERS[0].play(s.one);
  BUZZERS[1].play(s.two);
}
void buzzer_stop() {
  BUZZERS[0].stop();
  BUZZERS[1].stop();
}

void show_light(E_LIGHT light) {
  int led;
  sound play;
  switch(light) {
    case L_RED:
      Serial.println("showing red light");
      led = RED_LED;
      play = C_RED;
      break;
    case L_BLUE:
      Serial.println("showing blue light");
      led = BLUE_LED;
      play = C_BLUE;
      break;
    case L_YELLOW:
      Serial.println("showing yellow light");
      led = YELLOW_LED;
      play = C_YELLOW;
      break;
    case L_GREEN:
      Serial.println("showing green light");
      led = GREEN_LED;
      play = C_GREEN;
      break;
    default:
      Serial.print("passed: ");
      Serial.print((int) light);
      Serial.println(" to show_light. this light is unknown.");
      led = RED_LED;
      play = C_RED;
      break;
  }
  digitalWrite(led, HIGH);
  buzzer_play(play);
  delay(l_delay);
  digitalWrite(led, LOW);
  buzzer_stop();
  delay(max(100, l_delay));
}

E_LIGHT get_selection() {
  int selection = -1;
  while(selection == -1) {
    if(digitalRead(RED_BTN) == LOW) {
      selection = L_RED;
    } else if (digitalRead(BLUE_BTN) == LOW) {
      selection = L_BLUE;
    } else if(digitalRead(YELLOW_BTN) == LOW) {
      selection = L_YELLOW;
    } else if (digitalRead(GREEN_BTN) == LOW) {
      selection = L_GREEN;
    } 
    delay(5);
  }
  return (E_LIGHT)selection;
}

void lose() {
  Serial.println("LOSE!");
  while(true) {
    digitalWrite(RED_LED, HIGH);
    buzzer_play(C_LOSE);
    delay(500);
    digitalWrite(RED_LED, LOW);
    buzzer_stop();
    delay(500);
  }
}

void win() {
  Serial.println("WIN!");
  while(true) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(WEIRD_ROW, HIGH);
    buzzer_play(C_WIN);
    delay(500);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(WEIRD_ROW, LOW);
    buzzer_stop();
    delay(500);
  }
}

void debug_output() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  delay(2000);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  while(true) {
    if(digitalRead(RED_BTN) == LOW) {
      Serial.println("red button pressed");
      digitalWrite(RED_LED, HIGH); 
    } else {
      digitalWrite(RED_LED, LOW);
    }
    if(digitalRead(BLUE_BTN) == LOW) {
      Serial.println("blue button pressed");
      digitalWrite(BLUE_LED, HIGH);
    } else {
      digitalWrite(BLUE_LED, LOW);
    }
    if(digitalRead(GREEN_BTN) == LOW) {
      Serial.println("green button pressed");
      digitalWrite(GREEN_LED, HIGH);
    } else {
      digitalWrite(GREEN_LED, LOW);
    }
    if(digitalRead(YELLOW_BTN) == LOW) {
      Serial.println("yellow button pressed");
      digitalWrite(YELLOW_LED, HIGH);
    } else {
      digitalWrite(YELLOW_LED, LOW);
    }
  }
}

void setup() {
  randomSeed(analogRead(0));

  Serial.begin(9600);
  // while(!Serial.available()) {
  //   ; // wait for it to do that
  // }

  pinMode(RED_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  
  pinMode(RED_BTN, INPUT_PULLUP);
  pinMode(BLUE_BTN, INPUT_PULLUP);
  pinMode(GREEN_BTN, INPUT_PULLUP);
  pinMode(YELLOW_BTN, INPUT_PULLUP);

  pinMode(START_STOP_BTN, INPUT_PULLUP);

  BUZZERS[0].begin(BUZZER_1);
  BUZZERS[1].begin(BUZZER_2);

  // FIXME: remove when finished debugging
  //debug_output();

  memset(&levels, 0, sizeof(levels) / sizeof(levels[0]));

  for (int i = 0; i < NUM_LEVELS; i++) {
    levels[i] = (E_LIGHT)random(4);
    if(i >= 2) {
      while (levels[i] == levels[i - 1] && levels[i] == levels[i - 2]) {
        levels[i] = (E_LIGHT)random(4);
      }
    }
  }

  // debug_output();

  Serial.println("setup");

  digitalWrite(WEIRD_ROW, HIGH);
}

void loop() {
  while(!playing) {
    current_level = 0;

    if(digitalRead(START_STOP_BTN) == LOW) {
      playing = true;
    }
  }

  current_level++;

  Serial.print("on level ");
  Serial.println(current_level);

  if(current_level >= NUM_LEVELS) {
    win();
  }
  for(int i = 0; i < current_level; i++) {
    show_light(levels[i]);
  }
  for (int i = 0; i < current_level; i++) {
    E_LIGHT selection = get_selection();
    if(selection != levels[i]) {
      lose();
    }
    show_light(selection);

    if(l_delay >= MIN_DELAY) {
      if(l_delay - DELAY_MOD <= MIN_DELAY) {
        l_delay = MIN_DELAY;
      } else {
        l_delay -= DELAY_MOD;
      }
    }
  }
  delay(500);
}

