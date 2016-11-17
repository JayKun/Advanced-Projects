
int sensorPin = A1;
void setup() {
  Serial.begin(9600); 
  pinMode(sensorPin, INPUT);
}

void loop() {

int pinReading = analogRead(sensorPin); 
delay(100);
float voltageReading = pinReading*(3.3/1023); 
Serial.println(voltageReading); 

}
