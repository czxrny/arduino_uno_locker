/*
    Projekt zaliczeniowy z modułu Systemy Wbudowane, Uniwersytet Śląski, Informatyka II rok 2023/24
    Autor: Szymon Czarnecki, grupa IO1
    Projekt: Skrytka z prostym zamkiem sterowanym za pomocą silnika, odblokowywana za pomocą kodu.
*/
#include <LCD_I2C.h>
#include <Keypad.h>
#include <Stepper.h>
#include "include/Comunicator.h"
#include "include/PasswordManager.h"


// deklaracja wykorzystanych pinow mikrokontrolera
const byte buzzerPin = A0, redDiode = A1, greenDiode = A2, doorPin = A3;

// LCD - utworzenie obiektu klasy LCD_I2C
LCD_I2C lcd(0x27, 16, 2);

// obiekt klasy Comunicator. Przyjmuje instancje obiektu klasy LCD_I2C i wyswietla komunikaty na wyswietlaczu LCD 16x2.
Comunicator *comunicator = new Comunicator(lcd, buzzerPin);

// Zmienne zwiazane z obsluga keypada - ilosc rzedow oraz kolumn, piny obslugujace matryce przyciskow, oraz znaczenie przyciskow.
const byte rows = 4;
const byte cols = 4;
const byte buttonsRowPins[cols] = { 9, 8, 7, 6 };
const byte buttonsColPins[rows] = { 10, 11, 12, 13 };
const char keys[rows][cols] = { { '1', '2', '3', 0 },
                                { '4', '5', '6', 0 },
                                { '7', '8', '9', 'c' },
                                { '*', '0', '#', 'x' } };
// Obiekt klasy keypad, korzystajacy z przygotowanych zmiennych.
Keypad *keypad = new Keypad(makeKeymap(keys), buttonsRowPins, buttonsColPins, rows, cols);

//
byte stepperI1 = 2, stepperI2 = 3, stepperI3 = 2, stepperI4 = 5;
const int steps = 2048;
const byte motorSpeed = 10;
Stepper *stepperMotor = new Stepper(steps, stepperI1, stepperI2, stepperI3, stepperI4);

// Deklaracja zmiennych obslugujacych kwestie zapisu hasla + utworzenie obiektu passwordManager, zajmujacego sie zapisem oraz odczytem hasel z pamieci.
const int passwordAddress = 0;
bool isSecured = false;
char *psswd = new char[4];
char *passwordTried = new char[4];
const byte passwordLength = 4;  //
PasswordManager *passwordManager = new PasswordManager(comunicator, keypad, passwordAddress, passwordLength);

// Czas trwania mrugnięć dla komunikatow na wyswietlaczu LCD.
int cyclesTime = 30;
int cycles = cyclesTime;

// procedura ustawiajaca kolejne piny mikrokontrolera
void pinSetup() {
  pinMode(doorPin, INPUT);
  pinMode(redDiode, OUTPUT);
  pinMode(greenDiode, OUTPUT);
  analogWrite(greenDiode, 0);
  pinMode(buzzerPin, OUTPUT);
}

// funkcja pozwalajaca na podejrzenie hasla zapisanego w ram. Stworzona do debugowania.
void checkPsswd() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Haslo:");
  for (int i = 0, col = 0; i < 4; i++, col += 4) {
    lcd.setCursor(col, 1);
    lcd.print(psswd[i]);
  }
  while (!keypad->getKey()) {
  }
}

void setup() {
  pinSetup();
  stepperMotor->setSpeed(motorSpeed);
  // inicjalizacja działania ekranu lcd
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  comunicator->welcomeScreen();
  // odczyt pamieci mikrokontrolera: czy zostal juz zapisany jakis kod dostepu do skrytki?
  // jesli nie zostal, ustaw nowe haslo
  if (passwordManager->passwordIsSet())
    passwordManager->setNewPassword();
  // Zapis hasla z pamieci EEPROM do pamieci RAM mikrokontrolera
  passwordManager->readPassword(psswd);
  //checkPsswd();
  isSecured = true;
  analogWrite(redDiode, 255);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  char num = keypad->getKey();

  if (isSecured)
    comunicator->lockerIsClosed();
  else
    comunicator->lockerIsUnlocked();

  if (!analogRead(doorPin) && !isSecured) {
    comunicator->clearLCD();
    while (!analogRead(doorPin) && !isSecured) {
      comunicator->pressButton();
      char input = keypad->getKey();
      if (input == 'c') {
        isSecured = true;
        //stepperMotor->step(steps * -1);
        comunicator->changeLedStates(redDiode, greenDiode);
        comunicator->buzzerSound(200);
        comunicator->clearLCD();
      }
      // obsluga menu, ktora zostanie zakonczona dopiero po wcisnieciu '3' przez uzytkownika
      else if (input == 'x') {
        comunicator->buzzerSound(100);
        do {
          bool userDecided = false;
          comunicator->showMenu();
          comunicator->decision();
          while (!analogRead(doorPin) && !userDecided) {
            input = keypad->getKey();
            if(input < '4' && input > '0'){
              userDecided = true;
              comunicator->buzzerSound(300);
            switch (input) {
              case '1':
                passwordManager->setNewPassword();
                passwordManager->readPassword(psswd);
                break;

              case '2':
                char ledBrightness = 0;
                comunicator->chooseLedBrightness();
                comunicator->decision();
                while(ledBrightness < '1' || ledBrightness > '5'){
                  ledBrightness = keypad->getKey();
                }
                comunicator->setLedBrightness(int(ledBrightness - '0'));
                comunicator->changeLedStates(greenDiode, redDiode);
                break;

              case '3':
                userDecided = true;
                break;
            }
            }
          }
        } while (input != '3' && !analogRead(doorPin));
        comunicator->buzzerSound(400);
        comunicator->changesWereSaved();
      }
    }
  }


      while (isSecured && num) {
        comunicator->enterPassword();
        passwordManager->readKeyboard(passwordTried);
        delay(50);
        if (passwordManager->passwordsMatch(passwordTried, psswd)) {
          comunicator->lockerIsUnlocked();
          for (int i = 0; i < 2; i++) {
            comunicator->buzzerSound(200);
            delay(50);
          }
          comunicator->ledFlash(greenDiode, 2);
          //stepperMotor->step(steps);
          comunicator->changeLedStates(greenDiode, redDiode);
          isSecured = false;
        } else {
          comunicator->wrongPassword();
          comunicator->buzzerSound(400);
          comunicator->ledFlash(redDiode, 3);
        }
      }
      if (!cycles) {
        lcd.clear();
        delay(200);
        cycles = cyclesTime;
      }
      cycles--;
    }