#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "gamut.h"

int photocellPin = A3;

int randNum;
int randNums[10];

//Pin connected to ST_CP of 74HC595
int latchPin = 3;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 2;

int simonTimer = 0;

// 9 bottons
int button1 = A2;
int button2 = 5;
int button3 = 6;
int button4 = 7;
int button5 = 8;
int button6 = 9;
int button7 = 10;
int button8 = 11;
int button9 = 12;

int led1 = 128;
int led2 = 64;
int led3 = 32;
int led4 = 16;
int led5 = 8;
int led6 = 4;
int led7 = 2;
int led8 = 1;

LiquidCrystal_I2C lcd(0x27, 16, 2);
int mode = 0;// mode check and change swich

//SoftwareSerial mySerial(11, 12); // RX, TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);
  pinMode(button6, INPUT);
  pinMode(button7, INPUT);
  pinMode(button8, INPUT);
  pinMode(button9, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Timer1.initialize(1000000);//initial the timer to 1s
  Timer1.attachInterrupt(simonTime);//set the timer attached interrupt function

  lcd.init();
  lcd.backlight();
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("Piano&Simon Game");
  delay(3000);

  photocellPin = A3;
  int ar0 = analogRead(0);
  int seed = photocellPin * ar0;
  randomSeed(seed);
  for (int i = 0; i < 10; i++) {
    randNum = random(1, 8);
    randNums[i] = randNum;
  }
  randNum = random(1, 8);

  lcd.clear();
  lcd.print("1.Simon Game");
  lcd.setCursor(0, 1);
  lcd.print("2.Piano Mode");
  while (1) {
    int b1 = digitalRead(button1);
    int b2 = digitalRead(button2);
    if (b1 == 0) {
      b1 = digitalRead(button1);
      while (b1 == 0) {
        b1 = digitalRead(button1);
      }
      mode = 0;
      break;
    }
    if (b2 == 0) {
      b2 = digitalRead(button2);
      while (b2 == 0) {
        b2 = digitalRead(button2);
      }
      mode = 1;
      break;
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  int b9 = digitalRead(button9);
  int b8 = digitalRead(button8);

  if (mode == 0) {
    lcd.init();
    lcd.backlight();
    lcd.print("Simon Mode");

    randNum = random(1, 8);
    for (int i = 0; i < 10; i++) {
      randNum = random(1, 8);
      randNums[i] = randNum;
    }

    delay(3000);
    lcd.clear();

    while (1) {
      lcd.setCursor(1, 0);
      lcd.print("Push Start Btn");
      lcd.setCursor(4, 1);
      lcd.print("to Start");
      b8 = digitalRead(button8);
      b9 = digitalRead(button9);
      if (b8 == 0) {
        while (b8 == 0) {
          b8 = digitalRead(button8);
        }
        lcd.clear();
        int level = 1;

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Game Start!");
        delay(3000);


        int simonBeginEnd;
        for (int i = 1; i <= 10; i++) {
          level = i;
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Level:");
          lcd.print(level);
          delay(1000);

          showSimon(level);
          simonTimer = 10;
          simonBeginEnd = simon(randNums, level);
          if (simonBeginEnd != 1) {
            break;
          }
        }
        if (simonBeginEnd == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Congratulation!");
          lcd.setCursor(2, 1);
          lcd.print("Game Clear!");
          delay(3000);
          lcd.clear();
        } else if (simonBeginEnd == 9) {
          mode = 1 - mode;
          break;
        } else if (simonBeginEnd == 8) {
          break;
        } else if (simonBeginEnd == -1) {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print("Time Up!");
          tone(13, NOTE_D1);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 255);
          digitalWrite(latchPin, HIGH);
          delay(1000);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 0);
          digitalWrite(latchPin, HIGH);
          noTone(13);
          delay(1000);
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("Final Level: ");
          lcd.print(level);
          lcd.setCursor(0, 1);
          lcd.print("Game Over!");
          delay(3000);
          lcd.clear();
          break;
        } else if (simonBeginEnd == -2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong order!");
          lcd.setCursor(0, 1);
          lcd.print("Game Over!");
          tone(13, NOTE_D1);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 255);
          digitalWrite(latchPin, HIGH);
          delay(1000);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 0);
          digitalWrite(latchPin, HIGH);
          noTone(13);
          delay(1000);
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("Final Level: ");
          lcd.print(level);
          lcd.setCursor(0, 1);
          lcd.print("Game Over!");
          delay(3000);
          lcd.clear();
          break;
        }
      } else if (b9 == 0) {
        while (b9 == 0) {
          b9 = digitalRead(button9);
        }
        mode = 1 - mode;
        lcd.clear();
        lcd.print("Mode change");
        delay(1000);
        break;
      }
    }
  }

  if (mode == 1) {
    lcd.clear();
    lcd.print("Piano Mode");

    while (1) {
      int ledLight = piano();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, ledLight);
      digitalWrite(latchPin, HIGH);
      b9 = digitalRead(button9);
      if (b9 == 0) {
        while (b9 == 0) {
          b9 = digitalRead(button9);
        }
        mode = 1 - mode;
        lcd.clear();
        lcd.print("Mode change");
        delay(1000);
        break;
      }
    }
  }

}


// Simon Game Mode Functions
int simon(int nums[10], int level) {
  lcd.clear();
  int result = 1;

  int  b1 = digitalRead(button1);
  int  b2 = digitalRead(button2);
  int  b3 = digitalRead(button3);
  int  b4 = digitalRead(button4);
  int  b5 = digitalRead(button5);
  int  b6 = digitalRead(button6);
  int  b7 = digitalRead(button7);
  int  b8 = digitalRead(button8);
  int  b9 = digitalRead(button9);

  int counter = 0;

  while (1) {
    lcd.setCursor(0, 0);
    lcd.print("Level:");
    lcd.print(level);
    simonTimeDis();

    while (counter < level) {
      int inputNum = 0;
      simonTimeDis();
      b1 = digitalRead(button1);
      b2 = digitalRead(button2);
      b3 = digitalRead(button3);
      b4 = digitalRead(button4);
      b5 = digitalRead(button5);
      b6 = digitalRead(button6);
      b7 = digitalRead(button7);
      b8 = digitalRead(button8);
      b9 = digitalRead(button9);
      if (b1 == 0) {
        while (b1 == 0) {
          b1 = digitalRead(button1);
          tone(13, NOTE_C4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 128);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 1;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b2 == 0) {
        while (b2 == 0) {
          b2 = digitalRead(button2);
          tone(13, NOTE_D4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 64);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 2;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b3 == 0) {
        while (b3 == 0) {
          b3 = digitalRead(button3);
          tone(13, NOTE_E4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 32);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 3;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b4 == 0) {
        while (b4 == 0) {
          b4 = digitalRead(button4);
          tone(13, NOTE_F4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 16);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 4;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b5 == 0) {
        while (b5 == 0) {
          b5 = digitalRead(button5);
          tone(13, NOTE_G4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 8);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 5;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b6 == 0) {
        while (b6 == 0) {
          b6 = digitalRead(button6);
          tone(13, NOTE_A4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 4);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 6;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b7 == 0) {
        while (b7 == 0) {
          b7 = digitalRead(button7);
          tone(13, NOTE_B4);
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, MSBFIRST, 2);
          digitalWrite(latchPin, HIGH);
        }
        noTone(13);
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        inputNum = 7;
        if (randNums[counter] == inputNum) {
          counter++;
        } else {
          result = -2;
          break;
        }
      } else if (b8 == 0) {
        while (b8 == 0) {
          b8 = digitalRead(button8);
        }
        result = 8;
        break;
      } else if (b9 == 0) {
        while (b9 == 0) {
          b9 = digitalRead(button9);
        }
        result = 9;
        break;
      } else if (simonTimer <= 0) {
        result = -1;
        break;
      }
    }
    if (counter >= level || result == -1 || result == 8 || result == 9 || result == -2) {
      break;
    }
  }

  return result;
}

void showSimon(int level) {

  int ledlight = 0;
  int note;
  for (int i = 0; i < level; i++) {
    int simonNum = randNums[i];
    if (simonNum == 1) {
      note = NOTE_C4;
      ledlight = led1;
    } else if (simonNum == 2) {
      note = NOTE_D4;
      ledlight = led2;
    } else if (simonNum == 3) {
      note = NOTE_E4;
      ledlight = led3;
    } else if (simonNum == 4) {
      note = NOTE_F4;
      ledlight = led4;
    } else if (simonNum == 5) {
      note = NOTE_G4;
      ledlight = led5;
    } else if (simonNum == 6) {
      note = NOTE_A4;
      ledlight = led6;
    } else if (simonNum == 7) {
      note = NOTE_B4;
      ledlight = led7;
    }

    tone(13, note);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, ledlight);
    digitalWrite(latchPin, HIGH);
    delay(1500);

    noTone(13);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
    delay(500);
  }

}

void simonTime() {
  simonTimer--;
}

void simonTimeDis() {
  if (simonTimer != 10) {
    lcd.setCursor(12, 1);
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Time left:");
  lcd.print(simonTimer);
  lcd.print("s");
}


// Piano Toy Mode Function
int piano() {
  int  b1 = digitalRead(button1);
  int  b2 = digitalRead(button2);
  int  b3 = digitalRead(button3);
  int  b4 = digitalRead(button4);
  int  b5 = digitalRead(button5);
  int  b6 = digitalRead(button6);
  int  b7 = digitalRead(button7);
  int  b8 = digitalRead(button8);

  if (b1 == 0) {
    tone(13, NOTE_C4);
    delay(0);
    return 128;
  }
  else if (b2 == 0) {
    tone(13, NOTE_D4);
    delay(0);
    return 64;
  }
  else if (b3 == 0) {
    tone(13, NOTE_E4);
    delay(0);
    return 32;
  }
  else if (b4 == 0) {
    tone(13, NOTE_F4);
    delay(0);
    return 16;
  }
  else if (b5 == 0) {
    tone(13, NOTE_G4);
    delay(0);
    return 8;
  }
  else if (b6 == 0) {
    tone(13, NOTE_A4);
    delay(0);
    return 4;
  }
  else if (b7 == 0) {
    tone(13, NOTE_B4);
    delay(0);
    return 2;
  }
  else if (b8 == 0) {
    tone(13, NOTE_C5);
    delay(0);
    return 1;
  }
  else {
    noTone(13);
    return 0;
  }
}
