void TaskA(void *parameter){
    while(1){
        for(int i=0;i<50;i++)
            Serial.print("*");
        Serial.println("A");
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
void TaskB(void *parameter){
    while(1){
        for(int i=0;i<50;i++)
            Serial.print("$");
        Serial.println("B");
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
