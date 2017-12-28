#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

void desactivM1M2() {
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}


void moveForwardOrBackward(int m1Speed, int m2Speed, int startTime, int interval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + interval))) {
    motors.setSpeeds(m1Speed, m2Speed);
  }

  else {
    desactivM1M2();
  }
}


void moveM1Only (int speed, int startTime, int interval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + interval))) {
    motors.setM1Speed(speed);
  }
}

void moveM2Only (int speed, int startTime, int interval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + interval))) {
    motors.setM2Speed(speed);
  }
}


void moveSpinnerRight(int speed, int startTime, int interval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + interval))) {
    motors.setSpeeds(speed, -speed); // (M1Speed = -M2Speed)
  }
}


void moveSpinnerLeft(int speed, int startTime, int interval) { // speed=[-400;400]

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + interval))) {
    motors.setSpeeds(-speed, speed); // (M1Speed = -M2Speed)
  }
}



