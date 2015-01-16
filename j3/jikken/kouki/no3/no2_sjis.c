#include "h8-3052-iodef.h"
#include "lcd.h"

int main(void)
{
	unsigned char cf, key_data;
	//�\�����镶����ێ�����p
	char hyouzi_you = ' ';

	P9DDR = 0x30;  /* �|�[�g9�̏�����(P95-P94���o�͂ɐݒ�) */

	P6DDR &= ~0x07;  /* P60,1,2   ���� */
	PADDR |= 0x0f;   /* PA0,1,2,3 �o�� */

	lcd_init();
	lcd_cursor( 0, 0 );

	while(1) {
		key_data = 0;

		//key 1,2,3
		PADR = 0x07; // PA3 = L
		cf = P6DR;   // �f�[�^����
		cf = ~cf;    // cf�̔��]
		cf &= 0x07;  // P60,1,2�̂݌���
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

		if( key_data != 0 )
			hyouzi_you = key_data;

		lcd_cursor(0,0);
		lcd_printch( hyouzi_you );

	}

}
