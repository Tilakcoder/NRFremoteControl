//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";
float moveX, moveY;
float sp1, sp2;
float arr[4];

Servo grab;

class Motor{
  public:
    int in1, in2, en;
    Motor(int i1, int i2, int e){
      in1 = i1;
      in2 = i2;
      en = e;
      pinMode(i1, OUTPUT);
      pinMode(i2, OUTPUT);
      pinMode(en, OUTPUT);
      digitalWrite(i1, 1);
      digitalWrite(i2, 0);
      analogWrite(0, en);
    }

    void go(int sp){
      analogWrite(en, (abs(sp)>255)?255:abs(sp));
      digitalWrite(in1, sp>=0);
      digitalWrite(in2, sp<0);
    }
};

Motor m1(2, 4, 3);
Motor m2(5, 7, 6);

void setup()
{
  //while (!Serial);
  Serial.begin(9600);
  
  radio.begin();
  pinMode(A1, INPUT);
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();

  grab.attach(A0);
  grab.write(180);
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&arr, sizeof(arr));
//    radio.read(&moveY, sizeof(moveY));

    moveX = arr[0];
    moveY = arr[1];
    grab.write((arr[2]==1)?0:180);
    
    sp1 = moveY*255;
    sp2 = sp1;

    sp1 += moveX*255;
    sp2 = (sp2 - moveX*255)*-1;

    m1.go(sp1);
    m2.go(sp2);
  }
  int sig = Serial.read();
  if(sig+1){
    Serial.println(sig);
  }
  Serial.println(digitalRead(A1));
}
