#include<esp_task_wdt.h>

void setup(){
    Serial.begin(115200);
    Serial.println("Init WDT....");
    esp_task_wdt_init(1,true);
    esp_task_wdt_add(NULL);
}
void loop(){
    for(int i=0;i<5;i++){
        Serial.println("Resetting WDT....");
        esp_task_wdt_reset();
        delay(100);
    }
    while(1){
        Serial.println("STOP WDT reset.");
        delay(100);
    }
}