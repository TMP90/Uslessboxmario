#include <Servo.h>
#include <Adafruit_TiCoServo.h>
#include <known_16bit_timers.h>

#define SERVO_MIN 1000 // 1 ms pulse
#define SERVO_MAX 2000 // 2 ms pulse
#define SK_PIN 6
#define SA_PIN 5
#define SF_PIN 3

Adafruit_TiCoServo servoKlappe;
Adafruit_TiCoServo servoArm;
Adafruit_TiCoServo servoFahne;
// Servo servoFahne;

unsigned long timer = millis();
unsigned long lastUpdate =  millis();
int startAngle;
int countAngle;


////////////////   INITIALISATION SERVO   /////////////////////////////////////////////////////////////////////////////////////

void initServo() {
  Serial.begin(9600);
  servoKlappe.attach(SK_PIN);
  servoKlappe.write(70);
  servoArm.attach(SA_PIN);
  servoArm.write(0);
  delay(500);

  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////   SERVO KLAPPE   /////////////////////////////////////////////////////////////////////////////////////

void servoKlappeHochSlow(int angle, int startTime_SK, int runTime, int updateInterval)
{

  //pos = servoKlappe.read();
  if (((millis() - timer >= startTime_SK)) && ((millis() - timer < (startTime_SK) + runTime)))
  {

    if (millis() - timer <= startTime_SK + 30)
    {
      pinMode(SK_PIN, OUTPUT);
      startAngle = servoKlappe.read();
      countAngle = servoKlappe.read();
    }

    if (((millis() - lastUpdate) > updateInterval) && ((startAngle - servoKlappe.read()) <= (startAngle - angle))) // time to update
    {
      Serial.println("servoKLAPPE HOCH SLOW fonctionne");

      countAngle--;
      servoKlappe.write(countAngle);
      delay(5);
      lastUpdate = millis();
    }
  }
}


void servoKlappeRunterSlow(int angle, int startTime_SK, int runTime, int updateInterval)
{
  if ((((millis() - timer >= startTime_SK)) && (millis() - timer < (startTime_SK) + runTime))) //&&(servoKlappe.read() >= angle )
  {


    if (millis() - timer <= startTime_SK + 20)
    {
      pinMode(SK_PIN, OUTPUT);
      startAngle = servoKlappe.read();
      countAngle = servoKlappe.read();
    }

    if (((millis() - lastUpdate) > updateInterval) && ((startAngle - servoKlappe.read()) >= (startAngle - angle))) // time to update
    {
      Serial.println("servoKLAPPE RUNTER SLOW fonctionne");

      countAngle++;
      servoKlappe.write(countAngle);
      delay(5);
      lastUpdate = millis();
    }
  }
}


void servoKlappeHochFast(int angle, int startTime_SK, int runTime)
{

  if (((millis() - timer >= startTime_SK) && (millis() - timer < (startTime_SK) + runTime))) //&&(servoKlappe.read() < angle))
  {
    Serial.println("servo KLAPPE HOCH FAST fonctionne");
    pinMode(SK_PIN, OUTPUT);
    servoKlappe.write(angle);
  }
}

void servoKlappeRunterFast(int angle, int startTime_SK, int runTime)
{

  if (((millis() - timer >= startTime_SK) && (millis() - timer < (startTime_SK) + runTime))) //&&(servoKlappe.read() < angle))
  {
    Serial.println("servoKLAPPE RUNTER FAST fonctionne");
    pinMode(SK_PIN, OUTPUT);
    servoKlappe.write(angle);
    delay(15);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////   SERVO ARM   /////////////////////////////////////////////////////////////////////////////////////


void servoArmHochSlow(int angle, int startTime_SA, int runTime, int updateInterval)
{
  if ((((millis() - timer >= startTime_SA)) && (millis() - timer < (startTime_SA) + runTime))) //&&(servoKlappe.read() >= angle )
  {

    if (millis() - timer <= startTime_SA + 30)
    {
      pinMode(SA_PIN, OUTPUT);
      startAngle = servoArm.read();
      countAngle = servoArm.read();
    }

    if (((millis() - lastUpdate) >= updateInterval) && ((startAngle - servoArm.read()) >= (startAngle - angle))) // time to update
    {
      Serial.println("servo ARM HOCH SLOW fonctionne");

      countAngle++;
      servoArm.write(countAngle+2);
      delay(1);
      lastUpdate = millis();
    }
  }
}

void servoArmRunterSlow(int angle, int startTime_SA, int updateInterval)
{
  if (((millis() - timer >= startTime_SA))) //&& (millis() - timer < (startTime_SA) + runTime)))
  {

    if (millis() - timer <= startTime_SA + 30)
    {
      pinMode(SA_PIN, OUTPUT);
      startAngle = servoArm.read();
      countAngle = servoArm.read();
    }

    if (((millis() - lastUpdate) > updateInterval) && ((startAngle - servoArm.read()) < (startAngle - angle))) // time to update
    {
      Serial.println("servo ARM RUNTER SLOW fonctionne");
      countAngle--;
      servoArm.write(countAngle);
         delay(10);
      lastUpdate = millis();
    }
  }
}





void servoArmHochFast(int angle, int startTime_SA, int runTime)
{

  if (((millis() - timer >= startTime_SA) && (millis() - timer < (startTime_SA) + runTime)))
  {
    Serial.println("SERVO ARM HOCH FAST fonctionne");
    pinMode(SA_PIN, OUTPUT);
    servoArm.write(angle);
  }
}

void servoArmRunterFast(int angle, int startTime_SA, int runTime)
{

  if (((millis() - timer >= startTime_SA) && (millis() - timer < (startTime_SA) + runTime)))
  {
    Serial.println("SERVO ARM RUNTER FAST fonctionne");
    pinMode(SA_PIN, OUTPUT);
    servoArm.write(angle);
    delay(5);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////   SERVO FAHNE   /////////////////////////////////////////////////////////////////////////////////////

void servoFahneHochSlow(int angle, int startTime_SF, int runTime, int updateInterval)
{
  if ((((millis() - timer >= startTime_SF)) && (millis() - timer < (startTime_SF) + runTime)))
  {


    if (millis() - timer <= startTime_SF + 20)
    {
     
      pinMode(SF_PIN, OUTPUT);
      startAngle = servoFahne.read();
      countAngle = servoFahne.read();
    }

    if (((millis() - lastUpdate) > updateInterval) && ((startAngle - servoFahne.read()) >= (startAngle - angle))) // time to update
    {
      Serial.println("servo FAHNE RUNTER FAST fonctionne");
      countAngle++;
      servoFahne.write(countAngle+2);
      delay(2);
      lastUpdate = millis();
    }
  }
}

void servoFahneRunterSlow(int angle, int startTime_SF, int runTime, int updateInterval)
{

  if (((millis() - timer >= startTime_SF) && (millis() - timer < (startTime_SF) + runTime)))
  {

    if (millis() - timer <= startTime_SF + 20)
    {
      pinMode(SF_PIN, OUTPUT);
      startAngle = servoFahne.read();
      countAngle = servoFahne.read();
    }

    if (((millis() - lastUpdate) > updateInterval) && ((startAngle - servoFahne.read()) < (startAngle - angle))) // time to update
    {
      Serial.println("servo FAHNE HOCH Slow fonctionne");

      countAngle--;
      servoFahne.write(countAngle);
      lastUpdate = millis();
    }
  }
}


void servoFahneHochFast(int angle, int startTime_SF, int runTime)
{

  if (((millis() - timer >= startTime_SF) && (millis() - timer < (startTime_SF) + runTime)))
  {
    pinMode(SF_PIN, OUTPUT);
    servoFahne.write(angle);
  }
}

void servoFahneRunterFast(int angle, int startTime_SF, int runTime)
{

  if (((millis() - timer >= startTime_SF) && (millis() - timer < (startTime_SF) + runTime)))
  {
   // pinMode(SF_PIN, OUTPUT);
    servoFahne.write(angle);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////   DETACH SERVOS   /////////////////////////////////////////////////////////////////////////////////////

void servosDetach () {

  pinMode(SK_PIN, INPUT);
  pinMode(SA_PIN, INPUT);
  pinMode(SF_PIN, INPUT);

}

void attachFahne(int startTime, int runTime){
  
  if (((millis() - timer >= startTime) && (millis() - timer < (startTime) + runTime)))
  {
  servoArm.detach();
  servoFahne.attach(SF_PIN);
 
  }
}

void detachFahne(int startTime, int runTime){
  
  if (((millis() - timer >= startTime) && (millis() - timer < (startTime) + runTime)))
  {
  servoFahne.detach();
  servoArm.write(0);
  servoArm.attach(SA_PIN);
 
  }
}


