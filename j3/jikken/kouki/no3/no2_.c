#include "h8-3052-iodef.h"
/* #include "h8-3052-int.h" */
#include "lcd.h"
#include "ad.h"
#include "da.h"

#define BUFSIZE       30 /* バッファの大きさ(kB) */
#define SAMPLE         0 /* 動作選択(録音)       */
#define PLAY           1 /* 動作選択(再生)       */
#define NOSELECT      -1 /* 動作選択値(末選択)   */
#define TIME      120000 /* 録音/再生時間        */

#define LOOP (13200)

#define ROKUON_KEY ('*')
#define SAISEI_KEY ('#')

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile unsigned long bufptr;
volatile int play_mode;

unsigned char  menu(void);
void           sample_replay(int mode);

void delay( unsigned long int ms )
{
	unsigned long int i, j;

	for( i = 0; i < ms; i++ )
		for( j = 0; j < LOOP; j++);
}
void delay_u( unsigned long int us )
{
	unsigned long int i, j;

	for( i = 0; i < us; i++ )
		for( j = 0; j < (LOOP/1000); j++);
}




int main(void)
{
	/* キー入力情報を取得するための変数を宣言する */
	unsigned char key_data;

	lcd_init();    /* LCD の初期化            */
	ad_init();     /* A/D 変換器の初期化      */
	da_init();     /* D/A 変換器の初期化      */

	/* ◎ここにキー入力取得のためのポートの初期化を記述する */
	P6DDR &= ~0x07;  /* P60,1,2   入力 */
	PADDR |= 0x0f;  /* PA0,1,2,3 出力 */

	while (1)
	{
		play_mode = NOSELECT;
		key_data = menu();                    /* メニューを選ぶ   */
		/* 録音キーが押されたらSAMPLE, 
		 * 再生キーが押されたらPLAY
		 * をplay_modeに格納する処理を記述する */
		switch( key_data ){
			case ROKUON_KEY:
				play_mode = SAMPLE;
				break;

			case SAISEI_KEY:
				play_mode = PLAY;
				break;
		}

		/* ◎ここに「*」キーが入力されていれば録音/再生の関数を呼び出す
		 * 処理を記述する */
		sample_replay( play_mode );
	}

	return 1;  /* 通常，ここには辿り着かないので，異常状態「1」を返す */
}


/*
 * LCD にメニューを書いて動作を選択するための関数
 * 戻り値は入力キー情報
 */
unsigned char  menu(void)
{
	/* キー入力取得のための変数を宣言する */
	unsigned char cf, key_data;

	lcd_cursor(0,0);                   /* LCD にメッセージ表示 */
	lcd_printstr("Rec:[*] Play:[#]");
	lcd_cursor(0,1);
	lcd_printstr(" Push * or # key");

	/* キー入力判定用変数の初期化 */
	cf = 0;
	key_data = 0;

	while (cf == 0)  /* キー入力するまでループする */
	{
		/* ここにキー入力の処理を記述する */
		PADR = 0x0e;
		cf = P6DR;
		cf = ~cf;
		cf &= 0x07;

		/* ◎key 「*」,「#」のみの入力に対応させる処理を記述する */
		//key *,0,#
		switch(cf) {
			case 1 : key_data = '*'; break;
			case 4 : key_data = '#'; break;
		}  
		if( key_data != '*' && key_data != '#' ){
			cf = 0;
		}
	}

	/* 入力されたキーの情報を返す */
	return key_data;
}


/*
 * 録音または再生を行う関数
 * mode: PLAY, SAMPLE
 */
void sample_replay(int mode)
{
	bufptr = 0;                              /* バッファポインタを初期化  */
	int counter = 0;
	char counter_str[6] = {'\0'};
	lcd_clear();
	if (mode == PLAY)                        /* 再生モードの処理 */
	{
		/* スピーカ兼マイクをスピーカとして使用する */
		speaker_switch(SPEAKER);
		lcd_cursor(0,1);
		lcd_printstr(" Now Playing... ");


		/* 音声出力処理内容を記述する */
		while(bufptr <= TIME)                  /*   データ出力を繰り返す    */
		{
			da_out( 0, databuf[bufptr] );                                    /* ◎Ｄ／Ａにデータを出力    */
			delay_u(3);
			bufptr++;                                    /* ◎バッファポインタを +1   */

			if((counter % (TIME / 5)) == 0){
				lcd_cursor(0, 1);
				counter_str[counter / (TIME/5)] = '1' + (counter / (TIME/5));
				lcd_printstr(counter_str);
			}
			counter++;
		}   
	}
	if (mode == SAMPLE)                      /* 録音モードの処理 */
	{
		/* スピーカ兼マイクをマイクとして使用する */
		speaker_switch(MIC);
		lcd_cursor(0,1);

		/* 音声取込処理内容を記述する */
		while(bufptr <= TIME)                  /*   データ読込を繰り返す     */
		{
			ad_start( 0, 0 );                                    /* ◎Ａ／Ｄ変換スタート       */
			while( ADSTATUS() == 0 );
			databuf[bufptr] = ADREAD();                                    /* ◎変換データを格納　 　　　*/
			bufptr++;                                    /* ◎バッファポインタを +1    */

			if((counter % (TIME / 5)) == 0){
				lcd_cursor(0, 1);
				counter_str[counter / (TIME/5)] = '1' + (counter / (TIME/5));
				lcd_printstr(counter_str);
				delay(1000);
			}
			counter++;
		}
	}
	speaker_switch(MIC);                     /* スピーカーオフ   */
}

