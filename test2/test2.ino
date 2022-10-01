#define fsr_pin 34 //PB_3 (6) == arduino_A2 ...== esp32 GPIO27
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*定義pin腳與LED燈泡個數*/
//#define LED_PIN1   4
//#define LED_PIN2   3
#define LED_PIN3   4 //PA_27 (2) == arduino_2...== esp32 GPIO4
#define LED_COUNT1 8
#define LED_COUNT2 24

/*宣告全域變數*/
float return_value = 0;   //取得換算過後>2000的值(g)
float fsr_value;    //讀取FSR的值
long randnum;   //隨機選哪個燈亮
int flag = 0;   //決定亮燈flag
int tflag = 0;  //計算時間flag(用來判別是否重新計算時間)

/*時間全域變數*/
unsigned long time1 = millis();   //讀取開始時間(初始時間)
unsigned long time2;  //讀取結束時間
float timing;   //計算反應時間

/*初始LED燈*/
//Adafruit_NeoPixel strip1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);   //中間：8顆燈 pin腳 = 4
//Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);   //左：24顆燈 pin腳 = 3
Adafruit_NeoPixel strip3(LED_COUNT2, LED_PIN3, NEO_GRB + NEO_KHZ800);     //右：24顆燈 pin腳 = 2

void setup()
{
  Serial.begin(115200);
  pinMode(fsr_pin, INPUT);  //LEN pin

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
   clock_prescale_set(clock_div_1);
  #endif

  /*LED燈function宣告*/
  /*
  //中間的兩個8燈
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(50);

  //左邊的24燈
  strip2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();            // Turn OFF all pixels ASAP
  strip2.setBrightness(50);
  */

  //右邊的24燈
  strip3.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip3.show();            // Turn OFF all pixels ASAP
  strip3.setBrightness(50);

  randomSeed(analogRead(0));  //random seed
}

void loop()
{
  /*
  //產生隨機亮燈位置
  if(flag == 0){
    randnum = random(1, 4);
    Serial.println(randnum);
    flag = 1;
  }
  
  if(randnum == 1){//中間：8顆燈 pin腳=4
    //亮燈
    colorWipe1(strip1.Color(255,   0,   0), 0); 

    float fsr_value = analogRead(fsr_pin); // 讀取FSR
    fsr_value = change_kg(fsr_value);
    LED_control(fsr_value);

    //Serial.println(fsr_value);
    delay(100);
    //Serial.println(led_value);
    //Serial.println("-------------");
  }
  else if(randnum == 2){//左：24顆燈 pin腳 = 3
    //亮燈
    colorWipe2(strip2.Color(255,   0,   0), 0); 

    float fsr_value = analogRead(fsr_pin); // 讀取FSR
    fsr_value = change_kg(fsr_value);
    LED_control(fsr_value);

    //Serial.println(fsr_value);
    delay(100);
    //Serial.println(led_value);
    //Serial.println("-------------");
  }
  else if(randnum == 3){//右：24顆燈 pin腳 = 2
    //亮燈
    colorWipe3(strip3.Color(255,   0,   0), 0); 

    float fsr_value = analogRead(fsr_pin); // 讀取FSR
    fsr_value = change_kg(fsr_value);
    LED_control(fsr_value);

    //Serial.println(fsr_value);
    delay(100);
    //Serial.println(led_value);
    //Serial.println("-------------");
  }
  */

  /*判別是否要重新計時*/
  if(tflag == 1){
    time1 = millis();   //取得開始時間
    tflag = 0;    //重新設定flag
  }
  
  //亮燈
  colorWipe3(strip3.Color(255, 0, 0), 0);   //亮燈
  
  fsr_value = analogRead(fsr_pin); // 讀取FSR
  return_value = 0;   //初始return_value
  
  
  fsr_value = change_kg(fsr_value);       //將FSR讀取到的數值轉換為公斤數
  return_value = LED_control(fsr_value);  //檢查是否有達到踢擊標準(2kg)，若沒有達到則回傳0
  stop_time(return_value);                //將回傳的值傳入，若不是0則停止計時並顯示時間和力道
  
   
}

/*轉換kg function*/
float change_kg( float value ){
  value = 171.92 * pow ( 2.7182 , ( 0.001*value ) );  //轉換為公克(g) 
  value = 1e-3 * (value);   //轉換為公斤(kg)
  return value;
}

/*如果力量有達標，讓燈熄滅，燈滅function*/
float LED_control( float value){
  //控制LED的亮暗
  if( value > 2 ){  //大於2kg -> 有達到標準
    //熄滅
    //colorWipe1(strip1.Color(  0,   0,   0), 0);   //off
    //colorWipe2(strip2.Color(  0,   0,   0), 0);   //off
    colorWipe3(strip3.Color(  0,   0,   0), 0);     //off

    return value;
  }
  else{ //若沒有達標則return 0
    return 0;
  }
}

/*結束時間計算並顯示出踢擊力道和反應時間*/
void stop_time( float value){
  if(value != 0){
    time2 = millis(); // 結束時間
    timing = 1e-3*(float)(time2-time1);  //計算時間
    Serial.println(timing);         //顯示反應時間
    Serial.println(value);   //顯示踢擊力道
    Serial.println("-------------");//分隔線
    delay(3000);  //間隔三秒再下一次踢擊
    flag = 0;   //重新隨機亮燈
    tflag=1;    //重新計算時間
    
  }
}
/*
void colorWipe1(uint32_t color, int wait) {
  for(int i=0; i<strip1.numPixels(); i++) { // For each pixel in strip...
    strip1.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip1.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void colorWipe2(uint32_t color, int wait) {
  for(int i=0; i<strip2.numPixels(); i++) { // For each pixel in strip...
    strip2.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip2.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
*/
void colorWipe3(uint32_t color, int wait) {
  for(int i=0; i<strip3.numPixels(); i++) { // For each pixel in strip...
    strip3.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
