#define led1 5
#define led2 22

hw_timer_t* timer = NULL;
void blink_fn(){
    digitalWrite(led1,!digitalRead(led1));
}
void setup(){
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);

    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer,&blink_fn,true);
    timerAlarmWrite(timer,700000,true);
    timerAlarmEnable(timer);
}
void loop(){
    digitalWrite(led2,!digitalRead(led2));
    delay(200);
}