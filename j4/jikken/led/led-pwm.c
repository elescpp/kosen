#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

#include "lcd.h"
#include "ad.h"
#include "timer.h"
#include "key.h"

/* タイマ割り込みの時間間隔[μs] */
#define TIMER0 1000

/* 割り込み処理で各処理を行う頻度を決める定数 */
#define DISPTIME 100
#define KEYTIME 1
#define ADTIME  2
#define PWMTIME 1
#define CONTROLTIME 10

/* LED関係 */
/* LED制御のPWM値がとれる最小値と最大値 */
#define LEDMINVAL 0
#define LEDMAXVAL 10
/* LEDがP9に接続されているビット位置 */
#define REDLEDPOS   0x10
#define GREENLEDPOS 0x20

/* LCD表示関連 */
/* 1段に表示できる文字数 */
#define LCDDISPSIZE 16

/* PWM制御関連 */
/* 制御周期を決める定数 */
#define MAXPWMCOUNT 10

/* 割り込み処理に必要な変数は大域変数にとる */
volatile int disp_time, key_time, ad_time, pwm_time, control_time;
/* LED関係 */
unsigned char redval, greenval;
/* LCD関係 */
volatile int disp_flag;
volatile char lcd_str_upper[LCDDISPSIZE+1];
volatile char lcd_str_lower[LCDDISPSIZE+1];
/* モータ制御関係 */
volatile int pwm_count;

int main(void);
void int_imia0(void);
void pwm_proc(void);
void control_proc(void);

int main(void)
{
  /* 初期化 */
  ROMEMU();           /* ROMエミュレーションをON */

  /* ここでLEDポート(P9)の初期化を行う */
  P9DDR |= (REDLEDPOS | GREENLEDPOS);
  P9DR |= (REDLEDPOS | GREENLEDPOS);

  /* 割り込みで使用する大域変数の初期化 */
  pwm_time = pwm_count = 0;     /* PWM制御関連 */
  disp_time = 0; disp_flag = 1; /* 表示関連 */
  key_time = 0;                 /* キー入力関連 */
  ad_time = 0;                  /* A/D変換関連 */
  control_time = 0;             /* 制御関連 */
  /* ここまで */
  lcd_init();         /* LCD表示器の初期化 */
  key_init();         /* キースキャンの初期化 */
  ad_init();          /* A/Dの初期化 */
  timer_init();       /* タイマの初期化 */
  timer_set(0,1000);  /* タイマ0でシステム時間計測 */
  timer_start(0);     /* タイマ0スタート */
  ENINT();            /* 全割り込み受付可 */
  redval = greenval = 0; /* 赤・緑LEDの両方を消灯とする */

  /* ここでLCDに表示する文字列を初期化しておく */

  while (1){ /* 普段はこのループを実行している */

    /* ここで disp_flag によってLCDの表示を更新する */
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
  
  /* その他の処理はタイマ割り込みによって自動的に実行されるため  */
  /* タイマ 0 の割り込みハンドラ内に関数を登録しなければならない */
}


#pragma interrupt
void int_imia0(void)
     /* タイマ0(GRA) の割り込みハンドラ　　　　　　　　　　　　　　　 */
     /* 関数の名前はリンカスクリプトで固定している                   */
     /* 関数の直前に割り込みハンドラ指定の #pragama interrupt が必要 */
     /* タイマ割り込みによって各処理の呼出しが行われる               */
     /* 呼出しの頻度は KEYTIME,ADTIME,PWMTIME,CONTROLTIME で決まる */
     /* 全ての処理が終わるまで割り込みはマスクされる                 */
     /* 各処理は基本的に割り込み周期内で終わらなければならない       */
     /* 設定できる割り込み周期の最大値は 20971.2[μs]                */
{
  /* LCD表示の処理 */
  /* 他の処理を書くときの参考 */
  disp_time++;
  if (disp_time >= DISPTIME){
    disp_flag = 1;
    disp_time = 0;
  }

  /* ここにキー処理に分岐するための処理を書く */

  
  /* ここにPWM処理に分岐するための処理を書く */
  pwm_time++;
  if(pwm_time >= PWMTIME){
    pwm_time = 0;    
    pwm_proc();
  }
  
  /* ここにA/D変換開始の処理を書く */
  
  /* ここに制御処理に分岐するための処理を書く */

  timer_intflag_reset(0); /* 割り込みフラグをクリア */
  ENINT();                /* CPUを割り込み許可状態に */
}

void pwm_proc(void)
	/* PWM制御を行う関数                                        */
	/* この関数はタイマ割り込み0の割り込みハンドラから呼び出される */
{
	pwm_count++;


	char temp = P9DR;

	/* ここにPWM制御の中身を書く */
	if(pwm_count >= MAXPWMCOUNT){
		//turn led off
		temp |= (REDLEDPOS | GREENLEDPOS);
		//reset counter
		pwm_count = 0;
	}
	if(pwm_count >= redval){
		temp &= ~REDLEDPOS;
	}
	if(pwm_count >= greenval){
		temp &= ~GREENLEDPOS;
	}

	P9DR = temp;
}

void control_proc(void)
	/* 制御を行う関数                                           */
	/* この関数はタイマ割り込み0の割り込みハンドラから呼び出される */
{

	/* ここに制御処理を書く */

}
