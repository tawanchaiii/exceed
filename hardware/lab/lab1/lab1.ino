#define led1 5
#define led2 21
#define led3 22
#define sw 23
int value = 0;
int mode = 0;
int push = 0;
void setup()
{
    Serial.begin(115200);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(sw, INPUT_PULLUP);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    Serial.println(value);
}
void loop()
{
    if (digitalRead(sw) && push == 1)
    {

        if (value == 7 && !mode)
        {
            mode = 1;
            value--;
        }
        else if (value == 0 && mode)
        {
            mode = 0;
            value++;
        }
        else if (!mode)
            value++;
        else if (mode)
            value--;

        Serial.println(value);
    }
    if (!digitalRead(sw))
        push = 1;
    else
        push = 0;

    if ((value & 1) == 1)
        digitalWrite(led1, 0);
    else
        digitalWrite(led1, 1);
    if ((value & 2) == 2)
        digitalWrite(led2, HIGH);
    else
        digitalWrite(led2, LOW);
    if ((value & 4) == 4)
        digitalWrite(led3, HIGH);
    else
        digitalWrite(led3, LOW);
    delay(150);
}
