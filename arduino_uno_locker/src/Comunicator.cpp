#include "../include/Comunicator.h"

Comunicator::Comunicator(LCD_I2C &lcd, byte buzzerPin) {
  this->lcd = &lcd;
  this->buzzerPin = buzzerPin;
  this->buzzerMode = 255;
  this->ledBrightness = 255;
}

void Comunicator::welcomeScreen() {
  lcd->setCursor(5, 0);
  lcd->print("Witaj!");
  delay(2000);
}

void Comunicator::lockerIsClosed(){
  lcd->setCursor(1, 0);
  lcd->print("Wprowadz haslo");
  lcd->setCursor(1, 1);
  lcd->print("Aby otworzyc!");
}

void Comunicator::lockerIsUnlocked(){
  lcd->setCursor(4, 0);
  lcd->print("Skrytka");
  lcd->setCursor(4, 1);
  lcd->print("otwarta!");
}

void Comunicator::enterPassword(){
  lcd->clear();
  lcd->setCursor(5, 0);
  lcd->print("Haslo:");
}

void Comunicator::setNewPassword() {
  for (int i = 0; i < 2; i++) {
    lcd->clear();
    lcd->setCursor(3, 0);
    lcd->print("Ustaw nowe");
    lcd->setCursor(5, 1);
    lcd->print("haslo!");
    delay(1500);
    lcd->clear();
    delay(1500);
  }
  enterPassword();
}

void Comunicator::pressButton(){
  lcd->setCursor(0, 0);
  lcd->print("c> zamknij zamek");
  lcd->setCursor(0, 1);
  lcd->print("x> otworz menu");
}

void Comunicator::clearLCD(){
  lcd->clear();
}

void Comunicator::showMenu(){
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("1> zmien haslo");
  lcd->setCursor(0, 1);
  lcd->print("2> tryb buzzera");
  delay(2000);
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("3> wyjdz z menu");
  delay(2000);
}

void Comunicator::decision(){
  lcd->clear();
  lcd->print("Wybor >> ");
}

void Comunicator::printchar(byte col, byte row, char character) {
  lcd->setCursor(col, row);
  lcd->print("*");
}

void Comunicator::confirmPassword() {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Potwierdz haslo:");
}

void Comunicator::passwordDoesntMatch() {
  lcd->clear();
  lcd->setCursor(2, 0);
  lcd->print("Podane hasla");
  lcd->setCursor(0, 1);
  lcd->print("Nie zgadzaja sie!");
  delay(1000);
}

void Comunicator::wrongPassword(){
  lcd->clear();
  lcd->setCursor(1, 0);
  lcd->print("Nieprawidlowe");
  lcd->setCursor(5, 1);
  lcd->print("haslo!");
}

void Comunicator::buzzerSound(int time) {
  analogWrite(buzzerPin, buzzerMode);
  delay(time);
  analogWrite(buzzerPin, 0);
}
/*

*/
void Comunicator::chooseBuzzerMode(){
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("1> tryb normalny");
  lcd->setCursor(0,1);
  lcd->print("2> tryb cichy");
  delay(2000);
}

void Comunicator::setBuzzerMode(int mode){
  if(mode == 1)
	  this->buzzerMode = 255;
  else
	  this->buzzerMode = 0;
}

void Comunicator::changesWereSaved(){
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print("Wszelkie zmiany");
  lcd->setCursor(0,1);
  lcd->print("Zostaly zapisane");
  delay(1000);
  lcd->clear();
}

/*
  Procedura sluzaca do zamiany stanow diod - uzywana w momencie zmiany stanu urzadzenia (OTWARTE/ZAMKNIETE)
*/
void Comunicator::changeLedStates(byte ledPin1, byte ledPin2){
  analogWrite(ledPin1, this->ledBrightness);
  analogWrite(ledPin2, 0);
}

void Comunicator::ledFlash(byte ledPin, byte numberOfFlashes){
  for(byte i = 0; i < numberOfFlashes; i++){
    analogWrite(ledPin, 0);
    delay(400);
    analogWrite(ledPin, this->ledBrightness);
	delay(400);
  }
}