#define MQ2pin 2

float sensorValue;  //variable to store sensor value

void setup() {
	Serial.begin(9600); // sets the serial port to 9600
	Serial.println("MQ2 warming up!");
	delay(20000); // allow the MQ2 to warm up
}

void loop() {
	sensorValue = analogRead(MQ2pin); // read analog input pin 0

	Serial.print("Sensor Value: ");
	Serial.println(sensorValue);
	
	delay(2000); // wait 2s for next reading

  if(sensorValue > 700)
  {
    Serial.println("Smoke content is high. Only FireFighter vehicle can enter. Humans should not enter. ");
  }
  else{
    Serial.println("Smoke content is not high. ");
  }
}