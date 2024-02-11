/** PCF85053A RTC operation sample
 *  
 *  This sample code is showing PCF85053A RTC operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCF85053A:
 *    https://www.nxp.com/products/analog-and-mixed-signal/real-time-clocks/rtcs-with-ic-bus/bootable-cpu-rtc-with-two-ic-buses-128-byte-sram-and-alarm-function:PCF85053A
 */

#include <PCF85053A.h>
#include <time.h>

void set_time(void);

PCF85053A rtc;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCF85053A! *****");

  //  Set read/write access right to primary I2C bus
  //  For Arduino R3 and R4 are having I2C bus on A4/A5 and D14/15
  //    but those (A4 and D14, A5 and D15) pins are tied together.
  //    https://forum.arduino.cc/t/using-i2c-and-adc-on-a4-a5-and-d14-d15/531156
  //
  //  So if user need to operate primary and secondary I2C buses
  //    on the PCF85053A, hardware must be changed properly
  //  Next line is not required to operate PCF85053A on Arduino
  //    R3 and R4 because of reason above, but to make sure and
  //    show the steps to give read/write accress right to
  //    the primary I2C bus

  rtc.bit_op8(PCF85053A::Control_Register, ~0x01, 0x01);

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    set_time();
  } else {
    Serial.println("---- RTC has beeing kept running! :) ----");
  }
}

void loop() {
  time_t current_time = 0;

  current_time = rtc.time(NULL);
  Serial.print("time : ");
  Serial.print(current_time);
  Serial.print(", ");
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

  now_tm.tm_year = 2024 - 1900;
  now_tm.tm_mon = 2 - 1;  // It needs to be '3' if April
  now_tm.tm_mday = 11;
  now_tm.tm_hour = 18;
  now_tm.tm_min = 52;
  now_tm.tm_sec = 30;

  rtc.set(&now_tm);

  Serial.println("RTC got time information");
}
