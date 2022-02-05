#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>
#define led1 22
#define led2 5
#define ldr 34
#define tckMs portTICK_PERIOD_MS
const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=4";

static int ldrVal;
static int led1Val;
static int led2Val;
char str[50];
const int _size = 2*JSON_OBJECT_SIZE(3);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;
static TaskHandle_t taskGet = NULL;
static TaskHandle_t taskPost = NULL;

void assignParam(){
    ldrVal = analogRead(ldr);
    led1Val = digitalRead(led1);
    led2Val = digitalRead(led2);

}
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
void _get(void* param){
    while(1){
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
                    assignParam();
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
    vTaskDelay(500/tckMs);
    
}

void _post(void* param){
    while(1){
        if(WiFi.status() == WL_CONNECTED){
            HTTPClient http;
            http.begin(url);
            http.addHeader("Content-Type","application/json");

            JSONPost["ldr"] = ldrVal;
            JSONPost["led1"] = led1Val;
            JSONPost["led2"] = led2Val;
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
    vTaskDelay(500/tckMs);
}

void setup(){
    Serial.begin(115200);
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(ldr,INPUT);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    delay(4000);
    WiFi.mode(WIFI_STA);
    WiFi_Connect();

    assignParam();
    xTaskCreatePinnedToCore(_get,"get",1024*32,NULL,1,&taskGet,0);
    xTaskCreatePinnedToCore(_post,"post",1024*32,NULL,2,&taskPost,1);
}

void loop(){
    digitalWrite(led1,!digitalRead(led1));
    digitalWrite(led2,!digitalRead(led2));
    vTaskDelay(1000/tckMs);
}
