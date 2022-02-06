#define sw_pulldown 23
void setup()
{
    //pinMode(sw_pulldown,INPUT);
    pinMode(sw_pulldown,INPUT_PULLDOWN);
    pinMode(34,INPUT);
    Serial.begin(115200);
}
void loop()
{
    Serial.println(analogRead(34));
    delay(200);
}