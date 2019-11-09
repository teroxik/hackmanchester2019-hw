#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <math.h>
#include "sdkconfig.h"

#define INTR

#define MIN_DELAY pdMS_TO_TICKS(5)
#define BUTTON_GPIO GPIO_NUM_17
#define BLINK_GPIO GPIO_NUM_5
#define CLK_GPIO GPIO_NUM_15

// static xQueueHandle gpio_evt_queue = nullptr;
static xSemaphoreHandle semaphore_handle = nullptr;

void other_task_compute() { vTaskDelay(pdMS_TO_TICKS(100)); }

void led_task(void* arg) {
  bool x = false;
  while (true) {
    xSemaphoreTake(semaphore_handle, portMAX_DELAY);
    x = !x;
    gpio_set_level(BLINK_GPIO, x);

    // uint32_t dummy = 0;
    // if (xQueueReceive(gpio_evt_queue, &dummy, portMAX_DELAY)) {
    //   x = !x;
    //   gpio_set_level(BLINK_GPIO, x);
    // }
  }
}

void IRAM_ATTR button_isr_handler(void* arg) {
  // uint32_t gpio_num = 0;
  // static auto last_time = 0;
  // auto now = xTaskGetTickCountFromISR();
  // if (now - last_time > MIN_DELAY) {
  //   xQueueSendToBackFromISR(gpio_evt_queue, &gpio_num, nullptr);
  //   last_time = now;
  // }

  static auto last_time = 0;
  auto now = xTaskGetTickCountFromISR();
  if (now - last_time > MIN_DELAY) {
    auto ignored = pdFALSE;
    xSemaphoreGiveFromISR(semaphore_handle, &ignored);
    last_time = now;
  }
}

extern "C" void app_main(void) {
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(CLK_GPIO);
  gpio_set_direction(CLK_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(BUTTON_GPIO);
  gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
#ifdef INTR
  gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

  semaphore_handle = xSemaphoreCreateBinary();
  // gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(led_task, "led_task", 2048, NULL, 10, NULL);
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, nullptr);
#else
  bool x = false;
  // bool light = false;
  // bool x = false;
  // esp_task_wdt_init(portMAX_DELAY, false);
  // while (1) {
  //   gpio_set_level(CLK_GPIO, x);
  //   x = !x;
  //   if (gpio_get_level(BUTTON_GPIO) == 0) {
  //     light = !light;
  //     gpio_set_level(BLINK_GPIO, light);
  //   }
  //   vTaskDelay(pdMS_TO_TICKS(50));
  // }

  // auto last_time = 0;
  // bool light = false;
  // bool was_high = true;
  // while (1) {
  //   gpio_set_level(CLK_GPIO, x);
  //   x = !x;
  //   auto now = xTaskGetTickCount();
  //   if (gpio_get_level(BUTTON_GPIO) == 0) {
  //     if (was_high && now - last_time > MIN_DELAY) {
  //       last_time = now;
  //       light = !light;
  //       gpio_set_level(BLINK_GPIO, light);
  //     }
  //     was_high = false;
  //   } else {
  //     was_high = true;
  //   }

  //   vTaskDelay(pdMS_TO_TICKS(50));
  // }
#endif
}
