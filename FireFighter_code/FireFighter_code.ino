#include <WiFiNINA.h>
#include<pins_arduino.h>
#include "dht.h"6

#define AIO_USERNAME    "Adideva"
#define AIO_KEY         "aio_OdOh64Yf6Hm8Sm2dP4VIQDhojdoX"
#define AIO_TEMP_FEED    "temperature"
#define AIO_HUMID_FEED  "humidity"
#define AIO_GAS_FEED    "gas"
#define AIO_WATER_FEED  "water"
#define AIO_FIRE_FEED   "fire"
#define WIFI_SSID       "PhillMax"
#define WIFI_PASS       "daretodream"
#define USE_AIRLIFT     // required for Arduino Uno WiFi R2 board compatability
#include <AdafruitIO_WiFi.h>
AdafruitIO_WiFi aio(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS, SPIWIFI_ACK, SPIWIFI_RESET, NINA_GPIO0, &SPI);
AdafruitIO_Feed *tempFeednew = aio.feed(AIO_TEMP_FEED);
AdafruitIO_Feed *humidFeednew = aio.feed(AIO_HUMID_FEED);
AdafruitIO_Feed *gasfeed=aio.feed(AIO_GAS_FEED);
AdafruitIO_Feed *waterfeed=aio.feed(AIO_WATER_FEED);
AdafruitIO_Feed *firefeed=aio.feed(AIO_FIRE_FEED);
#define dht_apin A0 // Analog Pin sensor is connected to Arduino
dht DHT;
#define POWER_PIN  7
#define SIGNAL_PIN A5
#define MQ2pin A2
#define PUMP_PIN A3

float gas_value=0.0;
int water_value = 0;
int fire_value=0; 
int water_level_flag=0;
float temperature = 0.0;
float humidity = 0.0;
bool is_fire=0;
int smoke_high=0;
bool watering=0;

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
    
    Serial.begin(9600);
    delay(20000);
    pinMode(2, INPUT); //initialize Flame sensor output pin connected pin as input.
    pinMode(LED_BUILTIN, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
    pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(POWER_PIN, LOW);
    lcd.begin(16, 2);
    while(!Serial);  
    Serial.print("Connecting to Adafruit IO");
    aio.connect(); 
    while(aio.status() < AIO_CONNECTED) {
      Serial.print(".");
      delay(1000);  // wait 1 second between checks
   }
    Serial.println();
    Serial.println(aio.statusText());
    

}

int check_smoke(){
  if(gas_value>600){
    smoke_high=1;
    Serial.println("Gas too high. Avoid human intervention");

  }
  else if (200<gas_value<600){
    smoke_high=2;
    Serial.println("Gas manageable, fire nearby");
  }
  else if(gas_value<200){
    smoke_high=3;
    Serial.println("Gas is not a danger");
  }
  else{
    smoke_high=0;
  }
  return smoke_high;
}

int check_water(){
  if(water_value<300){
    //Serial.println("Water level too low, refill water");
    water_level_flag=0;

  }
  else{
    //Serial.println("Water level is okay, you can water");
    water_level_flag=1;
  }
  return water_level_flag;
}
bool start_water(){
  if(is_fire==1 && water_level_flag==1)
  {
    Serial.println("Fire detected, have enough water, initiating watering");
    digitalWrite(PUMP_PIN, HIGH);   // send signal from pin 9 to circuit
    watering=1;
  }
  else if (is_fire==1 && water_level_flag==0)
  {
    Serial.println("Fire detected, but cannot water, paani de do");
    //Add led indicator logic
    digitalWrite(PUMP_PIN,LOW);
    watering=0;
  }
  else{
    Serial.println("No watering required");
    digitalWrite(PUMP_PIN,LOW);
    watering=0;
  }
}
bool check_fire(){
if(fire_value==0 && temperature >= 25 && smoke_high==1){
  is_fire=1;
  Serial.println("Presence of Fire is 100% Confirmed & smoke is too high, humans should not enter");

}
else if(fire_value==0 && temperature < 25 &&  smoke_high==2){
  is_fire=1;
  Serial.println("Presence of Fire Confirmed and Temp of Area increasing and smoke is manageable");
}
else if(fire_value==1 && 15<temperature < 20 && smoke_high==2){
  is_fire=0;
  Serial.println("There could be fire in the proximity,some smoke is detected");
}
else if(fire_value==1 && temperature >= 25 && smoke_high>=2){
  is_fire=0;
  Serial.println("Temperature is high, there is a possibility of fire");
}
else if (fire_value==1 && temperature>=25 && smoke_high==1){
  is_fire=0;
  Serial.println("Fire has been extinguished but area is still dangerous");
}
else if(fire_value==0 && temperature>=25 && smoke_high==3){
  is_fire=1;
  Serial.println("Chemical fire , no smoke but fire present");
}
else if (fire_value==0 && temperature<20 && smoke_high==3){
  is_fire=1;
  Serial.println("Area has just caught fire");
}
else {
  is_fire=0;
  Serial.println("No fire detected");
}
return is_fire;
}
void loop() {
  aio.run();
  lcd.setCursor(0, 0);
  DHT.read11(dht_apin);
  lcd.print("Humidity= ");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temp= ");
  lcd.print(DHT.temperature); 
  delay(1000);
  lcd.clear();
  
  digitalWrite(POWER_PIN, HIGH);  
  delay(10);                      
  water_value = analogRead(SIGNAL_PIN); 
  digitalWrite(POWER_PIN, LOW);   
  delay(1000);
  fire_value=digitalRead(6);
  gas_value = analogRead(MQ2pin);
  
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Smoke Val= ");  
  lcd.print(gas_value);
  lcd.setCursor(0,1);
  lcd.print("Water Val= ");     
  lcd.print(water_value);
  delay(1000); 
  lcd.clear();   

  check_smoke();
  
  check_fire();
  
  check_water();
  
  start_water();

  delay(500);
  lcd.setCursor(0,0);
  if (is_fire==1)
  {
    lcd.print("Fire! Fire! ");    
  }
  else
  {
    lcd.print("No fire. Safe!");
  }

  delay(1000); 
  lcd.clear();
  tempFeednew->save(temperature);  // send potentiometer value to AIO
  humidFeednew->save(humidity);  // send potentiometer value to AIO
  gasfeed->save(gas_value);
  waterfeed->save(water_value);
  firefeed->save(is_fire);
  delay(2000);   
  
}