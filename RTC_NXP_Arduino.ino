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

//#define INTERFACE_I2C

#ifdef  INTERFACE_I2C
#pragma message "########## COMPILING FOR PCF2131 with I2C INTERFACE ##########"
PCF2131_I2C rtc;
#else
#pragma message "########## COMPILING FOR PCF2131 with SPI INTERFACE ##########"
PCF2131_SPI rtc;
#endif

void setup() {
  Serial.begin(9600);

#ifdef  INTERFACE_I2C
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
}

void loop() {
  time_t current_time = 0;

  current_time = rtc.time(NULL);
  Serial.print("time : ");
  Serial.print(current_time);
  Serial.print("    ");
  Serial.println(ctime(&current_time));

  delay(1000);
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