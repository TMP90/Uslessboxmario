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
int counter = 0;           // Init Zähler

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
        playMusik(16, 30, 3500, 20);                   // ("It's me Mario", Musikvolumen (1-30), Startzeit (ms), Aktivierungszeit (ms))
        servoArmHochSlow(100, 7000, 1000, 0);         // (angle (>0), Startzeit (ms), Dauerzeit (ms), updateInterval (ms))
        servoArmHochFast(140, 8000, 300);             // (angle, Startzeit (ms), Aktivierungszeit (ms))
        servoArmRunterFast(0, 8200, 300);             // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))
        servoKlappeRunterFast(70, 9300, 300);         // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))

        if (millis() - timer > 10000) {               // Dauer der Story
         
            playStoryActivation(false);               // Deaktievierung der Story 0   
             counter++;                                // Zähler hoch 
        }
      }
    }

////////////////////////  SchlussStory  ///////////////////////////////////

    if (counter == 6) {      // Zähler = 9


      while (playStory6 == true)
      { Serial.println("Story Ende");

        //  Task Scheduler Schluss Story
        
        NoneMode(0,900);
        servoKlappeHochSlow(50, 1000, 4000, 80);      // (angle (<70), Startzeit (ms), Aktivierungszeit (ms), updateInterval (ms))     -> SK: Langsame Schritte nach oben
        servoKlappeRunterFast(70, 5000, 200);          // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))                     -> SK: Schnelle Drehung nach unter     
        servoKlappeHochFast(10, 5500, 300);           // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))                          -> SK: Schnelle Drehung nach oben
        playMusik(2, 30, 6500, 40);                   // ("Hey you're very good!",Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit)-> Musique "Hey you're very good!"
        rainbowCycleInit(8000, 50, 5, REVERSE);       // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
        strip.Update();                               // Aktualisierung Leds-Strip 
        
        // attachFahne() und detachFahne(): Für die parallelisierung zwischen LEDs und servos erlaubt die "Adafruit_TiCoServo.h" nur 2 angehängte Servos
        // Deswegen wird der ServoArm desaktiviert, wenn der ServoFahne bewegen soll
         
        attachFahne(6500, 60);                        // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Fahne / detach servo Arm       
        servoFahneHochSlow(180, 7000, 5000, 20);      // (angle (>90), Startzeit (ms), Aktivierungszeit (ms), updateInterval))    -> SF: Langsame Schritte nach oben
        detachFahne(12000, 30);                       // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Arm / detach servo Fahne 
        
        textTheEndDisplay(12300, 50);
        
        servoArmHochSlow(90, 13000, 6000, 50);        // (angle (>0), Startzeit (ms), Aktivierungszeit (ms), updateInterval (ms)) -> SA: Langsame Schritte nach oben
        servoArmHochFast(140, 18000, 300);             // (angle, Startzeit (ms), Aktivierungszeit (ms))                           -> SA: Schnelle Drehung nach oben       
        servoArmRunterFast(0, 18300, 300);             // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))                 -> SA: Schnelle Drehung nach unter   

        playMusik(17, 28, 19000, 80);                 // ("Bye-Bye",Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Bye-Bye!"
        
        attachFahne(20500, 100);                      // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Fahne / detach servo Arm         
        servoFahneRunterFast(70, 21000, 300);          // (angle (<=90), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SF: Langsame Schritte nach oben               
        detachFahne(21500, 30);                       // (Startzeit (ms),Aktivierungszeit (ms))                                   -> Attach Servo Arm / detach servo Fahne            
     
       servoKlappeRunterSlow(75, 22500, 5000, 50);   // (angle (>=70), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SK: Langsame Schritte nach unten
        dimmenLow(23000,5000,50);                         // (Startzeit (ms), Aktivierungszeit (ms),dimmenSpeed (ms))                
        LightFadingDisplay(27000, 2000, 5);
       
        if (millis() - timer > 28000) {               // Dauer der Story
          playStoryActivation(false);                 // Deaktievierung der Story 9
          counter=0;                                  // Zähler hoch 
        }
      }
    }


////////////////////////  Stories  ///////////////////////////////////////

    if ((counter >= 1) && (counter < 6)&&(StoriesGate==true))    // Für Storie 1 zu 8
    {
      int storyNr = counter;             //random(1, 6);         // Zufallszahl [1;5]

      switch (storyNr) {
        
////////////////////////  Story 1: Ampel  ///////////////////////////////////////

        case 1:
          while (playStory1 == true)
          { Serial.println("story 1");

          // Task Scheduler Story 1
         
          NoneMode(0,900);
          strip.Update();
          playMusik(18, 30, 900, 60);                                     // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Countdown"   
          colorWipeMode(900, 70, strip.Color(255,0,0), 0, FORWARD);       // (Startzeit (ms), Aktivierungsdauer (ms), Farbe, Speed (klein=schnell), direction)
          countDown(1000, 20, 3, ILI9340_RED);         
         
          colorWipeMode(2000, 50, strip.Color(255, 82, 0), 0, FORWARD);   // (Startzeit (ms), Aktivierungsdauer (ms), Farbe, Speed (klein=schnell), direction)
          countDown(2000, 20, 2, ILI9340_ORANGE);                         // (Startzeit (ms), Aktivierungsdauer, ziffer (1,2 oder 3), color)
          
          colorWipeMode(3000, 50, strip.Color(243, 218, 11), 0, FORWARD); // (Startzeit (ms), Aktivierungsdauer (ms), Farbe, Speed (klein=schnell), direction)
          countDown(3000, 20, 1, ILI9340_YELLOW);                         // (Startzeit (ms), Aktivierungsdauer, ziffer (1,2 oder 3), color)
         
          colorWipeMode(4000, 50, strip.Color(0,255,0), 0, FORWARD);      // (Startzeit (ms), Aktivierungsdauer (ms), Farbe, Speed (klein=schnell), direction)
          goText(4000,20);
          
          
          moveForwardOrBackward(5500,300,350,350);           //(Startzeit (ms), Aktivierungsdauer (ms), Speed M1, Speed M2)  -> speed=[-400;400]      
          theaterChaseInit(5700,60, strip.Color(127, 127, 127), strip.Color(0, 0, 0),80,FORWARD);                   
          moveForwardOrBackward(5900,300,-350,-350);  
          deactivationDCMotors(6500,100);
         
          servoKlappeHochFast(10, 9000, 40);                              // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))                          -> SK: Schnelle Drehung nach oben  
          servoArmHochFast(145, 10000, 500);                    // (angle, Startzeit (ms), Aktivierungszeit (ms))                           -> SA: Schnelle Drehung nach oben       
          servoArmRunterFast(0, 10600, 500);                   // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))                 -> SA: Schnelle Drehung nach unter   
          servoKlappeRunterFast(70, 10600, 300);                // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
          
            if (millis() - timer > 11000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 1
              counter++;                     // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 2: Stern  ///////////////////////////////////////  
      
        case 2:
        
          while (playStory2 == true)
          { Serial.println("story 2");

          // Task Scheduler Story 2 
          NoneMode(0,900);
          strip.Update(); 

          playMusik(19, 30, 0, 60);                     // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Power_Star_Appears"            
          starPicture(0,100, "StarBunt.bmp");           // (Startzeit (ms), Aktivierungszeit (ms), Bild)
          fadeDisplay(2300,10700,12);                      // (Startzeit (ms), Aktivierungszeit (ms), Speed (gross=schnell: [3,5,15,17])
          servoKlappeHochFast(10, 4000, 40);              // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))                         -> SK: Schnelle Drehung nach oben  
          
          playMusik(7, 30, 4000, 120);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "I_got_a_star"            
          playMusik(20, 30, 8000, 100);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Star Power"      


          rainbowCycleInit(3000, 50, 2, FORWARD);         // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
          rainbowCycleInit(5000, 50, 8, REVERSE);         // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
          rainbowCycleInit(7000, 50, 2, FORWARD);         // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
          rainbowCycleInit(9000, 50, 8, REVERSE);         // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
          rainbowCycleInit(11000, 50, 2, FORWARD);         // (Startzeit (ms), Aktivierungszeit (ms),Speed (klein=schnell), direction)      -> Led RainbowCycle schnell 
          
          servoArmHochFast(140, 12000, 500);               // (angle, Startzeit (ms), Aktivierungszeit (ms))                           -> SA: Schnelle Drehung nach oben       
          servoArmRunterFast(0, 12500, 500);               // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))                 -> SA: Schnelle Drehung nach unter   
          servoKlappeRunterSlow(70, 12000, 5000, 50);     // (angle (>=70), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SK: Langsame Schritte nach unten
          dimmenLow(14000, 2000, 5);                        // (int startTime, int activationInterval, int dimSpeed)
          LightFadingDisplay(13000, 2000, 5);  
          
            if (millis() - timer > 17000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 2
              counter++;                       // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 3: Schnachen  ///////////////////////////////////////  
      
        case 3:
        
          while (playStory3 == true)
          { Serial.println("story 3");

          // Task Scheduler Story 3
          
          NoneMode(0,900);
          brightnessControl(0,80,1);                    // (Startzeit (ms), Aktivierungsdauer (ms), brightness [0;255] (kleinste = 1, grösste = 0) )
          strip.Update(); 
           
          playMusik(8, 30, 0, 150);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "dreaming_of_pasta"     
          setPicture(0, 100, "zzz.bmp",0,false);              // (Startzeit (ms), Aktivierungszeit (ms), Bild, y Position)                    ->  !Ladezeit >=3100 
         
          sunSetPicture(8000,100);                      // (Startzeit (ms), Aktivierungsdauer (ms))  ->  !Ladezeit >=3200
          LightIntensifyingDisplay(11200, 4000, 3);
          colorWipeMode(11200, 50, strip.Color(243, 218, 11), 0, FORWARD); // (Startzeit (ms), Aktivierungsdauer (ms), Farbe, Speed (klein=schnell), direction)
          dimmenHigh(11200, 6000, 90);   
         
           playMusik(23, 25, 11200, 50);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Boite à musique"    
           setPicture(15000, 100, "Reveil.bmp",0,false);          // (Startzeit (ms), Aktivierungszeit (ms), Bild, y Position)                    -> !Ladezeit >=3000
           playMusik(21, 30, 18100, 50);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Wake up"    
           playMusik(22, 30, 21150, 50);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "Oh No!"    
          
           servoKlappeHochSlow(35, 22000, 3000, 30);        // (angle (<70), Startzeit (ms), Dauerzeit (ms), updateInterval (ms))
           servoArmHochFast(149, 23500, 500);               // (angle, Startzeit (ms), Aktivierungszeit (ms))
           servoArmRunterFast(0, 24100, 500);               // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))
           servoKlappeRunterFast(70, 24200, 300);           // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
          
            if (millis() - timer > 25000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 3
              counter++;                       // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 4: Münze  ///////////////////////////////////////  

        case 4:
        
          while (playStory4 == true)
          { Serial.println("story 4");

          // Task Scheduler Story 4

          playMusik(24, 28, 50, 100);                    // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "First Place"             
          NoneMode(0,900); 
          strip.Update();
          setPicture(2300, 100, "Coin.bmp", 40, true); // (Startzeit (ms), Aktivierungszeit (ms), Bild, y Position, hintergrundfarbe aktivieren)  -> !Ladezeit >=2400
          
          servoKlappeHochFast(50, 7000, 30);           // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))
          playMusik(25, 30,7000, 100);                 // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "coin sound"    
          inverseDisplay(7000, 100,true);               // (Startzeit (ms), Aktivierungszeit (ms), zustand (true/false) )
          theaterChaseInit(7000,2000, strip.Color(255, 255, 0),strip.Color(255, 0, 0),50,FORWARD);    // (Startzeit (ms), Aktivierungszeit (ms), Farbe1, Farbe2, Speed (klein=schnell), direction ) {
          servoKlappeRunterFast(70, 7300, 300);        // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
          inverseDisplay(7500, 100,false);
          brightnessControl(7800,100,0);
       
          servoKlappeHochFast(50, 8000, 30);           // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))
          playMusik(25, 30,8000, 150);                 // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "coin sound"    
          inverseDisplay(8000, 100,true); 
          brightnessControl(8000,100,70);
          servoKlappeRunterFast(70, 8300, 300);        // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
          inverseDisplay(8500, 100,false); 
          brightnessControl(8800,100,0);
         
          servoKlappeHochFast(50, 9000, 30);           // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))
          playMusik(25, 30,9000, 150);                 // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "coin sound"    
          inverseDisplay(9000, 100,true); 
          brightnessControl(9000,100,70);
          servoKlappeRunterFast(70, 9300, 300);        // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
          inverseDisplay(9500, 100,false); 
          brightnessControl(9800,100,0);
          
          servoKlappeHochFast(30, 10000, 30);           // (angle (<70), Startzeit (ms), Aktivierungszeit (ms)) 
          servoArmHochFast(155, 10200, 600);               // (angle, Startzeit (ms), Aktivierungszeit (ms))
          playMusik(25, 30,10200, 150);                 // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))    -> Musique "coin sound"   
          servoArmRunterFast(0, 10900, 500);               // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))
          servoKlappeRunterFast(70, 11100, 300);           // (angle (gegen 70), Startzeit (ms), Aktivierungszeit (ms))
        
            if (millis() - timer > 11300) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 3
              counter++;                       // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 5: Banane  ///////////////////////////////////////  

  case 5:
        
          while (playStory5 == true)
          { Serial.println("story 5");

          // Task Scheduler Story 5

          NoneMode(0,500); 
          strip.Update();
          setPicture(0, 100, "Banane.bmp", 0, false);    // (Startzeit (ms), Aktivierungsdauer (ms), Bild, y Position, hintergrundfarbe aktivieren)   -> !Ladezeit >=2950
          playMusik(6, 30,2950, 150);                     // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))                 -> Musique "drop Banane"    
          playMusik(4, 30,3600, 150);                     // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))                 -> Musique "Awowowow"    
          
          
          theaterChaseInit(4500,60, strip.Color(255, 0, 0), strip.Color(0, 0, 0),50,FORWARD);
             
          moveSpinnerLeft(5000,200,400);                  // ( Speed, Startzeit (ms), Aktivierungsdauer (ms) )       -> speed=[-400;400]        
          deactivationDCMotors(9000,50);

           servoKlappeHochFast(35, 10000, 300);              
           servoArmHochSlow(90, 10500, 1000, 10);           // (angle (>0), Startzeit (ms), Aktivierungszeit (ms), updateInterval (ms)) -> SA: Langsame Schritte nach oben
           servoArmHochFast(150, 11500, 500);               // (angle (<70), Startzeit (ms), Aktivierungszeit (ms))        
           servoKlappeRunterSlow(70, 12000, 4000, 20);      // (angle (>=70), Startzeit (ms), Aktivierungszeit (ms), updateInterval))   -> SK: Langsame Schritte nach unten
           servoArmRunterFast(0, 12000, 700);               // (angle (gegen 0), Startzeit (ms), Aktivierungszeit (ms))
          
          
            if (millis() - timer > 14000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 3
              counter++;                     // Zähler hoch 
            }
          }
          break;

////////////////////////  Story 6: Boxer  /////////////////////////////////////// 

 /*case 6:
        
          while (playStory6 == true)
          { Serial.println("story 6");

          // Task Scheduler Story 6

          
       
          playMusik(4, 25,0, 60);                        // (Musiknummmer, Musikvolumen (1-30),Startzeit (ms), Aktivierungszeit (ms))                 -> Musique "Rocky"    

          setPicture(0, 100, "Box.bmp", 0, false);       // (Startzeit (ms), Aktivierungsdauer (ms), Bild, y Position, hintergrundfarbe aktivieren)   -> !Ladezeit >=2950
         
          moveForwardOrBackward(4700,200,-400,-400);        // ( Speed, Startzeit (ms), Aktivierungsdauer (ms), Speed M1, Speed M2 )                     -> speed=[-400;400]  
          deactivationDCMotors(5000,60);
          
          moveSpinnerLeft(5800,70,300);                     // ( Startzeit (ms), Aktivierungsdauer (ms), Speed)        -> speed=[0;400]   
          deactivationDCMotors(6200,60);   
          moveSpinnerRight(6900,70,300);                    // ( Startzeit (ms), Aktivierungsdauer (ms), Speed)        -> speed=[0;400]   
          deactivationDCMotors(7100,60);          
          moveSpinnerLeft(8000,70,300);                     // ( Startzeit (ms), Aktivierungsdauer (ms), Speed)        -> speed=[0;400]   
          deactivationDCMotors(8400,60);            
          
          moveForwardOrBackward(8400,200,400,400);          // ( Speed, Startzeit (ms), Aktivierungsdauer (ms), Speed M1, Speed M2 )                     -> speed=[-400;400]  
          deactivationDCMotors(8700,60);

          
          
            if (millis() - timer > 13000) {    // Dauer der Story
              playStoryActivation(false);      // Deaktievierung der Story 3
              //counter++;                     // Zähler hoch 
            }
          }
          break;*/

      }
    }
  }

  else {
    Serial.println("off");
    servosDetach();                // desaktivierung Servos
    desactivM1M2();                // desaktivierung DC Motors
    strip.clear();                 // desaktivierung leds
    strip.show();                  // desaktivierung leds
    strip.setBrightness(60);       // Contrast einstellen: 80
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

   StoriesGate = state;
}



