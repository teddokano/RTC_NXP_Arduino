#include "AFE_NXP.h"

void SPI_for_AFE::txrx( uint8_t *data, int size )
{
	digitalWrite( SS, LOW );
	SPI.transfer( data, size );
	digitalWrite( SS, HIGH );
}

void SPI_for_AFE::reg_w( uint8_t reg_adr, uint8_t *data, int size )
{
	uint8_t	v[ size + 1 ];
	
	v[ 0 ]	= reg_adr;
	memcpy( v + 1, data, size );
	
	txrx( v, sizeof( v ) );
}

void SPI_for_AFE::reg_r( uint8_t reg_adr, uint8_t *data, int size )
{
	uint8_t	v[ size + 1 ];
	
	for ( int i = 0; i < size + 1; i++ ) v[ i ]	= 0xFF;
	
	v[ 0 ]	= (uint8_t)(reg_adr | 0x80);
	
	txrx( v, sizeof( v ) );
	
	memcpy( data, v + 1, size );
}

void SPI_for_AFE::command( uint16_t reg )
{
	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF) };
	txrx( v, sizeof( v ) );
}

void SPI_for_AFE::write_r16( uint16_t reg, uint16_t val )
{
	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), (uint8_t)(val >> 8), (uint8_t)val };
	txrx( v, sizeof( v ) );
}

uint16_t SPI_for_AFE::read_r16( uint16_t reg )
{
	uint8_t	v[ 4 ]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), 0xFF, 0xFF };
	return (uint16_t)(v[ 2 ]) << 8 | v[ 3 ];
}

void SPI_for_AFE::write_r24( uint16_t reg, uint32_t val )
{
	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), (uint8_t)(val >> 16), (uint8_t)(val >> 8), (uint8_t)val };
	txrx( v, sizeof( v ) );
}

int32_t SPI_for_AFE::read_r24( uint16_t reg )
{
	uint8_t	v[]	= { (uint8_t)(reg >> 8), (uint8_t)(reg & 0xFF), 0xFF, 0xFF, 0xFF };
	txrx( v, sizeof( v ) );
	
	int32_t	r0	= v[ 2 ];
	int32_t	r1	= v[ 3 ];
	int32_t	r2	= v[ 4 ];
	int32_t	r	= ( (r0 << 24) | (r1 << 16) | (r2 << 8) );

	return r >> 8;
}
