
#define led1 22
#define led2 23
unsigned long r_ts, y_ts;
void setup() {

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);

}

void loop() {
  /*if(millis() % 500 == 0)
    digitalWrite(led1,!digitalRead(led1));
  if(millis() % 2000 == 0)
    digitalWrite(led2,!digitalRead(led2));
    */
   if(millis() - r_ts  > 500){
      digitalWrite(led1,!digitalRead(led1));
      r_ts = millis(); 
   }

   if(millis() - y_ts  > 2000){
      digitalWrite(led2,!digitalRead(led2));
      y_ts = millis(); 
   }
}
