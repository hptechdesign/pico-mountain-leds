#ifndef LED_CONTROLLER_HPP
#define LED_CONTROLLER_HPP

#include <cmath>
#include "pico/stdlib.h"
#include "plasma_stick.hpp"
#include "common/pimoroni_common.hpp"

using namespace pimoroni;
using namespace plasma;

class LEDController
{
private:
    const uint num_leds;
    WS2812 led_strip;

    uint8_t gamma_correct(float value);
    void set_all_leds(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

public:
    LEDController(uint num_leds);
    void init();
    void all_on(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void all_off();
    void transition(int duration_ms, bool is_sunrise);
    void sunrise(int duration_ms);
    void sunset(int duration_ms);
};

#endif // LED_CONTROLLER_HPP
