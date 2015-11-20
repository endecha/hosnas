/* GPIO pin numbers */
#define TOUCH 		2
#define RELAY_TOUCH 	3
#define RELAY_ULTRA 	4
#define BUZZER 		8
#define ULTRA_TRIG 	12
#define ULTRA_ECHO 	13

/* Constant values */
#define ULTRA_DISTANCE   10
#define ULTRA_CNT	     5
#define DURATION_ULTRA   10
#define DURATION_FACTOR 10 
#define DURATION_SEC      1000
#define SELECTOR_SEC      10

/* Selector pins */
#define SELECTOR_1    5
#define SELECTOR_2    6
#define SELECTOR_3    7
#define SELECTOR_4    11
#define SELECTOR_5    10

/* Operating Indicator LED */
#define LED    9

/* Selector values */
#define SELECTOR_V1   1
#define SELECTOR_V2   2 
#define SELECTOR_V3   3 
#define SELECTOR_V4   4 
#define SELECTOR_V5   5 
#define SELECTOR_V6   6 
#define SELECTOR_V1_T   10
#define SELECTOR_V2_T   20 
#define SELECTOR_V3_T   30 
#define SELECTOR_V4_T   40 
#define SELECTOR_V5_T   50 
#define SELECTOR_V6_T   60 
#define SELECTOR_V1_U   1
#define SELECTOR_V2_U   10 
#define SELECTOR_V3_U   20 
#define SELECTOR_V4_U   20 
#define SELECTOR_V5_U   20 
#define SELECTOR_V6_U   20 

/* Buzzer */
#define BUZZER_READY	1
#define BUZZER_START	2
#define BUZZER_FINISH	3
#define BUZZER_CONTINUE	4

/* Global variables */
volatile unsigned int touch_activate;
volatile unsigned int touch_flag;
volatile unsigned int touch_selector;
volatile unsigned int ultra_activate;
volatile unsigned int ultra_flag;
volatile unsigned int ultra_curr;
double dist_avg[ULTRA_CNT];
volatile unsigned int selector_cnt;
volatile unsigned int led_activate;

/* Functions prototype */
void interruptHandler();
void buzzerNotify(int);
void decreaseCheckTouch();
void decreaseCheckUltra();
void continueCheckTouch();  
void peridicCheckUltra();
void checkSelector();
double getUltraAvg();
void  operatingLED();

void setup() {
  Serial.begin(9600);
  pinMode(ULTRA_TRIG, OUTPUT);
  pinMode(ULTRA_ECHO, INPUT);
  pinMode(RELAY_TOUCH, OUTPUT);
  pinMode(RELAY_ULTRA, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SELECTOR_1, INPUT);
  pinMode(SELECTOR_2, INPUT);
  pinMode(SELECTOR_3, INPUT);
  pinMode(SELECTOR_4, INPUT);
  pinMode(SELECTOR_5, INPUT);
  pinMode(LED, OUTPUT);  
  touch_activate = 0;
  touch_flag = 0;
  touch_selector = 0;
  ultra_activate = 0;
  ultra_flag = 0;
  ultra_curr = 0;
  selector_cnt = 0;
  led_activate = 0;
  dist_avg[0] = ULTRA_DISTANCE * 10;
  dist_avg[1] = ULTRA_DISTANCE * 10;
  dist_avg[2] = ULTRA_DISTANCE * 10;
  dist_avg[3] = ULTRA_DISTANCE * 10;
  dist_avg[4] = ULTRA_DISTANCE * 10;
  attachInterrupt(0, interruptHandler, FALLING);
  digitalWrite(RELAY_TOUCH, HIGH);
  digitalWrite(RELAY_ULTRA, HIGH);
  buzzerNotify(BUZZER_READY);
  delay(3000);
  checkSelector();
}

void loop() {
  operatingLED();
  decreaseCheckTouch();
  decreaseCheckUltra();
  continueCheckTouch();
  peridicCheckUltra();
  checkSelector();
  delay(DURATION_SEC / DURATION_FACTOR);
}

void operatingLED()
{
  int time, pulse;
  
  if(touch_activate || ultra_activate) {
    if(touch_activate > ultra_activate)
      time = touch_activate;
    else
      time = ultra_activate;
      
   if(led_activate!=0) {
     if(time < 10) {
       if(time%3==0)
         digitalWrite(LED,HIGH);
       else
         digitalWrite(LED,LOW);
       return;
     }
     pulse = ((time * 100 / led_activate) * 255) / 100;
     analogWrite(LED, pulse);
   }
   else {
     led_activate = time;
     analogWrite(LED, 255);
   }
  }
  else {
    led_activate = 0;
    digitalWrite(LED,LOW);
  }
}

void interruptHandler()
{
  if(touch_activate==0) {
    detachInterrupt(0);
    touch_flag = DURATION_FACTOR;
    pinMode(TOUCH, INPUT);
    if(touch_selector==SELECTOR_V1)
      touch_activate = SELECTOR_V1_T * DURATION_FACTOR;
    else if(touch_selector==SELECTOR_V2)
      touch_activate = SELECTOR_V2_T * DURATION_FACTOR;
    else if(touch_selector==SELECTOR_V3)
      touch_activate = SELECTOR_V3_T * DURATION_FACTOR;
    else if(touch_selector==SELECTOR_V4)
      touch_activate = SELECTOR_V4_T * DURATION_FACTOR;
    else if(touch_selector==SELECTOR_V5)
      touch_activate = SELECTOR_V5_T * DURATION_FACTOR;
    else
      touch_activate = SELECTOR_V6_T * DURATION_FACTOR;
    
    digitalWrite(RELAY_TOUCH, LOW);
    Serial.print('t');
    buzzerNotify(BUZZER_START);
  }
  return;
}

void buzzerNotify(int state)
{
  switch(state) {
    case BUZZER_READY:
      tone(BUZZER, 600, 100);
      break;

    case BUZZER_START:
      tone(BUZZER, 600, 100);
      break;

    case BUZZER_FINISH:
      tone(BUZZER, 600, 100);
      break;

    case BUZZER_CONTINUE:
      tone(BUZZER, 600, 100);
      break;

    default:
      break;
  }
  return;
}

void decreaseCheckTouch()
{
  if (touch_activate) {
    if(touch_activate > 0) 
      touch_activate--;
    if(touch_flag > 0) 
      touch_flag--;
    if (touch_activate==0) {
      EIMSK &= ~(1 << INT0);
      EIFR |= (1 << INTF0);
      EIMSK |= (1 << INT0);
      attachInterrupt(0, interruptHandler, FALLING);
      digitalWrite(RELAY_TOUCH, HIGH);
      Serial.print('c');
      touch_flag = 0;
      buzzerNotify(BUZZER_FINISH);
    }
  }
  return;
}

void decreaseCheckUltra()
{
  if (ultra_activate) {
    if(ultra_activate > 0) 
      ultra_activate--;
    if(ultra_activate==0) {
      digitalWrite(RELAY_ULTRA, HIGH);
      EIMSK &= ~(1 << INT0);
      EIFR |= (1 << INTF0);
      EIMSK |= (1 << INT0);
      attachInterrupt(0, interruptHandler, FALLING);
      Serial.print('c');
      buzzerNotify(BUZZER_FINISH);
      delay(DURATION_SEC);
    }
  }
  return;
}

void continueCheckTouch()
{
  if(touch_flag > 0) {
    if(touch_flag==1) {
      if(digitalRead(TOUCH)==LOW) {
        if(touch_activate > 2)
          buzzerNotify(BUZZER_CONTINUE);
        touch_activate = 2;
        touch_flag = 2;
      }
    }
  }  
  if(touch_flag == 0 && touch_activate > 0) {
    if(digitalRead(TOUCH)==LOW) {
      touch_activate = 1;
    }
  }
  return;
}

double getUltraAvg()
{
  double sum = 0;
  int i;

  for(i = 0 ; i < ULTRA_CNT ; i++) {
    sum += dist_avg[i];
  }

  return (sum / ULTRA_CNT);
}

void peridicCheckUltra()
{
  if(touch_activate > 0)
    return;
  
  long duration = 0;
  double distance = 0;
  double dist_value = 0;
  digitalWrite(ULTRA_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIG, LOW);
  duration = pulseIn(ULTRA_ECHO,HIGH);
  distance = duration / 58.8;

  /* save distance ULTRA_CNT array */
  dist_avg[ultra_curr] = distance;
  ultra_curr++;
  if(ultra_curr==ULTRA_CNT)
    ultra_curr = 0;

  /* get the average value of ULTRA_CNT array */
  dist_value = getUltraAvg();

  /* during ultra_activate, if current distance less than ULTRA_DISTANCE,
     close Solenoid Valve immediately */
  if(ultra_activate && (distance < ULTRA_DISTANCE))
    dist_value = distance;

  if(dist_value > 0) {
    if(dist_value < ULTRA_DISTANCE) {
      if(ultra_flag==0 && ultra_activate==0) {
        ultra_flag = 1;
        if(touch_selector==SELECTOR_V1)
          ultra_activate = SELECTOR_V1_U * DURATION_FACTOR;
        else if(touch_selector==SELECTOR_V2)
          ultra_activate = SELECTOR_V2_U * DURATION_FACTOR;
        else if(touch_selector==SELECTOR_V3)
          ultra_activate = SELECTOR_V3_U * DURATION_FACTOR;
        else if(touch_selector==SELECTOR_V4)
          ultra_activate = SELECTOR_V4_U * DURATION_FACTOR;
        else if(touch_selector==SELECTOR_V5)
          ultra_activate = SELECTOR_V5_U * DURATION_FACTOR;
        else
          ultra_activate = SELECTOR_V6_U * DURATION_FACTOR;
        digitalWrite(RELAY_ULTRA, LOW);
        detachInterrupt(0);
        buzzerNotify(BUZZER_START);
        Serial.print('u');
      }
    }
    else {
      ultra_flag = 0;
      if(ultra_activate > 0) {
        ultra_activate = 1;
      }
    }
  }
  return;
}

void checkSelector()
{
  if(touch_activate || ultra_activate)
    return;

  int previous = touch_selector;

  if(digitalRead(SELECTOR_1) == LOW)
    touch_selector = SELECTOR_V1;
  else if(digitalRead(SELECTOR_2) == LOW)
    touch_selector = SELECTOR_V2;
  else if(digitalRead(SELECTOR_3) == LOW)
    touch_selector = SELECTOR_V3;
  else if(digitalRead(SELECTOR_4) == LOW)
    touch_selector = SELECTOR_V4;
  else if(digitalRead(SELECTOR_5) == LOW)
    touch_selector = SELECTOR_V5;
  else 
    touch_selector = SELECTOR_V6;
    
  if(previous!=touch_selector) {
    if(touch_selector == SELECTOR_V1) {
     Serial.print('1');
     buzzerNotify(BUZZER_START);
    }
    else if(touch_selector == SELECTOR_V2) {
     Serial.print('2');
     buzzerNotify(BUZZER_START);
    }
    else if(touch_selector == SELECTOR_V3) {
     Serial.print('3');
     buzzerNotify(BUZZER_START);
    }
    else if(touch_selector == SELECTOR_V4) {
     Serial.print('4');
     buzzerNotify(BUZZER_START);
    }
    else if(touch_selector == SELECTOR_V5) {
     Serial.print('5');
     buzzerNotify(BUZZER_START);
    }
    else {
     Serial.print('6');
     buzzerNotify(BUZZER_START);
    }
  }
}
