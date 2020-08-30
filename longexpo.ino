// Leds
#define FASTLED_ALLOW_INTERRUPTS 1
#include "FastLED.h"
#define LED_TOTAL_CNT 54
#define BRIGHTNESS 255
#define LED_PIN 5 
CRGB leds[LED_TOTAL_CNT];

// Mode and Speed
#define MODE_CNT 5
#define SPEED_CNT 3

#define RAINBOW_BEGIN 0
#define RAINBOW_END 0

#define SOLID_BEGIN 1
#define SOLID_END 4

byte mode_indx = 0;
byte speed_indx = 0;
#include "GyverButton.h"
GButton butt1(3);

// Colors
double update_speed[3] = {0.008, 0.024, 0.1};
int solid_color[4] = {HUE_RED, HUE_GREEN, HUE_BLUE, HUE_PURPLE};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, LED_TOTAL_CNT).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  butt1.setTimeout(300);
}

void buttonTick() {
  butt1.tick();
  if (butt1.isSingle())
    if (++mode_indx >= MODE_CNT) mode_indx = 0;
  if (butt1.isDouble())
    if (++speed_indx >= SPEED_CNT) speed_indx = 0;
}

void loop() {
  buttonTick();
  apply_mode_indx();
  disp();
}

void disp() {
  FastLED.show();
}

void fill_color(int val){
  for (int i = 0; i < LED_TOTAL_CNT; i++)
    leds[i] = CHSV(val, 255, BRIGHTNESS);
}

void apply_mode_indx() {
  
  if (mode_indx >= RAINBOW_BEGIN && mode_indx <= RAINBOW_END) {
    fill_color(update_speed[speed_indx]*millis());
    return;
  }
  
  if (mode_indx >= SOLID_BEGIN && mode_indx <= SOLID_END) {
    fill_color(solid_color[mode_indx - SOLID_BEGIN]);
    return;
  }
  
}
