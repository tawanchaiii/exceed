#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>
#define led1 22
#define led2 5
#define ldr 34
const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=4";

static int ldr_val;
static int led1_val;
static int led3_val;
char str[50];
const int _size = 2*JSON_OBJECT_SIZE(3);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;
void WiFi_Connect(){
    WiFi.disconnect();
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.println("Connecting WiFi...");
    }
    Serial.println("Connected Finish");
    Serial.print("IP = ");
    Serial.println(WiFi.localIP());
}
void _get(){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            String payload = http.getString();
            DeserializationError err = deserializeJson(JSONGet,payload);
            if(err){
                Serial.print(F("deserializeJson() failed with code "));
                Serial.println(err.c_str());
            }
            else{
                Serial.println(httpCode);
                Serial.print("ldr : ");
                Serial.println((int)JSONGet["ldr"]);
                Serial.print("led1 : ");
                Serial.println((int)JSONGet["led1"]);
                Serial.print("led2 : ");
                Serial.println((int)JSONGet["led2"]);
            }
        }
        else{
            Serial.println(httpCode);
            Serial.println("ERROR on HTTP Request");
        }
    }
    else {
        WiFi_Connect();
    }
}

void _post(int ldr_post,int led1_post,int led2_post){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url);
        http.addHeader("Content-Type","application/json");

        JSONPost["ldr"] = ldr_post;
        JSONPost["led1"] = led1_post;
        JSONPost["led2"] = led2_post;
        serializeJson(JSONPost,str);
        int httpCode = http.POST(str);
        if(httpCode == HTTP_CODE_OK){
            String payload = http.getString();
            Serial.println(httpCode);
            Serial.println(payload);
        }
        else{
            Serial.println(httpCode);
            Serial.println("ERROR on HTTP Request");
        }
    }
    else 
        WiFi_Connect();
    delay(100);
}

void setup(){
    Serial.begin(115200);
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(ldr,INPUT);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    delay(4000);
    WiFi_Connect();
}

void loop(){
    _post(analogRead(ldr),!digitalRead(led1),!digitalRead(led2));
    _get();
    digitalWrite(led1,(int)JSONGet["led1"]);
    digitalWrite(led2,(int)JSONGet["led2"]);
    delay(1000);
}
