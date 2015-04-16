#define ADSTATUS() (ADCSR & 0x80)
#define ADREAD() ADDRAH

/* A/D�Ѵ��Υ���ͥ���ȥХåե������� */
#define ADCHNUM   4
#define ADBUFSIZE 8

/* ʿ�Ѳ�����Ȥ��Υǡ����Ŀ� */
#define ADAVRNUM 4

/* ����ͥ���ꥨ�顼�����֤��� */
#define ADCHNONE -1

extern void ad_init();
extern void ad_start(unsigned char ch, unsigned char int_sw);
extern void ad_scan(unsigned char ch_grp, unsigned char int_sw);
extern void ad_stop(void);
extern int  ad_read(int ch);
