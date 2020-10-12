// Leds
#define FASTLED_ALLOW_INTERRUPTS 1
#include "FastLED.h"
#define LED_TOTAL_CNT 57
#define LED_FIRST_CNT 23
#define BRIGHTNESS 255
#define LED_PIN 5 
CRGB leds[LED_TOTAL_CNT];

// Mode and Speed
#define MODE_CNT 6
#define SPEED_CNT 3
#define SETTING_CNT 3

#define RAINBOW_BEGIN 0
#define RAINBOW_END 0

#define SOLID_BEGIN 1
#define SOLID_END 4

#define OFF_BEGIN 5
#define OFF_END 5

#define REGIONS 2
byte region_range[2][2] = {{0, LED_FIRST_CNT}, {LED_FIRST_CNT, LED_TOTAL_CNT}};
byte mode_indx[REGIONS] = {0, 0};
byte speed_indx[REGIONS] = {0, 0};
byte setting_state = 0;

#include "GyverButton.h"
GButton butt1(3);

// Colors
double update_speed[3] = {0.008, 0.024, 0.1};
int solid_color[4] = {HUE_RED, HUE_GREEN, HUE_BLUE, HUE_PURPLE};

void blynk(){
  CRGB leds_aux[LED_TOTAL_CNT];
  for(int i = 0; i < LED_TOTAL_CNT; i++) leds_aux[i] = leds[i];
  int from = 0, to = LED_TOTAL_CNT;
  if(setting_state != 0){
    from = region_range[setting_state-1][0];
    to = region_range[setting_state-1][1];
  }
  for(int j = 0 ; j < 3; j++){
    for(int i = from; i < to; i++) leds[i] = 0;
    disp();
    delay(200);
    for(int i = from; i < to; i++) leds[i] = CHSV(0, 255, BRIGHTNESS);
    disp();
    delay(200);
  }
  for(int i = from; i < to; i++) leds[i] = leds_aux[i];
    disp();

    
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, LED_TOTAL_CNT).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  butt1.setTimeout(300);
}

void update_setting(byte *setting_arr, int settings_cnt){
  if(setting_state == 0) {
    if (++setting_arr[0] >= settings_cnt) setting_arr[0] = 0;
    if (++setting_arr[1] >= settings_cnt) setting_arr[1] = 0;
  }else{
    if (++setting_arr[setting_state - 1] >= settings_cnt) setting_arr[setting_state - 1] = 0;
  }
}

void buttonTick() {
  butt1.tick();
  if (butt1.isSingle())
    update_setting(mode_indx, MODE_CNT);
  if (butt1.isDouble())
    update_setting(speed_indx, SPEED_CNT);
  if (butt1.isTriple()){
    if (++setting_state >= SETTING_CNT) setting_state = 0;
    blynk();
  }
}

void loop() {
  buttonTick();
  apply_mode_indx();
  disp();
}

void disp() {
  FastLED.show();
}

void fill_color(int val, int from, int to, int brightness){
  for (int i = from; i < to; i++)
    leds[i] = CHSV(val, 255, brightness);
}

void apply_mode_indx() {
  for(int i = 0; i < REGIONS; i++){
    if (mode_indx[i] >= RAINBOW_BEGIN && mode_indx[i] <= RAINBOW_END) {
      fill_color(update_speed[speed_indx[i]]*millis(), region_range[i][0], region_range[i][1], BRIGHTNESS);
      continue;
    }
    
    if (mode_indx[i] >= SOLID_BEGIN && mode_indx[i] <= SOLID_END) {
      fill_color(solid_color[mode_indx[i] - SOLID_BEGIN], region_range[i][0], region_range[i][1], BRIGHTNESS);
      continue;
    }
    
    if (mode_indx[i] >= OFF_BEGIN && mode_indx[i] <= OFF_END) {
      fill_color(0, region_range[i][0], region_range[i][1], 0);
      continue;
    }
    
  }
  
  
}
