#include "RTC_NXP.h"

PCF2131_SPI::PCF2131_SPI()
{
}

PCF2131_SPI::~PCF2131_SPI()
{
}

void PCF2131_SPI::_reg_w( uint8_t reg, uint8_t *vp, int len )
{
	reg_w( reg, vp, len );
}

void PCF2131_SPI::_reg_r( uint8_t reg, uint8_t *vp, int len )
{
	reg_r( reg, vp, len );
}

void PCF2131_SPI::_reg_w( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t PCF2131_SPI::_reg_r( uint8_t reg )
{
	return 	reg_r( reg );
}

void PCF2131_SPI::_bit_op8( uint8_t reg, uint8_t mask, uint8_t val )
{
	bit_op8( reg, mask, val );
}
