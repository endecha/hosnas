#include <LiquidCrystal.h>
#include <EEPROM.h>

#define TOUCH 8

#define OPER_CLOSE 0

#define SELECTOR_1_U 2
#define SELECTOR_2_U 10
#define SELECTOR_3_U 20
#define SELECTOR_4_U 20
#define SELECTOR_5_U 20
#define SELECTOR_6_U 20
#define SELECTOR_1_T 10
#define SELECTOR_2_T 20
#define SELECTOR_3_T 30
#define SELECTOR_4_T 40
#define SELECTOR_5_T 50
#define SELECTOR_6_T 60

#define MONTH   (60 * 60 * 24 * 30)
#define DAY     (60 * 60 * 24)
#define HOUR    (60 * 60)
#define MINUTE  (60)
#define SECOND  (1)

#define ONE_LITER_SEC  100

#define FILER1	0
#define FILER2	5
#define FILER3	10
#define FILER4	15
#define FILER5	20

#define FILTER1_TIME    15811200    // Sedi 3 ~ 6 month
#define FILTER1_LITER   1880        // Sedi 1880L
#define FILTER2_TIME    15811200    // pre 6 month
#define FILTER2_LITER   1850        // pre 1850L
#define FILTER3_TIME    63244800    // csm 12 ~ 24 month
#define FILTER3_LITER   5500        // csm 5500L
#define FILTER4_TIME    31622400    // ceramic 6 ~ 12 month
#define FILTER5_TIME    31622400    // post 9 ~ 12 month
#define FILTER5_LITER   5500        // post 3650L

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void record_operating_time();
void get_record_time();
void set_record_time();
void setting_func(int);

unsigned int selector_value;
unsigned int selector_changed;
unsigned int selector_time_u;
unsigned int selector_time_t;
unsigned int activate;
unsigned int operating_time;
unsigned int setting_mode;
unsigned int setting_mode_enter;
unsigned int setting_mode_age;
unsigned int setting_entry_count;
unsigned int setting_select;

unsigned int filter1_total;
unsigned int filter2_total;
unsigned int filter3_total;
unsigned int filter4_total;
unsigned int filter5_total;

unsigned char filter[25];

void setup() {
  Serial.begin(9600);
  pinMode(TOUCH, INPUT);
  selector_value = 0;
  selector_changed = 0;
  selector_time_u = 0;
  selector_time_t = 0;
  activate = 0;
  operating_time = 0;
  setting_mode = 0;
  setting_mode_enter = 0;
  setting_mode_age = 0;
  setting_entry_count = 0;
  setting_select = 0;
  lcd.begin(16, 2);
  get_record_time();
  lcd.print("Ready!!!");
  delay(2000);
}

void loop() {
  unsigned int selector;
  char msg;

  if (setting_mode) {
    if (!setting_mode_enter) {
      setting_mode_enter = 1;
      lcd.display();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Entering");
      lcd.setCursor(0, 1);
      lcd.print("Setting Mode");
      delay(3000);
      lcd.setCursor(0, 0);
      lcd.print("Setting mode");
      lcd.setCursor(0, 1);
      lcd.print("Filter info.");
      setting_select = 0;
      setting_mode_age = 0;
    }

    if (digitalRead(TOUCH)==LOW) {
      setting_select++;
      setting_mode_age = 0;
      if (setting_select > 6)
        setting_select = 0;
        
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Setting mode");
      lcd.setCursor(0, 1);
      if (setting_select==0)
        lcd.print("Filter info.");
      else if (setting_select==1)
        lcd.print("Reset filter #1");
      else if (setting_select==2)
        lcd.print("Reset filter #2");
      else if (setting_select==3)
        lcd.print("Reset filter #3");
      else if (setting_select==4)
        lcd.print("Reset filter #4");
      else if (setting_select==5)
        lcd.print("Reset filter #5");
      else if (setting_select==5)
        lcd.print("Reset filter #5");
      else if (setting_select==6)
        lcd.print("Reset filter ALL");
    }
    else {
      setting_mode_age++;
      if (setting_mode_age > 4)
        setting_func(setting_select);
    }
  }
  else {
    if (digitalRead(TOUCH)==LOW) {
      setting_entry_count++;

      if (setting_entry_count > 4)
        setting_mode = 1;
    }
    else {
      setting_entry_count = 0;
    }

    if (Serial.available()) {
      msg = Serial.read();

      if (msg >= '1' && msg <= '6') {
        selector = msg - '0';
        if (selector_value != selector) {
          selector_changed = 3;
          selector_value = selector;
        }
        else {
          selector_changed = 0;
        }
      }

      if (msg == 't' || msg == 'u' || msg == 'c') {
        if (msg == 't') {
          activate = selector_time_t - 1;
        }

        if (msg == 'u') {
          activate = selector_time_u - 1;
        }

        if (msg == 'c') {
          activate = OPER_CLOSE;
          record_operating_time();
          operating_time = 0;
        }
      }
    }

    if (selector_changed) {
      lcd.display();
      lcd.clear();
      lcd.setCursor(0, 0);

      if (selector_value == 1) {
        lcd.print("Level 1 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper T:10s U:2s");
        selector_time_u = SELECTOR_1_U;
        selector_time_t = SELECTOR_1_T;
      }
      else if (selector_value == 2) {
        lcd.print("Level 2 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper T:20s U:10s");
        selector_time_u = SELECTOR_2_U;
        selector_time_t = SELECTOR_2_T;
      }
      else if (selector_value == 3) {
        lcd.print("Level 3 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper T:30s U:20s");
        selector_time_u = SELECTOR_3_U;
        selector_time_t = SELECTOR_3_T;
      }
      else if (selector_value == 4) {
        lcd.print("Level 4 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper T:40s U:20s");
        selector_time_u = SELECTOR_4_U;
        selector_time_t = SELECTOR_4_T;
      }
      else if (selector_value == 5) {
        lcd.print("Level 5 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper T:50s U:20s");
        selector_time_u = SELECTOR_5_U;
        selector_time_t = SELECTOR_5_T;
      }
      else if (selector_value == 6) {
        lcd.print("Level 6 Selected");
        lcd.setCursor(0, 1);
        lcd.print("Oper U:60s T:20s");
        selector_time_u = SELECTOR_6_U;
        selector_time_t = SELECTOR_6_T;
      }

      selector_changed--;
    }

    if (activate) {
      lcd.display();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Open Valve");
      lcd.setCursor(1, 1);
      lcd.print(activate);
      activate--;
      operating_time++;
    }

    if(!selector_changed && !activate)
      lcd.noDisplay();

  }

  delay(1000);
}

void setting_func(int select)
{
  unsigned int liter;
  unsigned int percent;

  if (select==0) {
    /* filter1 info */
    lcd.clear();

    liter = filter1_total/ONE_LITER_SEC;
    percent = liter * 100 / FILTER1_LITER;

    lcd.setCursor(0, 0);
    lcd.print("#1");
    lcd.setCursor(3, 0);
    lcd.print("Lt:");
    lcd.setCursor(6, 0);
    lcd.print(liter);
    lcd.setCursor(10, 0);
    lcd.print("(");
    lcd.setCursor(11, 0);
    lcd.print(percent);
    lcd.setCursor(14, 0);
    lcd.print("%)");

    lcd.setCursor(0, 1);
    lcd.print(filter[0]);
    lcd.setCursor(2, 1);
    lcd.print("M");
    lcd.setCursor(3, 1);
    lcd.print(filter[1]);
    lcd.setCursor(5, 1);
    lcd.print("D");
    lcd.setCursor(6, 1);
    lcd.print(filter[2]);
    lcd.setCursor(8, 1);
    lcd.print("H");
    lcd.setCursor(9, 1);
    lcd.print(filter[3]);
    lcd.setCursor(11, 1);
    lcd.print("M");
    lcd.setCursor(12, 1);
    lcd.print(filter[4]);
    lcd.setCursor(14, 1);
    lcd.print("S");

    delay(3000);

    /* filter2 info */
    lcd.clear();

    liter = filter2_total/ONE_LITER_SEC;
    percent = liter * 100 / FILTER2_LITER;

    lcd.setCursor(0, 0);
    lcd.print("#2");
    lcd.setCursor(3, 0);
    lcd.print("Lt:");
    lcd.setCursor(6, 0);
    lcd.print(liter);
    lcd.setCursor(10, 0);
    lcd.print("(");
    lcd.setCursor(11, 0);
    lcd.print(percent);
    lcd.setCursor(14, 0);
    lcd.print("%)");

    lcd.setCursor(0, 1);
    lcd.print(filter[5]);
    lcd.setCursor(2, 1);
    lcd.print("M");
    lcd.setCursor(3, 1);
    lcd.print(filter[6]);
    lcd.setCursor(5, 1);
    lcd.print("D");
    lcd.setCursor(6, 1);
    lcd.print(filter[7]);
    lcd.setCursor(8, 1);
    lcd.print("H");
    lcd.setCursor(9, 1);
    lcd.print(filter[8]);
    lcd.setCursor(11, 1);
    lcd.print("M");
    lcd.setCursor(12, 1);
    lcd.print(filter[9]);
    lcd.setCursor(14, 1);
    lcd.print("S");

    delay(3000);

    /* filter3 info */
    lcd.clear();

    liter = filter3_total/ONE_LITER_SEC;
    percent = liter * 100 / FILTER3_LITER;

    lcd.setCursor(0, 0);
    lcd.print("#3");
    lcd.setCursor(3, 0);
    lcd.print("Lt:");
    lcd.setCursor(6, 0);
    lcd.print(liter);
    lcd.setCursor(10, 0);
    lcd.print("(");
    lcd.setCursor(11, 0);
    lcd.print(percent);
    lcd.setCursor(14, 0);
    lcd.print("%)");

    lcd.setCursor(0, 1);
    lcd.print(filter[10]);
    lcd.setCursor(2, 1);
    lcd.print("M");
    lcd.setCursor(3, 1);
    lcd.print(filter[11]);
    lcd.setCursor(5, 1);
    lcd.print("D");
    lcd.setCursor(6, 1);
    lcd.print(filter[12]);
    lcd.setCursor(8, 1);
    lcd.print("H");
    lcd.setCursor(9, 1);
    lcd.print(filter[13]);
    lcd.setCursor(11, 1);
    lcd.print("M");
    lcd.setCursor(12, 1);
    lcd.print(filter[14]);
    lcd.setCursor(14, 1);
    lcd.print("S");

    delay(3000);

    /* filter4 info */
    lcd.clear();

    liter = filter4_total/ONE_LITER_SEC;
    //percent = liter * 100 / FILTER4_LITER;

    lcd.setCursor(0, 0);
    lcd.print("#4");
    lcd.setCursor(3, 0);
    lcd.print("Lt:");
    lcd.setCursor(6, 0);
    lcd.print(liter);
#if 0
    lcd.setCursor(10, 0);
    lcd.print("(");
    lcd.setCursor(11, 0);
    lcd.print(percent);
    lcd.setCursor(14, 0);
    lcd.print("%)");
#endif

    lcd.setCursor(0, 1);
    lcd.print(filter[15]);
    lcd.setCursor(2, 1);
    lcd.print("M");
    lcd.setCursor(3, 1);
    lcd.print(filter[16]);
    lcd.setCursor(5, 1);
    lcd.print("D");
    lcd.setCursor(6, 1);
    lcd.print(filter[17]);
    lcd.setCursor(8, 1);
    lcd.print("H");
    lcd.setCursor(9, 1);
    lcd.print(filter[18]);
    lcd.setCursor(11, 1);
    lcd.print("M");
    lcd.setCursor(12, 1);
    lcd.print(filter[19]);
    lcd.setCursor(14, 1);
    lcd.print("S");

    delay(3000);

    /* filter5 info */
    lcd.clear();

    liter = filter5_total/ONE_LITER_SEC;
    percent = liter * 100 / FILTER5_LITER;

    lcd.setCursor(0, 0);
    lcd.print("#5");
    lcd.setCursor(3, 0);
    lcd.print("Lt:");
    lcd.setCursor(6, 0);
    lcd.print(liter);
    lcd.setCursor(10, 0);
    lcd.print("(");
    lcd.setCursor(11, 0);
    lcd.print(percent);
    lcd.setCursor(14, 0);
    lcd.print("%)");

    lcd.setCursor(0, 1);
    lcd.print(filter[20]);
    lcd.setCursor(2, 1);
    lcd.print("M");
    lcd.setCursor(3, 1);
    lcd.print(filter[21]);
    lcd.setCursor(5, 1);
    lcd.print("D");
    lcd.setCursor(6, 1);
    lcd.print(filter[22]);
    lcd.setCursor(8, 1);
    lcd.print("H");
    lcd.setCursor(9, 1);
    lcd.print(filter[23]);
    lcd.setCursor(11, 1);
    lcd.print("M");
    lcd.setCursor(12, 1);
    lcd.print(filter[24]);
    lcd.setCursor(14, 1);
    lcd.print("S");

    delay(3000);

    lcd.noDisplay();
  }
  else if (select==1) {
    filter1_total = 0;
    set_record_time();
  }
  else if (select==2) {
    filter2_total = 0;
    set_record_time();
  }
  else if (select==3) {
    filter3_total = 0;
    set_record_time();
  }
  else if (select==4) {
    filter4_total = 0;
    set_record_time();
  }
  else if (select==5) {
    filter5_total = 0;
    set_record_time();
  }
  else if (select==6) {
    filter1_total = 0;
    filter2_total = 0;
    filter3_total = 0;
    filter4_total = 0;
    filter5_total = 0;
    set_record_time();
  }

  setting_mode = 0;
  setting_mode_enter = 0;
}


void record_operating_time()
{
  // each filter, plus operating_time to global total time (second unit)
  filter1_total += operating_time;
  filter2_total += operating_time;
  filter3_total += operating_time;
  filter4_total += operating_time;
  filter5_total += operating_time;

  set_record_time();
}

void get_record_time()
{
  // each filter, get eeprom data, save to global total time (second unit)
  int i;

  for(i = 0 ; i < 25 ; i++) {
    filter[i] = EEPROM.read(i);
  }

  filter1_total = 
      filter[0] * 60 * 60 * 24 * 30 // month
    + filter[1] * 60 * 60 * 24      // day
    + filter[2] * 60 * 60           // hour
    + filter[3] * 60                // minute
    + filter[4];                    // second

  filter2_total = 
      filter[5] * 60 * 60 * 24 * 30 // month
    + filter[6] * 60 * 60 * 24      // day
    + filter[7] * 60 * 60           // hour
    + filter[8] * 60                // minute
    + filter[9];                    // second

  filter3_total = 
      filter[10] * 60 * 60 * 24 * 30 // month
    + filter[11] * 60 * 60 * 24      // day
    + filter[12] * 60 * 60           // hour
    + filter[13] * 60                // minute
    + filter[14];                    // second

  filter4_total = 
      filter[15] * 60 * 60 * 24 * 30 // month
    + filter[16] * 60 * 60 * 24      // day
    + filter[17] * 60 * 60           // hour
    + filter[18] * 60                // minute
    + filter[19];                    // second

  filter5_total = 
      filter[20] * 60 * 60 * 24 * 30 // month
    + filter[21] * 60 * 60 * 24      // day
    + filter[22] * 60 * 60           // hour
    + filter[23] * 60                // minute
    + filter[24];                    // second

}

void set_record_time()
{
  unsigned char month, day, hour, minute, second;
  unsigned int total;

  /* filter1 */
  total = filter1_total;
  month = (unsigned char) total / MONTH;

  total -= month * MONTH;
  day = (unsigned char) total / DAY;

  total -= day * DAY;
  hour = (unsigned char) total / HOUR;

  total -= hour * HOUR;
  minute = (unsigned char) total / MINUTE;

  total -= minute * MINUTE;
  second = (unsigned char) total / SECOND;

  filter[0] = month;
  filter[1] = day;
  filter[2] = hour;
  filter[3] = minute;
  filter[4] = second;

  EEPROM.write(0, month);
  EEPROM.write(1, day);
  EEPROM.write(2, hour);
  EEPROM.write(3, minute);
  EEPROM.write(4, second);

  /* filter2 */
  total = filter2_total;
  month = (unsigned char) total / MONTH;

  total -= month * MONTH;
  day = (unsigned char) total / DAY;

  total -= day * DAY;
  hour = (unsigned char) total / HOUR;

  total -= hour * HOUR;
  minute = (unsigned char) total / MINUTE;

  total -= minute * MINUTE;
  second = (unsigned char) total / SECOND;

  filter[5] = month;
  filter[6] = day;
  filter[7] = hour;
  filter[8] = minute;
  filter[9] = second;

  EEPROM.write(5, month);
  EEPROM.write(6, day);
  EEPROM.write(7, hour);
  EEPROM.write(8, minute);
  EEPROM.write(9, second);
  
  /* filter3 */
  total = filter3_total;
  month = (unsigned char) total / MONTH;

  total -= month * MONTH;
  day = (unsigned char) total / DAY;

  total -= day * DAY;
  hour = (unsigned char) total / HOUR;

  total -= hour * HOUR;
  minute = (unsigned char) total / MINUTE;

  total -= minute * MINUTE;
  second = (unsigned char) total / SECOND;

  filter[10] = month;
  filter[11] = day;
  filter[12] = hour;
  filter[13] = minute;
  filter[14] = second;

  EEPROM.write(10, month);
  EEPROM.write(11, day);
  EEPROM.write(12, hour);
  EEPROM.write(13, minute);
  EEPROM.write(14, second);

  /* filter4 */
  total = filter4_total;
  month = (unsigned char) total / MONTH;

  total -= month * MONTH;
  day = (unsigned char) total / DAY;

  total -= day * DAY;
  hour = (unsigned char) total / HOUR;

  total -= hour * HOUR;
  minute = (unsigned char) total / MINUTE;

  total -= minute * MINUTE;
  second = (unsigned char) total / SECOND;

  filter[15] = month;
  filter[16] = day;
  filter[17] = hour;
  filter[18] = minute;
  filter[19] = second;

  EEPROM.write(15, month);
  EEPROM.write(16, day);
  EEPROM.write(17, hour);
  EEPROM.write(18, minute);
  EEPROM.write(19, second);

  /* filter5 */
  total = filter5_total;
  month = (unsigned char) total / MONTH;

  total -= month * MONTH;
  day = (unsigned char) total / DAY;

  total -= day * DAY;
  hour = (unsigned char) total / HOUR;

  total -= hour * HOUR;
  minute = (unsigned char) total / MINUTE;

  total -= minute * MINUTE;
  second = (unsigned char) total / SECOND;

  filter[20] = month;
  filter[21] = day;
  filter[22] = hour;
  filter[23] = minute;
  filter[24] = second;

  EEPROM.write(20, month);
  EEPROM.write(21, day);
  EEPROM.write(22, hour);
  EEPROM.write(23, minute);
  EEPROM.write(24, second);
}
