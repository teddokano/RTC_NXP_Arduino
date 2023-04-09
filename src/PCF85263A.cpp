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
	alarm( digit, val, 0 );
}

void PCF85263A::alarm( alarm_setting digit, int val, int int_sel )
{
	if ( WEEKDAY == digit ) {
		Serial.println("*** 'WEEKDAY' for PCF85263A is unsupported alarm setting ***");		
		return;
	}

	set_alarm( digit, val, int_sel );
}

void PCF85263A::alarm( alarm_setting_85263A digit, int val, int int_sel )
{
	set_alarm( digit, val, int_sel );
}

void PCF85263A::set_alarm( int digit, int val, int int_sel )
{
	uint8_t	en;

	en	= ((val & 80) ? 0x0 : 0x1) << digit;
	
	reg_w( Second_alarm1 + digit, val );
	bit_op8( Alarm_enables, ~en, en );
	
	uint8_t en_bit	= (digit < 5) ? 0x10 : 0x80;
	bit_op8( INTA_enable + int_sel, ~en_bit, en_bit );	
}

void PCF85263A::alarm_clear( void )
{
	reg_w( Alarm_enables, 0x00 );
}

void PCF85263A::alarm_disable( void )
{
	bit_op8( INTA_enable, ~0x18, 0x18 );	
	bit_op8( INTB_enable, ~0x18, 0x18 );	
}

uint8_t PCF85263A::int_clear( void )
{
	uint8_t	status	= reg_r( Flags );
	
	reg_w( Flags, ~status );
	return status;
}

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

void PCF85263A::periodic_interrupt_enable( periodic_int_select sel, int int_sel )
{
	bit_op8( Function, ~0x60, sel << 5 );
	bit_op8( Function, ~0x60, sel << 5 );
	
	bit_op8( INTA_enable + int_sel, ~0x40, (sel ? 1 : 0) << 6 );
}