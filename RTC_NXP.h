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
	
	RTC_NXP();
	virtual ~RTC_NXP();
//	virtual void reg_access( uint8_t reg, uint8_t val  )	= 0;
//	virtual void reg_access( uint8_t reg, uint8_t *vp, int len )	= 0;

	/** time
	 *
	 * @param tp pointer to time_t variable
	 * @return time_t value of current time
	 */
	time_t time( time_t* tp );
	virtual time_t rtc_time( void )	= 0;

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
		Control_1, Control_2, Control_3, Control_4, Control_5,
		SR_Reset,
		_100th_Seconds, Seconds, Minutes,Hours, Days, Weekdays, Months, Years,
		Second_alarm, Minute_alarm, Hour_alarm, Day_alarm, Weekday_alarm,
		CLKOUT_ctl,
		Timestp_ctl1, Sec_timestp1, Min_timestp1, Hour_timestp1, Day_timestp1, Mon_timestp1, Year_timestp1,
		Timestp_ctl2, Sec_timestp2, Min_timestp2, Hour_timestp2, Day_timestp2, Mon_timestp2, Year_timestp2,
		Timestp_ctl3, Sec_timestp3, Min_timestp3, Hour_timestp3, Day_timestp3, Mon_timestp3, Year_timestp3,
		Timestp_ctl4, Sec_timestp4, Min_timestp4, Hour_timestp4, Day_timestp4, Mon_timestp4, Year_timestp4,
		Aging_offset,
		INT_A_MASK1, INT_A_MASK2, INT_B_MASK1, INT_B_MASK2,
		Watchdg_tim_ctl, Watchdg_tim_val
	};
	
	PCF2131_I2C();
	virtual ~PCF2131_I2C();
	
//	virtual void begin( float current =  0.1, board env = NONE, bool buffered = false );
//	virtual void init( float current )	= 0;

	time_t rtc_time( void );

};

#endif //	ARDUINO_LED_DRIVER_NXP_ARD_H
