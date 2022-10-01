/*改自Adafruit NeoPixel - strandtest.ino*/
#define fsr_pin1 39   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO39
#define fsr_pin2 35   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO35
#define fsr_pin3 34   //RTL:PB_3(6) == arduino:A0 == esp32:GPIO34
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*WIFI設定的定義與連線變數宣告*/
#include "ESPAsyncWebServer.h" 
#include<WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
const char *ssid = "test"; 
const char *password = "testpassword"; 
//WiFiServer server(80);  //設定網路伺服器 port number 為 80
AsyncWebServer server(80); 
WiFiClient client;
//MySQL_Connection conn((Client *)&client); //連接MySQL的物件

/*定義pin腳與LED燈泡個數*/
#define LED_PIN1   17  //RTL:PA_27(2) == arduino:2 == esp32:GPIO0
#define LED_PIN2   4  //RTL:PA_27(2) == arduino:2 == esp32:GPIO2
#define LED_PIN3   16  //RTL:PA_27(2) == arduino:2 == esp32:GPIO4
#define LED_COUNT1 8
#define LED_COUNT2 24

/*宣告全域變數*/
float return_value = 0;   //取得換算過後>2000的值(g)
float fsr_value;    //讀取FSR的值
float fsr_value1;    //讀取FSR的值
float fsr_value2;    //讀取FSR的值
float fsr_value3;    //讀取FSR的值
long randnum;   //隨機選哪個燈亮
int flag = 0;   //決定亮燈flag
int tflag = 0;  //計算時間flag(用來判別是否重新計算時間)
int start_flag = 0; //決定是否開始踢擊

/*踢擊結果紀錄加顯示*/
float time_speed[100];
float force_value[100];
int stack_num = 0;
String outputResult; //顯示踢擊結果

/*時間全域變數*/
unsigned long time1 = millis();   //讀取開始時間(初始時間)
unsigned long time2;  //讀取結束時間
float timing;   //計算反應時間
unsigned long game_start = 0;
unsigned long game_end = 0;
unsigned long game_time = 0;

/*web 變數設定*/
String header;  //讀取web資訊
String device_state = "off";  //裝置狀態
unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0;       // Previous time
const long timeoutTime = 2000;        // Define timeout time in milliseconds (example: 2000ms = 2s)

/*初始LED燈*/
Adafruit_NeoPixel strip1(LED_COUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);   //中間：8顆燈 pin腳 = 0
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);   //左：24顆燈 pin腳 = 2
Adafruit_NeoPixel strip3(LED_COUNT2, LED_PIN3, NEO_GRB + NEO_KHZ800);   //右：24顆燈 pin腳 = 4

/*宣告SQL資料庫相關變數
int MYSQLPort = 3306;   //輸入MySQL使用的port號(預設為3306)
char user[] = "username";  //MySQL使用者的帳號
char pass[] = "username";  //MySQL使用者的密碼*/

void setup()
{
  Serial.begin(115200);
  pinMode(fsr_pin1, INPUT);  //FSR pin
  pinMode(fsr_pin2, INPUT);  //FSR pin
  pinMode(fsr_pin3, INPUT);  //FSR pin

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
   clock_prescale_set(clock_div_1);
  #endif

  /*LED燈function宣告*/
  //中間的兩個8燈
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(50);

  //左邊的24燈
  strip2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();            // Turn OFF all pixels ASAP
  strip2.setBrightness(50);

  //右邊的24燈
  strip3.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip3.show();            // Turn OFF all pixels ASAP
  strip3.setBrightness(50);

  randomSeed(analogRead(0));  //random seed

  wifi_MySQL();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(200, "text/html", R"===(<html>
                                          <head>
                                            <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
                                            <link rel=\"icon\" href=\"data:,">
                                            <style>
                                              html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
                                              .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
                                              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
                                              .button2 {background-color: #555555;}
                                              
                                            </style>
                                          </head>
                                          <body>
                                            <h1>Taekwondo</h1>
                                            <a href="/start/on"><button class="button">START</button></a>
                                          </body>
                                          </html>
                                           
                                           )===");
  });
  
  //開始
  server.on("/start/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    device_state = "on";
    //request->send(200, "text/plain", "OK");
    request->send(200, "text/html", R"===(<html>
                                          <head>
                                            <meta name="viewport" content="width=device-width, initial-scale=1">
                                            <link rel="icon" href="data:,">
                                            <style>
                                              html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
                                              .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
                                              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
                                              .button2 {background-color: #555555;}
                                              
                                            </style>
                                          </head>
                                          <body>
                                            <h1>Taekwondo</h1>
                                            <a href="/show/result"><button class="button">Show result</button></a>
                                          </body>
                                          </html>
                                           
                                           )===");
  });
  
  //結束
  server.on("/show/result", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", outputResult);
  });
  server.begin();
  


  
}

void loop()
{ 
  
  if(device_state == "on"){
    stack_num = 0;  //初始stack
    //開始時全亮代表開始
    all_light();
    delay(1000);
    
    /*開始*/

    game_time = 0;
    time1 = millis();       //踢擊反應時間開始
    game_start = millis();
    while(game_time < 30000){
      random_LED();   //產生隨機亮燈的位置  
      client_start(); //開始感測
      game_end = millis();
      game_time = game_end - game_start;
    }
  
    /*結束*/
  
    //結束時全亮代表結束
    all_light();
    device_state = "off";

    outputResult = "";

    /*產生記錄*/
    outputResult += "<html>";
    outputResult += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    outputResult += "<link rel=\"icon\" href=\"data:,\">";
    outputResult += "<style>";
    outputResult += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    outputResult += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
    outputResult += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    outputResult += ".button2 {background-color: #555555;}";
    outputResult += "table {margin-left:auto; margin-right:auto;align:\"center\";width: 300px;border-collapse: collapse;}";
    outputResult += "table, td, th {border: 1px solid black;padding: 5px;}";
    outputResult += "th, td {text-align: center;}";
    outputResult += "</style></head><body>";
    outputResult += "<h1>Taekwondo</h1>";
    
    if(stack_num == 0){
      outputResult += "<h2>no result</h2>";
    }
    else{
      outputResult += "<table><tr><th>Force(kg)</th><th>Reaction time(sec)</th></tr>";                  
      for(int i = 0;i < stack_num;i++){
        outputResult += "<tr><td>" + String(force_value[i], 3) + "</td><td>" + String(time_speed[i], 3) + "</td></tr>";
      }
      outputResult += "</table>";
    }
    
    outputResult += "<p></p>";
    outputResult += "<a href=\"/start/on\"><button class=\"button\">Play again</button></a>";
    outputResult += "</body></html>";
  }


}
/*開始、結束全亮*/
void all_light(){
  colorWipe1(strip1.Color(255, 0, 0), 0);
  colorWipe2(strip2.Color(255, 0, 0), 0);
  colorWipe3(strip3.Color(255, 0, 0), 0);
  delay(3000);
  colorWipe1(strip1.Color(0, 0, 0), 0);
  colorWipe2(strip2.Color(0, 0, 0), 0);
  colorWipe3(strip3.Color(0, 0, 0), 0);
  
}

/*產生隨機亮燈位置*/
void random_LED(){
  //產生隨機亮燈位置
  if(flag == 0){
    randnum = random(1, 4);
    Serial.println(randnum);
    flag = 1;
  }
  
  if(randnum == 1){//中間：8顆燈 pin腳=0
    //亮燈
    colorWipe1(strip1.Color(255,   0,   0), 0); 
  }
  else if(randnum == 2){//左：24顆燈 pin腳 = 2
    //亮燈
    colorWipe2(strip2.Color(255,   0,   0), 0); 

  }
  else if(randnum == 3){//右：24顆燈 pin腳 = 4
    //亮燈
    colorWipe3(strip3.Color(255,   0,   0), 0); 

  }
  
}

/*client start*/
void client_start(){

  //執行程式
  //判別是否要重新計時
  if(tflag == 1){
    time1 = millis();   //取得開始時間
    tflag = 0;    //重新設定flag
  }
  

  /*選擇正確的感測器感測*/
  fsr_value1 = 0;
  fsr_value2 = 0;
  fsr_value3 = 0;
  
  fsr_value1 = analogRead(fsr_pin1); // 讀取FSR1
  fsr_value2 = analogRead(fsr_pin2); // 讀取FSR2
  fsr_value3 = analogRead(fsr_pin3); // 讀取FSR3

  
  if((fsr_value1 > fsr_value2 && fsr_value1 > fsr_value3) && randnum == 1){
    fsr_value = fsr_value1;
  }
  else if((fsr_value2 > fsr_value1 && fsr_value2 > fsr_value3) && randnum == 2){
    fsr_value = fsr_value2;
  }
  else if((fsr_value3 > fsr_value2 && fsr_value3 > fsr_value1) && randnum == 3){
    fsr_value = fsr_value3;
  }
  else{
    fsr_value = 0;
  }
  

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
    colorWipe1(strip1.Color(  0,   0,   0), 0);   //off
    colorWipe2(strip2.Color(  0,   0,   0), 0);   //off
    colorWipe3(strip3.Color(  0,   0,   0), 0);   //off

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
    //紀錄
    time_speed[stack_num] = timing;
    force_value[stack_num] = value;
    stack_num++;
    //finish
    Serial.println("-------------");//分隔線
    delay(500);  //間隔0.5秒再下一次踢擊
    flag = 0;   //重新隨機亮燈
    tflag=1;    //重新計算時間
    
  }
}

/*wifi與MySQL設定*/
void wifi_MySQL(){
  WiFi.softAP(ssid, password); 
  Serial.println(); 
  Serial.print("IP address: "); 
  Serial.println(WiFi.softAPIP()); 

 
  
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
