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

void delay_nops(int nops) {
  for (int i = 0; i < nops; i++) {
    __asm__ __volatile__("nop");
  }
}

void ws2812_naive_set(const int pixel_count, const rgbVal* pixels) {
  gpio_set_level(LED_STRIP_GPIO, 0);

  ets_delay_us(50);

  for (int i = 0; i < pixel_count; i++) {
    const auto pixel = pixels[i];
    std::bitset<24> p(pixel.num);

    for (int j = 0; j < 24; j++) {
      if (p[j]) {
        gpio_set_level(LED_STRIP_GPIO, 1);
        delay_nops(14);
        gpio_set_level(LED_STRIP_GPIO, 0);
      } else {
        gpio_set_level(LED_STRIP_GPIO, 1);
        gpio_set_level(LED_STRIP_GPIO, 0);
        delay_nops(13);
      }
    }
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

  // gpio_pad_select_gpio(LED_STRIP_GPIO);
  // gpio_set_direction(LED_STRIP_GPIO, GPIO_MODE_OUTPUT);

  ws2812_init(LED_STRIP_GPIO);
  const auto pixel_count = 1;
  const auto green = makeRGBVal(0, 255, 0);
  const auto red = makeRGBVal(255, 0, 0);
  const auto blue = makeRGBVal(0, 0, 255);
  const auto white = makeRGBVal(10, 10, 10);
  const auto none = makeRGBVal(0, 0, 0);
  auto pixels = new rgbVal[pixel_count];
  // for (uint8_t i = 0; i < pixel_count; i++) pixels[i] = none;
  // long count = 0;
  // while (true) {
  //   count += 16;
  //   pixels[0].num = count % UINT32_MAX;
  //   ws2812_set(pixel_count, pixels);
  //   vTaskDelay(1);
  // }

  const uint8_t anim_step = 10;
  const uint8_t anim_max = 250;
  rgbVal color = makeRGBVal(anim_max, 0, 0);
  uint8_t step = 0;

  // ets_delay_us();

  while (1) {
    step = step % 5;
    pixels[0] = color;

    switch (step) {
      case 0:
        color.g += anim_step;
        if (color.g >= anim_max) step++;
        break;
      case 1:
        color.r -= anim_step;
        if (color.r == 0) step++;
        break;
      case 2:
        color.b += anim_step;
        if (color.b >= anim_max) step++;
        break;
      case 3:
        color.g -= anim_step;
        if (color.g == 0) step++;
        break;
      case 4:
        color.r += anim_step;
        if (color.r >= anim_max) step++;
        break;
      case 5:
        color.b -= anim_step;
        if (color.b == 0) step = 0;
        break;
    }

    ws2812_set(pixel_count, pixels);

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
