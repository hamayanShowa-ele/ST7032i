/*********************************************************************************/
/* Initialization and display of the LCD AQM0802A using the ST7032i.             */
/*                                         designed by hamayan since 2015/04/21  */
/*********************************************************************************/
#include  "ST7032i.h"

extern "C" {
//  #include  <mul_tsk.h>
}

/*
  prototypes
*/

/*
  global valiables
*/


/*
  write data
*/
int ST7032i::writeData( uint8_t t_data )
{
  uint8_t dat[2];
  dat[0] = 0x40;  //Co=0,RS=1,another bit=0
  dat[1] = t_data;
  int ret = ST7032i_Base::write( myWire, addr, dat, sizeof(dat) );
  return ret;
}

/*
  write command
*/
int ST7032i::writeCommand( uint8_t t_command )
{
  uint8_t cmd[2];
  cmd[0] = 0x00;  //Co=0,RS=0,another bit=0
  cmd[1] = t_command;
  int ret = ST7032i_Base::write( myWire, addr, cmd, sizeof(cmd) );
  return ret;
}

/*
  update
*/
int ST7032i::update()
{
  int ret;
  for( int y = 0; y < row; y++ )
  {
    locate( 0, y );
    ret = ST7032i_Base::update( myWire, addr, (char *)&msg[ y * col ], col );
  }
  return ret;
}

/*
  initialize ST7032i
*/
void ST7032i::init()
{
  dly_tsk(40);
  writeCommand( 0x38 );  //function set
  delayMicroseconds(27);
  writeCommand( 0x39 );  //function set
  delayMicroseconds(27);
  writeCommand( 0x14 );  //internal OSC frequency
  delayMicroseconds(27);
  writeCommand( 0x70 );  //contrast set
  delayMicroseconds(27);
  writeCommand( 0x56 );  //power/icon/contrast control
  delayMicroseconds(27);
  writeCommand( 0x6c );  //follower control
  delayMicroseconds(27);
  writeCommand( 0x38 );  //function set
  delayMicroseconds(27);
  writeCommand( 0x0c );  //display on/off control
  delayMicroseconds(27);
  writeCommand( 0x01 );  //clear display
  dly_tsk(2);
}

/*
  clear display
*/
void ST7032i::clear()
{
  writeCommand( 0x01 );  //clear display
  dly_tsk(2);
  x = y = 0;
  memset( msg, ' ', row * col );
}


/*
  locate
*/
void ST7032i::locate( int _x, int _y )
{
  int temp = _x + (_y * col);

  if(temp >= col) temp = (temp - col) + 0x40;

  writeCommand( (uint8_t)temp | 0x80 );  //set ddram address
  dly_tsk(2);
}


/*
  set contrast
*/
void ST7032i::contrast( uint8_t cnt )
{
  //Contrast adjustments
  writeCommand( 0x2a );  //RE=1
  writeCommand( 0x79 );  //SD=1
  writeCommand( 0x81 );  //contrast set
  writeCommand( cnt );  //contrast max
  writeCommand( 0x78 );  //SD=0
  writeCommand( 0x28 );  //set character size is normal.
  dly_tsk(100);
}


/*
  set X,Y
*/
void ST7032i::setXY( int _x, int _y )
{
  if( x >= col ) return;
  if( y >= row ) return;
  x = _x;
  y = _y;
}

/*
  write
*/
void ST7032i::write( char c )
{
  if( c == '\r' )
  {
    x = 0;
  }
  else if( c == '\n' )
  {
    if( ++y >= row ) y = 0;
  }
  else if( isprint( c ) )
  {
    if( x == 0 ) memset( &msg[ y * col ], ' ', col );
    msg[ (y * col) + x ] = c;
    if( ++x >= col)
    {
      x = 0;
      if( ++y >= row ) y = 0;
    }
  }
}

void ST7032i::write( const char *str ) { while( *str ) write( *str++ ); }

void ST7032i::write( String str )
{
  const char *ptr = str.c_str();
  while( *ptr ) write( *ptr++ );
}

void ST7032i::write( const char *str, int _x, int _y )
{
  setXY( _x, _y );
  while( *str ) write( *str++ );
}

void ST7032i::write( String str, int _x, int _y )
{
  setXY( _x, _y );
  const char *ptr = str.c_str();
  while( *ptr ) write( *ptr++ );
}


/*********************************************************************************/
/* end of file                                                                   */
/*                                         designed by hamayan since 2015/04/21  */
/*********************************************************************************/
