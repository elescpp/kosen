#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

#include "lcd.h"
#include "ad.h"
#include "timer.h"
#include "key.h"

#include <string.h>

/* �����޳����ߤλ��ֳִ�[��s] */
#define TIMER0 1000

/* �����߽����ǳƽ�����Ԥ����٤������� */
#define DISPTIME 100
#define KEYTIME 1
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10

/* LED�ط� */
/* LED�����PWM�ͤ��Ȥ��Ǿ��ͤȺ����� */
#define LEDMINVAL 0
#define LEDMAXVAL 10
/* LED��P9����³����Ƥ���ӥåȰ��� */
#define REDLEDPOS   0x10
#define GREENLEDPOS 0x20

/* LCDɽ����Ϣ */
/* 1�ʤ�ɽ���Ǥ���ʸ���� */
#define LCDDISPSIZE 16

/* PWM�����Ϣ */
/* ��������������� */
#define MAXPWMCOUNT 10

/* �����߽�����ɬ�פ��ѿ�������ѿ��ˤȤ� */
volatile int disp_time, key_time, ad_time, pwm_time, control_time;
/* LED�ط� */
unsigned char redval, greenval;
/* LCD�ط� */
volatile int disp_flag;
volatile char lcd_str_upper[LCDDISPSIZE+1];
volatile char lcd_str_lower[LCDDISPSIZE+1];
/* �⡼������ط� */
volatile int pwm_count;

int main(void);
void int_imia0(void);
void pwm_proc(void);
void control_proc(void);

int main(void)
{
  /* ����� */
  ROMEMU();           /* ROM���ߥ�졼������ON */

  /* ������LED�ݡ���(P9)�ν������Ԥ� */
  P9DDR |= (REDLEDPOS | GREENLEDPOS);
  P9DR |= (REDLEDPOS | GREENLEDPOS);

  /* �����ߤǻ��Ѥ�������ѿ��ν���� */
  pwm_time = pwm_count = 0;     /* PWM�����Ϣ */
  disp_time = 0; disp_flag = 1; /* ɽ����Ϣ */
  key_time = 0;                 /* �������ϴ�Ϣ */
  ad_time = 0;                  /* A/D�Ѵ���Ϣ */
  control_time = 0;             /* �����Ϣ */
  /* �����ޤ� */
  lcd_init();         /* LCDɽ����ν���� */
  key_init();         /* �����������ν���� */
  ad_init();          /* A/D�ν���� */
  timer_init();       /* �����ޤν���� */
  timer_set(0,1000);  /* ������0�ǥ����ƥ���ַ�¬ */
  timer_start(0);     /* ������0�������� */
  ENINT();            /* �������߼��ղ� */
  redval = greenval = 0; /* �֡���LED��ξ��������Ȥ��� */

  /* ������LCD��ɽ������ʸ������������Ƥ��� */
  strcpy(lcd_str_upper, "RED:5");
  strcpy(lcd_str_lower, "GREEN:5");

  while (1){ /* ���ʤϤ��Υ롼�פ�¹Ԥ��Ƥ��� */

    /* ������ disp_flag �ˤ�ä�LCD��ɽ���򹹿����� */
    if(disp_flag == 1){
      lcd_cursor(0, 0);
      //lcd_printstr(lcd_str_upper);
      lcd_printstr("RED:3");
      
      lcd_cursor(0, 1);
      lcd_printstr("GREEN:5");
      //lcd_printstr(lcd_str_lower);
      disp_flag = 0;
    }
  }
  
  /* ����¾�ν����ϥ����޳����ߤˤ�äƼ�ưŪ�˼¹Ԥ���뤿��  */
  /* ������ 0 �γ����ߥϥ�ɥ���˴ؿ�����Ͽ���ʤ���Фʤ�ʤ� */
}


#pragma interrupt
void int_imia0(void)
     /* ������0(GRA) �γ����ߥϥ�ɥ顡���������������������������� */
     /* �ؿ���̾���ϥ�󥫥�����ץȤǸ��ꤷ�Ƥ���                   */
     /* �ؿ���ľ���˳����ߥϥ�ɥ����� #pragama interrupt ��ɬ�� */
     /* �����޳����ߤˤ�äƳƽ����θƽФ����Ԥ���               */
     /* �ƽФ������٤� KEYTIME,ADTIME,PWMTIME,CONTROLTIME �Ƿ�ޤ� */
     /* ���Ƥν����������ޤǳ����ߤϥޥ��������                 */
     /* �ƽ����ϴ���Ū�˳����߼�����ǽ����ʤ���Фʤ�ʤ�       */
     /* ����Ǥ�������߼����κ����ͤ� 20971.2[��s]                */
{
  /* LCDɽ���ν��� */
  /* ¾�ν�����񤯤Ȥ��λ��� */
  disp_time++;
  if (disp_time >= DISPTIME){
    disp_flag = 1;
    disp_time = 0;
  }

  /* �����˥���������ʬ�����뤿��ν������ */
  
  /* ������PWM������ʬ�����뤿��ν������ */
  pwm_time++;
  if(pwm_time >= PWMTIME){
    pwm_time = 0;    
    pwm_proc();
  }
  
  /* ������A/D�Ѵ����Ϥν������ */
  
  /* ���������������ʬ�����뤿��ν������ */

  timer_intflag_reset(0); /* �����ߥե饰�򥯥ꥢ */
  ENINT();                /* CPU������ߵ��ľ��֤� */
}

void pwm_proc(void)
     /* PWM�����Ԥ��ؿ�                                        */
     /* ���δؿ��ϥ����޳�����0�γ����ߥϥ�ɥ餫��ƤӽФ���� */
{
  pwm_count++;

  /* ������PWM�������Ȥ�� */
  if(pwm_count >= redval){
    P9DR |= REDLEDPOS;
  }
  
  if(pwm_count >= greenval){
    P9DR |= GREENLEDPOS;
  }

  if(pwm_count >= MAXPWMCOUNT){
    //turn led off
    P9DR |= (REDLEDPOS | GREENLEDPOS);
    //reset counter
    pwm_count = 0;
  }


}

void control_proc(void)
     /* �����Ԥ��ؿ�                                           */
     /* ���δؿ��ϥ����޳�����0�γ����ߥϥ�ɥ餫��ƤӽФ���� */
{

  /* ���������������� */

}
