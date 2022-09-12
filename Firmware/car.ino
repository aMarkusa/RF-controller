#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"0000A", "0000B"};
const int led = 6;
const int ledgnd = A1;
// alla värden för motorerna
int ena = 3;
int enb = 2;
int ENA = 44;
int ENB = 45;
int diff = 70;
int in1 = 5; // Bake vänster
int in2 = 4;
int in3 = 10; // Bake höger
int in4 = 11;
int IN1 = 42; // Framme höger
int IN2 = 40;
int IN3 = 41; // Framme vänster
int IN4 = 43;
int x;
int y;
int values[3];
int motorValuea = 0;
int motorValueb = 0;
int motorValueA = 0;
int motorValueB = 0;
unsigned long startMillis; 
unsigned long currentMillis;
const unsigned long period = 5; // perioden för rc-kommunikationen
bool ackn = false;

// alla värden för velocity()
int sensor = 32;
int steps = 0;
int steps_old = 0;
float temp = 0;
float rps = 0;
float v = 0;
const unsigned long v_period = 10; // perioden för hastightesmätningen
unsigned long v_startMillis; // velocity_startMillis
unsigned long endMillis; // velocity_currentMillis

// alla värden för obstacle
int trigPin1 = 19;
int trigPin2 = 20;
int echoPin1 = 21;
int echoPin2 = 22;

void setup(){
  pinMode(ledgnd, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(18, HIGH);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001 = kontrollerns adress
  radio.openReadingPipe(1, addresses[1]); // 00002 = "vår adress"
  radio.setPALevel(RF24_PA_HIGH);
  // motorernas pins
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  startMillis = millis();
}

float obstacle1(){
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  int duration1 = pulseIn(echoPin1, HIGH);
  int distance1 = duration1 * 0.034 / 2;
  if (distance1 <= 0)
    distance1 = 12;
  if (distance1 <= 15)
    return 1;
  else
    return 0;
}


float obstacle2(){
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  int duration2 = pulseIn(echoPin2, HIGH);
  int distance2 = duration2 * 0.034 / 2;
  if (distance2 < 0)
    distance2 = 12; 
  else if (distance2 <= 15)
    return 1;
  else
    return 0;
}

float velocity(){
  v_startMillis = millis();
  endMillis = v_startMillis + 75;
  while (millis() < endMillis){
    if (digitalRead(sensor)){
      steps = steps + 1;
      while (digitalRead(sensor));
    }
  }
  temp = steps - steps_old;
  steps_old = steps;
  rps = temp / 2;
  return 3.14159265358979323846264338328*0.065*rps*13.3333;
  
}

void forward(int y_value){
  y = map(y_value, 615, 1023, 0, 175); // mappar om 10-bit värdena till 8-bit pwm 
  motorValuea = y;
  motorValueB = y;
  motorValueb = y - diff;
  motorValueA = y - diff;
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverse(int y_value){
  y = map(y_value, 605, 0, 0, 175);
  motorValuea = y;
  motorValueB = y;
  motorValueb = y - diff;
  motorValueA = y - diff;
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left(int x_value){
  x = map(x_value, 594, 1023, 0, 175 );
  motorValueb = motorValueb + x;
  motorValuea = motorValuea - x;
  motorValueA = motorValueA + x;
  motorValueb = motorValueB - x;
  
  if(motorValuea < 0){
    motorValuea = 0;
    motorValueB = 0;
  }
  if(motorValueb >= 175){
    motorValueb = 175;
    motorValueA = 175;
  }
}

void right(int x_value){
  x = map(x_value, 586, 0, 0, 175);
  motorValueb = motorValueb - x;
  motorValuea = motorValuea + x;
  motorValueA = motorValueA - x;
  motorValueB = motorValueB + x;
  
  if(motorValueb < 0){
    motorValueb = 0;
    motorValueA = 0;
  }
  if(motorValuea >= 175){
    motorValuea = 175;
    motorValueB = 175;
  }
}

void halt(){
  analogWrite(ena, 0);
  analogWrite(enb, 0);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  
}

void loop(){ 
  digitalWrite(ledgnd, LOW);
  delay(period);
  radio.startListening(); // lyssnar
  if (radio.available())
    radio.read(&values, sizeof(values)); //läser all data som kommer in
  if (values[2] == 1)
    analogWrite(led, 200);
  else
    analogWrite(led, 0);
    
  // beroende på värdena så backa/kör fram eller sväng
  if (values[0] > 614)
    forward(values[0]);
    
  else if (values[0] < 606)
    reverse(values[0]);

  else{
    motorValuea = 0;
    motorValueb = 0;  
    motorValueA = 0;
    motorValueB = 0;
  }
 
  if (values[1] > 594)
    left(values[1]);
    
  else if (values[1] < 586)
    right(values[1]);


  analogWrite(ena, motorValuea);
  analogWrite(enb, motorValueb);
  analogWrite(ENA, motorValueA);
  analogWrite(ENB, motorValueB);
  float obs1 = obstacle1();
  float obs2 = obstacle2();
  if (motorValuea != 0 && motorValueb != 0)
    v = velocity();
  else
   v = 0;
    
  delay(period);
  radio.stopListening(); // sluta lyssna
  float data[3];
  data[0] = v; // skicka hastighet(m/s * 3.6 = km/h)
  data[1] = obs1; // data från sensorerna
  data[2] = obs2;
  radio.write(&data, sizeof(data)); // skicka till kontrollern
  if (data[1] == 1){
    reverse(0);
    analogWrite(ena, 100);
    analogWrite(enb, 80);
    analogWrite(ENA, 80);
    analogWrite(ENB, 100);
    delay(1000);
    data[1] = 0;
  }
   else if (data[2] == 1){
    forward(0);
    analogWrite(ena, 100);
    analogWrite(enb, 80);
    analogWrite(ENA, 80);
    analogWrite(ENB, 100);
    delay(1000);
    data[2] = 0;
  }
}
