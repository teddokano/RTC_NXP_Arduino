#include "RTC_NXP.h"

void warning( void )
{
	Serial.println( "** NO HARDWARE SUPPORT AVAILABLE FOR PCF85063TP **" );	
}


PCF85063TP::PCF85063TP( uint8_t i2c_address ) : PCF85063A( i2c_address )
{
}

PCF85063TP::PCF85063TP( TwoWire& wire, uint8_t i2c_address ) : PCF85063A( wire, i2c_address )
{
}

PCF85063TP::~PCF85063TP()
{	
}
