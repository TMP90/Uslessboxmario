#include "Arduino.h"
#include <SD.h>

extern "C" {
#include "ServoControl.h"                  // Externe  Bibliotheken.  
}                                          // Eine pro Funktionalität

extern "C" {

#include "BildschirmControl.h"
}

extern "C" {
#include "MusikControl.h"
}

extern "C" {
#include "LEDsControl.h"
}

extern "C" {
#include "DCMotorControl.h"
}
//////////////////////////////////////////////////////////////////////////

const int buttonPin = 4;    // PIN Schalter 
int buttonState = 0;        // Schalter eingeschaltet (1) /  ausgeschaltet (0)
int counter = 9;           // Init Zähler

  bool playStory0 = true;     // Story flag: Aktievierung der Sories
  bool playStory1 = true;
  bool playStory2 = true;
  bool playStory3 = true;
  bool playStory4 = true;
  bool playStory5 = true;
  bool playStory6 = true;
  bool playStory7 = true;
  bool playStory8 = true;
  bool playStory9 = true;
  bool StoriesGate = true;

//////////////////////////////////////////////////////////////////////////

void setup()                             // Init aller Fonktionen
{
  pinMode(buttonPin, INPUT);             // PIN Schalter als INPUT
  initMP3Player();
  initLedStreifen();
  initServo();
  initDisplay();
  playStoryActivation(true);
}

//////////////////////////////////////////////////////////////////////////

void loop()                              // Main 
{

  buttonLister();                        // Überwachung des Schalterzustandes

  if (buttonState == LOW) {              // Schalterzustand = ON 
    Serial.println("on");

////////////////////////  AnfangsStory  ///////////////////////////////////
 
    if (counter == 0) {                  // Zähler = 0

      while (playStory0 == true)         // Die While-Schleife ermöglicht, dass die nur diese Story gelesen und ausgeführt sein könne
      { Serial.println("Story Anfang");  // in einer bestimmten Zeit. Das ein zentraler Aspekt der Struktur des Programmes, da es wird hier ein 
                                         // Task Scheduler benutzt
                                         
        // Methoden mit parametern: Task Scheduler
                                          
        servoKlappeHochSlow(5, 1000, 6000, 20);       // (angle (<70), Startzeit (ms), Dauerzeit (ms), updateInterval (ms))
        playMusik(19, 5, 4000, 20);                   // ("It's me Mario", Musikvolumen (1-30), Startzeit (ms), Aktivierungszeit (ms))
        servoArmHochSlow(100, 8000, 5000, 0);         // (angle (>0), Startzeit (ms), Dauerzeit (ms), updateInterval (ms))
        servoArmHochFast(125, 13000, 30);             // (angle, Startzeit (ms), Aktivierungszeit (ms))
        servoArmRunterFast(0, 13500, 50);             // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))
        servoKlappeRunterFast(70, 13600, 30);         // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))

        if (millis() - timer > 15000) {               // Dauer der Story
         
            playStoryActivation(false);               // Deaktievierung der Story 0   
            counter++;                                // Zähler hoch 
        }
      }
    }

////////////////////////  SchlussStory  ///////////////////////////////////

    if (counter == 9) {      // Zähler = 9


      while (playStory9 == true)
      { Serial.println("Story Ende");

        // Methoden mit parametern: Task Scheduler
        
        servoKlappeHochSlow(50, 1000, 4000, 80);      // (angle (<70), Startzeit (ms), Aktivierungszeit (ms), updateInterval (ms))     -> SK: Langsame Schritte nach oben
        servoKlappeRunterFast(75, 5000, 20);          // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))                     -> SK: Schnelle Drehung nach unter     
        servoKlappeHochFast(5, 5500, 20);             // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))                          -> SK: Schnelle Drehung nach oben
        playMusik(2, 10, 6500, 10);                   // ("Hey you're very good!",Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit)-> Musique "Hey you're very good!"
        rainbowCycleInit(6000, 50, 5, REVERSE);       // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
        strip.Update();                               // Aktualisierung Leds-Strip 
        
        // attachFahne() und detachFahne(): Für die parallelisierung zwischen LEDs und servos erlaubt die "Adafruit_TiCoServo.h" nur 2 angehängte Servos
        // Deswegen wird der ServoArm desaktiviert, wenn der ServoFahne bewegen soll
         
        attachFahne(6000, 30);                        // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Fahne / detach servo Arm       
        servoFahneHochSlow(180, 7000, 7000, 0);       // (angle (>90), Startzeit (ms), Aktivierungszeit (ms), updateInterval))    -> SF: Langsame Schritte nach oben
        detachFahne(14000, 30);                       // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Arm / detach servo Fahne 
        servoArmHochSlow(90, 15000, 6000, 50);        // (angle (>0), Startzeit (ms), Aktivierungszeit (ms), updateInterval (ms)) -> SA: Langsame Schritte nach oben
        servoArmHochFast(120, 21000, 30);             // (angle, Startzeit (ms), Aktivierungszeit (ms))                           -> SA: Schnelle Drehung nach oben
      //playMusik(?, 10, 9000, 10);                   // ("By",Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))         -> Musique "Bye!"
        servoArmRunterFast(0, 21200, 50);             // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))                 -> SA: Schnelle Drehung nach unter   
      //dimmen(9500,14000,100);                       // (Startzeit (ms), Aktivierungszeit (ms),dimmenSpeed (ms))                 -> Leds: Langsam dimmen
        attachFahne(21400, 30);                       // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Fahne / detach servo Arm     
        servoFahneRunterFast(90, 23000, 30);          // (angle (<=90), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SF: Langsame Schritte nach oben
        detachFahne(23500, 30);                       // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Arm / detach servo Fahne 
        servoKlappeRunterSlow(75, 23000, 7000, 50);   // (angle (>=70), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SK: Langsame Schritte nach unten
        
        if (millis() - timer > 28000) {               // Dauer der Story
          playStoryActivation(false);                 // Deaktievierung der Story 9
          counter++;                                  // Zähler hoch 
        }
      }
    }


////////////////////////  Stories  ///////////////////////////////////////

    if ((counter >= 1) && (counter < 9)&&(StoriesGate==true))    // Für Storie 1 zu 8
    {
      int storyNr = random(1, 9);                                 // Zufallszahl [1;8]

      switch (storyNr) {
        
////////////////////////  Story 1  ///////////////////////////////////////

        case 1:
          while (playStory1 == true)
          { Serial.println("story 1");

          // Task Scheduler Story 1
          
    
            if (millis() - timer > 20000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 1
              counter++;                       // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 2  ///////////////////////////////////////  
      
        case 2:
        
          while (playStory2 == true)
          { Serial.println("story 2");

          // Task Scheduler Story 2
          
    
            if (millis() - timer > 20000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 2
              counter++;                       // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 3  ///////////////////////////////////////  
      
        case 3:
        
          while (playStory3 == true)
          { Serial.println("story 3");

          // Task Scheduler Story 3
          
    
            if (millis() - timer > 20000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 3
              counter++;                       // Zähler hoch 
            }
          }
          break;



      }
    }
  }

  else {
    Serial.println("off");
    servosDetach();                // desaktivierung Servos
    desactivM1M2();                // desaktivierung DC Motors
    strip.clear();                 // desaktivierung leds
    strip.show();                  // desaktivierung leds
    strip.setBrightness(80);       // Contrast einstellen: 80
    timer = millis();              // Reset Timer
    digitalWrite(BL, LOW);         // Blidschirm ausblenden
    //mySoftwareSerial.end();

    playStoryActivation(true);          // Stories flags: Aktievierung der Sories

  }
}


void buttonLister()                  // Überwachung des Schalterzustandes
{
  buttonState = digitalRead(buttonPin);
}

void playStoryActivation(bool state)
{
   playStory0 = state;     // Story flag: Aktievierung der Sories
   playStory1 = state;
   playStory2 = state;
   playStory3 = state;
   playStory4 = state;
   playStory5 = state;
   playStory6 = state;
   playStory7 = state;
   playStory8 = state;
   playStory9 = state;
   StoriesGate = state;
}



