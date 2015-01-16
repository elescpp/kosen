#include "h8-3052-iodef.h"
#include "lcd.h"

#define LOOP (13200)

void delay( unsigned long int ms )
{
	unsigned long int i, j;

	for( i = 0; i < ms; i++ )
		for( j = 0; j < LOOP; j++);
}

int main(void)
{
  unsigned char cf, key_data;
	char prev_pressed_key = 0;

  P9DDR = 0x30;  /* ポート9の初期化(P95-P94を出力に設定) */

  P6DDR &= ~0x07;  /* P60,1,2   入力 */
  PADDR |= 0x0f;   /* PA0,1,2,3 出力 */

	int pos = 0;

	lcd_init();
	lcd_clear();

  while(1) {
    key_data = 0;

    //key 1,2,3
    PADR = 0x07; // PA3 = L
    cf = P6DR;   // データ入力
    cf = ~cf;    // cfの反転
    cf &= 0x07;  // P60,1,2のみ見る
    switch(cf) {
    case 1 : key_data = '1'; break;
    case 2 : key_data = '2'; break;
    case 4 : key_data = '3'; break;
    }  
      
    //key 4,5,6
    PADR = 0x0b;
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '4'; break;
    case 2 : key_data = '5'; break;
    case 4 : key_data = '6'; break;
    }  
      
    //key 7,8,9
    PADR = 0x0d; /* This is a mistake code. */
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '7'; break;
    case 2 : key_data = '8'; break;
    case 4 : key_data = '9'; break;
    }  
      
    //key *,0,#
    PADR = 0x0e;
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '*'; break;
    case 2 : key_data = '0'; break;
    case 4 : key_data = '#'; break;
    }  

		if( key_data != prev_pressed_key ){
			delay( 15 );
			if( key_data != 0 ){
				lcd_cursor( pos % 16, pos / 16 );
				lcd_printch( key_data );
				pos++;
			}
		}
		prev_pressed_key = key_data;
	}

}
