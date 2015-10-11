/*
 * Copyright: 2015 Jonas Genannt <genannt@debian.org>
 * License:   GPLv2
 */
#include <RV8523.h>
#include "DCF77.h"
#include "Time.h"
#include "FastLED.h"
#include <Wire.h><
#include <RV8523.h>

#define LED_CLOCK_COUNT 40   // all LEDs
#define BUTTON_PIN     3     // digital PIN with interrupt
#define BUTTON_MODE_PIN 2     // Analog Button Mode
#define LED_CLOCK_PIN  9     // digital data pin
#define DCF_PIN 2            // Digital Pin Connection pin to DCF 77 device
#define DCF_INTERRUPT 0      // Interrupt number associated with pin

RV8523 rtc;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);
CRGB clock_leds[LED_CLOCK_COUNT];

const CRGB::HTMLColorCode colors[35] = { CRGB::Crimson,CRGB::Red,CRGB::DarkRed,CRGB::DeepPink,CRGB::OrangeRed,CRGB::Orange,CRGB::Gold,CRGB::Yellow,CRGB::Violet,CRGB::Navy,CRGB::RosyBrown,
CRGB::Fuchsia,CRGB::MediumOrchid,CRGB::BlueViolet,CRGB::Purple,CRGB::Indigo,CRGB::DarkSlateBlue,CRGB::GreenYellow,CRGB::Lime,CRGB::MediumSpringGreen,CRGB::ForestGreen,CRGB::DarkGreen,
CRGB::LightSeaGreen,CRGB::Teal,CRGB::Cyan,CRGB::CadetBlue,CRGB::SteelBlue,CRGB::SkyBlue,CRGB::Blue,CRGB::DarkBlue,CRGB::Chocolate,CRGB::SaddleBrown,CRGB::Maroon,CRGB::DarkSlateGray,CRGB::Brown };

/*
 * Global variables for Loop
 */
time_t time_old;
uint8_t led_brightness = 255;
uint8_t led_color_brightness = 80;
uint8_t led_color_index = 0;
boolean buttonMode = 0;
CRGB::HTMLColorCode led_color = CRGB::DarkBlue;

/*
 * Set the RTC timer from the time_t struct
 */
void rtc_set_time(time_t time_c) {
  rtc.stop();
  rtc.set(
    second(time_c),
    minute(time_c),
    hour(time_c),
    day(time_c),
    month(time_c),
    year(time_c)
  );
  rtc.start();
}

/*
 * Get the RTC time and return time_t
 */
time_t rtc_get_time() {
  tmElements_t tm;
  uint8_t sec, min, hour, day, month;
  uint16_t year;

  rtc.get(&sec, &min, &hour, &day, &month, &year);

  tm.Second = sec;
  tm.Minute = min;
  tm.Hour = hour;
  tm.Day = day;
  tm.Month = month;
  tm.Year = (year - 1970);
  return makeTime(tm);
}

/*
 * Clear LEDs
 */
void leds_clock_clear() {
  for (int dot = 0; dot < LED_CLOCK_COUNT; dot++) {
    clock_leds[dot] = CRGB::Black;
  }
}

/*
 * some nice loopings
 */
void leds_do_looping() {
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t dot = 0; dot < LED_CLOCK_COUNT; dot++) {
      clock_leds[dot] = CRGB::Blue;
      FastLED.show();
      clock_leds[dot] = CRGB::Red;
      delay(10);
    }
    leds_clock_clear();
    FastLED.show();
  }
}


/*
 * Setup RTC, DCF77
 */
void setup() {
  time_t DCFtime = 0;
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(A2, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), action_button, RISING);

  FastLED.addLeds<NEOPIXEL, LED_CLOCK_PIN>(clock_leds, LED_CLOCK_COUNT);
  FastLED.setBrightness(led_brightness);
  leds_do_looping();
  
  Serial.println("Starting program, fetching now time from DCF77...");

  rtc.set24HourMode();
  rtc.batterySwitchOverOff();

  DCF.Start();
  delay(100);

  while(DCFtime == 0) {
    Serial.println("Waiting for Time...");
    delay(200);
    DCFtime = DCF.getTime();
  }
  rtc_set_time(DCFtime);
  leds_do_looping();

  /*
  rtc.set(
    0,
    33,
    16,
    5,
    7,
    2014
  );
  rtc.start();
  */
}

void action_button() {
  if (buttonMode == 1) {
    led_color_brightness+=5;
    Serial.print("Light: ");
    Serial.println(led_color_brightness);
  }
  else {
    led_color_index++;
    if (led_color_index >34) {
      led_color_index = 0;
    }
    led_color = colors[led_color_index];
    Serial.print("Color: ");
    Serial.println(led_color);
  }
}

void write_time(uint8_t offset_first, uint8_t offset_second, uint8_t number) {
  // split the numer if the first and second
  uint8_t t_first = ((number / 10 ) % 10);
  uint8_t t_second = (number % 10 );
  byte bt_first = t_first;
  byte bt_second = t_second;

  for (uint8_t i=0; i<4; i++) {
    if (bitRead(bt_first, i) == 1) {
      clock_leds[offset_first + i * 2]       = led_color;
      clock_leds[offset_first + (i * 2) + 1] = led_color;
      clock_leds[offset_first + i * 2].nscale8(led_color_brightness);
      clock_leds[offset_first + (i * 2) + 1].nscale8(led_color_brightness);
    }
    else {
      clock_leds[offset_first + i * 2]       = CRGB::Grey;
      clock_leds[offset_first + (i * 2) + 1] = CRGB::Grey;
      clock_leds[offset_first + i * 2].nscale8(20);
      clock_leds[offset_first + (i * 2) + 1].nscale8(20);
    }
  }

  for (uint8_t i=0; i<4; i++) {
    if (bitRead(bt_second, i) == 1) {
      clock_leds[offset_second + i * 2 ]     = led_color;
      clock_leds[offset_second + i * 2 + 1]  = led_color;
      clock_leds[offset_second + i * 2 ].nscale8(led_color_brightness);
      clock_leds[offset_second + i * 2 + 1].nscale8(led_color_brightness);
    }
    else {
      clock_leds[offset_second + i * 2 ]     = CRGB::Grey;
      clock_leds[offset_second + i * 2 + 1]  = CRGB::Grey;
      clock_leds[offset_second + i * 2 ].nscale8(20);
      clock_leds[offset_second + i * 2 + 1].nscale8(20);
    }
  }
}

/*
 * Main Loop
 */
void loop() {
  // check if one second is gone, we need to update the LEDs
  time_t time_current = rtc_get_time();
  if (time_old != time_current) {
    int a_button = analogRead(BUTTON_MODE_PIN);
    if (a_button > 10) {
      buttonMode = 1;
    }
    else {
      buttonMode = 0;
    }
    leds_clock_clear();
    time_old = time_current;
    Serial.print("Time: ");
    Serial.println(time_current);

    write_time( 0,  4, hour(time_current));
    write_time(12, 18, minute(time_current));
    write_time(26, 32, second(time_current));
    // write LEDs....
    FastLED.show();
  }
}
