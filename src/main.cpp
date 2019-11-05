#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include "sdkconfig.h"

#define BUTTON_GPIO GPIO_NUM_17
#define BLINK_GPIO GPIO_NUM_5

static xQueueHandle gpio_evt_queue = nullptr;

extern "C" void led_task(void* arg) {
  uint32_t x = 0;
  while (true) {
    if (xQueueReceive(gpio_evt_queue, &x, portMAX_DELAY)) {
      printf("Recvd\n");
      if (x == 0)
        x = 1;
      else
        x = 0;
      gpio_set_level(BLINK_GPIO, x);
    }
  }
}

extern "C" void IRAM_ATTR button_isr_handler(void* arg) {
  uint32_t gpio_num = 0;
  static auto last_time = 0;
  auto now = gettimeofday();
  if (now - last_time > 1000L) {
    ets_printf("Intr!\n");
    xQueueSendToBackFromISR(gpio_evt_queue, &gpio_num, nullptr);
    last_time = now;
  }
}

extern "C" void app_main(void) {
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(BUTTON_GPIO);
  gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
  gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

  gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(led_task, "led_task", 2048, NULL, 10, NULL);
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, nullptr);

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // printf("Tick!\n");
  }
}
