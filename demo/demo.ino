#define fsr_pin1 39   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO39
#define fsr_pin2 35   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO35
#define fsr_pin3 34   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO34
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*定義pin腳與LED燈泡個數*/
#define LED_PIN1   17  //RTL:PA_27(2) == arduino:2 == esp32:GPIO0
#define LED_PIN2   4  //RTL:PA_27(2) == arduino:2 == esp32:GPIO2
#define LED_PIN3   16  //RTL:PA_27(2) == arduino:2 == esp32:GPIO4
#define LED_COUNT1 8
#define LED_COUNT2 24

/*定義pin腳與LED燈泡個數*/
#define LED_PIN1   17  //RTL:PA_27(2) == arduino:2 == esp32:GPIO0
#define LED_PIN2   4  //RTL:PA_27(2) == arduino:2 == esp32:GPIO2
#define LED_PIN3   16  //RTL:PA_27(2) == arduino:2 == esp32:GPIO4
#define LED_COUNT1 8
#define LED_COUNT2 24

/*初始LED燈*/
Adafruit_NeoPixel strip1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);   //中間：8顆燈 pin腳 = 0
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);   //左：24顆燈 pin腳 = 2
Adafruit_NeoPixel strip3(LED_COUNT2, LED_PIN3, NEO_GRB + NEO_KHZ800);   //右：24顆燈 pin腳 = 4

void setup() {
  // put your setup code here, to run once:
  /*LED燈function宣告*/
  //中間的兩個8燈
  strip1.begin();           
  strip1.show();            
  strip1.setBrightness(50);

  //左邊的24燈
  strip2.begin();           
  strip2.show();            
  strip2.setBrightness(50);

  //右邊的24燈
  strip3.begin();           
  strip3.show();          
  strip3.setBrightness(50);


}

void loop() {
  all_light_start();
  delay(1000);

  colorWipe1(strip1.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe1(strip1.Color(0, 0, 0), 0);
  delay(500);
  colorWipe2(strip2.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe2(strip2.Color(0, 0, 0), 0);
  delay(500);
  colorWipe3(strip3.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe3(strip3.Color(0, 0, 0), 0);
  delay(500);
  colorWipe1(strip1.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe1(strip1.Color(0, 0, 0), 0);
  delay(500);
  colorWipe2(strip2.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe2(strip2.Color(0, 0, 0), 0);
  delay(500);
  colorWipe3(strip3.Color(0, 255, 0), 0);
  delay(1000);
  colorWipe3(strip3.Color(0, 0, 0), 0);
  
  all_light_end();
  delay(1500);
}


/*開始全亮*/
void all_light_start(){
  colorWipe1(strip1.Color(0, 0, 255), 0);
  colorWipe2(strip2.Color(0, 0, 255), 0);
  colorWipe3(strip3.Color(0, 0, 255), 0);
  delay(3000);
  colorWipe1(strip1.Color(0, 0, 0), 0);
  colorWipe2(strip2.Color(0, 0, 0), 0);
  colorWipe3(strip3.Color(0, 0, 0), 0);
  
}
/*結束全亮*/
void all_light_end(){
  colorWipe1(strip1.Color(255, 0, 0), 0);
  colorWipe2(strip2.Color(255, 0, 0), 0);
  colorWipe3(strip3.Color(255, 0, 0), 0);
  delay(3000);
  colorWipe1(strip1.Color(0, 0, 0), 0);
  colorWipe2(strip2.Color(0, 0, 0), 0);
  colorWipe3(strip3.Color(0, 0, 0), 0);
  
}

/*亮燈初始化*/
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

void colorWipe3(uint32_t color, int wait) {
  for(int i=0; i<strip3.numPixels(); i++) { // For each pixel in strip...
    strip3.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip3.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
