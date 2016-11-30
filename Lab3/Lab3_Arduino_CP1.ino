
int sensorPin = A1; //Arduino pin A1

void setup() {
  Serial.begin(9600); 
  pinMode(sensorPin, INPUT);
}

void loop() {

int pinReading = analogRead(sensorPin); 
delay(100);
float voltageReading = pinReading*(3.338/1023); //Vout
float finalVolt = voltageReading*8400/6600; //Vin
Serial.println(finalVolt); 

}
