#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

/* A/D�Ѵ��Υ���ͥ�� */
#define ADCHNUM   4

/* A/D�Ѵ��ѤΥХåե������� */
#define ADBUFSIZE 8

/* ʿ�Ѳ�����Ȥ��Υǡ����Ŀ� */
#define ADAVRNUM 4

/* ����ͥ���ꥨ�顼�����֤��� */
#define ADCHNONE -1

void ad_init();
void ad_start(unsigned char ch, unsigned char int_sw);
void ad_scan(unsigned char ch_grp, unsigned char int_sw);
void ad_stop(void);
void int_adi(void);
int  ad_read(int ch);

/* A/D�Ѵ��ط� */
volatile unsigned char adbuf[ADCHNUM][ADBUFSIZE];
volatile int adbufdp;

void ad_init()
     /* A/D �Ѵ����Ȥ�����ν�����ؿ� */
{
  unsigned char dmy;

  ADCR = 0x7e;  /* �����ȥꥬ�Ѵ����Ϥζػ� */
  dmy = ADCSR;  /* �Ѵ���λ�ե饰���ꥢ�Τ���Υ��ߡ��꡼�� */
  ADCSR = 0x08; /* ñ�쥹�����, �����߶ػ�, A/D�Ѵ����, */
                /* 134���ơ����Ѵ�����, ����ͥ� 0 ����     */
  adbufdp = 0;  /* A/D�Ѵ��ǡ����Хåե��ݥ��󥿤ν���� */
}

void ad_start(unsigned char ch, unsigned char int_sw)
     /* A/D�Ѵ��򥹥����Ȥ�����ؿ�                                */
     /* ch:���ϥ���ͥ��ֹ�(0-7), int_sw:�Ѵ���λ�����ߵ���(0,1) */
     /* �����ߵ��Ĥξ��, ���ӳ����ߥϥ�ɥ餬ɬ��             */
     /* �ޤ�, CPU������ߵ��ľ��֤ˤ��ʤ��ȳ����ߤ�ȯ�����ʤ�  */
     /* �����߽����ˤϿ�us�λ��֤������뤳�Ȥ��θ���뤳��       */
{
  if (int_sw != 0) int_sw = 0x40;
  else int_sw = 0;
  ADCSR = (ADCSR & 0xd8) | (ch & 0x07);   /* �Ѵ����, ����ͥ�ν񤭴��� */
  ADCSR = (ADCSR & 0x1f) | 0x20 | int_sw; /* ����ɥե饰���ꥢ, �Ѵ����� */
}

void ad_scan(unsigned char ch_grp, unsigned char int_sw)
     /* A/D�Ѵ��򥹥����⡼�ɤǥ������Ȥ�����ؿ�                */
     /* ch_grp:���ϥ���ͥ륰�롼�׻���(0,1)                       */
     /*   0:AN0-AN3 ��ưŪ�˥�����󤷤Ʒ�̤�쥸�����˳�Ǽ     */
     /*   1:AN4-AN7 ��ưŪ�˥�����󤷤Ʒ�̤�쥸�����˳�Ǽ     */
     /* int_sw:���롼�ץ�������Ѵ���λ�����ߵ���(0,1)           */
     /* �����ߵ��Ĥξ��, ���ӳ����ߥϥ�ɥ餬ɬ��             */
     /* �ޤ�, CPU������ߵ��ľ��֤ˤ��ʤ��ȳ����ߤ�ȯ�����ʤ�  */
     /* �����߽����ˤϿ�us�λ��֤������뤳�Ȥ��θ���뤳��       */
{
  if (int_sw != 0) int_sw = 0x40; /* �Ѵ���λ���˳��������� */
  else int_sw = 0;                /* �����ߤ���Ѥ��ʤ�     */
  if (ch_grp != 0) ch_grp = 7;    /* AN4-7�μ�ư������� */
  else ch_grp = 3;                /* AN0-3�μ�ư������� */
  ADCSR = ADCSR & 0x5f;           /* ����ɥե饰�Υ��ꥢ���Ѵ������ */
  ADCSR = 0x38 | int_sw | ch_grp; /* ��ư������󥹥����� */
}

void ad_stop(void)
     /* A/D�Ѵ�����ߤ�����ؿ�                  */
     /* �Ѵ���λ�ե饰�򥯥ꥢ�����Ѵ���ߤ����� */
{
  ADCSR = ADCSR & 0x5f;  /* A/D����ɥե饰�Υ��ꥢ, �Ѵ���� */
}

#pragma interrupt
void int_adi(void)
     /* A/D�Ѵ���λ�γ����ߥϥ�ɥ�                               */
     /* �ؿ���̾���ϥ�󥫥�����ץȤǸ��ꤷ�Ƥ���                   */
     /* �ؿ���ľ���˳����ߥϥ�ɥ����� #pragma interrupt ��ɬ��  */
{
  ad_stop();    /* A/D�Ѵ�����ߤ��Ѵ���λ�ե饰�Υ��ꥢ */

  /* �����ǥХåե��ݥ��󥿤ι�����Ԥ� */
  /* ��â�����Хåե��ζ�������դ��ƹ������뤳�� */

  /* �����ǥХåե���A/D�γƥ���ͥ���Ѵ��ǡ���������� */
  /* ������󥰥롼�� 0 ����ꤷ������ */
  /*   A/D ch0��3 (�������Ǥ�AN0��3)���ͤ� ADDRAH��ADDRDH �˳�Ǽ����� */
  /* ������󥰥롼�� 1 ����ꤷ������ */
  /*   A/D ch4��7 (�������Ǥ�AN4��7)���ͤ� ADDRAH��ADDRDH �˳�Ǽ����� */

  ENINT();      /* �����ߤε��� */
}

int ad_read(int ch)
     /* A/D����ͥ��ֹ�������Ϳ�����, �������ͥ��ʿ�Ѳ������ͤ��֤��ؿ� */
     /* ����ͥ��ֹ�ϡ�0��ADCHNUM ���ϰ� ����������������������             */
     /* ����ͤ�, �������ͥ��ʿ�Ѳ������� (����ͥ���ꥨ�顼����ADCHNONE) */
{
  int i,ad,bp;

  if ((ch > ADCHNUM) || (ch < 0)) ad = ADCHNONE; /* ����ͥ��ϰϤΥ����å� */
  else {

    /* �����ǻ������ͥ�Υǡ�����Хåե�����ADAVRNUM�ļ��Ф���ʿ�Ѥ��� */
    /* �ǡ�������Ф��Ȥ��ˡ��Хåե��ζ�������դ��뤳�� */
    /* ʿ�Ѥ����ͤ�����ͤȤʤ� */

  }
  return ad; /* �ǡ�����ʿ���ͤ��֤� */
}
