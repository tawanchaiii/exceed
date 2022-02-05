#define led1 22
#define led2 23


void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  xTaskCreatePinnedToCore(
    TaskA,
    "Task A",
    1024,
    NULL,
    1,
    NULL,
    1

  );
  xTaskCreatePinnedToCore(
    TaskB,
    "Task B",
    1024,
    NULL,
    1,
    NULL,
    1

  );
}
void TaskA(void *parameter){
  while(1){
    digitalWrite(led1, !digitalRead(led1));
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void TaskB(void *parameter){
  while(1){
    digitalWrite(led2, !digitalRead(led2));
    vTaskDelay(3000/portTICK_PERIOD_MS);
  }
}
void loop() {
  // put your main code here, to run repeatedly:

}
