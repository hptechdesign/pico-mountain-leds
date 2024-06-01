/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "pico/stdlib.h"
#include "plasma_stick.hpp"
#include "common/pimoroni_common.hpp"

/*
This example sets up two alternating colours, great for festive lights, and updates them via serial input.
*/

using namespace pimoroni;
using namespace plasma;

// Forward declarations of the LED control functions
void sunrise(int duration, int start_percent);
void sunset(int duration, int start_percent);
void on(float percent);
void off();

// Set how many LEDs you have
const uint NUM_LEDS = 60; // 5m strip = 300; 1m strip = 60

// Set up speed (wait time between colour changes, in seconds)
constexpr float SPEED = 0.01f;

// Set up the WS2812 / NeoPixel™ LEDs, with RGB color order to work with the LED wire that comes with Skully
WS2812 led_strip(NUM_LEDS, pio0, 0, plasma_stick::DAT, WS2812::DEFAULT_SERIAL_FREQ, true, WS2812::COLOR_ORDER::GRB);

bool parse_int(const char *str, int &value)
{
  char *endptr;
  value = strtol(str, &endptr, 10);

  // Return true if strtol successfully parsed the integer
  return true;
}

bool parse_float(const char *str, float &value)
{
  char *endptr;
  value = strtof(str, &endptr);

  // Return true if strtof successfully parsed the float
  return true;
}

void set_leds(uint32_t color)
{
  uint8_t r = (color >> 24) & 0xFF;
  uint8_t g = (color >> 16) & 0xFF;
  uint8_t b = (color >> 8) & 0xFF;
  uint8_t w = color & 0xFF;

  for (auto i = 0u; i < NUM_LEDS; i++)
  {
    led_strip.set_rgb(i, r, g, b, w, true);
  }
}

uint32_t parse_hex_color(const char *hex_str)
{
  // Check if the string starts with "0x" or "0X"
  if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X'))
  {
    hex_str += 2; // Skip the "0x" prefix
  }
  return strtoul(hex_str, NULL, 16);
}

// Command parser and executor
void parse_and_execute_command(const char *input)
{
  if (strncmp(input, "sunrise(", 8) == 0)
  {
    const char *params = input + 8;
    char *comma = strchr(params, ',');
    if (comma)
    {
      *comma = '\0';
      int duration, start_percent;
      if (parse_int(params, duration) && parse_int(comma + 1, start_percent) && start_percent >= 0 && start_percent <= 100)
      {
        sunrise(duration, start_percent);
        return;
      }
    }
  }
  else if (strncmp(input, "sunset(", 7) == 0)
  {
    const char *params = input + 7;
    char *comma = strchr(params, ',');
    if (comma)
    {
      *comma = '\0';
      int duration, start_percent;
      if (parse_int(params, duration) && parse_int(comma + 1, start_percent) && start_percent >= 0 && start_percent <= 100)
      {
        sunset(duration, start_percent);
        return;
      }
    }
  }
  else if (strncmp(input, "on(", 3) == 0)
  {
    // Parse on command
    const char *params = input + 3;
    float percent;
    if (parse_float(params, percent) && percent >= 0.0f && percent <= 100.0f)
    {
      on(percent);
      return;
    }
  }
  else if (strcmp(input, "off()") == 0)
  {
    off();
    return;
  }

  // If no valid command was recognized
  std::cerr << "Error: Invalid command" << std::endl;
}

// Function to calculate the color value based on the percentage
uint32_t calculate_color(float percent)
{
  if (percent < 0.0f)
    percent = 0.0f;
  if (percent > 100.0f)
    percent = 100.0f;

  // Calculate the scaled values for each channel
  int r = (int)((percent / 100.0f) * 255);
  int g = (int)((percent / 100.0f) * 255);
  int b = (int)((percent / 100.0f) * 255);
  int w = (int)((percent / 100.0f) * 255);

  // Construct the 32-bit color value
  uint32_t color = (r << 24) | (g << 16) | (b << 8) | w;
  return color;
}

// Dummy implementations of the LED control functions
void sunrise(int duration, int start_percent)
{
  std::cout << "Sunrise: duration=" << duration << ", start_percent=" << start_percent << std::endl;
}

void sunset(int duration, int start_percent)
{
  std::cout << "Sunset: duration=" << duration << ", start_percent=" << start_percent << std::endl;
}

void on(float percent)
{
  std::cout << "Turn on: percent=" << percent << std::endl;

  uint32_t color = calculate_color(percent);
  set_leds(color);
}

void off()
{
  std::cout << "Turn off" << std::endl;
}

int main()
{
  stdio_init_all();

  // Start updating the LED strip
  led_strip.start();

  char input[128];      // Buffer for the incoming command
  unsigned int pos = 0; // Position in the buffer

  while (true)
  {
    int c = getchar(); // Read a single character from the serial port
    if (c != EOF)
    {
      if (c == '\n' || c == '\r')
      {
        // Null-terminate the string and process the command
        input[pos] = '\0';
        parse_and_execute_command(input);
        pos = 0; // Reset position for the next command
      }
      else if (pos < sizeof(input) - 1)
      {
        // Add character to buffer if there is space
        input[pos++] = c;
      }
      else
      {
        // Buffer overflow, reset position
        std::cerr << "Error: Command too long" << std::endl;
        pos = 0;
      }
    }
  }

  return 0;
}
