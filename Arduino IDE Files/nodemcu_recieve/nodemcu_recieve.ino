#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

void setup() {
  //Serial S Begin at 9600 Baud
  s.begin(9600);
}

void loop() {
  //Write '123' to Serial
  char my_str[] = "Hello";
  s.println(my_str);
  
  delay(1000);
}