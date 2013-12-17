//Pour R1

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <XBee.h>
#include <DmxSimple.h>

SoftwareSerial xbeeSerial(5, 6); // RX, TX

XBeeAddress64 r2Addr = XBeeAddress64(0x0013a200, 0x40b08747);
XBeeAddress64 r1Addr = XBeeAddress64(0x0013a200, 0x409365dc);//Network coordinator forwards data to Pachube
XBeeAddress64 c1Addr = XBeeAddress64(0x0013a200, 0x409365c4);
#define XBEE_PAYLOAD_LEN 40      //this should agree with the definition in the remote code
XBee xbee = XBee();              //create the XBee object
union {
  byte B;
  char C;
}
xbeePayload[XBEE_PAYLOAD_LEN];
ZBTxRequest zbTx = ZBTxRequest();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

int distanceTrain = 0; //Distance du train
int distance = 0; //Valeur du range finder
int range = 0;

//les commandes
char cSetDistanceTrain[] = "#st";
char cGetDistanceTrain[] = "#gt";
char cGetDistance[] = "#gd";
char cGetRunningTime[] = "#gr";
char cStartDMX[] = "$sd";
char cTempsTrain[] = "#tt";
char cTempsDMX[] = "#td";
char cSetRange[] = "#sr";
char cActiverSenseur[] = "#as";
char cDesactiverSenseur[] = "#ds";


unsigned long tempsDepuisStartDMX = 0;
unsigned long tempsDepuisTrain = 0;

unsigned long tempsDMX = 0;
unsigned long tempsTrain = 0;
boolean dmxON = false;
boolean train = false;
boolean senseur = true;




void setup() {
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);
  xbee.setSerial(xbeeSerial); //On utilise l'objet serial xbeeSerial pour le XBEE
  delay(5000);                    //give the XBee time to associate

  DmxSimple.usePin(3); //DMX utilise la pin 5

  //Récupération de la hauteur du train (mm) dans le EEPROM
  byte distanceTrainHB = EEPROM.read(0);
  byte distanceTrainLB = EEPROM.read(1);
  tempsDMX = EEPROM.read(2);
  tempsTrain = EEPROM.read(3);
  range = EEPROM.read(4);
  distanceTrain = distanceTrainLB | (distanceTrainHB << 8);

  Serial.println("initiated"); //pour indiquer la fin du setup
  DmxSimple.write(1, 0);
  DmxSimple.write(2, 0);
  DmxSimple.write(3, 0);


}

void loop() {
  //distance = readDistance();

  readXBee();
  updateDistance();
  updateDMX();
  if (senseur = true) {
    updateTrain();
  }
  //On regarde si l'objet que percoit le range finder équivaut a plus ou moins 5 centimetre la hauteur du train

}



void updateDistance() {
  char distanceData[] = "    ";
  while (Serial.available() <= 0) {
  }
  char incoming = Serial.read();
  while (incoming != 'R') {
    incoming = Serial.read();
  }
  while (Serial.available() < 4) {
  }
  distanceData[0] = Serial.read();
  delay(2);
  distanceData[1] = Serial.read();
  delay(2);
  distanceData[2] = Serial.read();
  delay(2);
  distanceData[3] = Serial.read();

  distance = atoi(distanceData);
}

void updateDMX() {
  if (millis() > (tempsDepuisStartDMX + (tempsDMX * 1000)) && dmxON) {
    DmxSimple.write(1, 0);
    dmxON = false;
  }

}


void updateTrain() {
  if (distance > (distanceTrain - range) && distance < (distanceTrain + range) && train == false) {
    Serial.println("train = true");
    startDMX();
    tempsDepuisTrain = millis();
    train = true;
    xbeeSend("$sd", c1Addr);
  }
  if (millis() > (tempsDepuisTrain + (tempsTrain * 1000)) && train) {
    Serial.println("train = false");
    train = false;
  }

}









