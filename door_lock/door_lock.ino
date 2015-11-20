#include <Servo.h> 

#define SW1 0
#define SW2 1
#define SW3 7
#define SW4 8

#define DR1 6
#define DR2 9
#define DR3 10
#define DR4 11

#define DR1_OPEN  180
#define DR1_CLOSE 0
#define DR2_OPEN  0
#define DR2_CLOSE 180
#define DR3_OPEN  0
#define DR3_CLOSE 180
#define DR4_OPEN  180
#define DR4_CLOSE 0

void attachAllServo();
void detachAllServo();

Servo door1;
Servo door2;
Servo door3;
Servo door4;
 
int opened1;
int opened2;
int opened3;
int opened4;
int active;

void setup() 
{ 
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  opened1 = 0;
  opened2 = 0;
  opened3 = 0;
  opened4 = 0;
  active  = 0;
} 

void loop() 
{ 
  active = 0;

  if (digitalRead(SW1)==HIGH) {
    if (opened1==0) {
      if (!active)
        attachAllServo();
      opened1 = 1;
      active = 1;
      door1.write(DR1_OPEN);
    }
  }
  else {
    if (opened1==1) {
      if (!active)
        attachAllServo();
      opened1 = 0;
      active = 1;
      door1.write(DR1_CLOSE);
    }
  }

  if (digitalRead(SW2)==HIGH) {
    if (opened2==0) {
      if (!active)
        attachAllServo();
      opened2 = 1;
      active = 1;
      door2.write(DR2_OPEN);
    }
  }
  else {
    if (opened2==1) {
      if (!active)
        attachAllServo();
      opened2 = 0;
      active = 1;
      door2.write(DR2_CLOSE);
    }
  }

  if (digitalRead(SW3)==HIGH) {
    if (opened3==0) {
      if (!active)
        attachAllServo();
      opened3 = 1;
      active = 1;
      door3.write(DR3_OPEN);
    }
  }
  else {
    if (opened3==1) {
      if (!active)
        attachAllServo();
      opened3 = 0;
      active = 1;
      door3.write(DR3_CLOSE);
    }
  }

  if (digitalRead(SW4)==HIGH) {
    if (opened4==0) {
      if (!active)
        attachAllServo();
      opened4 = 1;
      active = 1;
      door4.write(DR4_OPEN);
    }
  }
  else {
    if (opened4==1) {
      if (!active)
        attachAllServo();
      opened4 = 0;
      active = 1;
      door4.write(DR4_CLOSE);
    }
  }

  if (active) {
    delay(2000);
    detachAllServo();
  }
  else
    delay(300);
} 

void attachAllServo()
{
  door1.attach(DR1);
  door2.attach(DR2);
  door3.attach(DR3);
  door4.attach(DR4);
  delay(100);
}

void detachAllServo()
{
  door1.detach();
  door2.detach();
  door3.detach();
  door4.detach();
  delay(100);
}
