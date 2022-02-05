#define led1 22
#define led2 23
TaskHandle_t blinkTaskHandle = NULL;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  digitalWrite(led1, LOW);
}

void loop() {
  while(!Serial.available()){
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
  int interval = Serial.parseInt();
  if(interval){
    Serial.println(interval);
    startBlinkTask(interval);
  }
}

void startBlinkTask(int interval){
  if (blinkTaskHandle)
    vTaskDelete(blinkTaskHandle);

  xTaskCreatePinnedToCore(
    blinkLEDTask,
    "Blink LED",
    1023,
    (void*) interval,
    1,
    &blinkTaskHandle,
    1
  );
}

void blinkLEDTask(void* parameter){
  int interval = (int) parameter;
  digitalWrite(led1,LOW);
  while(1){
    digitalWrite(led1, !digitalRead(led1));
    vTaskDelay(interval/portTICK_PERIOD_MS);
  }
}
