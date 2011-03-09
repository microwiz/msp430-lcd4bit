#include <signal.h>
#include <io.h>

/*

4bit LCD drive code posted by Dan Bloomquist on MSP430 Yahoo Group.

Modified by Chetan Bhargava
Compiler: MSPGCC4
Testbed: TI Launchpad

                MSP430G22xx                    HD44780 LCD
             -----------------              ----------------
            |                 |            |
            |                 |    |-------|GND
            |                 |    |-------|DB0
            |                 |    |-------|DB1
            |                 |    |-------|DB2
            |                 |    |-------|DB3
            |                 |    |-------|RW 
            |                 |    V       |   
            |                 |            |   
            |             P1.4|------------|DB4
            |             P1.5|------------|DB5
            |             P1.6|------------|DB6
            |             P1.7|------------|DB7
            |                 |            |
            |             P1.3|------------|EN
            |             P1.1|------------|RS
            |                 |            |   

  Original code uses RW line of the LCD but as we will only
  write to the LCD it might not be needed.

*/

// LCD connection definitions
#define LCD_R_W_BIT 0x04	//probably not used in the code
#define LCD_BF_FLAG 0x10 	//probably not used in the code
#define LCD_MODE_BIT 0x02	//BIT1
#define LCD_ENABLE_BIT 0x08	//BIT3

#define LCD_CMD_MASK 0x0e
#define LCD_DATA_MASK 0xf0
#define LCD_PORT_OUT P1OUT
#define LCD_PORT_DIR P1DIR


void brief_pause( int delay )	{
	while (!delay)	{
	  delay--;
	}
}

int putchar( uint16_t in )
{
	LCD_PORT_OUT= ( in & 0xf0 ) | LCD_MODE_BIT;
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	//
	LCD_PORT_OUT= ( ( in << 4 ) & 0xf0 ) | LCD_MODE_BIT;
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT= 0;
	brief_pause( 200 );
	return (0);
}

void lcd_command( uint16_t cmd )
{
	LCD_PORT_OUT= ( cmd & 0xf0 );
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	//
	LCD_PORT_OUT= ( ( cmd << 4 ) & 0xf0 );
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT= 0;
	brief_pause( 3000 );
}


void lcd_init( )
{
	//lcd
	LCD_PORT_OUT= 0;
	LCD_PORT_DIR|= LCD_DATA_MASK | LCD_CMD_MASK;

	//assume VCC+ > 20ms away

	//first init
	LCD_PORT_OUT= 0x30;
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	//pause should be set by mclk div, > 4.1ms
	brief_pause( 15000 );

	//second init
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	//pause should be set by mclk div, > 100us
	brief_pause( 500 );

	//four bit mode
	LCD_PORT_OUT= 0x20;
	LCD_PORT_OUT|= LCD_ENABLE_BIT;
	nop( );
	LCD_PORT_OUT&= ~LCD_ENABLE_BIT;
	brief_pause( 1000 );

	//from here 'lcd_command can be used
	//operational mode 5x8 1/16 duty( two lines )
	lcd_command( 0x28 );
	brief_pause( 1000 );

	//display off
	lcd_command( 0x08 );
	brief_pause( 1000 );

	//display on, no cursor, no blink...
	lcd_command( 0x0c );
	brief_pause( 1000 );
	LCD_PORT_OUT= 0;
}


int main (void)	{

  putchar ('H');
  putchar ('e');
  putchar ('l');
  putchar ('l');
  putchar ('o');
  return (0);	//fake return to avoid warning
}
