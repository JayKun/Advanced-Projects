/* #define ButtonRS 10
#define ButtonYS 11
#define ButtonGS 12 */
#define ButtonG  9
#define ButtonY  8
#define ButtonR  7

#include <SoftwareSerial.h>
#include<stdio.h>

//init obj with length and array
struct Data
{
  uint8_t c[20]; 
  uint16_t len = 0; 
};

Data packetReceived; //send yes or no back to teensy
  
void setup() {
  // put your setup code here, to run once:

  /* pinMode(ButtonRS, OUTPUT);
  digitalWrite(ButtonRS, HIGH);
  
  pinMode(ButtonYS, OUTPUT);
  digitalWrite(ButtonYS, HIGH);

  pinMode(ButtonGS, OUTPUT);
  digitalWrite(ButtonGS, HIGH); */

  pinMode(ButtonR, INPUT);
  pinMode(ButtonY, INPUT);
  pinMode(ButtonG, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available())
{
  
  // something is received
  Serial.readBytes((char*) &packetReceived, sizeof(Data));
  //memcpy(&packetReceived, r, sizeof(Data)); //below line is better approach?
  //Serial.println((char *)packetReceived.c);
  //packetReceived = (Data)r; //this won't work***HELP  

  Serial.println(packetReceived.len);
  
  for(int i = 0; i < packetReceived.len; i++)
  {
    char lightColor = packetReceived.c[i]; 
    char buttonColor; 
    int ButtonRIn;
    int ButtonYIn;
    int ButtonGIn;
  
    ButtonRIn = digitalRead(ButtonR);
    //delay(300); 
    ButtonYIn = digitalRead(ButtonY);
    //delay(300); 
    ButtonGIn = digitalRead(ButtonG);
    //delay(300); 
      
      while(ButtonRIn == LOW && ButtonYIn == LOW && ButtonGIn == LOW)
      {
        ButtonRIn = digitalRead(ButtonR);
        //delay(100); 
        ButtonYIn = digitalRead(ButtonY);
        //delay(100); 
        ButtonGIn = digitalRead(ButtonG);
        //delay(100);
      }

      while(digitalRead(ButtonR)== HIGH || digitalRead(ButtonY) == HIGH || digitalRead(ButtonG) == HIGH)
      {;}
      if ((ButtonRIn == HIGH && lightColor == 'R') || 
          (ButtonYIn == HIGH && lightColor == 'Y') ||
          (ButtonGIn == HIGH && lightColor == 'G'))
      {
        continue;
      }
      else
      {
        Serial.write('W');
      }   
  }    
  Serial.write('C'); 
}

}

