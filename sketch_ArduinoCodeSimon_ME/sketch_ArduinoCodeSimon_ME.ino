#include <Arduino.h>

typedef enum {
  L_RED = 0,
  L_BLUE = 1,
  L_YELLOW = 2,
  L_GREEN = 3,
} E_LIGHT;

#define RedBtn 8
#define BlueBtn 9
#define YellowBtn 10
#define GreenBtn 11
#define RedLight 2
#define BlueLight 3
#define YellowLight 4
#define GreenLight 5

#define NUM_LEVELS 30
int CurrentLevel = 0;
E_LIGHT levels[NUM_LEVELS];

void setup() {
  Serial.begin(9600);

  pinMode(RedLight,OUTPUT);
  pinMode(BlueLight,OUTPUT);
  pinMode(YellowLight,OUTPUT);
  pinMode(GreenLight,OUTPUT);
  pinMode(RedBtn,INPUT_PULLUP);
  pinMode(BlueBtn,INPUT_PULLUP);
  pinMode(YellowBtn,INPUT_PULLUP);
  pinMode(GreenBtn,INPUT_PULLUP);
  
  
  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_LEVELS; i++) {
    levels[i] = (E_LIGHT)random(0, 4);
  }
  // put your setup code here, to run once:
}

void loop() { 
  Serial.print("New Level: ");
  Serial.println(CurrentLevel);
  if (CurrentLevel == NUM_LEVELS) {
    while (true) {
      digitalWrite(RedLight,HIGH);
      digitalWrite(BlueLight,HIGH);
      digitalWrite(YellowLight,HIGH);
      digitalWrite(GreenLight,HIGH);
    }
  }
 for (int i = 0; i < CurrentLevel; i++) {
  int led;
  if (levels[i] == L_RED) {
    led = RedLight;
  } else if (levels[i] == L_BLUE) {
    led = BlueLight;
  } else if (levels[i] == L_YELLOW) {
    led = YellowLight;
  } else if (levels[i] == L_GREEN) {
    led = GreenLight;
  } else {
    led = RedLight;
    Serial.print("error");
  }
    digitalWrite(led,HIGH);
    Serial.print("showing led: ");
    Serial.println(led);
    delay(500);
    digitalWrite(led,LOW);
    delay(1500);
  }
  for (int i = 0; i < CurrentLevel; i++) {
          Serial.println("Level:");
      Serial.print(levels[i]);
    int buttonPressed;
    while (true) {
      if (digitalRead(RedBtn) == 0) {
        buttonPressed = L_RED;
        break;
      } else if (digitalRead(BlueBtn) == 0) {
        buttonPressed = L_BLUE;
        break;
      } else if (digitalRead(YellowBtn) == 0) {
        buttonPressed = L_YELLOW;
        break;
      } else if (digitalRead(GreenBtn) == 0) {
        buttonPressed = L_GREEN;
        break;
      }
    }
    Serial.println("Button Pressed:");
    Serial.print(buttonPressed);
    if (buttonPressed != levels[i]) {
      Serial.println("YOU LOST LLLLLLBOZO");
      Serial.println("Button Pressed:");
      Serial.print(buttonPressed);
      Serial.println("Level:");
      Serial.print(levels[i]);
    } 
  }

  CurrentLevel++;
}

 