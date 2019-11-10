#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <math.h>
#include "sdkconfig.h"
#include "ws2812.h"

#define MIN_DELAY 50
#define BUTTON_GPIO GPIO_NUM_17
#define LED_GPIO GPIO_NUM_18
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

void rainbow(void* pvParameters) {
  ws2812_init(GPIO_NUM_18);

  const auto pixel_count = 28;
  const auto delay = 1000;
  const auto green = makeRGBVal(0, 100, 0);
  const auto none = makeRGBVal(0, 0, 0);
  auto pixels = new rgbVal[pixel_count];

  uint8_t day = 0;
  while (1) {
    day++;
    if (day > 6) day = 0;

    for (uint8_t i = 0; i < pixel_count; i++) {
      if (i / 4 == day)
        pixels[i] = green;
      else
        pixels[i] = none;
    }

    ws2812_setColors(pixel_count, pixels);
    vTaskDelay(pdMS_TO_TICKS(delay));
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
  gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

  semaphore_handle = xSemaphoreCreateBinary();
  // gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(led_task, "led_task", 2048, NULL, 10, NULL);
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, nullptr);

  xTaskCreate(rainbow, "ws2812 rainbow demo", 4096, NULL, 10, NULL);
}
