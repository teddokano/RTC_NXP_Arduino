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

PCF2131_I2C rtc;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, RTC! *****");

  Wire.begin();
}

time_t setTime(void) {
  struct tm now_tm;
  struct tm* cnv_tmp;

  now_tm.tm_sec = 29;
  now_tm.tm_min = 32;
  now_tm.tm_hour = 15;
  now_tm.tm_mday = 6;
  now_tm.tm_mon = 3;
  now_tm.tm_year = 123;
  now_tm.tm_isdst = 0;

  time_t newtime = mktime(&now_tm);
  cnv_tmp  = localtime( &newtime );
  
  Serial.println(cnv_tmp->tm_wday);
  Serial.println(cnv_tmp->tm_yday);

 return newtime;
 // return 0L;
}

time_t time(time_t* tp) {
  uint32_t day = (3600L * 24L);
  time_t ltime = day * 365L * 23L + day * 101L;

  *tp = ltime;
  return ltime;
}

void loop() {
  struct tm* newtime;
  time_t ltime = 0;

  //  time(&ltime);
  //  ltime = time(NULL);
  ltime = rtc.time( NULL );
  Serial.print("time : ");
  Serial.println(ltime);


  newtime = localtime(&ltime);
  // Serial.print("time : ");
  Serial.println(ctime(&ltime));
  Serial.println(asctime(newtime));

  delay(1000);
}
