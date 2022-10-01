#include <Adafruit_NeoPixel.h>
#include <rp2040_pio.h>

#define led_pin 11
#define fsr_pin A0


void setup()
{
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  
  float fsr_value = analogRead(fsr_pin); // 讀取FSR
  /*
  int led_value = map(fsr_value, 0, 1023, 0, 255); // 從0~1023映射到0~255
  analogWrite(led_pin, led_value); // 改變LED亮度
  //Serial.begin(115200);
  */
  Serial.println(fsr_value);
  float value = 8.8362 * pow ( 2.7182 , ( 0.008*fsr_value ) ); 
  Serial.println(value);
  Serial.println("-------------------");
  delay(100);
  /*//Serial.println(led_value);
  Serial.println("-------------");
  */
}
