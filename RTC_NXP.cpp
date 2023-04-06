#include "RTC_NXP.h"

RTC_NXP::RTC_NXP()
{
}

RTC_NXP::~RTC_NXP()
{
}

time_t RTC_NXP::time( time_t* tp )
{
	time_t	t	= rtc_time();
	if ( tp )
		*tp	= t;
	return t;
}

uint8_t	RTC_NXP::bcd2dec( uint8_t v )
{
	return (v >> 4) * 10 + (v & 0x0F);
}

uint8_t	RTC_NXP::dec2bcd( uint8_t v )
{
	return ((v / 10) << 4) + (v % 10);
}

PCF2131_I2C::PCF2131_I2C( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

PCF2131_I2C::~PCF2131_I2C()
{
}

time_t PCF2131_I2C::rtc_time()
{
	struct tm	now_tm;
	struct tm*	cnv_tmp;
	time_t		newtime;

	uint8_t		bf[ 8 ];
	
	reg_r( _100th_Seconds, bf, sizeof( bf ) );
	
	now_tm.tm_sec	= bcd2dec( bf[ 1 ] );
	now_tm.tm_min	= bcd2dec( bf[ 2 ] );
	now_tm.tm_hour	= bcd2dec( bf[ 3 ] );
	now_tm.tm_mday	= bcd2dec( bf[ 4 ] );
	now_tm.tm_mon	= bcd2dec( bf[ 6 ] ) - 1;
	now_tm.tm_year	= bcd2dec( bf[ 7 ] ) + 100;
	now_tm.tm_isdst	= 0;

	newtime	= mktime(&now_tm);
	cnv_tmp	= localtime( &newtime );
	
	Serial.println( "PCF2131_I2C" );
	Serial.println( cnv_tmp->tm_wday );
	Serial.println( cnv_tmp->tm_yday );

   return newtime;
}

time_t PCF2131_I2C::rtc_set( void )
{
	struct tm	now_tm;
	struct tm*	cnv_tmp;
	time_t		newtime;
	
	now_tm.tm_sec	= 29;
	now_tm.tm_min	= 32;
	now_tm.tm_hour	= 15;
	now_tm.tm_mday	= 7;
	now_tm.tm_mon	= 3;
	now_tm.tm_year	= 2023 - 1900;
	now_tm.tm_isdst	= 0;

	newtime	= mktime(&now_tm);
	cnv_tmp	= localtime( &newtime );
	
	Serial.println( "PCF2131_I2C" );
	Serial.println( cnv_tmp->tm_wday );
	Serial.println( cnv_tmp->tm_yday );

   return newtime;
}

