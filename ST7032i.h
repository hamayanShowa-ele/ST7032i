/****************************************************************************/
/* ST7032i  header                                                          */
/*                         Copyright (C) 2014 hamayan All Rights Reserved.  */
/****************************************************************************/
#ifndef ST7032i_h
#define ST7032i_h

#include  <Arduino.h>
#include  <Wire.h>

extern "C" {
  #include  <ctype.h>
  #include  <string.h>
}

//#define  ST7032i_ADR          0x7C
#define  ST7032i_ADR          0x3e
#define  AQM0802A_LINES       2
#define  AQM0802A_COLUMN      8
#define  AQM0802A_DISPLAY_ON  0x04
#define  AQM0802A_DISPLAY_OFF 0x00
#define  AQM0802A_CURSOR_ON   0x02
#define  AQM0802A_CURSOR_OFF  0x00
#define  AQM0802A_BLINK_ON    0x01
#define  AQM0802A_BLINK_OFF   0x00

#if !defined( _MULTITASK_H_ )
#define  dly_tsk(tim)  delay(tim)
#define  rot_rdq()
#define  loc_cpu()     interrupts()
#define  unl_cpu()     noInterrupts()
#endif /*_MULTITASK_H_ */


class ST7032i_Base
{
private:
public:
  int write( TwoWire *wire, uint8_t addr, const uint8_t *dat, int len )
  {
    wire->beginTransmission( addr );
    wire->write( dat, len );
    int ret = wire->endTransmission();
    return ret;
  }

  int update( TwoWire *wire, uint8_t addr, const char *str, int len )
  {
    wire->beginTransmission( addr );
    wire->write( 0x40 );
    wire->write( str, len );
    int ret = wire->endTransmission();
    return ret;
  }
};

class ST7032i : public ST7032i_Base
{
private:
  TwoWire* myWire;
  char *msg;
  int row,col;
  int x,y;
  uint8_t addr;

  int writeCommand( uint8_t t_command );
  int writeData( uint8_t t_data );
  void locate( int _x, int _y );
  void setXY( int _x, int _y );

public:
  /*
    constructor and destructor.
  */
  ST7032i(
    TwoWire* wire,
     uint8_t a = ST7032i_ADR, int r = AQM0802A_LINES, int c = AQM0802A_COLUMN )
  {
    myWire = wire;
    addr = a;
    row = r;
    col = c;
    x = y = 0;
    msg = new char[ row * col ];
  }
  ~ST7032i() { delete [] msg; }

  void init();
  void clear();
  void OnOff( uint8_t onoff ) { writeCommand( onoff | 0x08 ); dly_tsk(2); }  //display on/off,cursor on/off,blink on/off
  void contrast( uint8_t cnt );
  int  update();
  void write( char c );
  void write( const char *str );
  void write( String str );
  void write( const char *str, int _x, int _y );
  void write( String str, int _x, int _y );
};

#endif  /*ST7032i_H*/

/****************************************************************************/
/*                         Copyright (C) 2014 hamayan All Rights Reserved.  */
/****************************************************************************/
