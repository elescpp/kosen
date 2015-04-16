#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

/* A/D変換のチャネル数 */
#define ADCHNUM   4

/* A/D変換用のバッファサイズ */
#define ADBUFSIZE 8

/* 平均化するときのデータ個数 */
#define ADAVRNUM 4

/* チャネル指定エラー時に返す値 */
#define ADCHNONE -1

void ad_init();
void ad_start(unsigned char ch, unsigned char int_sw);
void ad_scan(unsigned char ch_grp, unsigned char int_sw);
void ad_stop(void);
void int_adi(void);
int  ad_read(int ch);

/* A/D変換関係 */
volatile unsigned char adbuf[ADCHNUM][ADBUFSIZE];
volatile int adbufdp;

void ad_init()
     /* A/D 変換器を使うための初期化関数 */
{
  unsigned char dmy;

  ADCR = 0x7e;  /* 外部トリガ変換開始の禁止 */
  dmy = ADCSR;  /* 変換終了フラグクリアのためのダミーリード */
  ADCSR = 0x08; /* 単一スキャン, 割り込み禁止, A/D変換停止, */
                /* 134ステート変換時間, チャネル 0 選択     */
  adbufdp = 0;  /* A/D変換データバッファポインタの初期化 */
}

void ad_start(unsigned char ch, unsigned char int_sw)
     /* A/D変換をスタートさせる関数                                */
     /* ch:入力チャネル番号(0-7), int_sw:変換終了割り込み許可(0,1) */
     /* 割り込み許可の場合, 別途割り込みハンドラが必要             */
     /* また, CPUを割り込み許可状態にしないと割り込みは発生しない  */
     /* 割り込み処理には数usの時間がかかることを考慮すること       */
{
  if (int_sw != 0) int_sw = 0x40;
  else int_sw = 0;
  ADCSR = (ADCSR & 0xd8) | (ch & 0x07);   /* 変換停止, チャネルの書き換え */
  ADCSR = (ADCSR & 0x1f) | 0x20 | int_sw; /* エンドフラグクリア, 変換開始 */
}

void ad_scan(unsigned char ch_grp, unsigned char int_sw)
     /* A/D変換をスキャンモードでスタートさせる関数                */
     /* ch_grp:入力チャネルグループ指定(0,1)                       */
     /*   0:AN0-AN3 を自動的にスキャンして結果をレジスタに格納     */
     /*   1:AN4-AN7 を自動的にスキャンして結果をレジスタに格納     */
     /* int_sw:グループスキャン変換終了割り込み許可(0,1)           */
     /* 割り込み許可の場合, 別途割り込みハンドラが必要             */
     /* また, CPUを割り込み許可状態にしないと割り込みは発生しない  */
     /* 割り込み処理には数usの時間がかかることを考慮すること       */
{
  if (int_sw != 0) int_sw = 0x40; /* 変換終了時に割り込み通知 */
  else int_sw = 0;                /* 割り込みを使用しない     */
  if (ch_grp != 0) ch_grp = 7;    /* AN4-7の自動スキャン */
  else ch_grp = 3;                /* AN0-3の自動スキャン */
  ADCSR = ADCSR & 0x5f;           /* エンドフラグのクリアと変換の停止 */
  ADCSR = 0x38 | int_sw | ch_grp; /* 自動スキャンスタート */
}

void ad_stop(void)
     /* A/D変換を停止させる関数                  */
     /* 変換終了フラグをクリアし、変換停止させる */
{
  ADCSR = ADCSR & 0x5f;  /* A/Dエンドフラグのクリア, 変換停止 */
}

#pragma interrupt
void int_adi(void)
     /* A/D変換終了の割り込みハンドラ                               */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragma interrupt が必要  */
{
  ad_stop();    /* A/D変換の停止と変換終了フラグのクリア */

  /* ここでバッファポインタの更新を行う */
  /* 　但し、バッファの境界に注意して更新すること */

  /* ここでバッファにA/Dの各チャネルの変換データを入れる */
  /* スキャングループ 0 を指定した場合は */
  /*   A/D ch0〜3 (信号線ではAN0〜3)の値が ADDRAH〜ADDRDH に格納される */
  /* スキャングループ 1 を指定した場合は */
  /*   A/D ch4〜7 (信号線ではAN4〜7)の値が ADDRAH〜ADDRDH に格納される */

  ENINT();      /* 割り込みの許可 */
}

int ad_read(int ch)
     /* A/Dチャネル番号を引数で与えると, 指定チャネルの平均化した値を返す関数 */
     /* チャネル番号は，0〜ADCHNUM の範囲 　　　　　　　　　　　             */
     /* 戻り値は, 指定チャネルの平均化した値 (チャネル指定エラー時はADCHNONE) */
{
  int i,ad,bp;

  if ((ch > ADCHNUM) || (ch < 0)) ad = ADCHNONE; /* チャネル範囲のチェック */
  else {

    /* ここで指定チャネルのデータをバッファからADAVRNUM個取り出して平均する */
    /* データを取り出すときに、バッファの境界に注意すること */
    /* 平均した値が戻り値となる */

  }
  return ad; /* データの平均値を返す */
}
