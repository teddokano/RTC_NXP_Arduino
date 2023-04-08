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

PCF2131_base::PCF2131_base()
{
}

PCF2131_base::~PCF2131_base()
{
}

void PCF2131_base::begin( void )
{
	int_clear();
}


bool PCF2131_base::oscillator_stop( void )
{
	return _reg_r( Seconds ) & 0x80;
}

time_t PCF2131_base::rtc_time()
{
	struct tm	now_tm;

	uint8_t		bf[ 8 ];
	
	_reg_r( _100th_Seconds, bf, sizeof( bf ) );
	
	now_tm.tm_sec	= bcd2dec( bf[ 1 ] );
	now_tm.tm_min	= bcd2dec( bf[ 2 ] );
	now_tm.tm_hour	= bcd2dec( bf[ 3 ] );
	now_tm.tm_mday	= bcd2dec( bf[ 4 ] );
	now_tm.tm_mon	= bcd2dec( bf[ 6 ] ) - 1;
	now_tm.tm_year	= bcd2dec( bf[ 7 ] ) + 100;
	now_tm.tm_isdst	= 0;

   return mktime(&now_tm);
}

int PCF2131_base::rtc_set( struct tm* now_tmp )
{
	time_t		now_time;
	struct tm*	cnv_tmp;

	uint8_t		bf[ 8 ];
	
	bf[ 0 ]	= 0;
	bf[ 1 ]	= dec2bcd( now_tmp->tm_sec  );
	bf[ 2 ]	= dec2bcd( now_tmp->tm_min  );
	bf[ 3 ]	= dec2bcd( now_tmp->tm_hour );
	bf[ 4 ]	= dec2bcd( now_tmp->tm_mday );
	bf[ 6 ]	= dec2bcd( now_tmp->tm_mon + 1 );
	bf[ 7 ]	= dec2bcd( now_tmp->tm_year - 100 );

	now_time	= mktime( now_tmp );
	cnv_tmp		= localtime( &now_time );
	bf[ 5 ]		= dec2bcd( cnv_tmp->tm_wday);
	
	_bit_op8( Control_1, ~0x28, 0x20 );
	_bit_op8( SR_Reset,  (uint8_t)(~0x80), 0x80 );

	_reg_w( _100th_Seconds, bf, sizeof( bf ) );

	_bit_op8( Control_1, ~0x20, 0x00 );
	
	return 0;
}

void PCF2131_base::alarm( alarm_setting digit, int val )
{
	alarm( digit, val, 0 );
}

void PCF2131_base::alarm( alarm_setting digit, int val, int int_sel )
{
	_reg_w( Second_alarm + digit, dec2bcd( val ) );
	_bit_op8( int_mask_reg[ int_sel ][ 0 ], ~0x04, 0x00 );
	_bit_op8( Control_2, ~0x02, 0x02 );
}

void PCF2131_base::alarm_clear( void )
{
	_bit_op8( Control_2, ~0x10, 0x00 );	
}

void PCF2131_base::alarm_disable( void )
{
	_bit_op8( Control_2, ~0x02, 0x00 );
}

void PCF2131_base::timestamp( int num, timestanp_setting ts_setting, int int_sel )
{
	const int r_ofst	= 7;
	const int fst		= ts_setting ? 0x80 : 0x00;
	
	num	-=1;

	uint8_t	reg	= Timestp_ctl1 + num * r_ofst;

	_bit_op8( reg, (uint8_t)(~0x80), fst );
	_bit_op8( int_mask_reg[ int_sel ][ 1 ], ~(0x1 << (3 - num)), (0x0 << (3 - num)) );

	_bit_op8( Control_5, ~(0x1 << (7 - num)), (0x1 << (7 - num)) );
	
	Serial.println("timestamp");
//	Serial.println(_reg_r(INT_A_MASK1), HEX);
	Serial.println(_reg_r(INT_A_MASK2), HEX);
	Serial.println(_reg_r(Control_5), HEX);
//	Serial.println(_reg_r(INT_B_MASK1), HEX);
//	Serial.println(_reg_r(INT_B_MASK2), HEX);
	
}

time_t PCF2131_base::timestamp( int num )
{
	const int r_ofst	= 7;

	num	-=1;

	uint8_t	reg	= Timestp_ctl1 + num * r_ofst;
	uint8_t	v[ 7 ];
	
	_reg_r( reg, v, sizeof( v ) );
	
	struct tm	ts_tm;

	ts_tm.tm_sec	= bcd2dec( v[ 1 ] );
	ts_tm.tm_min	= bcd2dec( v[ 2 ] );
	ts_tm.tm_hour	= bcd2dec( v[ 3 ] );
	ts_tm.tm_mday	= bcd2dec( v[ 4 ] );
	ts_tm.tm_mon	= bcd2dec( v[ 5 ] ) - 1;
	ts_tm.tm_year	= bcd2dec( v[ 6 ] ) + 100;
	ts_tm.tm_isdst	= 0;

   return mktime(&ts_tm);

}

void PCF2131_base::int_clear( void )
{
	uint8_t dummy[ 3 ];
	int_clear( dummy );
}

void PCF2131_base::int_clear( uint8_t* rv )
{
	_reg_r( Control_2, rv, 3 );

	if ( rv[ 0 ] & 0x90 )	// if interrupt flag set in Control_2
		_reg_w( Control_2, rv[ 0 ] & ~((rv[ 0 ] & 0x90) | 0x49) );	// datasheet 7.11.5
	
	if ( rv[ 1 ] & 0x08 )	// if interrupt flag set in Control_3
		_reg_w( Control_3, rv[ 1 ] & ~(0x08) );
	
	if ( rv[ 2 ] & 0xF0 )	// if interrupt flag set in Control_4
		_reg_w( Control_4, rv[ 2 ] & ~(rv[ 2 ] & 0xF0) );
}

void PCF2131_base::periodic_interrupt_enable( periodic_int_select sel, int int_sel )
{
	if ( !sel ) {
		_bit_op8( Control_1, ~0x03, 0x00 );
		_bit_op8( int_mask_reg[ int_sel ][ 0 ], ~0x30, 0x30 );
		return;
	}
	
	uint8_t v	= (sel == EVERY_MINUTE) ? 0x02 : 0x01;

	v=3;
	_bit_op8( Control_1, ~0x03, v );
	_bit_op8( int_mask_reg[ int_sel ][ 0 ], ~0x30, ~(v << 4) );
}


PCF2131_I2C::PCF2131_I2C( uint8_t i2c_address ) : I2C_device( i2c_address )
{
}

PCF2131_I2C::~PCF2131_I2C()
{
}

void PCF2131_I2C::_reg_w( uint8_t reg, uint8_t *vp, int len )
{
	reg_w( reg, vp, len );
}

void PCF2131_I2C::_reg_r( uint8_t reg, uint8_t *vp, int len )
{
	reg_r( reg, vp, len );
}

void PCF2131_I2C::_reg_w( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t PCF2131_I2C::_reg_r( uint8_t reg )
{
	return 	reg_r( reg );
}

void PCF2131_I2C::_bit_op8( uint8_t reg, uint8_t mask, uint8_t val )
{
	bit_op8( reg, mask, val );
}



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

void SPI_for_RTC::txrx( uint8_t *data, uint16_t size )
{
	digitalWrite( SS, LOW );
	SPI.transfer( data, size );
	digitalWrite( SS, HIGH );
}

void SPI_for_RTC::reg_w( uint8_t reg_adr, uint8_t *data, uint16_t size )
{
	uint8_t	v[ size + 1 ];
	
	v[ 0 ]	= reg_adr;
	memcpy( v + 1, data, size );
	
	txrx( v, sizeof( v ) );
}

void SPI_for_RTC::reg_w( uint8_t reg_adr, uint8_t data )
{
	uint8_t	v[]	= { reg_adr, data };
	
	txrx( v, sizeof( v ) );
}

void SPI_for_RTC::reg_r( uint8_t reg_adr, uint8_t *data, uint16_t size )
{
	uint8_t	v[ size + 1 ] = { 0xFF };
	
	v[ 0 ]	= (uint8_t)(reg_adr | 0x80);
	
	txrx( v, sizeof( v ) );
	
	memcpy( data, v + 1, size );
}

uint8_t	SPI_for_RTC::reg_r( uint8_t reg_adr )
{
	uint8_t	v[]	= { (uint8_t)(reg_adr | 0x80), 0xFF };
	
	txrx( v, sizeof( v ) );
	
	return v[ 1 ];
}

void SPI_for_RTC::write_r8( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t SPI_for_RTC::read_r8( uint8_t reg )
{
	return reg_r( reg );
}

void SPI_for_RTC::bit_op8(  uint8_t reg,  uint8_t mask,  uint8_t value )
{
	uint8_t	tmp	= reg_r( reg ) & mask;
	reg_w( reg, tmp | value );
}

