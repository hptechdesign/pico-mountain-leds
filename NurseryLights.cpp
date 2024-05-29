/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "plasma_stick.hpp"
#include "common/pimoroni_common.hpp"

/*
This example sets up two alternating colours, great for festive lights, and updates them via serial input.
*/

using namespace pimoroni;
using namespace plasma;

// Set how many LEDs you have
const uint NUM_LEDS = 60;

// Set up speed (wait time between colour changes, in seconds)
constexpr float SPEED = 0.01f;

// Set up the WS2812 / NeoPixel™ LEDs, with RGB color order to work with the LED wire that comes with Skully
WS2812 led_strip(NUM_LEDS, pio0, 0, plasma_stick::DAT, WS2812::DEFAULT_SERIAL_FREQ, true, WS2812::COLOR_ORDER::GRB);

void set_leds(uint32_t color) {
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;
    uint8_t w = color & 0xFF;

    for (auto i = 0u; i < NUM_LEDS; i++) {
        led_strip.set_rgb(i, r, g, b, w, true);
    }
}

uint32_t parse_hex_color(const char* hex_str) {
    // Check if the string starts with "0x" or "0X"
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        hex_str += 2; // Skip the "0x" prefix
    }
    return strtoul(hex_str, NULL, 16);
}

int main() {
    stdio_init_all();

    // Start updating the LED strip
    led_strip.start();

    char input[11];  // Buffer for the incoming hex color (8 chars + null terminator)

    while (true) {
        // Read data from serial port
        if (fgets(input, sizeof(input), stdin) != NULL) {
          // Check for successful read and newline character
          if (feof(stdin)) {
            // Handle end-of-file condition (optional)
            break;
          }

          // Remove newline character if present
          char* newline = strchr(input, '\n');
          if (newline) {
            *newline = '\0';
          }

          // Parse the hex color value
          uint32_t color = parse_hex_color(input);
          set_leds(color);

          printf("Set color to #%08X\n", color);
        }
}
}
