#ifndef COMUNICATOR_H
#define COMUNICATOR_H
#include <LCD_I2C.h>
#include "Arduino.h"

/*
  Klasa comunicator tworzy obiekty, ktore beda zajmowac sie komunikacja z uzytkownikiem, tj:
  - wyswietlaniem wiadomosci na ekranie lcd,
  - sterowanie buzzerem
  - sterowanie diodami led - aby sygnalizowac stan urzadzenia
*/
class Comunicator {
public:
  Comunicator(LCD_I2C &lcd, byte buzzerPin);
  void welcomeScreen();
  void enterPassword();
  void setNewPassword();
  void presscToClose();
  void printchar(byte col, byte row, char character);
  void confirmPassword();
  void passwordDoesntMatch();
  void wrongPassword();
  void buzzerSound(int time);
  void changeLedStates(byte ledPin1, byte ledPin2);
  void ledFlash(byte ledPin, byte numberOfFlashes);
private:
  LCD_I2C *lcd;
  byte buzzerPin;
};

#endif