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
  Serial.println("\n***** Hello, PCF2131! *****");

  Wire.begin();

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected ====");
  } else {
  }
}

void loop() {
  time_t current_time = 0;

  current_time = rtc.time(NULL);
  Serial.print("time : ");
  Serial.println(current_time);
  Serial.println(ctime(&current_time));

  delay(1000);
}
