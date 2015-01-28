#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "lcd.h"
#include "ad.h"
#include "da.h"
#include "timer.h"

#define BUFSIZE       30 /* バッファの大きさ(kB) */
#define SAMPLINGTIME 100 /* 録音/再生時のサンプリング周期(us) */
#define SAMPLE         0 /* 動作選択値(録音)   */
#define PLAY           1 /* 動作選択値(再生)   */
#define NOSELECT      -1 /* 動作選択値(未選択) */

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile unsigned long bufptr;
volatile int play_mode;

int            main(void);
unsigned char  menu(void);
void           sample_replay(int mode);
void           int_imia0(void);

int main(void)
{
  /* キー入力情報を取得するための変数を宣言する */
  unsigned char key_data;

  ROMEMU();      /* ROMエミュレーションをON */
  lcd_init();    /* LCD の初期化            */
  ad_init();     /* A/D 変換器の初期化      */
  da_init();     /* D/A 変換器の初期化      */
  timer_init();  /* タイマの初期化          */
  
  /* タイマ(チャネル0)の割り込み間隔 */
  timer_set(0,SAMPLINGTIME);

  /* ここにキー入力取得のためのポートの初期化を記述する */
                                      /* P60,1,2   入力 */
                                      /* PA0,1,2,3 出力 */

  while (1) {
    play_mode = NOSELECT;
    key_data = menu();                    /* メニューを選ぶ */
    /* 録音キーが押されたらSAMPLE，再生キーが押されたらPLAY */
    /* をplay_modeに格納する処理を記述する                  */
   


    /* キー入力されていれば録音/再生の関数を呼び出す処理を記述する */



  }
  return 1;
}

unsigned char  menu(void)
     /* LCD にメニューを書いて動作を選択するための関数 */
     /* 戻り値は入力キー情報                           */
{
  /* キー入力取得のための変数を宣言する */
  unsigned char cf, key_data;

  lcd_cursor(0,0);                   /* LCD にメッセージ表示 */
  lcd_printstr("Rec:[*] Play:[#]");
  lcd_cursor(0,1);
  lcd_printstr(" Push * or # key");

  /* キー入力判定用変数の初期化*/
  cf = 0;
  key_data = 0;

  while (cf == 0 ){  /* キー入力するまでループする */
    /* ここにキー入力の処理を記述する */
    

  }

  /* 入力されたキーの情報を返す */
  return key_data;
}

void sample_replay(int mode)
     /* 録音または再生を行う関数 */
     /*   mode: PLAY, SAMPLE     */
{
  if (mode == PLAY){                    /* 再生モードの処理 */
    /* ここにスピーカをスピーカとして使用する命令を記述する */

    lcd_cursor(0,1);
    lcd_printstr(" Now playing... ");
  }
  if (mode == SAMPLE){                  /* 録音モードの処理 */
    /* ここにスピーカをマイクとして使用する命令を記述する   */
    
    lcd_cursor(0,1);
    lcd_printstr(" Now Sampling...");
  }
  bufptr = 0;               /* バッファポインタを初期化 */
  timer_start(0);           /* サンプリングタイマ(チャネル0)のスタート */
  ENINT();                  /* CPU割り込み許可 */
  while (bufptr < ((unsigned long)BUFSIZE * 1024));
                            /* バッファが一杯になるまで実行 */
  speaker_switch(MIC);      /* スピーカーオフ */
  timer_stop(0);            /* タイマのストップ */
}

#pragma interrupt
void int_imia0(void)
     /* 録音・再生用のタイマ割り込みハンドラ         */
     /* プレイモードによってデータの格納か出力を行う */
{
  if (play_mode == SAMPLE){      
    /* ここに録音のときの処理を記述する(以下のコメントを参照のこと) */
                                       /* A/D変換スタート           */
                                       /* A/D変換終了まで待つ 約5us */
                                       /* 変換データを格納          */
  }
  if (play_mode == PLAY){        
    /* ここに再生のときの処理を記述する(以下のコメントを参照のこと) */
                                       /* D/Aにデータを出力         */
  }
  bufptr++;                      /* バッファポインタを +1           */
  timer_intflag_reset(0);        /* タイマ0の割り込みフラグをクリア */
  ENINT();                       /* CPUを割り込み許可状態に         */
}
