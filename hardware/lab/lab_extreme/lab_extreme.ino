#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>
#include<string.h>
#include<esp_task_wdt.h>
#include<math.h>
hw_timer_t* timer1 = NULL;
hw_timer_t* timer2 = NULL;
volatile bool state = false;

const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url_get = "https://ecourse.cpe.ku.ac.th/exceed50/api/get_ex_question";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed50/api/post_ex_answer";
#define MIN_LDR 2900
#define led 5
#define led1 21
#define led2 25
#define ldr 34
#define sw 23
#define touch 2

xTaskHandle TaskHandle_1;
xTaskHandle TaskHandle_2;
xTaskHandle TaskHandle_3;
xTaskHandle TaskHandle_4;

int res = 8;
int ch = 0;
int freq = 1000;
int cnt = 0;
int chk1 = 0;
int chk2 = 0;
bool press = false;
bool is_on = false;

//// task4
int a;
int b;
String op;
char str[100];
const int _size = 2*JSON_OBJECT_SIZE(4);

StaticJsonDocument<_size> JSONPost;
StaticJsonDocument<_size> JSONGet;
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
///////

void _get(){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url_get);
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            String payload = http.getString();
            /*JSONVar myObject = JSON.parse(payload);
            JSONVar keys = myObject.keys();*/
            DeserializationError err = deserializeJson(JSONGet,payload);
            if(err){
                Serial.print(F("deserializeJson() failed with code "));
                Serial.println(err.c_str());
            }
            else{
                Serial.print("HTTP CODE -> ");
                Serial.println(httpCode);
                Serial.print("a : ");
                Serial.println((int)JSONGet["a"]);
                Serial.print("b : ");
                Serial.println((int)JSONGet["b"]);
                Serial.print("op : ");
                Serial.println((const char*)JSONGet["op"]);
                a = (int)JSONGet["a"];
                b = (int)JSONGet["b"];
                op = (const char*)JSONGet["op"];
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
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void _post(int arg_a,int arg_b,String arg_op){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(url_post);
        http.addHeader("Content-Type","application/json");

        JSONPost["a"] = arg_a;
        JSONPost["b"] = arg_b;
        JSONPost["op"] = arg_op;
        if(arg_op == "+")
            JSONPost["c"] = (int)arg_a+arg_b;
        else if(arg_op == "-")
            JSONPost["c"] = (int)arg_a-arg_b;
        else if(arg_op == "*")
            JSONPost["c"] = (int)arg_a*arg_b;
        else if(arg_op == "^")
            JSONPost["c"] = (int)pow(arg_a,arg_b);

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
    vTaskDelay(100 / portTICK_PERIOD_MS);
}


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
void ledOff(){
    digitalWrite(led,1);
    is_on = false;
}

void setup()
{
    pinMode(ldr, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(sw,INPUT);
    digitalWrite(led,HIGH);
    digitalWrite(led1,LOW);
    Serial.begin(115200);
   
   
    xTaskCreatePinnedToCore(Task1, "Task 1", 10240, NULL, 1, &TaskHandle_1, 1); // stable
    xTaskCreatePinnedToCore(Task2, "Task 2", 10240, NULL, 1, &TaskHandle_2, 1); // stable
    xTaskCreatePinnedToCore(Task3, "Task 3", 10240, NULL, 1, &TaskHandle_3, 0); // stable*/
    xTaskCreatePinnedToCore(Task4, "Task 4", 10240, NULL, 2, &TaskHandle_4, 0);
   
    
}
void loop() // task 4
{
    /*
    _get();
    _post(a,b,op);
    delay(5000);
    */
    
}


void Task2(void* parameter){
    while(1){
        Serial.println(touchRead(touch));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void Task3(void* parameter){
    while(1){
        int freq = analogRead(ldr);
        int timee = (int)map(freq,850,3500,700,50);
        digitalWrite(led1,HIGH);
        vTaskDelay(timee / portTICK_PERIOD_MS);
        digitalWrite(led1,LOW);
        vTaskDelay(timee / portTICK_PERIOD_MS);
    }
}
void Task4(void* parameter){
    while(1){
        vTaskSuspend(TaskHandle_1);
        _get();
        _post(a,b,op);
        vTaskResume(TaskHandle_1);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
void Task1(void* parameter){
    //vTaskSuspend(TaskHandle_4);
    timer1 = timerBegin(0,80,true);
    timerAttachInterrupt(timer1,debounce,true);
    timerAlarmWrite(timer1,100,true);
    timerAlarmEnable(timer1);

    timer2 = timerBegin(1,80,true);
    timerAttachInterrupt(timer2,ledOff,true);
    timerAlarmWrite(timer2,5000000,true);
    
    while(1){
        if(state){
            if(!press){ // press first time 
                digitalWrite(led,0);
                timerAlarmEnable(timer2);
                is_on = true;
                press = true;
            }
            else{
                if(is_on){ // press second before 5 minutes
                    digitalWrite(led, 1);
                    is_on = false;
                }
                else{ // press second after 5 minutes
                    digitalWrite(led,0);
                    is_on = true;
                    timerWrite(timer2,0);  
                }
            
            }
            while(state);
        }
       
        
    }
    //vTaskResume(TaskHandle_4);
    //vTaskDelay(100 / portTICK_PERIOD_MS);
}
