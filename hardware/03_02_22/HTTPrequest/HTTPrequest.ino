#include<WiFi.h>
#include<HTTPClient.h>
const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=4";

void setup(){
    Serial.begin(115200);
    delay(4000);
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.println("Connecting WiFi...");
    }
    Serial.println("Connected Finish");
}

void loop(){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode < 400){
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        }
    }
    else 
        Serial.println("Error on HTTP Request");
    delay(10000);
}
