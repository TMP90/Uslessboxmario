extern "C" {
#include "Adafruit_NeoPixel_Extention.h"
}
NeoPatterns strip(34, 12, NEO_GRB + NEO_KHZ800);
unsigned long blinklastUpdate =  millis();

void initLedStreifen() {

  strip.begin();
  strip.setBrightness(80);
  strip.show();
  strip.RainbowCycle(1, FORWARD);
}


void rainbowCycleInit(int startTime, int activationInterval, uint8_t ledSpeed, direction dir ) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.RainbowCycle(ledSpeed, dir);
  }
}



void theaterChaseMode(int startTime, int activationInterval, uint32_t color1, uint32_t color2, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.TheaterChase(color1, color2, ledSpeed, dir );
  }

  if (millis() - timer > (startTime + activationInterval))
  {
    strip.clear();
    strip.show();
  }
}

void colorWipeMode(int startTime, int activationInterval, uint32_t color, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.ColorWipe(color, ledSpeed, dir);
  }

  if (millis() - timer > (startTime + activationInterval))
  {
    strip.clear();
    strip.show();
  }
}


void scannerMode(int startTime, int activationInterval, uint32_t color, int ledSpeed) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.Scanner(color, ledSpeed);
  }

  if (millis() - timer > (startTime + activationInterval))
  {
    strip.clear();
    strip.show();
  }
}

void fadeMode(int startTime, int activationInterval, uint32_t color1, uint32_t color2, uint16_t steps, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.Fade(color1, color2, steps, ledSpeed, dir);
  }

  if (millis() - timer > (startTime + activationInterval))
  {
    strip.clear();
    strip.show();
  }
}


void blinkMode(int startTime, int activationInterval, uint32_t color, int blinkInterval )
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.ColorSet(color);
    
    if ((millis() - blinklastUpdate) > blinkInterval)
    {
      blinklastUpdate = millis();
      strip.clear();
      strip.show();
    }
  }
}


void dimmen(int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    
    if (((millis() - blinklastUpdate) > dimSpeed)&&(strip.getBrightness()>=0))
    {
          strip.setBrightness(strip.getBrightness()-1);
          blinklastUpdate = millis();
    }
  }
}



