int sw = 22;
int led = 5;
int ledg = 23;
int ldr = 34;
void setup() {
  pinMode(led,OUTPUT);
  pinMode(sw,INPUT_PULLUP);
  pinMode(ledg,OUTPUT);
  pinMode(ldr,INPUT);
  
  digitalWrite(led,HIGH);
  Serial.begin(9600);
}

void loop() {
  /* led switch
  digitalWrite(led,digitalRead(sw)); 
  */
  if(analogRead(ldr)>1500)
    digitalWrite(ledg,HIGH);
  else 
    digitalWrite(ledg,LOW);
    
  digitalWrite(led,digitalRead(sw)); 
 
  
  Serial.println(analogRead(ldr));
  delay(10);
  //Serial.println(digitalRead(sw));
}
