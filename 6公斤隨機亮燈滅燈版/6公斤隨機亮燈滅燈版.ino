#define fsr_pin A0
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN1   4
#define LED_PIN2   3
#define LED_PIN3   2
#define LED_COUNT1 8
#define LED_COUNT2 24

long randnum;
int flag=0;

Adafruit_NeoPixel strip1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);//中間：8顆燈 pin腳 = 4
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);//左：24顆燈 pin腳 = 3
Adafruit_NeoPixel strip3(LED_COUNT2, LED_PIN3, NEO_GRB + NEO_KHZ800);//右：24顆燈 pin腳 = 2

void setup()
{
  Serial.begin(115200);
  //pinMode(LED_PIN, OUTPUT);

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
   clock_prescale_set(clock_div_1);
  #endif
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(50);

  strip2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();            // Turn OFF all pixels ASAP
  strip2.setBrightness(50);

  strip3.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip3.show();            // Turn OFF all pixels ASAP
  strip3.setBrightness(50);

  randomSeed(analogRead(0));
}

void loop()
{
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

}

float change_kg( float value ){
  value = 1.677 * pow ( 2.7182 , ( 0.011*value ) ); 
  return value;
}

void LED_control( float value){
  //控制LED的亮暗
  if( value > 6000 ){
    //熄滅
    colorWipe1(strip1.Color(  0,   0,   0), 0); // off
    colorWipe2(strip2.Color(  0,   0,   0), 0); // off
    colorWipe3(strip3.Color(  0,   0,   0), 0); // off

    //value = change_kg(value);
    Serial.println(value);
    delay(3000);
    flag = 0;
    Serial.println("-------------");
  }
}

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