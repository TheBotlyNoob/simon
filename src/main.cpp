#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

typedef enum E_LIGHT {
  L_RED = 0,
  L_BLUE = 1,
  L_YELLOW = 2,
  L_GREEN = 3,
} E_LIGHT;

#define RED_LED 2
#define BLUE_LED 7
#define YELLOW_LED 5
#define GREEN_LED 12

#define RED_BTN 8
#define BLUE_BTN 9
#define YELLOW_BTN 10
#define GREEN_BTN 11

#define BUZZER 6

#define NUM_LEVELS 10

int current_level = 0;

E_LIGHT levels[30];

void show_light(E_LIGHT light) {
  int led;
  int freq;
  switch(light) {
    case L_RED:
      Serial.println("showing red light");
      led = RED_LED;
      freq = 255;
      break;
    case L_BLUE:
      Serial.println("showing blue light");
      led = BLUE_LED;
      freq = 200;
      break;
    case L_YELLOW:
      Serial.println("showing yellow light");
      led = YELLOW_LED;
      freq = 125;
      break;
    case L_GREEN:
      Serial.println("showing green light");
      led = GREEN_LED;
      freq = 75;
      break;
    default:
      Serial.print("passed: ");
      Serial.print((int) light);
      Serial.println(" to show_light. this light is unknown.");
      led = RED_LED;
      freq = 255;
      break;
  }
  digitalWrite(led, HIGH);
  analogWrite(BUZZER, freq);
  delay(1000);
  digitalWrite(led, LOW);
  analogWrite(BUZZER, LOW);
  delay(500);
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
  }
  return (E_LIGHT)selection;
}

void lose() {
  Serial.println("LOSE!");
  while(true) {
    digitalWrite(RED_LED, HIGH);
    analogWrite(BUZZER, 255);
    delay(500);
    digitalWrite(RED_LED, LOW);
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
    analogWrite(BUZZER, 50);
    delay(500);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    analogWrite(BUZZER, LOW);
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

  randomSeed(analogRead(0));

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
}

void loop() {
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
  }
  delay(500);
}

