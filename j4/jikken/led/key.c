#include "h8-3052-iodef.h"

#define KEYBUFSIZE 10  /* �����Хåե����礭�� */
#define KEYCHKCOUNT 5  /* ������Ϣ³���֤�Ĵ�٤�Хåե����Ĺ���� */
                         /* �������Хåե����礭�����⾮�������뤳�� */
                         /*   ;͵�����ʤ����������ɤ�ʤ����Ȥ����� */
#define KEYROWNUM  4   /* ������������(�Ĥ��¤�Ǥ���Ŀ�) */
#define KEYCOLNUM  3   /* ��������ιԿ�(�����¤�Ǥ���Ŀ�) */
#define KEYMINNUM  1   /* �����ֹ�κǾ��� */
#define KEYMAXNUM 12   /* �����ֹ�κ����� */
#define KEYNONE   -1   /* ���ꤷ���������ʤ� */
#define KEYOFF     0   /* ���ꤷ�������Ϥ��ä�Υ����Ƥ������ */
#define KEYON      1   /* ���ꤷ�������Ϥ��äȲ�����Ƥ������ */
#define KEYTRANS   2   /* ���ꤷ�����������ܾ��� */
#define KEYPOSEDGE 3   /* ���ꤷ�������Ϻ������줿���� */
#define KEYNEGEDGE 4   /* ���ꤷ�������Ϻ�Υ���줿���� */

// ������������Ԥäơ����֤�Ĵ�٤�ؿ���
// ������֡ʿ�ms���١���� keysense() ��ƤӽФ����Ȥ�����
// Ǥ�դΥ������֤��ɤ߽Ф��ˤ� key_check() ��ƤӽФ�

/* �����޳����߽����Τ���, �Хåե���Ϣ������ѿ��Ȥ��Ƴ��� */
/* �������ѿ��� key.c ��Τߤǻ��Ѥ���Ƥ���               */
int keybufdp; /* �����Хåե����ȥݥ��� */
unsigned char keybuf[KEYBUFSIZE][KEYROWNUM]; /* �����Хåե� */

/* �������ѿ��ϥ������������˻��� */
/* �����δʰײ��Τ��������ѿ��Ȥ��Ƴ��� */
unsigned char keyoldval[KEYMAXNUM];
unsigned char keynewval[KEYMAXNUM];

void key_init(void);
void key_sense(void);
int key_check(int keynum);
int key_read(int keynum);

void key_init(void)
     /* �������ɤ߽Ф������ɬ�פʽ������Ԥ��ؿ� */
     /* PA4-6 �� LCD �ȴ�Ϣ���뤬, �к��Ѥ�       */
{
  int i,j;

  PADR = 0x0f;       /* PA0-3 ��0�����ƥ���, PA4-6 ��1�����ƥ��� */
  PADDR = 0x7f;      /* PA0-3 �ϥ����ܡ��ɥޥȥꥯ���ν����� */
                     /* PA4-6 ��LCD����(E,R/W,RS)�ν����� */
  P6DDR = 0;         /* P60-2 �ϥ����ܡ��ɥޥȥꥯ���������� */
                     /* P63-6 ��CPU�ΥХ�����Ȥ��Ƹ���(�⡼��6�λ�) */
  keybufdp = 0;
  /* �����Хåե��Υ��ꥢ */
  for (i = 0; i < KEYBUFSIZE; i++){
    for (j = 0; j < KEYROWNUM; j++){
      keybuf[i][j] = 0x07; /* ���⥭����������Ƥ��ʤ����֤˽���� */
    }
  }
  /* ���������ͤΥ��ꥢ */
  /* �ɤΥ����ⲡ����Ƥ��ʤ����֤˽���� */
  for (i = 0; i < KEYMAXNUM; i++)
    keynewval[i] = keyoldval[i] = KEYOFF;
}

void key_sense(void)
     /* ����������󤷤ƥ����Хåե��������ؿ�          */
     /*   ��ms ���٤˰���, �����޳��������ǸƤӽФ����� */
     /*   ����ѿ� keybuf �ϥ����ǡ������Ǽ����Хåե�  */
{
  int row;
  unsigned char r;

  /* ��󥰥Хåե��ݥ������� */
  keybufdp++;
  if (keybufdp >= KEYBUFSIZE) keybufdp = 0;
  /* ����������� */
  for(row = 0; row < KEYROWNUM; row++){ /* �󤴤Ȥ˥������ */
    r = ~(1 << row) & 0x0f;  /* ������󤹤���ΥӥåȤ��� 0 �ˤ��� */
    r = r | (PADR & 0x70);   /*  LCD ������˱ƶ����ʤ�������к� */
    PADR = r;                /*  �����ǡ������ɤ߹��� (0:ON, 1:OFF) */
    keybuf[keybufdp][row] = P6DR & 0x07; /* �����Хåե��˳�Ǽ */
  }
}

int key_check(int keynum)
     /* �����ֹ�������Ϳ�����, �����ξ��֤�Ĵ�٤��֤��ؿ�                 */
     /* �����ֹ�(keynum)�� 1-12 �ǻ���(��ϩ�ޤ� sw1-sw12 ���б�)            */
     /* ���ľ�� 1-9 �Υ����� sw1-sw9 ���б����Ƥ���                        */
     /* ���ľ�� *,0,# �Υ����� sw10,sw11,sw12 �ˤ��줾���б����Ƥ���       */
     /* ����ͤ�, KEYOFF, KEYON, KEYTRANS, KEYNONE �Τ����줫              */
     /* �����å���γ����ߤˤ��Хåե��񤭴����к��ϥХåե����礭�����б� */
{
  int bitpos,row,r,i,dp;
  int kbdp,count_swon,count_swoff;
  unsigned char bitmask,keydata;

  if ((keynum < KEYMINNUM) || (keynum > KEYMAXNUM))
    r = KEYNONE; /* �����ֹ���꤬�������ʤ��Ȥ���KEYNONE���֤� */
  else {
    keynum--; /* �����ֹ�� 0-11���ϰϤ��ѹ� */
    bitpos = keynum % KEYCOLNUM; /* ���������֤���Ƥ���ӥåȰ��� */
    bitmask = 1 << bitpos;       /* �ӥåȰ��֤˥ޥ��������� */
    row = (keynum - bitpos) / KEYCOLNUM;
    row = KEYROWNUM - 1 - row;   /* ���������֤���Ƥ�������� */
    kbdp = keybufdp;             /* �����Хåե��ݥ��󥿤�Ф��Ƥ��� */
    count_swoff = count_swon = 0;    /* Ϣ³�������󥿤ν���� */
    /* ���ꤵ�줿Ĺ��ʬ���������ξ��֤�Ĵ�٤� */
    for (i = 0; i < KEYCHKCOUNT; i++){
      dp = kbdp - i;
      if (dp < 0) dp = dp + KEYBUFSIZE; /* �����Хåե��ݥ��󥿤��ϰϥ����å� */
      keydata = keybuf[dp][row];        /* �Хåե�����ǡ�������Ф� */
      /* �Хåե�������֤�Ĵ�٤� (�����ϲ������� 0 �ˤʤ뤳�Ȥ����) */
      if ((keydata & bitmask) != 0) count_swoff++;  /* ������Ƥ��ʤ� */
      else count_swon++;                            /* ������Ƥ���   */
    }
    /* �������֤�Ƚ�� */
    /* ���ƥ�����ON(KEYON)��OFF(KEYOFF)���������Ǥʤ���(KEYTRANS)��Ƚ�� */
    r = KEYTRANS;
    if (count_swoff == KEYCHKCOUNT) r = KEYOFF;
    if (count_swon  == KEYCHKCOUNT) r = KEYON;
  }
  return r;
}

int key_read(int keynum)
     /* �����ֹ�������Ϳ�����, �����ξ��֤�Ĵ�٤��֤��ؿ�                  */
     /* �����ֹ�(keynum)�� 1-12 �ǻ���(��ϩ�ޤ� sw1-sw12 ���б�)             */
     /* ���ľ�� 1-9 �Υ����� sw1-sw9 ���б����Ƥ���                         */
     /* ���ľ�� *,0,# �Υ����� sw10,sw11,sw12 �ˤ��줾���б����Ƥ���        */
     /* ����ͤ�, KEYOFF, KEYON, KEYPOSEDGE, KEYNEGEDGE, KEYNONE �Τ����줫 */
     /*      Υ����Ƥ��롤������Ƥ��롤�������줿����Υ���줿�������ʤ�      */
{
  int keyval;

  if ((keynum < KEYMINNUM) || (keynum > KEYMAXNUM))
    keyval = KEYNONE; /* �����ֹ���꤬�������ʤ��Ȥ���KEYNONE���֤� */
  else {

    /* �����ǥ������֤�Ƚ���Ԥ� */
    /* ���㥿��󥰽����ؿ� key_check() ��������򼨤� KEYTRANS �� */
    /* �ޤ���ͤ��֤��Τǡ�������������Ԥ����� */
    /* ���Υ���������Τ�ʤ��ȡ��������������Ǥ��ʤ� */

  }
  return keyval;
}
