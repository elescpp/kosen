// lcd.c �����Ѥ��뤿���ɬ�פʥإå��ե�����
// �����黲�Ȥ����ؿ������

extern void lcd_init(void);
     /* LCD ɽ�����Ȥ�����ν�����ؿ�             */
     /* ����, �������󥹤Ȥη�͹礤�����뤬�к��Ѥ� */
     /* LCD ɽ����ν�����ˤϻ��֤�������Τ����   */

extern void lcd_cursor(int x, int y);
     /* ɽ�����֤��ѹ�����ؿ� */
     /* ɽ���ﺸ�夬 (0,0) �Ȥʤ� */

extern void lcd_clear(void);
     /* ɽ����������̤򥯥ꥢ���뤿��δؿ� */

extern void lcd_printstr(unsigned char *str);
     /* ɽ�����ʸ�����ɽ������ؿ� */
     /* ʸ�������ɤ� ASCII �����ɤ��ϰϤʤ�����ʤ� */

extern void lcd_printch(unsigned char ch);
     /* ɽ�����1ʸ������ɽ������ؿ� */
     /* ʸ�������ɤ� ASCII �����ɤ��ϰϤʤ�����ʤ� */

extern void wait1ms(int ms);
     /* ms = 1 �ΤȤ�, ��1ms��������ʥ롼�פ��ԤĴؿ�       */
     /* LCD��ư����ݾڤ��뤿���ɬ�פʻ��֤���ݤ���Τ���Ū */
