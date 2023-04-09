#include "RTC_NXP.h"

PCF85263A::PCF85263A( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

PCF85263A::~PCF85263A()
{
}

void PCF85263A::begin( void )
{
}

void PCF85263A::set( struct tm* now_tmp )
{
	//	refer datasheet 7.2.6
	
	time_t		now_time;
	struct tm*	cnv_tmp;

	uint8_t		bf[ 10 ];
	
	bf[ 0 ]	= 0x01;
	bf[ 1 ]	= 0xA4;
	bf[ 2 ]	= 0;
	bf[ 3 ]	= dec2bcd( now_tmp->tm_sec  );
	bf[ 4 ]	= dec2bcd( now_tmp->tm_min  );
	bf[ 5 ]	= dec2bcd( now_tmp->tm_hour );
	bf[ 6 ]	= dec2bcd( now_tmp->tm_mday );
	bf[ 8 ]	= dec2bcd( now_tmp->tm_mon + 1 );
	bf[ 9 ]	= dec2bcd( now_tmp->tm_year - 100 );

	now_time	= mktime( now_tmp );
	cnv_tmp		= localtime( &now_time );
	bf[ 5 ]		= dec2bcd( cnv_tmp->tm_wday );
	
	reg_w( Stop_enable, bf, sizeof( bf ) );
	reg_w( Stop_enable, 0x00 );
}

bool PCF85263A::oscillator_stop( void )
{
	return reg_r( Seconds ) & 0x80;
}

void PCF85263A::alarm( alarm_setting digit, int val )
{

}

#if 0
void PCF85263A::alarm( alarm_setting digit, int val )
{
	uint8_t	en;
	
	en	= ((val & 80) ? 0x0 : 0x1) << digit;
	
	reg_r( Second_alarm1 + digit, val );
	bit_op8( Alarm_enables, ~en, en );
}
#endif
void PCF85263A::alarm_clear( void ){}
void PCF85263A::alarm_disable( void )
{
	reg_w( Alarm_enables, 0x00 );
}

uint8_t PCF85263A::int_clear( void ){}

time_t PCF85263A::rtc_time( void )
{
	struct tm	now_tm;

	uint8_t		bf[ 8 ];
	
	reg_r( _100th_seconds, bf, sizeof( bf ) );
	
	now_tm.tm_sec	= bcd2dec( bf[ 1 ] );
	now_tm.tm_min	= bcd2dec( bf[ 2 ] );
	now_tm.tm_hour	= bcd2dec( bf[ 3 ] );
	now_tm.tm_mday	= bcd2dec( bf[ 4 ] );
	now_tm.tm_mon	= bcd2dec( bf[ 6 ] ) - 1;
	now_tm.tm_year	= bcd2dec( bf[ 7 ] ) + 100;
	now_tm.tm_isdst	= 0;

   return mktime(&now_tm);
}


