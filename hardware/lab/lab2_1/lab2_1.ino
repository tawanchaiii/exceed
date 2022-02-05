int res = 8;
int ch = 0;
int freq = 1000;
int led = 5;
void setup() {
  pinMode(led,OUTPUT);
  ledcSetup(ch,freq,res);
  ledcAttachPin(led,ch);
  ledcWrite(ch,0);

}

void loop() {
  for(int i=0;i<256;i++){
    ledcWrite(ch,i);
    delay(10);
  }
  for(int i=255 ;i>0;i--){
    ledcWrite(ch,i);
    delay(10);
  }
  
}
