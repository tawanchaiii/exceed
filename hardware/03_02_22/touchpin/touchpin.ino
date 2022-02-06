#define touch 2
void setup()
{
    pinMode(touch,INPUT);
    Serial.begin(115200);
}
void loop()
{
    Serial.println(touchRead(touch));
    delay(100);
}
