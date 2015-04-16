#define ADSTATUS() (ADCSR & 0x80)
#define ADREAD() ADDRAH

/* A/D変換のチャネル数とバッファサイズ */
#define ADCHNUM   4
#define ADBUFSIZE 8

/* 平均化するときのデータ個数 */
#define ADAVRNUM 4

/* チャネル指定エラー時に返す値 */
#define ADCHNONE -1

extern void ad_init();
extern void ad_start(unsigned char ch, unsigned char int_sw);
extern void ad_scan(unsigned char ch_grp, unsigned char int_sw);
extern void ad_stop(void);
extern int  ad_read(int ch);
