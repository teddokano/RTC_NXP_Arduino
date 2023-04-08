/** PCA9955B LED driver operation sample
 *  
 *  This sample code is showing PCA9955B LED driver operation with PWM value setting change.
 *  The ledd.pwm() takes channel number and PWM ratio. 
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCA9955B:
 *    https://www.nxp.jp/products/power-management/lighting-driver-and-controller-ics/led-drivers/24-channel-spi-serial-bus-32-ma-5-5-v-constant-current-led-driver:PCA9955B
 */

#include <time.h>
#include "RTC_NXP.h"


void set_time(void);
void int_cause_monitor(uint8_t* status);

//#define INTERFACE_I2C

#ifdef INTERFACE_I2C
#pragma message : == == == == == COMPILING FOR PCF2131 with I2C INTERFACE == == == == ==
PCF2131_I2C rtc;
#else
#pragma message : == == == == == COMPILING FOR PCF2131 with SPI INTERFACE == == == == ==
PCF2131_SPI rtc;
#endif

#pragma message : TO RUN THIS SKETCH ON ARDUINO UNO R3P AND CF2131 - ARD BOARDS, PIN8 and PIN9 MUST BE SHORTED TO HANDLE INTERRUPT CORRECTLY

const uint8_t intPin0 = 2;
const uint8_t intPin1 = 3;
bool int_flag0 = false;
bool int_flag1 = false;

void pin_int_callback0() {
  int_flag0 = true;
}

void pin_int_callback1() {
  int_flag1 = true;
}

void setup() {
  Serial.begin(9600);

#ifdef INTERFACE_I2C
  Serial.println("\n***** Hello, PCF2131! (I2C interface)*****");
  Wire.begin();
#else
  Serial.println("\n***** Hello, PCF2131! (SPI interface) *****");
  SPI.begin();
#endif

  rtc.begin();

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    set_time();
  } else {
    Serial.println("RTC was kept running :)");
  }

  rtc.int_clear();
  pinMode(intPin0, INPUT_PULLUP);
  pinMode(intPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin0), pin_int_callback0, FALLING);
  attachInterrupt(digitalPinToInterrupt(intPin1), pin_int_callback1, FALLING);

  rtc.timestamp(1, PCF2131_base::LAST, 1);
  rtc.timestamp(2, PCF2131_base::LAST, 1);
  rtc.timestamp(3, PCF2131_base::LAST, 1);
  rtc.timestamp(4, PCF2131_base::LAST, 1);

  rtc.periodic_interrupt_enable(PCF2131_base::EVERY_SECOND);
  rtc.alarm(PCF2131_base::SECOND, 15, 1);
}


void loop() {
  if (int_flag0 || int_flag1) {
    if (int_flag0) {
      int_flag0 = false;
      Serial.print("[INT-A] ");
    }
    if (int_flag1) {
      int_flag1 = false;
      Serial.print("[INT-B] ");
    }

    uint8_t status[3];
    rtc.int_clear(status);
    int_cause_monitor(status);
  }
}

void set_time(void) {
  /*  !!!! "strptime()" is not available in Arduino's "time.h" !!!!
  const char* current_time  = "2023-4-7 05:25:30";
  const char* format  = "%Y-%m-%d %H:%M:%S";
  struct tm	tmv;
  strptime( current_time, format, &tmv );
  */

  struct tm now_tm;

  now_tm.tm_year = 2023 - 1900;
  now_tm.tm_mon = 4 - 1;  // It needs to be '3' if April
  now_tm.tm_mday = 7;
  now_tm.tm_hour = 18;
  now_tm.tm_min = 52;
  now_tm.tm_sec = 30;

  rtc.set(&now_tm);

  Serial.println("RTC got time information");
}

void int_cause_monitor(uint8_t* status) {
  Serial.print("status:");

  for (int i = 0; i < 3; i++) {
    Serial.print(" ");
    Serial.print(status[i], HEX);
  }
  Serial.print(", ");

  if (status[0] & 0x80) {
    Serial.print("INT:every min/sec, ");

    time_t current_time = rtc.time(NULL);
    Serial.print("time:");
    Serial.print(current_time);
    Serial.print(" ");
    Serial.println(ctime(&current_time));
  }
  if (status[0] & 0x40) {
    Serial.print("INT:watchdog");
  }
  if (status[0] & 0x10) {
    Serial.print("INT:alarm ");
    Serial.println("########## ALARM ########## ");
  }
  if (status[1] & 0x08) {
    Serial.print("INT:battery switch over");
  }
  if (status[1] & 0x04) {
    Serial.print("INT:battery low");
  }
  if (status[2] & 0xF0) {
    for (int i = 0; i < 4; i++) {
      if (status[2] & (0x80 >> i)) {
        Serial.print("INT:timestamp");
        Serial.print(i + 1);
        Serial.println("");
      }
    }
    for (int i = 0; i < 4; i++) {
      Serial.print("  TIMESTAMP");
      Serial.print(i + 1);
      Serial.print(": ");
      time_t ts = rtc.timestamp(i + 1);
      Serial.println(ctime(&ts));
    }
  }
}