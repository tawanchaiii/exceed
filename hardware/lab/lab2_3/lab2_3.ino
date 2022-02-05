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
}
void loop()
{
    int reading=analogRead(ldr);
    int bright=reading/4;  
    //map(analogRead(ldr), 2000, 300, 255, 0)
    dacWrite(led2,bright);
    vTaskDelay(10 / portTICK_PERIOD_MS);
}
