int res = 8;
int ch = 0;
int freq = 1000;
#define ldr 34
#define led2 25
void setup()
{
    Serial.begin(115200);
    pinMode(ldr, INPUT);
    pinMode(led2, OUTPUT);
    led2_task();
}
void loop()
{
}
void Task1(void* parameter){
    
}
void Task2(void* parameter)
{
    while(1){
        while (digitalRead(sw));
        delay(200);
        cnt++;
        Serial.println(cnt);
        while (!digitalRead(sw));
            if (chkPrime(cnt))
            {
                digitalWrite(led1, 1);
                delay(250);
                digitalWrite(led1, 0);
                delay(250);
                digitalWrite(led1, 1);
                delay(250);
                digitalWrite(led1, 0);
                delay(250);
            }
        delay(150);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);

}

void Task3(void *parameter)
{
    while (1)
    {
        dacWrite(led2, map(analogRead(ldr), 1300, 200, 255, 0));
        Serial.println(analogRead(ldr));
        delay(10);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
}