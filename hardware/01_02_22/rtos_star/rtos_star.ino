void setup() {
  Serial.begin(9600);
  vTaskDelay(500/portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(
    TaskA,
    "Task A",
    1024,
    NULL,
    2,
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

void loop() {
  // put your main code here, to run repeatedly:

}
