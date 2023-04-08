#include "RTC_NXP.h"

void warning( void )
{
	Serial.println( "** NO HARDWARE SUPPORT AVAILABLE FOR PCF85063TP **" );	
}


PCF85063TP::PCF85063TP( uint8_t i2c_address ) : PCF85063A( i2c_address )
{
}

PCF85063TP::~PCF85063TP()
{	
}

void PCF85063TP::alarm( alarm_setting digit, int val )
{
	(void)digit; // suppressing warning
	(void)val;   // suppressing warning
	warning();
}

void PCF85063TP::alarm_clear( void )
{
	warning();
}

void PCF85063TP::alarm_disable( void )
{
	warning();
}

uint8_t PCF85063TP::int_clear( void )
{
	warning(); // suppressing warning
	return 0;
}

float PCF85063TP::timer( float period )
{
	(void)period;
	warning();
	return 0;
}
