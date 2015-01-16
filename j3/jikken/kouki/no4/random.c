#include "h8-3052-iodef.h"

/* リフレッシュコントローラのカウンタを使って */
/* 擬似的に乱数を発生させるためのプログラム   */

void random_init(void)
     /* 乱数を使用するときに必要な初期化を行う関数      */
     /* 中身はリフレッシュコントローラの初期化          */
     /* DRAM が接続されている場合は実行してはいけない！ */
{
  RFSHCR = 0x02; /* リフレッシュ端子の開放, リフレッシュサイクル禁止 */
  RTCOR  = 0xff; /* リフレッシュカウンタの最大値 */
  RTMCSR = 0x0f; /* リフレッシュカウンタφ/2動作, 割り込み禁止 */
}

unsigned char random(void)
     /* リフレッシュカウンタの値を使って疑似乱数を生成する関数 */
     /* あまり乱数としてはよくない                           */
     /* 戻り値(乱数)は符号無しの 8 ビット長 (0-255)          */
{
  unsigned char i,j;

  j = RTCNT;
  if ((j & 0x02) == 1){
    i = j >> 4;
    j = (j << 4) + i;
  }
  return j;
}
