#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include "U8glib.h"

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE); // no fucking idea

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"0000A", "0000B"};
int y = A0; // y-axelns pin
int x = A1; // x-axelns pin
const int period = 5;
const int led = 6;

void setup() {
  Serial.begin(9600);
  pinMode(y, INPUT);
  pinMode(x, INPUT);
  pinMode(6, INPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_HIGH);
}

void draw(float Speed, int obs1, int obs2) {  
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 20); 
  
  if(obs1 == 1) // om obstacle = 1 -> hinder framför
    u8g.print("Obstacle ahead!");

  else if(obs2 == 1) // om obstacle = 2 -> hinder bakom
    u8g.print("Obstacle behind!");
  
  else{ // om obstacle = 0 -> inget hinder
    u8g.print("Speed: ");
    u8g.print(Speed);
    u8g.print(" km/h");
  }
}

void loop() {
  delay(period);
  radio.stopListening();
  int values[3];
  values[0] = analogRead(x);
  values[1] = analogRead(y);
  values[2] = digitalRead(led);
  radio.write(&values, sizeof(values)); // skicka joystickens data till bilen

  delay(period);
  radio.startListening();
  float data[3];
  while (!radio.available());
  radio.read(&data, sizeof(data));
  u8g.firstPage();
  do {
  draw(data[0], data[1], data[2]);
  } while( u8g.nextPage() );

    
}
  
