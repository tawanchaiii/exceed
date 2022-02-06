volatile bool state = false;
#define sw 23
#define led 5
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;
bool press = false;
bool is_on = false;
int i =0;

void debounce(){
    static int count = 20;
    if(digitalRead(sw)){
        count--;
        if(count<0) 
            count = 0;
        if(count==0)
            state = true;
    }
    else{
        count++;
        if(count>20)
            count=20;
        if(count==20)
            state = false;

    }
}
void ledOff(){
    digitalWrite(led,1);
    is_on = false;
}
void setup()
{
    pinMode(sw,INPUT);
    pinMode(led,OUTPUT);
    digitalWrite(led,HIGH);
    Serial.begin(115200);

    delay(2000);

    timer1 = timerBegin(0,80,true);
    timerAttachInterrupt(timer1,debounce,true);
    timerAlarmWrite(timer1,100,true);
    timerAlarmEnable(timer1);

    timer2 = timerBegin(1,80,true);
    timerAttachInterrupt(timer2,ledOff,true);
    timerAlarmWrite(timer2,5000000,true);
}
void loop()
{
    if(state){
        if(!press){ // press first time 
            digitalWrite(led,0);
            timerAlarmEnable(timer2);
            is_on = true;
            press = true;
        }
        else{
            if(is_on){ // press second before 5 minutes
                digitalWrite(led, 1);
                is_on = false;
            }
            else{ // press second after 5 minutes
                digitalWrite(led,0);
                is_on = true;
                timerWrite(timer2,0);  
            }
        
        }
        while(state);
    }
}