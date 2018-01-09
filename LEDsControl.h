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
  strip.Color1 = strip.Color(255, 0, 0);
  strip.ActivePattern = NONE;

}


void rainbowCycleInit(int startTime, int activationInterval, uint8_t ledSpeed, direction dir ) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.RainbowCycle(ledSpeed, dir);
  }
}



void theaterChaseInit(int startTime, int activationInterval, uint32_t color1, uint32_t color2, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.TheaterChase(color1, color2, ledSpeed, dir );
    //theaterChase(strip.Color(127, 127, 127), 80); // White
  }
}


void colorWipeMode(int startTime, int activationInterval, uint32_t color, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {

    strip.ColorWipe(color, ledSpeed, dir);
  }
}


void scannerMode(int startTime, int activationInterval, uint32_t color, int ledSpeed) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.Scanner(color, ledSpeed);
  }
}

void fadeMode(int startTime, int activationInterval, uint32_t color1, uint32_t color2, uint16_t steps, int ledSpeed, direction dir) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.Fade(color1, color2, steps, ledSpeed, dir);
  }
}

void NoneMode(int startTime, int activationInterval) {

  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    strip.ActivePattern = NONE;
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


void dimmenLow(int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    if (((millis() - blinklastUpdate) > dimSpeed) && (strip.getBrightness() > 0))
    {
      strip.setBrightness(strip.getBrightness() - 1);
      blinklastUpdate = millis();
    }
  }
}

void dimmenHigh(int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    if (((millis() - blinklastUpdate) > dimSpeed) && (strip.getBrightness() < 80))
    {
      strip.setBrightness(strip.getBrightness() + 1);
      blinklastUpdate = millis();
    }
  }
}

void brightnessControl (int startTime, int activationInterval, int brightness )
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {
    
    strip.setBrightness(brightness);
  }
}

