#include "h8-3052-iodef.h"
#include "lcd.h"

int main(void)
{
  lcd_init();

  lcd_cursor(0,0);
  lcd_printch('a');

  lcd_cursor(1,0);
  lcd_printch('b');

  lcd_cursor(2,0);
  lcd_printch('c');

  lcd_cursor(3,1);
  lcd_printch('D');

  return 0;
}
