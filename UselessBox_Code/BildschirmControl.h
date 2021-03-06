
#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_ILI9340.h" // Hardware-specific library
#include <SPI.h>


#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.
#define TFT_RST 49
#define TFT_DC 46
#define TFT_CS 53
#define SD_CS 48
#define BL 2
Adafruit_ILI9340 tft = Adafruit_ILI9340(TFT_CS, TFT_DC, TFT_RST);
unsigned long dimLastUpdate =  millis();
int brightness;
int counterFade;

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
uint16_t read16(File & f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File & f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void bmpDraw(char *filename, uint16_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.Color565(r, g, b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println("BMP format not recognized.");
}


void initDisplay() {
  // Serial.begin(9600);

  pinMode(SD_CS, OUTPUT);
  pinMode(BL, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(BL, LOW);
  tft.begin();
  tft.setRotation(45);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");
}

void starPicture(int startTime, int activationInterval, char *picture)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    tft.fillScreen(ILI9340_WHITE);
    bmpDraw(picture, 40, 0);
  }
}

void setPicture(int startTime, int activationInterval, char *picture, int x, bool fillScreen)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    if (fillScreen == true)
    {
      tft.fillScreen(ILI9340_WHITE);
    }
    
    digitalWrite(BL, LOW);
    bmpDraw(picture, x, 0);
    digitalWrite(BL, HIGH);
    fillScreen == false;
  }
}

void sunSetPicture(int startTime, int activationInterval)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval))) {

    digitalWrite(BL, LOW);
    bmpDraw("sunSet.bmp", 0, 0);

  }
}

void countDown (int startTime, int activationInterval, int ziffer, uint16_t color)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {
    tft.setTextColor(ILI9340_BLACK);
    tft.setTextSize(28);

    digitalWrite(BL, LOW);
    tft.fillScreen(color);
    tft.setCursor(100, 25);
    tft.println(ziffer);
    digitalWrite(BL, HIGH);
  }
}

void goText (int startTime, int activationInterval)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {
    tft.setTextColor(ILI9340_BLACK);
    digitalWrite(BL, LOW);
    tft.fillScreen(ILI9340_GREEN);
    tft.setCursor(25, 80);
    tft.setTextSize(12);
    tft.println("*GO*");
    digitalWrite(BL, HIGH);
  }
}

void LightIntensifyingDisplay (int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {

    if (millis() - timer <= (startTime + 50))
    {
      brightness = 0;
      counterFade = 0;
    }

    if ((counterFade < 255))
    {
      brightness = brightness + dimSpeed;
      counterFade = counterFade + dimSpeed;
      analogWrite(BL, brightness);
      dimLastUpdate = millis();
    }

    if (counterFade >= 255)
    {
      digitalWrite(BL, HIGH);
    }
  }
}

void LightFadingDisplay (int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {

    if (millis() - timer <= startTime + 50)
    {
      brightness = 255;
      counterFade = 0;
    }

    if ((counterFade > 0))
    {
      brightness = brightness - dimSpeed;
      counterFade = counterFade - dimSpeed;
      analogWrite(BL, brightness);
      Serial.print("set Brightness: ");
      Serial.println(brightness);
      dimLastUpdate = millis();
    }

    if (counterFade <= 0)
    {
      digitalWrite(BL, LOW);
    }
  }
}

void fadeDisplay (int startTime, int activationInterval, int dimSpeed)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {

    if (millis() - timer <= (startTime + 50))
    {
      brightness = 0;
      counterFade = 0;
    }

    if ((counterFade < 255))
    {
      counterFade = counterFade + dimSpeed;
      brightness = brightness + dimSpeed;
      analogWrite(BL, brightness);
      dimLastUpdate = millis();
    }

    if ((counterFade >= 255) && (counterFade <= 320))
    {
      digitalWrite(BL, HIGH);
      counterFade++;
    }

    if ((counterFade > 320))
    {
      brightness = brightness - dimSpeed;
      analogWrite(BL, brightness);
      dimLastUpdate = millis();

      if ((brightness == 0))
      {
        counterFade = 0;
      }
    }
  }
}

void inverseDisplay(int startTime, int activationInterval, bool state)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {
    tft.invertDisplay(state);
  }
}

void textTheEndDisplay (int startTime, int activationInterval)
{
  if ((millis() - timer > startTime) && (millis() - timer < (startTime + activationInterval)))
  {
    digitalWrite(BL, LOW);
    tft.fillScreen(ILI9340_BLACK);
    tft.setCursor(80, 40);
    tft.setTextColor(ILI9340_WHITE);
    tft.setTextSize(10);

    tft.println("THE");
    tft.setCursor(70, 150);
    tft.println("END!");
    //digitalWrite(BL, HIGH);
    analogWrite(BL, 255);
  }
}
