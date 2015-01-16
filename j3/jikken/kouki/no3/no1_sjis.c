#include "h8-3052-iodef.h"

int main(void)
{
	unsigned char cf, key_data;

	P9DDR = 0x30;  /* �|�[�g9�̏�����(P95-P94���o�͂ɐݒ�) */
	P9DR = 0x30;

	P6DDR &= ~0x07;  /* P60,1,2   ���� */
	PADDR |= 0x0f;   /* PA0,1,2,3 �o�� */

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
		PADR = 0x0b; /* This is a mistake code. */ /*�C���ς�*/
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

		//1�������ꂽ��A���点��
		if(key_data == '1' ) {
			P9DR = 0x20;  /* D1(��)�_��, D2(��)���� */
		}
		//0�������ꂽ��A����
		if(key_data == '0'){
			P9DR = 0x30;  /* D1(��)����, D2(��)���� */
		}

	}

}
