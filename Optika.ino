#include <AnalogKey.h>
#include <SoftwareSerial.h>
#include "FastLED.h"


#define LED_PIN 2
#define NUM_LEDS 4
#define But_PIN A6
#define ADC_PIN A2



CRGB leds[NUM_LEDS];
CRGB leds_buf[NUM_LEDS];

byte counter;


int val = 0; //значение потенциометра
int valADC = 0; //значение потенциометра

bool bSwitch = 0;
bool bOnOff = 0;

void setup() {

  Serial.begin (9600);


  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);

  //Входные пины

  pinMode(But_PIN, INPUT);
  pinMode(ADC_PIN, INPUT);

  for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети

    leds[i] = CHSV(map(analogRead(A0), 0, 1023, 0, 255), 255, 255); // HSV. Увеличивать HUE (цвет)

  }

}

void loop() {
  //Быдлокод для кнопки
  static int cButt = 0;
  val = analogRead(But_PIN);
  if (!(val < 600 || val > 800)) {
    cButt++;
  }
  else  {
    cButt = 10;
  }
  static int cPush = 0;
  static int otfPush = 0;
  if (cButt > 30)
  {
    otfPush = 1;
    cPush++;
  }
  else {
    if ((cPush < 30) && otfPush) {
      Serial.println("PUSH");
      otfPush = 0;
      bSwitch = !bSwitch;
    }
    if (!(cPush < 30) && otfPush) {
      Serial.println("Long_PUSH");
      otfPush = 0;
      bOnOff = !bOnOff;
    }
    cPush = 0;
  }
  //Быдлокод для кнопки

  valADC = analogRead(ADC_PIN);
  Serial.println(valADC);
  valADC = map(valADC, 0, 1023, 0, 255);


  for (int i = (int)bSwitch; i < NUM_LEDS; i += 2 ) {       // от 0 до первой трети

    leds[i] = CHSV(valADC, 255, 255);  // HSV. Увеличивать HUE (цвет)

  }
  static bool otfOnOff = 0;
  if (bOnOff)
  {

    for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети

      leds[i] = CHSV(0, 0, 0);  // HSV. Увеличивать HUE (цвет)

    }
    otfOnOff = 1;
  }
  else
  {
    if (otfOnOff)
    {

      for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети

        leds[i] = CHSV(map(analogRead(A0), 0, 1023, 0, 255), 255, 255); // HSV. Увеличивать HUE (цвет)

      }
      otfOnOff = 0;
    }


  }

  Serial.println(bOnOff);

  FastLED.show();
  delay(5);         // скорость движения радуги
}
