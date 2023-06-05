// #include<SoftwareSerial.h> //Included SoftwareSerial Library
// //Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
// SoftwareSerial s(3,1);

// void setup() {
//   //Serial S Begin at 9600 Baud
//   s.begin(9600);
// }

// void loop() {
//   //Write '123' to Serial
//   s.write(123);
//   delay(1000);
// }

int data; //Initialized variable to store recieved data

void setup() {
  //Serial Begin at 9600 Baud 
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // data = Serial.read(); //Read the serial data and store it
  
  if (Serial.available())
  {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  data = Serial.read();
  Serial.write(data);
  // digitalWrite(LED_BUILTIN, HIGH);
  }
  
  delay(1000);
}

