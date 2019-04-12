#include <ISADefinitions.h>
#include <ISALiquidCrystal.h>
#include <DueTimer.h>
#include <ISAButtons.h>
#include <ISA7SegmentDisplay.h>

ISALiquidCrystal lcd;
ISAButtons button;
ISA7SegmentDisplay sseg;

int player1 = 0;
int player2 = 0;
int timer = 0;

String mapButtons[] = {"7", "8", "9", "/", "*", "6", "5", "4", "1", "2", "3", "-", "+", ",", "0", "C", "GORA", "DOL", "LEWO", "PRAWO", "J1 LEWO", "J1 PRAWO", "J1 GORA", "J1 DOL", "J2 LEWO", "J2 PRAWO", "J2 GORA", "J2 DOL"};

void setup() {
  lcd.begin();
  initSeed();
  initArrow();
  sseg.init();
  button.init();
  Timer4.attachInterrupt(increaseTimer);
  game();
}

void initArrow() {
  for(int i = 0; i < 4; i++) {
    pinMode(KEY_ARROWS[i], INPUT);
  }
}

void increaseTimer() {
  timer++;
}

void loop() {}

void initSeed() {
  long seed = 0;
  for(int i = 0; i < 12; i++) {
    seed+= analogRead(i);
  }
  randomSeed(seed);
}

void game() {
  int randomButton = 0;
  int timeOfPlayer1 = 0;
  int timeOfPlayer2 = 0;
  sseg.displayDigit(0, 0); //GRACZ2
  sseg.displayDigit(0, 3); //GRACZ1
  clearAndPrint("Witaj w grze!");
  delay(3000);
  while(true) {
    clearAndPrint("GRACZ1");
    delay(1000);
    randomButton = random(sizeof(mapButtons) / sizeof(mapButtons[0]));
    counter(3);
    clearAndPrint("Nacisnij: ", mapButtons[randomButton]);
    checkPressedButtonByPlayer(randomButton);
    clearAndPrint("Twoj czas to: ", String(timer) +"ms");
    timeOfPlayer1 = timer;
    delay(1000);
    clearAndPrint("GRACZ2");
    delay(1000);
    randomButton = random(sizeof(mapButtons) / sizeof(mapButtons[0]));
    counter(3);
    clearAndPrint("Nacisnij: ", mapButtons[randomButton]);
    checkPressedButtonByPlayer(randomButton);
    clearAndPrint("Twoj czas to: ", String(timer) +"ms");
    timeOfPlayer2 = timer;
    delay(1000);
    winnerIs(timeOfPlayer1, timeOfPlayer2);
    delay(5000);
  }
}

void winnerIs(int timeOfPlayer1, int timeOfPlayer2) {
    clearAndPrint("GRACZ1: "+String(timeOfPlayer1)+"ms", "GRACZ2: "+String(timeOfPlayer2)+"ms");
    
    if(timeOfPlayer2 < timeOfPlayer1) {
      sseg.displayDigit(++player2, 0); //GRACZ2
    } else if(timeOfPlayer1 < timeOfPlayer2) {
      sseg.displayDigit(++player1, 3); //GRACZ1
    } else {
      sseg.displayDigit(++player1, 3); //GRACZ1
      sseg.displayDigit(++player2, 0); //GRACZ1
    }
}

void counter(int from) {
  for(int i = from; i > 0; i--) {
    lcd.clear();
    lcd.print(String(i)+"..");
    delay(1000);
  }
}

void checkPressedButtonByPlayer(int randomButton) {
  timer = 0;
  boolean running = true;
  
  Timer4.start(1000);
  while(running) {
    if(randomButton < 16) {
      if (button.buttonPressed(randomButton)) {
        running = false;
	  }
    } else if(randomButton < 20) {
      if(!digitalRead(KEY_ARROWS[randomButton-16])) {
		running = false;
	  }
    } else {
      switch(randomButton) {
        case 20: if(analogRead(JOY1X) == 1023) { running = false; break; }
        case 21: if(analogRead(JOY1X) == 0) { running = false; break; }
        case 22: if(analogRead(JOY1Y) == 1023) { running = false; break; }
        case 23: if(analogRead(JOY1Y) == 0) { running = false; break; }
        case 24: if(analogRead(JOY2X) == 0) { running = false; break; }
        case 25: if(analogRead(JOY2X) == 1023) { running = false; break; }
        case 26: if(analogRead(JOY2Y) == 1023) { running = false; break; }
        case 27: if(analogRead(JOY2Y) < 100) { running = false; break; } // < 100, bo słabo działa ten dżoj xd
      }
    }
  delay(30);
  }
  Timer4.stop();
}

void clearAndPrint(String text) {
  lcd.clear();
  lcd.print(text);
}

void clearAndPrint(String text, String text2) {
  lcd.clear();
  lcd.print(text);
  lcd.setCursor(0, 1);
  lcd.print(text2);
}
