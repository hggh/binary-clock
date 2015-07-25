#include <RV8523.h>
#include "DCF77.h"
#include "Time.h"
#include "FastLED.h"
#include <Wire.h><
#include <RV8523.h>
#include <dht.h>
#include <LiquidCrystal.h>

/*
 * define custom struct for handle DHT22 values
 */
struct tmDHT22_t {
  float humidity;
  float temperature;
};
tmDHT22_t readDHT22Sensor();

#define LED_CLOCK_COUNT 26   // all LEDs
#define LED_CLOCK_PIN  9     // digital data pin
#define DHT22_PIN 6          // Digital PIN
#define DCF_PIN 2            // Digital Pin Connection pin to DCF 77 device
#define DCF_INTERRUPT 0      // Interrupt number associated with pin
#define BUTTON_LCD_OFF A3    // click button to on/off display

RV8523 rtc;
DCF77 DCF = DCF77(DCF_PIN,DCF_INTERRUPT);
dht DHT;
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 7);
CRGB clock_leds[LED_CLOCK_COUNT];

/*
 * Read the DHT22 return the struct with floats inside
 */
struct tmDHT22_t readDHT22Sensor() {
  tmDHT22_t data;
  int chk = DHT.read22(DHT22_PIN);
  if (chk != DHTLIB_OK) {
    for (int i = 0; i > 2; i++) {
      delay(60);
      chk = DHT.read22(DHT22_PIN);
      if (chk == DHTLIB_OK) {
        break;
      }
    }
  }
  data.humidity = DHT.humidity;
  data.temperature = DHT.temperature;

  return data;
}

/*
 * Display temperature
 */
void display_values(time_t current_time, tmDHT22_t data) {
  lcd.clear();
  lcd.setCursor(0,0);
  
  
  char buffer[40];
  sprintf(buffer, "%d.%d.%4d - %02d:%02d:%02d",
      day(current_time),
      month(current_time),
      year(current_time),
      hour(current_time),
      minute(current_time),
      second(current_time)
  );
  lcd.print(buffer);
  lcd.setCursor(0,2);
  lcd.print("Temp.: ");
  lcd.setCursor(11,2);
  lcd.print(data.temperature);
  lcd.setCursor(17,2);
  lcd.print("C");

  lcd.setCursor(0,3);
  lcd.print("Humidity: ");
  lcd.setCursor(11,3);
  lcd.print(data.humidity);
  lcd.setCursor(17,3);
  lcd.print("%");
}


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
  for (int i = 0; i < 4; i++) {
    for (int dot = 0; dot < LED_CLOCK_COUNT; dot++) {
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
 * Global variables for Loop
 */
time_t time_old;
tmDHT22_t dht22_current;
time_t dht_last_read = 0;


/*
 * Setup RTC, DCF77, DHT22
 */
void setup() {
  time_t DCFtime = 0;
  Serial.begin(9600);

  lcd.begin(20, 4);
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting the clock...");
  lcd.setCursor(0,1);
  lcd.print("getting radio signal...");
  lcd.setCursor(0,2);
  lcd.print("takes 2-5min");

  FastLED.addLeds<NEOPIXEL, LED_CLOCK_PIN>(clock_leds, LED_CLOCK_COUNT);
  FastLED.setBrightness(30);
  leds_do_looping();
  
  Serial.println("Starting program, fetching now time from DCF77...");

  rtc.set24HourMode();
  rtc.batterySwitchOverOff();

  DCF.Start();
  delay(100);
  dht22_current = readDHT22Sensor();

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

void write_time(int offset_first, int offset_second, int number) {
  // split the numer if the first and second
  int t_first = ((number / 10 ) % 10);
  int t_second = (number % 10 );
  byte bt_first = t_first;
  byte bt_second = t_second;

  for (int i=0; i<4; i++) {
    if (bitRead(bt_first, i) == 1) {
      clock_leds[offset_first + i * 2]       = CRGB::Red;
      clock_leds[offset_first + (i * 2) + 1] = CRGB::Red;
    }
  }

  for (int i=0; i<4; i++) {
    if (bitRead(bt_second, i) == 1) {
      clock_leds[offset_second + i * 2 ]     = CRGB::Red;
      clock_leds[offset_second + i * 2 + 1]  = CRGB::Red;
    }
  }
}

/*
 * Main Loop
 */
void loop() {
   if (analogRead(BUTTON_LCD_OFF) > 600) {
    lcd.display();
   }
   else {
    lcd.noDisplay();
   }
  // check if one second is gone, we need to update the LEDs
  time_t time_current = rtc_get_time();
  if (time_old != time_current) {
    leds_clock_clear();
    time_old = time_current;
    Serial.print("Time: ");
    Serial.println(time_current);

    int seconds = second(time_current);

    write_time( 0,  4, hour(time_current));
    write_time(12, 18, minute(time_current));
    // write LEDs....
    FastLED.show();
    display_values(time_current, dht22_current);

    // Get DHT Values every 60sec
    if ( ((dht_last_read) < time_current - 60) || dht_last_read == 0) {
      dht_last_read = time_current;
      dht22_current = readDHT22Sensor();
      Serial.print("Temp:");
      Serial.println(dht22_current.temperature);
    }
  }
}
