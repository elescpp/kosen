/* �����߽�����Ԥ������ɬ�פʥإå��ե����� */
/* 2008/04/23 �º� */
/* 2010/01/06 USE_UI���ɲ� */

/* CCR�쥸�����˳����ߵ���/�Ե��Ĥ�񤭹���                  */
/*     ENINT();   <= ����ʹߤ��������ߵ��ľ��֤ˤʤ�        */
/*     ENINT1();  <= �ץ饤����ƥ�1�γ����ߵ��ľ��֤ˤʤ�   */
/*     DISINT();  <= ����ʹߤ����������Ե��ľ��֤ˤʤ�      */
/* ����¾�������ߴ�Ϣ��ɬ�פʤ��                            */
/*     ROMEMU();  <= ROM���ߥ�졼�������֤ˤ���            */
/*     USE_UI();  <= �ץ饤����ƥ�1�γ����ߤ�UI�ӥåȤ�Ȥ� */
/* ���ա�����¾�˳����ߥ���ȥ���������꤬ɬ��!!             */

#define ENINT()   asm volatile ("andc.b #0x7f,ccr") 
#define ENINT1()  asm volatile ("andc.b #0xbf,ccr") 
#define DISINT()  asm volatile ("orc.b #0x80,ccr")
#define ROMEMU()  RAMCR=0xf8
#define USE_UI()  SYSCR=0x03