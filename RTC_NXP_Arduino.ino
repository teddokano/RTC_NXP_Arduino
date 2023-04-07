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


void pin_int_callback();
void set_time(void);
void int_cause_monitor(uint8_t* status);

//#define INTERFACE_I2C

#ifdef INTERFACE_I2C
#pragma message "########## COMPILING FOR PCF2131 with I2C INTERFACE ##########"
PCF2131_I2C rtc;
#else
#pragma message "########## COMPILING FOR PCF2131 with SPI INTERFACE ##########"
PCF2131_SPI rtc;
#endif


const uint8_t interruptPin = 2;
bool int_flag = false;


void setup() {
  Serial.begin(9600);

#ifdef INTERFACE_I2C
  Serial.println("\n***** Hello, PCF2131! (I2C interface)*****");
  Wire.begin();
#else
  Serial.println("\n***** Hello, PCF2131! (SPI interface) *****");
  SPI.begin();
#endif


  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    set_time();
  } else {
    Serial.println("RTC was kept running :)");
  }

  rtc.int_clear();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);

  rtc.periodic_interrupt_enable(PCF2131_base::EVERY_SECOND);
  rtc.alarm( PCF2131_base::SECOND, 15 );
}


void loop() {
  if (int_flag) {
    int_flag = false;

    uint8_t status[3];
    rtc.int_clear(status);
    int_cause_monitor(status);

    time_t current_time = rtc.time(NULL);
    Serial.print("time:");
    Serial.print(current_time);
    Serial.print(" ");
    Serial.println(ctime(&current_time));
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

  rtc.rtc_set(&now_tm);

  Serial.println("RTC got time information");
}

void pin_int_callback() {
  int_flag = true;
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
  }
  if (status[0] & 0x40) {
    Serial.print("INT:watchdog, ");
  }
  if (status[0] & 0x10) {
    Serial.print("INT:alarm, ");
  Serial.print("########## ALARM ########## ");
  }
  if (status[1] & 0x08) {
    Serial.print("INT:battery switch over, ");
  }
  if (status[1] & 0x04) {
    Serial.print("INT:battery low, ");
  }
  if (status[2] & 0x80) {
    Serial.print("INT:timestamp1, ");
  }
  if (status[2] & 0x40) {
    Serial.print("INT:timestamp2, ");
  }
  if (status[2] & 0x20) {
    Serial.print("INT:timestamp3, ");
  }
  if (status[2] & 0x10) {
    Serial.print("INT:timestamp4, ");
  }
}