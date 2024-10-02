#include <iostream>
#include <cstdio>
#include <cstring>
#include "pico/stdlib.h"
#include "plasma_stick.hpp"
#include "common/pimoroni_common.hpp"

using namespace pimoroni;
using namespace plasma;

// Class to handle LED strip operations
class LEDController
{
private:
  WS2812 led_strip;
  uint num_leds;

public:
  // Constructor
  LEDController(uint num_leds, PIO pio = pio0, int sm = 0)
      : led_strip(num_leds, pio, sm, plasma_stick::DAT, WS2812::DEFAULT_SERIAL_FREQ, true, WS2812::COLOR_ORDER::GRB), num_leds(num_leds)
  {
    led_strip.start();
  }

  // Function to set all LEDs to a specific color
  void set_all_leds(uint8_t r, uint8_t g, uint8_t b)
  {
    for (uint i = 0; i < num_leds; ++i)
    {
      led_strip.set_rgb(i, r, g, b);
    }
  }

  // Function to turn all LEDs off
  void all_off()
  {
    set_all_leds(0, 0, 0);
  }

  // Function to simulate a sunrise
  void sunrise(int duration)
  {
    for (int level = 0; level <= 1000; ++level) // Adjusted to 1000 levels
    {
      uint8_t brightness = static_cast<uint8_t>((level / 1000.0f) * 255);
      set_all_leds(brightness, brightness / 2, 0); // Warm yellow color
      sleep_ms(duration);                          // Adjust as needed
    }
  }

  // Function to simulate a sunset
  void sunset(int duration)
  {
    for (int level = 1000; level >= 0; --level) // Adjusted to 1000 levels
    {
      uint8_t brightness = static_cast<uint8_t>((level / 1000.0f) * 255);
      set_all_leds(brightness, brightness / 2, 0); // Warm yellow color
      sleep_ms(duration);                          // Adjust as needed
    }
  }
};

// Command parser and executor
void parse_and_execute_command(const char *input, LEDController &led_controller)
{
  if (strncmp(input, "all_on(", 7) == 0)
  {
    int r, g, b;
    if (sscanf(input + 7, "%d,%d,%d", &r, &g, &b) == 3)
    {
      led_controller.set_all_leds(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
    }
  }
  else if (strcmp(input, "all_off()") == 0)
  {
    led_controller.all_off();
  }
  else if (strncmp(input, "sunrise(", 8) == 0)
  {
    int duration;
    if (sscanf(input + 8, "%d", &duration) == 1)
    {
      led_controller.sunrise(duration);
    }
  }
  else if (strncmp(input, "sunset(", 7) == 0)
  {
    int duration;
    if (sscanf(input + 7, "%d", &duration) == 1)
    {
      led_controller.sunset(duration);
    }
  }
  else
  {
    std::cerr << "Error: Invalid command" << std::endl;
  }
}

int main()
{
  stdio_init_all();

  // Create an instance of the LEDController with the number of LEDs
  LEDController led_controller(60); // Adjust 60 to your actual LED strip length

  char input[128];
  unsigned int pos = 0;

  while (true)
  {
    int c = getchar();
    if (c != EOF)
    {
      if (c == '\n' || c == '\r')
      {
        input[pos] = '\0';
        parse_and_execute_command(input, led_controller);
        pos = 0;
      }
      else if (pos < sizeof(input) - 1)
      {
        input[pos++] = c;
      }
      else
      {
        std::cerr << "Error: Command too long" << std::endl;
        pos = 0;
      }
    }
  }
  return 0;
}
