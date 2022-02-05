#define sw 23
#define led1 21
int cnt = 0;
int chkPrime(int x){
    if(x==0 || x == 1) return 0;
    for(int i=2;i*i<=x;i++){
        if(x%i==0)  
            return 0;
    }
    return 1;
}
void setup()
{
    Serial.begin(115200);
    pinMode(sw, INPUT_PULLUP);
    pinMode(led1,OUTPUT);
    Serial.println(cnt);
}
void loop(){
    while(digitalRead(sw)) 
        delay(200);
        cnt++;
        Serial.println(cnt);
    while(!digitalRead(sw))
    if(chkPrime(cnt)){
        digitalWrite(led1,1);
        delay(250);
        digitalWrite(led1,0);
        delay(250);
        digitalWrite(led1,1);
        delay(250);
        digitalWrite(led1,0);
        delay(250);
    }
    delay(150);
}