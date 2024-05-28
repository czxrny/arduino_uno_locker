#include "../include/PasswordManager.h"

// funkcja wywolywana przy odczycie klawiatury - wymaga przekazania komunikatora do interaktywnej obslugi ekranu w momencie wcisniecia przycisku!
PasswordManager::PasswordManager(Comunicator *comunicator, Keypad *keypad, int passwordAddress, byte passwordLength) {
  this->comunicator = comunicator;
  this->keypad = keypad;
  this->passwordAddress = passwordAddress;
  this->passwordLength = passwordLength;
}

void PasswordManager::setNewPassword() {
  // wybranie dlugosci hasla, oraz utworzenie dwoch dynamicznie alokowanych tablic, ktore pozwola na zapis i sprawdzenie poprawnosci hasla(czy sie zgadzaja).
  char *newPsswd = new char[passwordLength];
  char *psswdCheck = new char[passwordLength];
  bool passwordsAreTheSame;

  do {
    passwordsAreTheSame = true;
    this->comunicator->buzzerSound(300);
    this->comunicator->setNewPassword();

    // TU JEST KOD Z SPRAWDZENIEM CZY KTORYS PRZYCISK ZOSTAL KLIKNIETY
    readKeyboard(newPsswd);  // Przekazujemy wskaźnik na pierwszy element tablicy
    this->comunicator->buzzerSound(300);
    this->comunicator->confirmPassword();
    readKeyboard(psswdCheck);  // Przekazujemy wskaźnik na pierwszy element tablicy

    // Porównanie haseł
    passwordsAreTheSame = passwordsMatch(newPsswd, psswdCheck);
    if (!passwordsAreTheSame) {
      this->comunicator->buzzerSound(500);
      this->comunicator->enterPassword();
    }
  } while (!passwordsAreTheSame);

  // zapis nowego hasla
  for (int i = 0; i < passwordLength; i++)
    EEPROM.write(passwordAddress + i, newPsswd[i]);

  // zwolnienie pamieci
  delete[] newPsswd;
  delete[] psswdCheck;
}

/*
  Funkcja sprawdzajaca dwa hasla, zwracajaca wartosc true jesli sie zgadzaja
*/
bool PasswordManager::passwordsMatch(char *password1, char *password2) {
  bool areTheSame = true;
  for (byte i = 0; i < this->passwordLength; i++) {
    if (password1[i] != password2[i]) {
      areTheSame = false;
      break;
    }
  }
  return areTheSame;
}
/*
  Wypelnia wskazana tablice aktualnie zapisanym haslem w pamieci.
*/
void PasswordManager::readPassword(char *passwordTable) {
  for (int i = 0; i < this->passwordLength; i++)
    passwordTable[i] = EEPROM.read(this->passwordAddress + i);
}

/*
  Funkcja zwracajaca wartosc boolean, majaca na celu okreslenie czy w okreslonym obszarze pamieci istnieje jakies haslo?
*/
bool PasswordManager::passwordIsSet() {
  if (EEPROM.read(passwordAddress) == 255)
    return true;
  else
    return false;
}

/*
  Procedura sluzaca do czytania wprowadzonych danych przez uzytkowanika przez klawiature, do momentu sczytania ilosci znakow rownej dlugosci hasla. Zmienia zawartosc tablicy.
*/
void PasswordManager::readKeyboard(char *psswd) {
  int cursor = 16 / this->passwordLength;
  int counter = 0;

  while (counter < this->passwordLength) {
    char num = this->keypad->getKey();
    if (num >= 48 && num <= 57) {
      this->comunicator->buzzerSound(300);
      psswd[counter] = num;
      this->comunicator->printchar(cursor, 1, '*');
      cursor += 2;
      counter++;
    }
  }
}