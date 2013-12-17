void manageCommande(char input[40], int lenght) {
  char chiffreToSet[] = "    ";
  char commande[] = "   ";
  for (int i = 0; i < 3; i++)
  {
    commande[i] = input[i];
  }

  if (strcmp(commande, cSetDistanceTrain)  == 0) {
    if (lenght > 3) {
      for (int i = 3; i < lenght; i++) {
        chiffreToSet[i - 3] = input[i];
      }
      setDistanceTrain(chiffreToSet);
    }
    else
      setDistanceTrainSenseur();
  }
  if (strcmp(commande, cGetDistanceTrain)  == 0)
    getDistanceTrain();
  if (strcmp(commande, cGetDistance)  == 0)
    getDistance();
  if (strcmp(commande, cGetRunningTime)  == 0)
    getRunningTime();
  if (strcmp(commande, cStartDMX)  == 0)
    startDMX();
  if (strcmp(commande, cActiverSenseur)  == 0)
    activerSenseur();
  if (strcmp(commande, cDesactiverSenseur)  == 0)
    desactiverSenseur();
  if (strcmp(commande, cTempsTrain)  == 0) {
    if (lenght > 3) {
      for (int i = 3; i < lenght; i++) {
        chiffreToSet[i - 3] = input[i];
      }
      setTempsTrain(chiffreToSet);
    }
  }
  if (strcmp(commande, cSetRange)  == 0) {
    if (lenght > 3) {
      for (int i = 3; i < lenght; i++) {
        chiffreToSet[i - 3] = input[i];
      }
      setRange(chiffreToSet);
    }
  }
}

void startDMX() {
  xbeeSend("startDMX recu", r2Addr);
  if (dmxON == false) {
    DmxSimple.write(1, 255);
    dmxON = true;
    tempsDepuisStartDMX = millis();
  }
}

void getDistanceTrain() {
  char buffer[12];
  xbeeSend(itoa(distanceTrain, buffer, 10), r2Addr);
}

void getRunningTime() {
  char buffer[12];
  xbeeSend(itoa(millis() / 1000, buffer, 10), r2Addr);
}

void getDistance() {
  char buffer[12];
  xbeeSend(itoa(distance, buffer, 10), r2Addr);
}


void setDistanceTrain(char chiffreToSet[]) {
  char buffer[12];
  distanceTrain = atoi(chiffreToSet);
  EEPROM.write(0, highByte(distanceTrain));
  EEPROM.write(1, lowByte(distanceTrain));
  xbeeSend(itoa(distanceTrain, buffer, 10), r2Addr);
}

void setDistanceTrainSenseur() {
  char buffer[12];
  distanceTrain = distance;
  EEPROM.write(0, highByte(distanceTrain));
  EEPROM.write(1, lowByte(distanceTrain));
  xbeeSend(itoa(distanceTrain, buffer, 10), r2Addr);
}

void setTempsDMX(char tempsToSet[]) {
  char buffer[12];
  tempsDMX = atoi(tempsToSet);
  EEPROM.write(2, tempsDMX);
  xbeeSend(itoa(tempsDMX, buffer, 10), r2Addr);
}

void setTempsTrain(char tempsToSet[]) {
  char buffer[12];
  tempsTrain = atoi(tempsToSet);
  EEPROM.write(3, tempsTrain);
  xbeeSend(itoa(tempsTrain, buffer, 10), r2Addr);
}

void setRange(char rangeToSet[]) {
  char buffer[12];
  range = atoi(rangeToSet);
  EEPROM.write(4, range);
  xbeeSend(itoa(range, buffer, 10), r2Addr);
}


void activerSenseur() {
  senseur = true;
}

void desactiverSenseur() {
  senseur = true;
}










