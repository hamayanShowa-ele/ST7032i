#include <Wire.h>
#include <ST7032i.h>

#define  LCD_RESET  PB5

TwoWire i2c1;  /* SDA,SCL */
ST7032i lcd( &i2c1, ST7032i_ADR, 2, 16 );
void setup()
{
  pinMode( LCD_RESET, OUTPUT );
  digitalWrite( LCD_RESET, LOW );
  delay( 10UL );
  digitalWrite( LCD_RESET, HIGH );

  i2c1.setSDA( PB7 );  /* SDA */
  i2c1.setSCL( PB6 );  /* SCL */
  i2c1.begin();
  lcd.init();
  lcd.clear();
  lcd.write( "designed by hamayan." );
  lcd.update();
}

int count;
void loop()
{
  delay( 1 * 1000UL );
  lcd.clear();
  String msg = "hello world.\r\n";
  msg += count++;
  lcd.write( msg );
  lcd.update();
}
