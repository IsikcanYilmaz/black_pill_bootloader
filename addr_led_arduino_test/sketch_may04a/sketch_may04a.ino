ls#include "FastLED.h"
#define NUM_LEDS (4 * 4)
#define DATA_PIN 5
#define TWO_HUNDRED_PI 628

CRGB leds[NUM_LEDS];

int element = 0;
int last_element = 0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

#define test 1
void loop() {
  #if test
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB(1, 0, 0);
    } 
    FastLED.show();
  #endif
  
  #if NEAT_BREATHE
    static uint8_t j = 0;
    static bool add = true;
    static uint8_t d = 2;
    for(int i=0; i<NUM_LEDS; i++){
      leds[i] = CRGB(255-j, 0, j);
      /*
      FastLED.show();
      delay(10);
      leds[i] = CRGB(0,0,0);
      FastLED.show();
      */
    }
    FastLED.show();
    if (j == 255)
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
    #if 0
    for(int i=0; i<TWO_HUNDRED_PI; i++){
        element = round((NUM_LEDS-1)/2*(sin(i/100.0)+1));
        leds[element].g = 64;
        FastLED.show();
         
        delay(1);
         
        if(element < last_element){
            leds[element].g = 0;
            FastLED.show();
        }
         
        last_element = element;
    }
    #endif
}
