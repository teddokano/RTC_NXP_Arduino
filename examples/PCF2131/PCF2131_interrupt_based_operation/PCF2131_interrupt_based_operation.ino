/** PCF2131 RTC operation sample
 *  
 *  Demonstrates PCF2131 using 2 interrupt lines
 *
 *  *** IMPORTANT 0 ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCF2131-ARD BOARDS, PIN8 and PIN9 MUST BE SHORTED TO PIN2 and PIN3 (RESPECTIVELY) TO HANDLE INTERRUPT CORRECTLY
 *
 *  *** IMPORTANT 1 ***
 *  *** TO USE "SPI INTERFACE", COMMENT OUT THE LINE OF "#define INTERFACE_I2C"
 *
 *  *** IMPORTANT 2 ***
 *  *** COMPILATION WARNING MESSAGE IS INTENTIONALLY ADDED TO TELL USER THAT WHICH INTERFACE OPTION IS SELECTED :)
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCF2131:
 *    https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-temperature-compensation/nano-power-highly-accurate-rtc-with-integrated-quartz-crystal:PCF2131   
 */

#include <PCF2131_I2C.h>
#include <PCF2131_SPI.h>
#include <time.h>


void set_time(void);
void int_cause_monitor(uint8_t* status);

#define INTERFACE_I2C

#ifdef INTERFACE_I2C
#pragma message : == == == == == COMPILING FOR PCF2131 with I2C INTERFACE == == == == ==
PCF2131_I2C rtc;
#else
#pragma message : == == == == == COMPILING FOR PCF2131 with SPI INTERFACE == == == == ==
PCF2131_SPI rtc;
#endif

#pragma message : TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCF2131 - ARD BOARDS, PIN8 and PIN9 MUST BE SHORTED TO PIN2 and PIN3(RESPECTIVELY) TO HANDLE INTERRUPT CORRECTLY

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
  while (!Serial)
    ;

#ifdef INTERFACE_I2C
  Serial.println("\n***** Hello, PCF2131! (I2C interface) *****");
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
    Serial.println("---- RTC has beeing kept running! :) ----");
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
  rtc.alarm(PCF2131_base::SECOND, 37, 1);

  Serial.println("    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println("    ***   D2<--->D8, D3<--->D9 are needed to be connected       ***");
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