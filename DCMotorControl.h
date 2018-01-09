#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

void desactivM1M2() {
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}


void moveForwardOrBackward(int startTime, int activationInterval, int m1Speed, int m2Speed) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    motors.setSpeeds(m1Speed, m2Speed);
  }
}


void deactivationDCMotors(int startTime, int activationInterval)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {
    desactivM1M2();
  }
}

void moveM1Only (int speed, int startTime, int activationInterval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    motors.setM1Speed(speed);
  }
}

void moveM2Only (int speed, int startTime, int activationInterval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    motors.setM2Speed(speed);
  }
}


void moveSpinnerRight(int startTime, int activationInterval, int speed) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    motors.setSpeeds(speed, -speed); // (M1Speed = -M2Speed)
  }
}


void moveSpinnerLeft(int startTime, int activationInterval, int speed) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    motors.setSpeeds(-speed, speed); // (M1Speed = -M2Speed)
  }
}



