/** PCF2131 RTC operation sample
 *  
 *  This sample code is showing PCF2131 RTC operation using I2C interface
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCF85263A:
 *    https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar-with-alarm-function-battery-switch-over-time-stamp-input-and-ic-bus:PCF85263A   
 */

#include <time.h>
#include "RTC_NXP.h"

void set_time(void);
void int_cause_monitor(uint8_t status);

PCF85263A rtc;

const uint8_t intPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCF85063A! *****");

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    set_time();
  } else {
    Serial.println("---- RTC has beeing kept running! :) ----");
  }

  rtc.int_clear();
  pinMode(intPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin), pin_int_callback, FALLING);

  rtc.alarm(RTC_NXP::SECOND, 37);
}

void loop() {
  if (int_flag) {
    int_flag = false;
    Serial.print("[INT] ");
    int_cause_monitor(rtc.int_clear());
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

void int_cause_monitor(uint8_t status) {
  Serial.print("status:");
  Serial.print(status, HEX);

  Serial.print(", ");

  if (status & 0x08) {
    Serial.print("INT:timer, ");

    time_t current_time = rtc.time(NULL);
    Serial.print("time:");
    Serial.print(current_time);
    Serial.print(" ");
    Serial.println(ctime(&current_time));
  }
  if (status & 0x40) {
    Serial.print("INT:alarm ");
    Serial.println("########## ALARM ########## ");
  }
}
