//#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(62, 63); // RX, TX (A8,A9)
DFRobotDFPlayerMini Mp3Module;


void initMP3Player() {

  mySoftwareSerial.begin(9600);
  //Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!Mp3Module.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
}

void playMusik(int musikNr, int musikVol, int startTime, int activationInterval) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    
    Mp3Module.volume(musikVol);
    Mp3Module.play(musikNr);

  }
}

deactivationMusic(int startTime, int activationInterval)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)  ))
  {
    mySoftwareSerial.end();
  }
}

