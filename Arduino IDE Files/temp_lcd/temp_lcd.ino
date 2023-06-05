#include "dht.h"
// include the library code:
#include <LiquidCrystal.h>
#define dht_apin A3 // Analog Pin sensor is connected to Arduino

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
dht DHT;
void setup(){  

  Serial.begin(9600);
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(500);//Wait before accessing Sensor
  lcd.begin(16, 2);
}
void loop(){
    lcd.setCursor(0, 0);
    DHT.read11(dht_apin);
    lcd.print("Humidity= ");
    lcd.print(DHT.humidity);
    //lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp= ");
    lcd.print(DHT.temperature); 
    //lcd.println("C");  
    delay(500);//Wait 5 seconds before accessing sensor again. 
    lcd.clear();

    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);
    Serial.print("% Temperature ");
    Serial.print(DHT.temperature);
    Serial.print(" C "); 
}