#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <math.h>
#include "pins.h"
#include "pod.h"
#include "sdkconfig.h"

#define MIN_DELAY 50

static xSemaphoreHandle semaphore_handle = nullptr;

void led_task(void* arg) {
  bool x = false;
  while (true) {
    xSemaphoreTake(semaphore_handle, portMAX_DELAY);
    x = !x;
    gpio_set_level(BLINK_GPIO, x);
  }
}

void IRAM_ATTR button_isr_handler(void* arg) {
  static auto last_time = 0;
  auto now = xTaskGetTickCountFromISR();
  if (now - last_time > MIN_DELAY) {
    auto ignored = pdFALSE;
    xSemaphoreGiveFromISR(semaphore_handle, &ignored);
    last_time = now;
  }
}

extern "C" void app_main(void) {
  // gpio_pad_select_gpio(BLINK_GPIO);
  // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  // gpio_pad_select_gpio(CLK_GPIO);
  // gpio_set_direction(CLK_GPIO, GPIO_MODE_OUTPUT);

  // gpio_pad_select_gpio(BUTTON_GPIO);
  // gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
  // gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
  // gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

  // semaphore_handle = xSemaphoreCreateBinary();
  // // gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  // xTaskCreate(led_task, "led_task", 2048, nullptr, 10, nullptr);
  // gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  // gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, nullptr);

  start_pod_tasks();
}
