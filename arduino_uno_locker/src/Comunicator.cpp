#include "../include/Comunicator.h"

Comunicator::Comunicator(LCD_I2C &lcd, byte buzzerPin) {
  this->lcd = &lcd;
  this->buzzerPin = buzzerPin;
}


void Comunicator::welcomeScreen() {
  lcd->setCursor(5, 0);
  lcd->print("Witaj!");
  delay(2000);
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

void Comunicator::presscToClose(){
    lcd->clear();
    delay(500);
    lcd->setCursor(0, 0);
    lcd->print("Nacisnij 'c' aby");
    lcd->setCursor(2, 1);
    lcd->print("zamknac skrytke!");
    delay(500);
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
  delay(1000);
}

void Comunicator::buzzerSound(int time) {
  analogWrite(buzzerPin, HIGH);
  delay(time);
  analogWrite(buzzerPin, LOW);
}

/*
  Procedura sluzaca do zamiany stanow diod - uzywana w momencie zmiany stanu urzadzenia (OTWARTE/ZAMKNIETE)
*/
void Comunicator::changeLedStates(byte ledPin1, byte ledPin2){
  analogWrite(ledPin1, HIGH);
  analogWrite(ledPin2, LOW);
}

void Comunicator::ledFlash(byte ledPin, byte numberOfFlashes){
  for(byte i = 0; i < numberOfFlashes; i++){
    analogWrite(ledPin, LOW);
    delay(1000);
    analogWrite(ledPin, HIGH);
  }
}