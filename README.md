# RTC_NXP_Arduino
RTC device operation sample code for [Arduino](https://www.arduino.cc) 

> **Note**
This library works with [`I2C_device`](https://github.com/teddokano/I2C_device_Arduino) library together. Please be sure the `I2C_device` library is imported in your environment before trying to build. 

![Boards](https://github.com/teddokano/additional_files/blob/main/RTC_NXP_Arduino/boards4.jpg)  
_PCF2131-ARD, PCF85063TP-ARD, PCF85063AT-ARD and PCF85263ATL-ARD : Arduino® Shield Evaluation Board for RTCs_


## What is this?
An Arduino library for RTC devices with its sample code.   
A method of `time()` in each RTC device classes are cpmpatible to `time()` function in 'time.h'. The method is defined as same as `time_t time( time_t* tp )`.  
It returns a value in the systems time stamp format of 'time.h'.  

To display the calendar/time, it can be written in this way. 

```cpp
#include <PCF2131_I2C.h>  // including PCF2131_I2C class
#include <time.h>

PCF2131_I2C rtc;
...
..
current_time = rtc.time( NULL );
Serial.println( ctime( &current_time ) );  // Result will appear like -> "Sun Apr 09 07:11:37 2023"
```

## Supported devices
PCF2131, PCF85063A, PCF85063TP and PCF85263A are supported.  
On PCF2131, both I²C and SPI can be used for interfacing MCU. 
Include device name header file (`PCF85063A.h`, `PCF85063TP.h`, `PCF85063TP.h` and for PCF2131, `PCF2131_I2C.h`, `PCF2131_SPI.h`) to use those class libraries. 

Type#|Header file|Features|Interface|Evaluation board
---|---|---|---|---
[PCF2131](https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-temperature-compensation/nano-power-highly-accurate-rtc-with-integrated-quartz-crystal:PCF2131)	|`PCF2131_I2C.h`<br/>`PCF2131_SPI.h`	|Nano-Power Highly Accurate RTC with Integrated Quartz Crystal	|I²C and SPI	|[PCF2131/PCA2131 Arduino® Shield Evaluation Board](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcf2131-pca2131-arduino-shield-evaluation-board:PCF2131-ARD)
[PCF85053A](https://www.nxp.com/products/analog-and-mixed-signal/real-time-clocks/rtcs-with-ic-bus/bootable-cpu-rtc-with-two-ic-buses-128-byte-sram-and-alarm-function:PCF85053A)						|`PCF85053A.h`	|Bootable CPU RTC with Two I2C Buses, 128 Byte SRAM and Alarm Function				|I²C			|[PCF85053ATK-ARD – PCF85053ATK Evaluation Board](https://www.nxp.com/design/design-center/development-boards/analog-toolbox/arduino-shields-solutions/pcf85053atk-ard-pcf85053atk-evaluation-board:PCF85053ATK-ARD)
[PCF85063A](https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar-with-alarm-function-and-ic-bus:PCF85063A)					|`PCF85063A.h`	|Tiny Real-Time Clock/Calendar with Alarm Function				|I²C			|[PCF85063AT-ARD Arduino® Shield Evaluation Board](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcf85063at-ard-arduino-shield-tiny-real-time-clock:PCF85063AT-ARD)
[PCF85063TP](https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar:PCF85063TP)												|`PCF85063TP.h`		|Tiny Real-Time Clock/Calendar									|I²C			|[PCF85063TP-ARD Arduino® Shield Evaluation Board](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcf85063tp-ard-arduino-shield-evaluation-board:PCF85063TP-ARD)
[PCF85263A](https://www.nxp.com/products/peripherals-and-logic/signal-chain/real-time-clocks/rtcs-with-ic-bus/tiny-real-time-clock-calendar-with-alarm-function-battery-switch-over-time-stamp-input-and-ic-bus:PCF85263A)	|`PCF85263A.h`		|Tiny Real-Time Clock/Calendar with Alarm Function, Battery Switch-Over, Time Stamp Input	|I²C|[PCF85263ATL-ARD Arduino® Shield Evaluation Board](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcf85263atl-ard-arduino-shield-tiny-real-time-clock:PCF85263ATL-ARD)

## Code sample

```cpp
// Very simple sample to use the 'PCF2131-ARD : Arduino® Shield Evaluation Board'

#include <PCF2131_I2C.h>
#include <time.h>

void set_time(void);

PCF2131_I2C rtc;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCF2131! (I2C interface)*****");

  Wire.begin();

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
...
..
```

# Getting started

Use Library manager in Arduino IDE for easy install


# What's inside?

## Examples
Examples are provided as scketch files.

### How to use?

After library install, Choose menu on Arduino-IDE: `File`→`Examples`→`RTC_NXP_Arduino`→ **sketch for your try**

### List of sample code

Sketch|Target|Feature
---|---|---
PCF2131_simple_I2C					|PCF2131	|**Simple** sample for just getting current time **via I²C** in every second
PCF2131_simple_SPI					|PCF2131	|**Simple** sample for just getting current time **via SPI** in every second
PCF2131_interrupt_based_operation	|PCF2131	|**Interrupt based** operation: Demonstrates second/minute, alarm and timestamp features.<br/>**IMPORTANT:** On the PCF2131-ARD shield board, **short D8<-->D2 and D9<-->D3 pins**
PCF85053A_simple					|PCF85053A	|**Simple** sample for just getting current time in every second
PCF85053A_interrupt_based_operation	|PCF85053A	|**Interrupt based** operation: Demonstrates alarm feature
PCF85063A_simple					|PCF85063A	|**Simple** sample for just getting current time in every second
PCF85063A_interrupt_based_operation	|PCF85063A	|**Interrupt based** operation: Demonstrates timer and alarm features
PCF85063TP							|PCF85063TP	|**Simple** sample for just getting current time in every second
PCF85263A_simple					|PCF85263A	|**Simple** sample for just getting current time in every second
PCF85263A_interrupt_based_operation	|PCF85263A	|**Interrupt based** operation: Demonstrates using 2 interrupt lines
PCF85263A_timestamp					|PCF85263A	|**Timestamp** operation: Demonstrates timestamp feature

### TIPS
If you need to use different I²C bus on Arduino, it can be done like this. This sample shows how the `Wire1` on Arduino Due can be operated.  
```cpp
include <PCF2131_I2C.h>

PCF2131_I2C rtc(Wire1);

void setup() {
  Wire1.begin();
```

# Document

For details of the library, please find descriptions in [this document](https://teddokano.github.io/RTC_NXP_Arduino/annotated.html).

# References

## Related libraries
Library|Feature|Target devices|Required library
---|---|---|---
[GPIO_NXP_Arduino](https://github.com/teddokano/GPIO_NXP_Arduino)				|GPIO libraries						|PCAL6408A, PCAL6416A, PCAL6524, PCAL6534, PCA9555, PCA9554	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[LCDDriver_NXP_Arduino](https://github.com/teddokano/LCDDriver_NXP_Arduino)		|LCD driver libraries				|PCA8561						|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[LEDDriver_NXP_Arduino](https://github.com/teddokano/LEDDriver_NXP_Arduino)		|LED driver libraries				|PCA9955B, PCA9956B, PCA9957	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[MUX_SW_NXP_Arduino](https://github.com/teddokano/MUX_SW_NXP_Arduino)			|I²C mux/switch libraries			|PCA9846						|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[RTC_NXP_Arduino](https://github.com/teddokano/RTC_NXP_Arduino)					|RTC libraries						|PCF2131, PCF85053A, PCF85063A, PCF85063TP, PCF85263A	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[TempSensor_NXP_Arduino](https://github.com/teddokano/TempSensor_NXP_Arduino)	|Temperature sensor libraries		|LM75B, P3T1084, P3T1085, P3T1035, P3T2030, PCT2075	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)			|Base library for I²C operations	|none (can be applied as base class for all I²C targets)|---
