// Built on heltec ESP32 kit samples

#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <OneWire.h>
#include <DallasTemperature.h>

#include "config.h"
#include <Adafruit_Sensor.h>
// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *hottubairtemp = io.feed("hottubairtemp");
AdafruitIO_Feed *hottubwatertemp = io.feed("hottubwatertemp");

const int led = 13;
#define ONE_WIRE_BUS 5

//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

#define SDA    4
#define SCL   15
#define RST   16 //RST must be set by software

#define PROG 0

SSD1306  display(0x3c, SDA, SCL, RST);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


float t_c;
float t_f;
float h;
float dp_f;
String tempStr;
String humidityStr;
String dewpointStr;
String tupleString;

void setup() {
  pinMode(PROG,INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  pinMode(RST,OUTPUT);
  delay(50); 

  Serial.begin(115200);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  Serial.println("SHT31 test");
  sensors.begin();

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
 
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.println(io.statusText());
    display.clear();
    display.drawString(0, 0, io.statusText());
    display.display();
    delay(500);
  }
 
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

#define VALUE_MARGIN 50

void drawVals(String temp1, String temp2) {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/

    //Serial.println("Updating display");
    display.clear();
    display.setFont(ArialMT_Plain_24);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "T1:");
    //display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(VALUE_MARGIN, 0, temp1);

    //display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 26, "T2:");
    //display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(VALUE_MARGIN, 26, temp2);

    display.display();
}

bool display_en = true;

unsigned long last_run = 0;
bool time_to_update() {
  bool gt_1s = false;
  if (millis()-last_run > 1000) {
    gt_1s = true;
    last_run = millis();
  }
  return gt_1s;
}

unsigned long last_AIO_post = 0;
bool time_to_AIO_post() {
  bool gt_1m = false;
  if (millis()-last_AIO_post > 60000) {
    gt_1m = true;
    last_AIO_post = millis();
  }
  return gt_1m;
}

void loop() {
  io.run();
  
  if(time_to_update()){
    sensors.requestTemperatures(); // Send the command to get temperatures

    float temp1 = sensors.getTempFByIndex(0);
    float temp2 = sensors.getTempFByIndex(1);
    if(temp1 != DEVICE_DISCONNECTED_F) 
    {
      Serial.print("Temp1: ");
      Serial.println(temp1);
    } 
    else
    {
      Serial.println("Error: Could not read temp1");
    }
    if(temp2 != DEVICE_DISCONNECTED_F) 
    {
      Serial.print("Temp2: ");
      Serial.println(temp2);
    } 
    else
    {
      Serial.println("Error: Could not read temp2");
    }
    if( (temp1 != DEVICE_DISCONNECTED_F) && (temp2 != DEVICE_DISCONNECTED_F)) {
      if (time_to_AIO_post()) {
        hottubairtemp->save(temp1);
        hottubwatertemp->save(temp2);
        Serial.println("Posted to AIO");
      }
    }
    if(digitalRead(PROG) == LOW){
      display_en = !display_en;
    }
    if(display_en){
      drawVals(String(temp1) , String(temp2));
    }else{
      display.clear();
      display.display();
    }
  }
}
