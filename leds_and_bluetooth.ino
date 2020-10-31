#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LEDS_COUNT      10    // количество светодиодов WS2812
#define LED_PIN         5    // пин для подключения WS2812
#define CHANGE_TIME     30   // интервал смены эффектов, сек
#define EFFECTS_COUNT   3    // количество эффектов

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDS_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
byte effectNumber;         // переменная для хранения номера эффекта
unsigned long timeStamp;   // переменная для хранения времени последней смены эффекта

int val;
boolean repeatVal = false;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
}

void loop()
{

  if (Serial.available() || repeatVal)  {
    if (repeatVal == true)
    {
      repeatVal = false;
    }
    else
    {
      val = Serial.read();
    }
    // Serial.println(val);
    if ( val == '0')  // При символе "0" (ноль) выключаем
    {
      turnOffAllLeds();
    }
    if (val == '1')  // При символе "1" включаем светодиод
    {
      turnOnAllLeds();
    }
    if (val == '2')  // При символе "1" включаем светодиод
    {
      turnOffAllLeds();
      stairway();
    }
    if (val == '3')  // При символе "1" включаем светодиод
    {
      rainbowCycle(30);
      turnOffAllLeds();
    }
  }
}

void turnOffAllLeds() {
  for ( int i = 0; i < LEDS_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void turnOnAllLeds() {
  for ( int i = 0; i < LEDS_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
}


void rainbowCycle(int wait)
{
  int j;
  int i;
  boolean stopLoopFlag = false;
  for (j = 0; j < 255 && stopLoopFlag == false; j++) { // 5 cycles of all colors on wheel

    for (i = 0 ; i < LEDS_COUNT && stopLoopFlag == false; i++)
    {
      pixels.setPixelColor(i, wheel(((255 / 1) + j) & 255));
      pixels.show();
      delay(wait);
    }

    // stop for condition
    if (Serial.available() ) {
      val = Serial.read();
      repeatVal = true;
      stopLoopFlag = true;
    }

  }
}

void stairway()
{
  int i;
  boolean stopLoopFlag = false;
  while (stopLoopFlag == false)
  {
    for (i = 0 ; i < LEDS_COUNT && stopLoopFlag == false; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      pixels.show();
      delay(300);
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();


      // stop for condition
      if (Serial.available() ) {
        val = Serial.read();
        repeatVal = true;
        stopLoopFlag = true;
      }
    }
  }
}

uint32_t wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



