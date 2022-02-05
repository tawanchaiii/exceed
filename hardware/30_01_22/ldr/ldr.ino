int ldr = 34;
int led = 5;

void setup() {
  pinMode(ldr,INPUT);
  Serial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(ldr));
  delay(100);
}
