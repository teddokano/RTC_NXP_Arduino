#include "RTC_NXP.h"

PCF85063A::PCF85063A( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

PCF85063A::~PCF85063A()
{	
}

void PCF85063A::begin( void )
{
}

void PCF85063A::set( struct tm* now_tmp )
{
	time_t		now_time;
	struct tm*	cnv_tmp;

	uint8_t		bf[ 7 ];
	
	bf[ 0 ]	= dec2bcd( now_tmp->tm_sec  );
	bf[ 1 ]	= dec2bcd( now_tmp->tm_min  );
	bf[ 2 ]	= dec2bcd( now_tmp->tm_hour );
	bf[ 3 ]	= dec2bcd( now_tmp->tm_mday );
	bf[ 5 ]	= dec2bcd( now_tmp->tm_mon + 1 );
	bf[ 6 ]	= dec2bcd( now_tmp->tm_year - 100 );

	now_time	= mktime( now_tmp );
	cnv_tmp		= localtime( &now_time );
	bf[ 4 ]		= dec2bcd( cnv_tmp->tm_wday);
	
	bit_op8( Control_1, ~0x20, 0x20 );
	reg_w( Seconds, bf, sizeof( bf ) );
	bit_op8( Control_1, ~0x20, 0x00 );
}

bool PCF85063A::oscillator_stop( void )
{
	return reg_r( Seconds ) & 0x80;
}

void PCF85063A::alarm( alarm_setting digit, int val )
{
	int	v = (val == 0x80) ? 0x80 : dec2bcd( val );
	reg_w( Second_alarm + digit, v );
	bit_op8( Control_2, (uint8_t)(~0x80), 0x80 );
}

void PCF85063A::alarm_clear( void )
{
	//	will be implemented later
}

void PCF85063A::alarm_disable( void )
{
	bit_op8( Control_2, (uint8_t)(~0x80), 0x00 );	
}

uint8_t PCF85063A::int_clear( void )
{
	uint8_t v = reg_r( Control_2 );
	reg_w( Control_2, v & ~0x48 );

	return v;
}

float PCF85063A::timer( float period )
{
	float	sf[] = { 1 / 4096.0, 1 / 64.0, 1.0, 60 };
	int		tcf;
	
	period	= ((255 * 60.0) < period) ? 255 * 60.0 : period;
	
	for ( tcf = 0; tcf < 4; tcf++ ) {
		if ( period <= sf[ tcf ] * 255 )
			break;
	}

	uint8_t	v	= (uint8_t)(period / sf[ tcf ]);

#if 0
	Serial.print("  period:");
	Serial.print( period, 10 );
	Serial.print("  tcf:");
	Serial.print( tcf );
	Serial.print("  v:");
	Serial.print( v );
	Serial.print("  v*tcf:");
	Serial.print( v * sf[tcf], 10 );
	Serial.println( "" );
#endif
	
	reg_w( Timer_value, v );
	reg_w( Timer_mode, tcf << 3 | 0x06 );
	
	return v * sf[tcf];
}

time_t PCF85063A::rtc_time( void )
{
	struct tm	now_tm;

	uint8_t		bf[ 7 ];
	
	reg_r( Seconds, bf, sizeof( bf ) );
	
	now_tm.tm_sec	= bcd2dec( bf[ 0 ] );
	now_tm.tm_min	= bcd2dec( bf[ 1 ] );
	now_tm.tm_hour	= bcd2dec( bf[ 2 ] );
	now_tm.tm_mday	= bcd2dec( bf[ 3 ] );
	now_tm.tm_mon	= bcd2dec( bf[ 5 ] ) - 1;
	now_tm.tm_year	= bcd2dec( bf[ 6 ] ) + 100;
	now_tm.tm_isdst	= 0;

   return mktime(&now_tm);
}
