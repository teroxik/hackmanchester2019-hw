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
#include "ws2812.h"

#include <bitset>
#include <vector>

inline void delay_nops(int nops) {
  for (int i = 0; i < nops; i++) {
    __asm__ __volatile__("nop");
  }
}

void ws2812_naive_set(const int pixel_count, const rgb_t* pixels) {
  gpio_set_level(LED_STRIP_GPIO, 0);
  ets_delay_us(50);

  portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
  const auto pixel = pixels[0].num;
  bool x[24] = {0};
  for (uint8_t j = 0; j < 24; j++) x[j] = ((pixel >> j) & 1) == 0;
  for (uint8_t j = 0; j < 24; j++) {
    if (!x[j]) {
      gpio_set_level(LED_STRIP_GPIO, 1);
      delay_nops(20);
      gpio_set_level(LED_STRIP_GPIO, 0);
    }
    if (x[j]) {
      gpio_set_level(LED_STRIP_GPIO, 1);
      gpio_set_level(LED_STRIP_GPIO, 0);
      delay_nops(17);
    }
  }
}

void led_strip_task(void*) {
  bool naive = false;
  if (naive) {
    gpio_pad_select_gpio(LED_STRIP_GPIO);
    gpio_set_direction(LED_STRIP_GPIO, GPIO_MODE_OUTPUT);
  } else {
    ws2812_init(LED_STRIP_GPIO);
  }
  const auto pixel_count = 1;
  const auto green = makeRGBVal(0, 255, 0);
  const auto red = makeRGBVal(255, 0, 0);
  const auto blue = makeRGBVal(0, 0, 255);
  const auto white = makeRGBVal(10, 10, 10);
  const auto none = makeRGBVal(0, 0, 0);
  auto pixels = new rgb_t[pixel_count];
  // for (uint8_t i = 0; i < pixel_count; i++) pixels[i] = none;
  // long count = 0;
  // while (true) {
  //   count += 16;
  //   pixels[0].num = count % UINT32_MAX;
  //   ws2812_set(pixel_count, pixels);
  //   vTaskDelay(1);
  // }
  auto color = makeRGBVal(200, 0, 0);
  uint8_t step = 0;

  // ets_delay_us();

  while (1) {
    switch (step) {
      case 0:
        color.r++;
        if (color.r == 255) step = 1;
        break;
      case 1:
        color.g++;
        if (color.g == 255) step = 2;
        break;
      case 2:
        color.b++;
        if (color.b == 255) step = 3;
        break;
      case 3:
        color.r--;
        if (color.r == 0) step = 4;
        break;
      case 4:
        color.g--;
        color.b--;
        if (color.g == 0) step = 0;
        break;
    }
    pixels[0] = color;

    if (naive)
      ws2812_naive_set(pixel_count, pixels);
    else
      ws2812_set(pixel_count, pixels);

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

extern "C" void app_main(void) {
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

  // start_pod_tasks();
  xTaskCreatePinnedToCore(led_strip_task, "x", 1024, nullptr,
                          configMAX_PRIORITIES - 1, nullptr, 0);
}
