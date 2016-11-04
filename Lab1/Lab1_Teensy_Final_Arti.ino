#define RLED 12
#define YLED 16
#define GLED 15
//0 RX, 1 TX

struct Data
{
  uint8_t c[20]; 
  uint16_t len = 0;
};

char array[3] = {'R', 'Y', 'G'}; 
Data packet;

char s[sizeof(Data)];

// Adds one random character to the existing sequence
void genChar(Data& packet)
{
  int randNum = random(0,3);
  packet.c[packet.len] =  array[randNum]; 
  packet.len ++; //increase length per round
}

void reset(Data& packet)
{
  packet.len = 0;
}

void setup() {
  
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  Serial.begin(9600); 
  randomSeed(analogRead(0));
  Serial1.begin(9600); //write to Arduino Pro
  //Serial2.begin(9600);
  genChar(packet);  
}

void loop() {
  // 1. Send bytes
  //memcpy(s, &packet, sizeof(Data)); //not necessary, below line was modified
  Serial1.write((uint8_t*)&packet, sizeof(Data)); //or cast as char 
  Serial.println(sizeof(Data));
  Serial.println((char*)packet.c);

  // 2. Flash each corresponding LED to Player 1
  for(int k = 0 ; k < packet.len; k++)
  {
    Serial.println("In loop");
    char color = packet.c[k]; 
    if (color == 'R')
    {
      digitalWrite(RLED, HIGH);
      delay(500);
      digitalWrite(RLED, LOW);
      delay(100);
     }
     else if (color == 'Y')
     {
      digitalWrite(YLED, HIGH);
      delay(500);
      digitalWrite(YLED, LOW);
      delay(100);
      }
     else if (color == 'G')
     {
      digitalWrite(GLED, HIGH);
      delay(500);
      digitalWrite(GLED, LOW);
      delay(100);
      }
    }

    Serial.println("Pass color flashing");
    // Goes into while loop to wait for Arduino to respond
    char res; 
    
    while(true)
{
    
    res = Serial1.read();
    //Serial.println("In While loop");
    Serial.println(res);
    if (res == 'C') //correct
   {
      digitalWrite(GLED, HIGH);
      delay(5000);
      digitalWrite(GLED, LOW);
      delay(500);
      genChar(packet);  
      break;
   }
     
    if (res == 'W') //lose
    {
     digitalWrite(RLED, HIGH);
     delay(5000);
     digitalWrite(RLED, LOW);
     delay(500);
     reset(packet); 
     //packet.len = 0;
     //genChar(packet); //restart
     break;
    }
}
}      
