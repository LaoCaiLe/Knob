#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "TFT_eSPI.h" 
#include "lvgl.h"
#include "motor.h"
#include "lcd.h"
#include "led.h"

Motion motion;
Screen screen;
ColorLed Cled(LED_PIN, LED_COUNT);

void TaskOnMotor(void* params)
{
    motion.task_motor();
}

static void lv_timer_cb(lv_timer_t *t)
{
    screen.task_tft();
}

void TaskOnLed(void* params)
{
    Cled.task();
}

void setup() {

    Serial.begin(115200);

    screen.init();
    motion.init();
    Cled.init();

    xTaskCreatePinnedToCore(TaskOnMotor, "TaskOnMotor", 4096, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(TaskOnLed, "TaskOnLed", 50000, NULL, 1, NULL, 1);

    lv_timer_create(lv_timer_cb, 1, NULL);
}

void loop() 
{
    lv_task_handler();
}