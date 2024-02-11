/** PCF85263A RTC operation sample
 *  
 *  This sample code is showing PCF85263A RTC operation with INTA and INT_B interrupts
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCF85263A:
 *    https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar-with-alarm-function-battery-switch-over-time-stamp-input-and-ic-bus:PCF85263A   
 */

#include <PCF85263A.h>
#include <time.h>

void set_time(void);
void int_cause_monitor(uint8_t status);

PCF85263A rtc;

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

  Wire.begin();

  Serial.println("\n***** Hello, PCF85263A! *****");

  if (rtc.oscillator_stop()) {
    Serial.println("==== oscillator_stop detected :( ====");
    set_time();
  } else {
    Serial.println("---- RTC has beeing kept running! :) ----");
  }

  rtc.pin_congfig(PCF85263A::INTA_INTTERRUPT, PCF85263A::INTB_INTTERRUPT);

  rtc.int_clear();
  pinMode(intPin0, INPUT_PULLUP);
  pinMode(intPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(intPin0), pin_int_callback0, FALLING);
  attachInterrupt(digitalPinToInterrupt(intPin1), pin_int_callback1, FALLING);

  rtc.alarm(RTC_NXP::SECOND, 37);
  rtc.periodic_interrupt_enable(PCF85263A::EVERY_SECOND, 1);
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

  if (status & 0x80) {
    Serial.print("INT:Periodic_Interrupt, ");

    time_t current_time = rtc.time(NULL);
    Serial.print("time:");
    Serial.print(current_time);
    Serial.print(" ");
    Serial.println(ctime(&current_time));
  }
  if (status & 0x40) {
    Serial.print("INT:Alarm2 ");
    Serial.println("########## ALARM2 ########## ");
  }
  if (status & 0x20) {
    Serial.print("INT:Alarm1 ");
    Serial.println("########## ALARM1 ########## ");
  }
  if (status & 0x10) {
    Serial.print("INT:WatchDog ");
  }
  if (status & 0x08) {
    Serial.print("INT:Battery_Switch ");
  }
  if (status & 0x04) {
    Serial.print("INT:Timestamp_Register_3_event ");
  }
  if (status & 0x02) {
    Serial.print("INT:Timestamp_Register_2_event ");
  }
  if (status & 0x01) {
    Serial.print("INT:Timestamp_Register_1_event ");
  }
}
