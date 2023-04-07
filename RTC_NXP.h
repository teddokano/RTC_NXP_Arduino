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
	/** time
	 *
	 * @param tp pointer to time_t variable
	 * @return time_t value of current time
	 */
	time_t time( time_t* tp );
	virtual time_t rtc_time( void )	= 0;
	virtual bool oscillator_stop( void )	= 0;

protected:
	static uint8_t	bcd2dec( uint8_t v );
	static uint8_t	dec2bcd( uint8_t v );

	virtual void w_seq( uint8_t reg, uint8_t *vp, int len )	= 0;
	virtual void r_seq( uint8_t reg, uint8_t *vp, int len )	= 0;
	virtual void w_reg( uint8_t reg, uint8_t val )	= 0;
	virtual uint8_t r_reg( uint8_t reg )	= 0;
	virtual void ow_reg( uint8_t reg, uint8_t mask, uint8_t val )	= 0;
private:
};



/** PCF2131_I2C class
 *	
 *  @class PCF2131_I2C
 *
 */

class PCF2131_base : public RTC_NXP
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
	
	PCF2131_base();
	virtual ~PCF2131_base();
	
	bool oscillator_stop( void );

	time_t rtc_time( void );
	int rtc_set( struct tm* now_tm );
};

class PCF2131_I2C : public PCF2131_base, public I2C_device
{
public:
	PCF2131_I2C( uint8_t i2c_address = (0xA6 >> 1) );
	virtual ~PCF2131_I2C();
	
private:
	void w_seq( uint8_t reg, uint8_t *vp, int len );
	void r_seq( uint8_t reg, uint8_t *vp, int len );
	void w_reg( uint8_t reg, uint8_t val );
	uint8_t r_reg( uint8_t reg );
	void ow_reg( uint8_t reg, uint8_t mask, uint8_t val );
};

#endif //	ARDUINO_LED_DRIVER_NXP_ARD_H


class SPI_for_RTC
{
public:
	/** Send data
	 * 
	 * @param data pointer to data buffer
	 * @param size data size
	 * @param stop option: generating STOP-condition after transaction. Defailt: true
	 * @return transferred data size
	 */
	void txrx( uint8_t *data, uint16_t size );

	/** Multiple register write
	 * 
	 * @param reg register index/address/pointer
	 * @param data pointer to data buffer
	 * @param size data size
	 * @return transferred data size
	 */
	void reg_w( uint8_t reg_adr, uint8_t *data, uint16_t size );

	/** Single register write
	 * 
	 * @param reg register index/address/pointer
	 * @param data pointer to data buffer
	 * @param size data size
	 * @return transferred data size
	 */
	void reg_w( uint8_t reg_adr, uint8_t data );

	/** Multiple register read
	 * 
	 * @param reg register index/address/pointer
	 * @param data pointer to data buffer
	 * @param size data size
	 * @return transferred data size
	 */
	void reg_r( uint8_t reg_adr, uint8_t *data, uint16_t size );

	/** Single register read
	 * 
	 * @param reg register index/address/pointer
	 * @return read data
	 */
	uint8_t	reg_r( uint8_t reg_adr );

	/** Register write, 8 bit
	 *
	 * @param reg register index/address/pointer
	 * @param val data value
	 */
	void write_r8( uint8_t reg, uint8_t val );

	/** Register read, 8 bit
	 *
	 * @param reg register index/address/pointer
	 * @return data value
	 */
	uint8_t read_r8( uint8_t reg );

	/** Register overwriting with bit-mask
	 *	
	 *	Register can be updated by bit level
	 *
	 * @param reg register index/address/pointer
	 * @param mask bit-mask to protect overwriting
	 * @param value value to overwrite
	 */
	void bit_op8(  uint8_t reg,  uint8_t mask,  uint8_t value );
};

class PCF2131_SPI : public PCF2131_base, public SPI_for_RTC
{
public:
	PCF2131_SPI();
	virtual ~PCF2131_SPI();
	
private:
	void txrx( uint8_t *data, uint16_t size );
	void w_seq( uint8_t reg, uint8_t *vp, int len );
	void r_seq( uint8_t reg, uint8_t *vp, int len );
	void w_reg( uint8_t reg, uint8_t val );
	uint8_t r_reg( uint8_t reg );
	void ow_reg( uint8_t reg, uint8_t mask, uint8_t val );
};

