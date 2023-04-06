/** RTC operation library for Arduino
 *
 *  @class  RTC_NXP
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_LED_DRIVER_NXP_ARD_H
#define ARDUINO_LED_DRIVER_NXP_ARD_H

#include <Arduino.h>
#include <stdint.h>
#include <time.h>

#include <I2C_device.h>
#include <SPI.h>

/** RTC_NXP class
 *	
 *  @class RTC_NXP
 *
 *	RTC_NXP class is a base class for all RTC drivers
 *	All actual device class will be derived from this class
 */

class RTC_NXP
{
public:
	enum board {
		NONE,
		ARDUINO_SHIELD,
	};
	
	RTC_NXP( uint8_t n_ch, uint8_t PWM_r, uint8_t oe );
	virtual ~RTC_NXP();
	virtual void reg_access( uint8_t reg, uint8_t val  )	= 0;
	virtual void reg_access( uint8_t reg, uint8_t *vp, int len )	= 0;

	/** time
	 *
	 * @param tp pointer to time_t variable
	 * @return time_t value of current time
	 */
	time_t time( uint8_t ch, float value );

protected:
private:
};



/** PCF2131_I2C class
 *	
 *  @class PCF2131_I2C
 *
 */

class PCF2131_I2C : public RTC_NXP
{
public:
	enum reg_num {
		MODE1, MODE2,
		LEDOUT0, LEDOUT1, LEDOUT2, LEDOUT3, LEDOUT4, LEDOUT5,
		GRPPWM, GRPFREQ,
		PWM0,  PWM1,  PWM2,  PWM3,  PWM4,  PWM5,
		PWM6,  PWM7,  PWM8,  PWM9,  PWM10, PWM11,
		PWM12, PWM13, PWM14, PWM15, PWM16, PWM17,
		PWM18, PWM19, PWM20, PWM21, PWM22, PWM23,
		IREF0,  IREF1,  IREF2,  IREF3,  IREF4,  IREF5,
		IREF6,  IREF7,  IREF8,  IREF9,  IREF10, IREF11,
		IREF12, IREF13, IREF14, IREF15, IREF16, IREF17,
		IREF18, IREF19, IREF20, IREF21, IREF22, IREF23,
		OFFSET,
		SUBADR1, SUBADR2, SUBADR3, ALLCALLADR,
		PWMALL, IREFALL,
		EFLAG0, EFLAG1, EFLAG2, EFLAG3, EFLAG4, EFLAG5
	};
	PCF2131_I2C( uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe = 8 );
	virtual ~PCF2131_I2C();
	
	virtual void begin( float current =  0.1, board env = NONE, bool buffered = false );
	virtual void init( float current )	= 0;

protected:
};

#endif //	ARDUINO_LED_DRIVER_NXP_ARD_H
