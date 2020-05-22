#include "addressable_led_driver.h"

typedef enum {
  NORTH,
  SOUTH,
  WEST,
  EAST,
  CENTER,
  NONE
} Position_e;

// LEDs are arranged in a 1 dimensional array, even though they are physically arranged in a square shape (or else)
// Below structure describes that. we'll see if i keep it...
// I realize a Panel may be in the middle or in some arbitrary location of the LED strip/chain. To take that into account
// the field $stripRange is there. $stripRange[0] - $stripRange[1] denote the beginning and ending LED indices that correspond
// to the beginning and the ending LEDs in the strip
// Example:
//
//              Panel 0            Panel 1             ..
//              ____________       _______________
//        IN-> |0  1  2  3  |  _> |16  17  18  19 |  _> ...
//             |4  5  6  7  | |   |20  21  22  23 | |
//             |8  9  10 11 | |   |24  25  26  27 | |
//             |12 13 14 15 |/    |28  29  30  31 |/
//     
// stripRange      0 - 15           16 - 31
//
// Meaning that the 0th pixel of Panel 1 is actually LED 16 on the strip/chain.
//
// Panels have "Position" and "Neighbors". Below is how the current plan of panel layout looks like:
//
//              [ N ]
//        [ W ] [ C ] [ E ]
//              [ S ]
// 
// This is how the cube of panels will be imagined in 2d. 
// Neighboring panels are obvious; like the neighbors of N are W, C, E. Center panel's neighbors are all other panels, etc. 

typedef struct {
  AddrLEDStrip_t *strip;
  uint16_t numLeds;
  uint16_t stripRange[2]; // Inclusive
  Position_e position;
  struct AddrLEDPanel_t *neighborPanels[4];

} AddrLEDPanel_t;

void AddrLEDManager_Init(void);

void AddrLEDManager_SanityTest(void);
