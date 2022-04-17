//TeaWatch OS
//From TeaSoftOffice ---Birkhoff Carl

//EEPROM ADDRESS(SETTINGS) LIST:
//Version:             0(x)
//POWER SAVE MODE:     1(0:off/1:on)
//12h OR 24h:          2(0:24/1:12)
//ALARM HOUR1/2/3:     3/5/7(xxH)
//ALARM MINUTE1/2/3:   4/6/8(xxM)
//BIRTHDAY MONTH:      9(xx)
//BIRTHDAY DATE:       10(xx)
//AUTO ALARM1/2/3:     11/12/13(0:off/1:on)
//PRESSURE BASELINE:   14(x.x)
//LOW BATTERY VOLTAGE: 15(xxxx)
//SLEEP CYCLE:         16(xx)
//CHECK FREQUENCY:     17(x)
//FINGER VOLTAGE:      18(xxxx)



#define PT_USE_TIMER
#define PT_USE_SEM

#include "pt.h"
#include <Adafruit_ssd1306syp.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <SFE_BMP180.h>
#include <AM2320.h>
#include <Wire.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

//HARDWARE_ADDRESS:
#define SDA_OLED_HW 8
#define SCL_OLED_HW 9
#define ADDR_GY271_HW 0x1E

const int Button_INPIN = 2;
const int CHANGING_INPIN = 3;
const int Motor_OUTPIN = 5;
const int Buzzer_OUTPIN = 11;
const int Led_OUTPIN = 13;
const int BLE_INPIN = A1;
const int BLE_OUTPIN = A2;
const int Finger_PIN = A7;

//VARIABLES:
byte bySecond = 00;
byte byMinute = 00;
byte byHour = 00;
byte byDate = 01;
byte byMonth = 01;
byte byYear = 16;
byte byDoW = 5;
double dBaseline = 1024.00;
double dPressure = 101000;
double dHeight = 0;
double dTemperature = 15;
double dHumidity = 50;
unsigned short unFinger = 512;
unsigned short unPower = 3300;
bool bCentury = false;
bool bH12;
bool bPM;
bool bBACKGROUND_REQUEST = false;
volatile short TnX = 0;
volatile short TnY = 0;
volatile short TnZ = 0;
volatile bool bKEY_INTERRUPT = false;
volatile byte TbyKEY_TIMER = 0;
volatile unsigned long lWDT_TIMER = 0;

//LIBRARIES INITIALIZE:
Adafruit_ssd1306syp display(SDA_OLED_HW, SCL_OLED_HW);
DS3231 Clock;
SFE_BMP180 pressure;
AM2320 AM2320;

static struct pt_sem OLED;
static struct pt TIME, CHECK, BEEP, ALARM, SLEEP, TIMER, STOPWATCH, MEMO, APP, CONFIG;

//Control each thread
bool bTIME_Switch = false;
bool bBEEP_Switch = false;
bool bALARM_Switch = false;
bool bSLEEP_Switch = false;
bool bTIMER_Switch = false;
bool bSTOPWATCH_Switch = false;
bool bMEMO_Switch = false;
bool bAPP_Switch = false;
bool bCONFIG_Switch = false;

const unsigned char Sec_X[60] = {
  32, 35, 38, 41, 43,
  45, 48, 50, 52, 54,
  56, 58, 59, 60, 60,
  60, 60, 60, 59, 58,
  56, 54, 52, 50, 48,
  45, 43, 41, 38, 35,
  32, 29, 26, 23, 21,
  19, 16, 14, 12, 10,
  8, 6, 5, 4, 4,
  4, 4, 4, 5, 6,
  8, 10, 12, 14, 16,
  19, 21, 23, 26, 29,
};//Sec X-Coordinate

const unsigned char Sec_Y[60] = {
  4, 4, 4, 5, 6,
  8, 10, 12, 14, 16,
  19, 21, 23, 26, 29,
  32, 35, 38, 41, 43,
  45, 48, 50, 52, 54,
  56, 58, 59, 60, 60,
  60, 60, 60, 59, 58,
  56, 54, 52, 50, 48,
  45, 43, 41, 38, 35,
  32, 29, 26, 23, 21,
  19, 16, 14, 12, 10,
  8, 6, 5, 4, 4,
};//Sec Y-Coordinate

const unsigned char Min_X[60] = {
  32, 34, 36, 38, 40,
  41, 42, 44, 46, 48,
  49, 50, 51, 52, 53,
  54, 53, 52, 51, 50,
  49, 46, 45, 44, 42,
  40, 39, 38, 37, 35,
  32, 29, 27, 26, 25,
  24, 22, 20, 19, 18,
  15, 14, 13, 12, 11,
  10, 11, 12, 13, 14,
  15, 16, 18, 20, 22,
  23, 24, 26, 28, 30,
};//Min X-Coordinate

const unsigned char Min_Y[60] = {
  10, 10, 10, 11, 13,
  14, 15, 17, 19, 21,
  23, 24, 26, 28, 30,
  32, 34, 36, 38, 40,
  41, 44, 45, 46, 48,
  49, 50, 51, 52, 53,
  54, 53, 52, 51, 50,
  49, 48, 46, 45, 44,
  41, 40, 38, 36, 34,
  32, 30, 28, 26, 24,
  23, 21, 19, 17, 15,
  14, 13, 11, 10, 10,
};//Min Y-Coordinte

const unsigned char Hour_X[12] = {
  32, 38, 44, 47, 44, 38,
  32, 26, 20, 15, 20, 26,
};//Hour X-Coordinate

const unsigned char Hour_Y[12] = {
  17, 19, 26, 32, 38, 45,
  47, 45, 38, 32, 26, 19,
};//Hour Y-Coordinate

void setup() {
  //INITIALIZE:
  Wire.begin();

  fnINITIALIZE_CONFIG();

  pinMode(Button_INPIN, INPUT_PULLUP);
  pinMode(Motor_OUTPIN, OUTPUT);
  pinMode(Buzzer_OUTPIN, OUTPUT);
  pinMode(Led_OUTPIN, OUTPUT);

  attachInterrupt(0, fnKEY, FALLING);

  pressure.begin();

  if (EEPROM.read(2) == 0)
  {
    bH12 = false;
    Clock.setClockMode(bH12);
  }
  else
  {
    if (EEPROM.read(2) == 1)
    {
      bH12 = true;
      Clock.setClockMode(bH12);
    }
  }

  PT_SEM_INIT(&OLED, 1);

  PT_INIT(&TIME);
  PT_INIT(&CHECK);
  PT_INIT(&BEEP);
  PT_INIT(&ALARM);
  PT_INIT(&SLEEP);
  PT_INIT(&TIMER);
  PT_INIT(&STOPWATCH);
  PT_INIT(&MEMO);
  PT_INIT(&APP);
  PT_INIT(&CONFIG);

  //BOOT LOGO:
  display.initialize();

  fnBOOT_LOGO();

  bTIME_Switch = true;
  if (EEPROM.read(11) == 1 || EEPROM.read(12) == 1 || EEPROM.read(13) == 1)
  {
    bALARM_Switch = true;
    if (EEPROM.read(1) == 1)
    {
      wdt_enable(WDTO_8S);//Set Watchdog 8S
      WDTCSR |= _BV(WDIE);//Set Watchdog Interrupt
    }
  }
  if (EEPROM.read(1) == 1)
  {
    bSLEEP_Switch = true;
  }
}



void loop() { //Check each thread by priority
  TIME_entry(&TIME);
  TIMER_entry(&TIMER);
  STOPWATCH_entry(&STOPWATCH);
  MEMO_entry(&MEMO);
  APP_entry(&APP);
  CONFIG_entry(&CONFIG);
  CHECK_entry(&CHECK);
  SLEEP_entry(&SLEEP);
  ALARM_entry(&ALARM);
  BEEP_entry(&BEEP);
}



//Other Functions
void fnBOOT_LOGO() {
  static int Tni;

  display.clear();
  analogWrite(Motor_OUTPIN, 128);
  digitalWrite(Led_OUTPIN, HIGH);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10, 2);
  display.println("SHIELD");
  display.update();
  for (Tni = 200; Tni <= 700; Tni++)
  {
    tone(Buzzer_OUTPIN, Tni);
    delay(2);
  }
  analogWrite(Motor_OUTPIN, 255);
  display.setCursor(19, 27);
  display.setTextSize(5);
  display.println("O S");
  display.update();
  for (Tni = 700; Tni >= 200; Tni--)
  {
    tone(Buzzer_OUTPIN, Tni);
    delay(4);
  }
  analogWrite(Motor_OUTPIN, 0);
  digitalWrite(Led_OUTPIN, LOW);
  noTone(Buzzer_OUTPIN);
}



void fnINITIALIZE_CONFIG () {
  if (EEPROM.read(0) != 1)
  {
    Clock.setSecond(bySecond);
    Clock.setMinute(byMinute);
    Clock.setHour(byHour);
    Clock.setDoW(byDoW);
    Clock.setDate(byDate);
    Clock.setMonth(byMonth);
    Clock.setYear(byYear);
    EEPROM.write(1, 1);
    EEPROM.write(2, 0);
    EEPROM.write(3, 12);
    EEPROM.write(4, 00);
    EEPROM.write(5, 12);
    EEPROM.write(6, 00);
    EEPROM.write(7, 12);
    EEPROM.write(8, 00);
    EEPROM.write(9, 01);
    EEPROM.write(10, 01);
    EEPROM.write(11, 0);
    EEPROM.write(12, 0);
    EEPROM.write(13, 0);
    EEPROM.write(14, dBaseline);
    EEPROM.write(15, unPower);
    EEPROM.write(16, 30);
    EEPROM.write(17, 3);
    EEPROM.write(18, 512);

    EEPROM.write(0, 1);
  }

}



void fnINITIALIZE_GY271() {
  Wire.beginTransmission(ADDR_GY271_HW);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
}



void fnREAD_GY271() {
  Wire.beginTransmission(ADDR_GY271_HW);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(ADDR_GY271_HW, 6);
  if (6 <= Wire.available())
  {
    TnX = Wire.read() << 8;
    TnX |= Wire.read();
    TnZ = Wire.read() << 8;
    TnZ |= Wire.read();
    TnY = Wire.read() << 8;
    TnY |= Wire.read();
  }
}



double fnREAD_BMP180() {
  static char Tcstatus;
  static double TdT, TdP;
  Tcstatus = pressure.startTemperature();
  if (Tcstatus != 0)
  {
    delay(Tcstatus);
    Tcstatus = pressure.getTemperature(TdT);
    if (Tcstatus != 0)
    {
      Tcstatus = pressure.startPressure(3);
      if (Tcstatus != 0)
      {
        delay(Tcstatus);
        Tcstatus = pressure.getPressure(TdP, TdT);
        if (Tcstatus != 0)
        {
          return TdP;
        }
      }
    }
  }
}



void fnQUICK_SHOW() {

}//Beta



long fnREAD_VCC() {
  static long Tlresult;
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  Tlresult = ADCL;
  Tlresult |= ADCH << 8;
  Tlresult = 1126400L / Tlresult;
  return Tlresult;
}

void fnReset() { //Reboot
  asm volatile ("  jmp 0");
}

