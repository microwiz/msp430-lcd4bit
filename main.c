#define LCD_MODE_BIT 0x02
#define LCD_R_W_BIT 0x04
#define LCD_ENABLE_BIT 0x08
#define LCD_BF_FLAG 0x10
#define LCD_CMD_MASK 0x0e
#define LCD_DATA_MASK 0xf0
#define LCD_PORT_OUT P4OUT
#define LCD_PORT_DIR P4DIR

void putchar( uint16_t in )
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


