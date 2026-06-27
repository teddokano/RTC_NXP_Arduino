/** PCF2131 RTC operation sample
 *  
 *  This sample code is showing PCF2131 RTC operation using SPI interface
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCF2131:
 *    https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-temperature-compensation/nano-power-highly-accurate-rtc-with-integrated-quartz-crystal:PCF2131   
 */

#include <PCF2131_SPI.h>
#include <time.h>

void set_time(void);

PCF2131_SPI rtc;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  Serial.println("\n***** Hello, PCF2131! (SPI interface) *****");

  delay(2000);  //  2 seconds wait for oscillator stabilization (just in case for if the PCF2131 is cold start)

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    rtc.bit_op8(PCF2131_SPI::Control_3, 0x1F, 0x00);  //  Battery switch-over function is enabled in standard mode and battery low detection function is enabled
    set_time();
  } else {
    Serial.println("---- RTC has been kept running! :) ----");
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
  const char* current_time  = "2026-6-27 18:52:30";
  const char* format  = "%Y-%m-%d %H:%M:%S";
  struct tm	tmv;
  strptime( current_time, format, &tmv );
  */

  struct tm now_tm;

  now_tm.tm_year = 2026 - 1900;
  now_tm.tm_mon = 6 - 1;  // It needs to be '5' if June
  now_tm.tm_mday = 27;
  now_tm.tm_hour = 18;
  now_tm.tm_min = 52;
  now_tm.tm_sec = 30;

  rtc.set(&now_tm);

  Serial.println("RTC got time information");
}