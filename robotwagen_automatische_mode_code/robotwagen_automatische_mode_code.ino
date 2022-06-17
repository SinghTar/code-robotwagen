#include <Wire.h>
byte IR_data[8] = {};
int i = 0;
//motor pin
int motor_left[] = {18, 5};
int motor_right[] = {2, 15};

//byte to dec
int myVariable = 0;
//Afstand sensor
int echo = 23;
int trig = 19;
long duration;
int distance;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200); // start serial for output
  // Setup motors
  int i;
  for (i = 0; i < 2; i++) {
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  Wire.requestFrom(8, 8);                  
  //Serial.println("IR data");
  while (Wire.available()) {              
    byte c = Wire.read();                
    //Serial.print(c);             
    IR_data[i] = c;
    i++;
  }
  i = 0;
  for (byte i = 0; i < 8; i = i + 1) {
    Serial.print(IR_data[i]);
  }
  Serial.println("");
  //Binar naar decimaal omzetten
  myVariable = binNaarDec(IR_data);
  Serial.println(myVariable);
  Serial.println("");
  //Afstand van de ultrasoon sensor bepallen
  int d = afstand();
  Serial.print("Afstand: ");
  Serial.println(d);
  //Als afstand klieneer dan 4cm moet de auto stoppen
  if (d < 4) {
    motor_stop();
  }
  //Als afstand kleiner is dan 10 cm moet de auto bij kruisput links of recht gaan
  else if (d < 10) {
    Serial.println("in liks of r");
    //als de IR sensor volledig op de pad is gaat die gewoon recht door
    if (myVariable == 255) {
      Serial.println("RechtDoor af < 10");
      //motor_stop();
      drive_forward(90);
    }
    //Gaat naar linksdraaien
    else if (myVariable >= 48) {
      Serial.println("LINKS af < 10");
      //motor_stop();
      if (myVariable > 240 && myVariable < 250) {
        Serial.println("LINKS af < 10");
        //motor_stop();
        turn_left(255);
        delay(150);
      }
      else {
        turn_left(90);
      }
    }
    //Gaat rechtdoor
    else if (myVariable < 48 && myVariable > 15) {
      //48 voor eerste links
      //15 voor recht
      Serial.println("recht door af < 10");
      //Gaat naar rechtdraaien
      if (myVariable > 25 && myVariable < 35) {
        Serial.println("RECHTS af < 10");
        //motor_stop();
        turn_right(255);
        delay(150);
        //turn_right(255);
      }
      else {
        drive_forward(90);
      }
    }
    //Gaat naar rechtdraaien
    else if (myVariable <= 15) {
      Serial.println("RECHTS af < 10");
      //motor_stop();
      turn_right(90);
    }
    else {
      motor_stop();
    }
  }
  else {
    Serial.println("in normaal");
    if (myVariable == 255) {
      Serial.println("RechtDoor");
      //motor_stop();
      drive_forward(180);
    }
    //Gaat naar linksdraaien
    else if (myVariable >= 48) {
      Serial.println("LINKS");
      //motor_stop();
      turn_left(100);
    }
    //Gaat rechtdoor
    else if (myVariable < 48 && myVariable > 15) {
      //48 voor eerste links
      //15 voor recht
      Serial.println("recht door");
      drive_forward(180);
    }
    //Gaat naar rechtssdraaien
    else if (myVariable <= 15) {
      Serial.println("RECHTS");
      //motor_stop();
      turn_right(100);
    }
    else {
      motor_stop();
    }
  }
  delay(150);
}

//Gaat 8 bit naar decimaal omzetten
unsigned int binNaarDec(byte s[])
{
  int n = 0;
  int i;
  for (i = 0; i < 8; ++i) {
    n <<= 1;
    n += s[i] - 0;
  }
  return n;
}

//motor gaat stoppen
void motor_stop() {
  analogWrite(motor_left[0], LOW);
  analogWrite(motor_left[1], LOW);
  analogWrite(motor_right[0], LOW);
  analogWrite(motor_right[1], LOW);
  delay(25);
}

//wagen gaat achteruit
void drive_backward(int v) {
  analogWrite(motor_left[0], LOW);
  analogWrite(motor_left[1], v);
  analogWrite(motor_right[0], LOW);
  analogWrite(motor_right[1], v);
}

//wagen gaat vooruit
void drive_forward(int v) {
  analogWrite(motor_left[0], v);
  analogWrite(motor_left[1], LOW);
  analogWrite(motor_right[0], v);
  analogWrite(motor_right[1], LOW);
}

//wagen gaat rechts afslaan
void turn_right(int v) {
  analogWrite(motor_left[0], LOW);
  analogWrite(motor_left[1], v);
  analogWrite(motor_right[0], v);
  analogWrite(motor_right[1], LOW);
}

//wagen gaat linksafslaan
void turn_left(int v) {
  analogWrite(motor_left[0], v);
  analogWrite(motor_left[1], LOW);
  analogWrite(motor_right[0], LOW);
  analogWrite(motor_right[1], v);
}

//Gaat de afstand van de Ultrasoon sensor bepallen.
int afstand() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2; 
  return distance;
}
