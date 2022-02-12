#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>
#include<string.h>
#include<esp_task_wdt.h>
#include<math.h>
const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url_get = "https://ecourse.cpe.ku.ac.th/exceed48/api/get_question";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed49/api/payload?gid=4";

#define led 5
#define led1 22
#define led2 21
#define ldr 34
#define sw 23
#define touch1 2
#define touch2 0

char str[100];
const int _size = 2*JSON_OBJECT_SIZE(4);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;

hw_timer_t *timer1 = NULL;

volatile bool state = false;

void debounce(){
    static int count = 20;
    if(digitalRead(sw)){
        count--;
        if(count<0) 
            count = 0;
        if(count==0)
            state = true;
    }
    else{
        count++;
        if(count>20)
            count=20;
        if(count==20)
            state = false;

    }
}

void WiFi_Connect(){
    WiFi.disconnect();
    WiFi.begin(ssid,password);
    while(WiFi.status() != WL_CONNECTED){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        Serial.println("Connecting WiFi...");
    }
    Serial.println("Connected Finish");
    Serial.print("IP = ");
    Serial.println(WiFi.localIP());
}
void _post(){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url_post);
        http.addHeader("Content-Type","application/json");
        /*JSONPost["room"] = room;
        if(room==1)
            JSONPost["status"]  = (int)digitalRead(led);
        else if (room == 2)
            JSONPost["status"]  = (int)digitalRead(led1);
        else if (room == 3)
            JSONPost["status"]  = (int)digitalRead(led2);
            
        */
        JSONPost["ldr"]  = (int)digitalRead(led);
        JSONPost["led1"]  = (int)digitalRead(led1);
        JSONPost["led2"]  = (int)digitalRead(led2);
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
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
void setup(){
    Serial.begin(115200);
    pinMode(ldr, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(sw,INPUT);
    digitalWrite(led,LOW);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    WiFi_Connect();
    timer1 = timerBegin(0,80,true);
    timerAttachInterrupt(timer1,debounce,true);
    timerAlarmWrite(timer1,100,true);
    timerAlarmEnable(timer1);
    xTaskCreatePinnedToCore(check_toilet_sw, "sw_task", 1024*32, NULL, 1, NULL, 0); // stable
    xTaskCreatePinnedToCore(check_toilet_touch1, "touch_task", 1024*32, NULL, 1,NULL, 0); // stable
    xTaskCreatePinnedToCore(check_toilet_ldr, "ldr_task", 1024*32, NULL, 1,NULL, 1); // stable
}


void loop(){
    //_post();
    //vTaskDelay(5000/portTICK_PERIOD_MS);
}

void check_toilet_sw(void *parameter){
    while(1){
      if(state){
        digitalWrite(led,!digitalRead(led));
        _post();
        while(state);
      }
    vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void check_toilet_touch1(void *parameter){
    while(1){
        if(touchRead(touch1) < 30 && digitalRead(led1) == 1){
            digitalWrite(led1,0);
            _post();
        }
        if(touchRead(touch1) > 30 && digitalRead(led1) == 0){
            digitalWrite(led1,1);
            _post();
        }
        
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}


void check_toilet_ldr(void *parameter){
    while(1){

        if(analogRead(ldr) > 950 && digitalRead(led2) == 1){
            digitalWrite(led2,0);
            _post();
        }
        if(analogRead(ldr) < 950 && digitalRead(led2) == 0){
            digitalWrite(led2,1);
            _post();
        }
        
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    
}
void check_toilet_touch2(void *parameter){
    while(1){
        if(touchRead(touch2) < 30 && digitalRead(led1) == 1){
            digitalWrite(led1,0);
        }
        if(touchRead(touch2) > 30 && digitalRead(led1) == 0){
            digitalWrite(led1,1);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
