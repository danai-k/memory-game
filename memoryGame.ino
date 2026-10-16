#include <Wire.h>
#include "WaveshareLCD.h"

#define BUTTON_R 6 
#define BUTTON_Y 8
#define BUTTON_G 10
#define BUTTON_B 12
#define LED_R 5
#define LED_Y 7
#define LED_G 9
#define LED_B 11
#define BUZZER 13

enum SystemState {
  IDLE, // waiting for user to press any button to start
  SHOW_PATTERN, // random LED pattern
  WAIT_INPUT, // awaiting for user to press said pattern
  SUCCESS, // correct, motor slides door open
  ERROR // fail 
};

// Functions
void openDoor(); 
void stopDoor();
void closeDoor();
void genSequence();
void playPattern();
bool userInput();
void feedback(int index);

SystemState state = IDLE;
const int MAX_LEVEL = 10;
int level = 1;
int seq[MAX_LEVEL];
int led[4] = {LED_R, LED_Y, LED_G, LED_B};

void setup() {
  // Screen
  Wire.begin();
  lcd_init();
  lcd_set_cursor(0, 0);
  lcd_print("Color Memory !"); 
  lcd_set_cursor(0, 1);
  lcd_print("Press Any Key");

  // Pins
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_Y, INPUT_PULLUP);
  pinMode(BUTTON_G, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT); 

  randomSeed(analogRead(A0));  // Create a different random pattern after each restart.
}

void loop() {
  switch (state){
    case IDLE:
      // Wait for user to press ANY button to begin
      while (digitalRead(BUTTON_R) == HIGH && digitalRead(BUTTON_Y) == HIGH && 
        digitalRead(BUTTON_G) == HIGH && digitalRead(BUTTON_B) == HIGH) 
      {
        delay(10); // Do nothing until a button is pressed
      }
      // Wait for user to let go of the button
      while (digitalRead(BUTTON_R) == LOW || digitalRead(BUTTON_Y) == LOW || 
        digitalRead(BUTTON_G) == LOW || digitalRead(BUTTON_B) == LOW) 
      {
        delay(10); 
      }
      level = 1;
      genSequence();
      state = SHOW_PATTERN;
      delay(2000);
      break;
    
    case SHOW_PATTERN:
      lcd_send_cmd(0x01);
      delay(2);
      lcd_set_cursor(0, 0);
      lcd_print("Level: ");
      lcd_print(level);
      lcd_set_cursor(0, 1);
      lcd_print("Watch Pattern");

      delay(500);
      playPattern();
      state = WAIT_INPUT;
      break;

    case WAIT_INPUT:
      lcd_send_cmd(0x01);
      delay(2);
      lcd_set_cursor(0, 0);
      lcd_print("Your Turn !");
      if ( userInput())
      {
        state = SUCCESS;
      }
      else
      {
        state = ERROR;
      }
      break;

    case SUCCESS:
      if (level < MAX_LEVEL)
      {
        lcd_send_cmd(0x01);
        delay(2);
        lcd_set_cursor(0, 0);
        lcd_print("Correct!");
        lcd_set_cursor(0, 1);
        lcd_print("Next Level");
        delay(1000);

        level++;
        state = SHOW_PATTERN;
      }
      else
      {
        lcd_send_cmd(0x01);
        delay(2);
        lcd_set_cursor(0, 0);
        lcd_print("Congrats !");
        lcd_set_cursor(0, 1);
        lcd_print("Game Cleared");
        state = IDLE;
      }
      break;

    case ERROR:
      lcd_send_cmd(0x01);
      delay(2);
      lcd_set_cursor(0, 0);
      lcd_print("Game Over !");
      lcd_set_cursor(0, 1);
      lcd_print("Try again");
      tone(BUZZER, 1000, 1000);
      delay(2000);

      state = IDLE;
      break;
  }
}

// --- PATTERN ---

// Generation of Sequence
void genSequence(){
  for (int i = 0; i < MAX_LEVEL; i++)
  {
    seq[i] = random(0, 4);
  }
}

// Trigger the matching LED and buzzer tone
void feedback(int index){
  int tones[4] = { 500, 600, 700, 800}; //unique for each color
  digitalWrite(led[index], HIGH);
  tone(BUZZER, tone[index]);
  delay(300);

  noTone(BUZZER);
  digitalWrite(led[index], LOW); 
}

void playPattern(){
  for (int i = 0; i < level; i++)
  {
    feedback(seq[i]);
    delay(150);
  }
}

bool userInput(){
  for (int i = 0; i < level; i++)
  {
    int pressedButton = -1;
    while (pressedButton == -1)
    {
      if (digitalRead(BUTTON_R) == LOW) { pressedButton = 0; }
      else if (digitalRead(BUTTON_Y) == LOW) { pressedButton = 1; }
      else if (digitalRead(BUTTON_G) == LOW) { pressedButton = 2; }
      else if (digitalRead(BUTTON_B) == LOW) { pressedButton = 3; }
    }
    feedback(pressedButton);

    if ( pressedButton != seq[i] )
    {
      return false;
    }

    while (digitalRead(BUTTON_R) == LOW || digitalRead(BUTTON_Y) == LOW || 
      digitalRead(BUTTON_G) == LOW || digitalRead(BUTTON_B) == LOW) 
    {
      delay(10);
    }
    delay(50); // Small debounce delay
  }
  return true;
}