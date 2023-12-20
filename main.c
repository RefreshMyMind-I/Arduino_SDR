//SI5351_vfo2.1 + LCD + Output Clk0/Clk2 

#include "si5351.h"
#include "Wire.h"
#include <LiquidCrystal.h>

Si5351 si5351;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

unsigned long  freq;
void setup(void)

{

  Serial.begin(9600);
  Serial.println("Si5351 Clockgen"); Serial.println("");
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);  
  si5351.set_correction(162100, SI5351_PLL_INPUT_XO); 
  Serial.println(10100);  
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(4040000000ULL,  SI5351_CLK1);
  si5351.output_enable(SI5351_CLK1, 1);
  freq = 10100;
  lcd.begin(16, 2);
  lcd.print(freq);
}

static void flush_input(void)
{
  while (Serial.available() > 0)
  Serial.read();
}

void loop(void)
{
  si5351.update_status();
  if (si5351.dev_status.SYS_INIT == 1) {
     setup();
     delay(500);
  }
  if (Serial.available()) {
    int ch = Serial.read();
    freq = Serial.parseInt();
    Serial.println (freq);
    int ch2 = Serial.read();
    delay(1);
    ch2 = Serial.read();
   
    if (freq> 8){
      lcd.setCursor(0, 0);
      lcd.print(freq);
      lcd.print("     ");
      if (ch == 32) si5351.set_freq(freq*400000ULL, SI5351_CLK1);  //" "
      if (ch == 70) si5351.set_freq(freq*400000ULL, SI5351_CLK1);  //F
      if (ch == 102) si5351.set_freq(freq*400ULL, SI5351_CLK1);    //f 
    
      if (ch == 65) si5351.set_freq(freq*100000ULL, SI5351_CLK0);  //A
      if (ch == 97) si5351.set_freq(freq*100ULL, SI5351_CLK0);     //a
      if (ch == 66) si5351.set_freq(freq*100000ULL, SI5351_CLK2);  //B
      if (ch == 98) si5351.set_freq(freq*100ULL, SI5351_CLK2);     //b
    } 
    if (freq==0){
      if (ch == 65) si5351.output_enable(SI5351_CLK0, 0);
      if (ch == 66) si5351.output_enable(SI5351_CLK2, 0);
      if (ch == 70) si5351.output_enable(SI5351_CLK1, 0);
    }
     if (freq==1){
      if (ch == 65) si5351.output_enable(SI5351_CLK0, 1);
      if (ch == 66) si5351.output_enable(SI5351_CLK2, 1);
      if (ch == 70) si5351.output_enable(SI5351_CLK1, 1);
    }
  }
}


