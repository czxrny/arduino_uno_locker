#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H
#include "Comunicator.h"
#include "Arduino.h"
#include <EEPROM.h>
#include <Keypad.h>

class PasswordManager {
public:
  PasswordManager(Comunicator *comunicator, Keypad *keypad, int passwordAddress, byte passwordLength);
  void setNewPassword();
  void readPassword(char *passwordTable);
  bool passwordIsSet();
  void readKeyboard(char *psswd);
  bool passwordsMatch(char *password1, char *password2);
private:
  Comunicator *comunicator;
  Keypad *keypad;
  int passwordAddress;
  byte passwordLength;
};

#endif