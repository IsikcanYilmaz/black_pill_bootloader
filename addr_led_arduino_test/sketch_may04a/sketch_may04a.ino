#include "FastLED.h"
#define NUM_LEDS 1//(4 * 4)
#define DATA_PIN 5
#define TWO_HUNDRED_PI 628
#define ONBOARD_LED_PIN 13

CRGB leds[NUM_LEDS];

int element = 0;
int last_element = 0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // On board LED
  pinMode(ONBOARD_LED_PIN, OUTPUT);


}

#define test true
//#define NEAT_BREATHE true
void loop() {
  static bool toggle = true;
  static uint16_t toggleCtr = 0;
  digitalWrite(ONBOARD_LED_PIN, (toggleCtr > 0x7fff) ? HIGH : LOW);
  toggleCtr++;
  
  #ifdef test
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB(0x00, 0x01, 0x00);
    } 
    FastLED.show();
    while(1){}

  #endif
  
  #ifdef NEAT_BREATHE

  
    static uint8_t j = 0;
    static bool add = true;
    static uint8_t d = 20;
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB(0, 255-j, j);
      /*
      FastLED.show();
      delay(10);
      leds[i] = CRGB(0,0,0);
      FastLED.show();
      */
    }
    FastLED.show();
    if (j == 100)
    {
      add = false;
    }
    if (j == 0)
    {
      add = true;
      if (d > 0)
        d--;
    }
    delay(d);
    j += (add) ? 1 : -1;
    #endif

}
