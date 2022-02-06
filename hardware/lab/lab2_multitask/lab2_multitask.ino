#define led 5
#define led1 21
#define led2 25
#define ldr 34
#define sw 23
// task 1
int res = 8;
int ch = 0;
int freq = 1000;
int cnt = 0;
int chkPrime(int x)
{
    if (x == 0 || x == 1)
        return 0;
    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0)
            return 0;
    }
    return 1;
}

void setup()
{
    Serial.begin(115200);
    pinMode(ldr, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    ledcSetup(ch, freq, res);
    ledcAttachPin(led, ch);
    ledcWrite(ch, 0);
    xTaskCreatePinnedToCore(Task1, "Task 1", 1024, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task2, "Task 2", 1024, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Task3, "Task 3", 1024, NULL, 1, NULL, 0);
}
void loop()
{
}

void Task1(void *parameter)
{
    while (1)
    {
        for (int i = 0; i < 256; i++)
        {
            ledcWrite(ch, i);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        for (int i = 255; i > 0; i--)
        {
            ledcWrite(ch, i);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
}
void Task2(void *parameter)
{
    while (1)
    {
        while (digitalRead(sw));
        vTaskDelay(200 / portTICK_PERIOD_MS);
        cnt++;
        Serial.println(cnt);
        while (!digitalRead(sw));
        if (chkPrime(cnt))
        {
            digitalWrite(led1, 1);
            //delay(250);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(led1, 0);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(led1, 1);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(led1, 0);
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
         vTaskDelay(150 / portTICK_PERIOD_MS);
    }
   
}

void Task3(void *parameter)
{
    while (1)
    {
        int reading=analogRead(ldr);
        int bright=reading/4;  
        //map(analogRead(ldr), 2000, 300, 255, 0)
        dacWrite(led2,bright);
        vTaskDelay(10 / portTICK_PERIOD_MS);
}
    }
    