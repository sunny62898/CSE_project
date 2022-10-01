#include "WiFi.h" 
#include "ESPAsyncWebServer.h" 
const char *ssid = "test"; 
const char *password = "testpassword"; 
AsyncWebServer server(80); 
void setup(){ 
  Serial.begin(115200); 
  WiFi.softAP(ssid, password); 
  Serial.println(); 
  Serial.print("IP address: "); 
  Serial.println(WiFi.softAPIP()); 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(200, "text/html", R"===(<html>
                                          <head>
                                            <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">
                                            <link rel=\"icon\" href=\"data:,\">
                                            <style>
                                              html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
                                              .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
                                              text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
                                              .button2 {background-color: #555555;}
                                              
                                            </style>
                                          </head>
                                          <body>
                                            <h1>Taekwondo</h1>
                                            <a href=\"/start/on\"><button class=\"button\">START</button></a>
                                            <a href=\"/end/off\"><button class=\"button button2\">END</button></a>
                                          </body>
                                          </html>
                                           
                                           )===");
  });
    
    
  server.begin(); 
} 
void loop(){}
