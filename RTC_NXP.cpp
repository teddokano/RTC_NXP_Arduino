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

PCF2131_I2C::PCF2131_I2C()
{
}

PCF2131_I2C::~PCF2131_I2C()
{
}

time_t PCF2131_I2C::rtc_time( void )
{
	struct tm	now_tm;
	struct tm*	cnv_tmp;
	time_t		newtime;
	
	now_tm.tm_sec	= 29;
	now_tm.tm_min	= 32;
	now_tm.tm_hour	= 15;
	now_tm.tm_mday	= 6;
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

