/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"


#include "pico/stdlib.h"
#include "plasma_stick.hpp"
#include "common/pimoroni_common.hpp"

/*
This super simple example sets up two alternating colours, great for festive lights!
*/

using namespace pimoroni;
using namespace plasma;

// Set how many LEDs you have
const uint NUM_LEDS = 60;

// Set up speed (wait time between colour changes, in seconds)
constexpr float SPEED = 0.01f;

// Set up the WS2812 / NeoPixel™ LEDs, with RGB color order to work with the LED wire that comes with Skully
WS2812 led_strip(NUM_LEDS, pio0, 0, plasma_stick::DAT, WS2812::DEFAULT_SERIAL_FREQ, true, WS2812::COLOR_ORDER::GRB);

int main() {
    stdio_init_all();

 // Start updating the LED strip
  led_strip.start();

  uint8_t rVal = 160;
  uint8_t bVal = 0;
  uint8_t gVal = 128;
  uint8_t BRIGHTNESS = 64;

  while(true) {

    for(auto i = 0u; i < NUM_LEDS; i++) {
            led_strip.set_rgb(i, rVal, gVal, bVal, BRIGHTNESS, true);
    }
    printf("Hello, world!\n");
    printf("\nR = %d, G = %d, B = %d", rVal++, gVal++, bVal++);
    printf("\n Brightness = %d", BRIGHTNESS);
    sleep_ms(SPEED * 1000.0f);
  }
}
