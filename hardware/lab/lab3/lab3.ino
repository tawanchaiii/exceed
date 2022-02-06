#include<WiFi.h>
#include<HTTPClient.h>
#include<ArduinoJson.h>
#include <string.h>
const char* ssid = "Champ SE";
const char* password = "qazwsxedc123";
const char* url_get = "https://ecourse.cpe.ku.ac.th/exceed48/api/get_question";
const char* url_post = "https://ecourse.cpe.ku.ac.th/exceed48/api/post_answer";

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
    delay(100);
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


void setup()
{
    Serial.begin(115200);
    delay(4000);
    WiFi_Connect();
}
void loop()
{
    _get();
    _post(a,b,op);
    delay(5000);
}