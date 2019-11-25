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

inline void delay_65ns() {
  __asm__ __volatile__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
}

inline void delay_960ns() {
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
  delay_65ns();
}

void ws2812_naive_set(const int pixel_count, const rgb_t* pixels) {
  gpio_set_level(LED_STRIP_GPIO, 0);
  ets_delay_us(52);
  // portDISABLE_INTERRUPTS();
  const auto pixel = pixels[0];
  bool x[24] = {0};
  for (uint8_t j = 0; j < 8; j++) x[7 - j] = ((pixel.g >> j) & 1) == 0;
  for (uint8_t j = 0; j < 8; j++) x[15 - j] = ((pixel.r >> j) & 1) == 0;
  for (uint8_t j = 0; j < 8; j++) x[23 - j] = ((pixel.b >> j) & 1) == 0;
  for (uint8_t j = 0; j < 24; j++) {
    if (!x[j]) {
      GPIO.out_w1ts = 1 << LED_STRIP_GPIO;
      delay_960ns();
      GPIO.out_w1tc = 1 << LED_STRIP_GPIO;
      delay_65ns();
    }
    if (x[j]) {
      GPIO.out_w1ts = 1 << LED_STRIP_GPIO;
      delay_65ns();
      GPIO.out_w1tc = 1 << LED_STRIP_GPIO;
      delay_960ns();
    }
  }
  // portENABLE_INTERRUPTS();
}

void led_strip_task(void*) {
  esp_task_wdt_init(portMAX_DELAY, false);
  bool naive = true;
  if (naive) {
    gpio_pad_select_gpio(LED_STRIP_GPIO);
    gpio_set_direction(LED_STRIP_GPIO, GPIO_MODE_OUTPUT);
  } else {
    ws2812_init(LED_STRIP_GPIO);
  }
  const auto pixel_count = 1;
  const auto green = makeRGBVal(0, 100, 0);
  const auto red = makeRGBVal(100, 0, 0);
  const auto blue = makeRGBVal(0, 0, 100);
  auto pixels = new rgb_t[pixel_count];
  auto color = makeRGBVal(200, 0, 0);
  uint8_t step = 0;

  // ets_delay_us();

  while (1) {
    // switch (step) {
    //   case 0:
    //     color = red;
    //     step = 1;
    //     break;
    //   case 1:
    //     color = green;
    //     step = 2;
    //     break;
    //   case 2:
    //     color = blue;
    //     step = 0;
    //     break;
    // }
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
        color.g--;
        if (color.g == 0) step = 4;
        break;
      case 4:
        color.r--;
        if (color.r == 0) step = 5;
        break;
      case 5:
        color.b--;
        if (color.b == 0) {
          vTaskDelay(pdMS_TO_TICKS(10000));
          step = 0;
        }
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
