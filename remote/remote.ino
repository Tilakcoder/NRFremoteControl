//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int Xpin = A0, Ypin = A1, grabPin=7, shootPin=6;
float moveX, moveY;
float xmid, ymid;
float arr[4];

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

  pinMode(Xpin, INPUT);
  pinMode(Ypin, INPUT);
  pinMode(grabPin, INPUT);
  pinMode(shootPin, INPUT);
  delay(10);
  xmid = analogRead(Xpin);
  ymid = analogRead(Ypin);
}
void loop()
{
  //Send message to receiver
  moveX = analogRead(Xpin)-xmid;
  moveX = (moveX>0)?moveX/(1023-xmid):moveX/xmid;
  moveX *= -1;
  moveY = analogRead(Ypin)-ymid;
  moveY = (moveY>0)?moveY/(1023-ymid):moveY/ymid;
  arr[0] = moveX;
  arr[1] = moveY;
  arr[2] = digitalRead(grabPin);
  arr[3] = digitalRead(shootPin);
  radio.write(&arr, sizeof(arr));
//  radio.write(&moveY, sizeof(moveY));
//  delay(10);
}
