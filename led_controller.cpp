#include "led_controller.hpp"

LEDController::LEDController(uint num_leds)
    : num_leds(num_leds), led_strip(num_leds, pio0, 0, plasma_stick::DAT, WS2812::DEFAULT_SERIAL_FREQ, true, WS2812::COLOR_ORDER::GRB) {}

void LEDController::init()
{
    stdio_init_all();
    led_strip.start();
}

uint8_t LEDController::gamma_correct(float value)
{
    float gamma = 2.2f;
    return static_cast<uint8_t>(pow(value / 255.0f, gamma) * 255.0f);
}

void LEDController::set_all_leds(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    for (uint i = 0; i < num_leds; ++i)
    {
        led_strip.set_rgb(i, r, g, b, w);
    }
}

void LEDController::all_on(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    set_all_leds(r, g, b, w);
}

void LEDController::all_off()
{
    set_all_leds(0, 0, 0, 0);
}

void LEDController::transition(int duration_ms, bool is_sunrise)
{
    const int steps = 10000;
    uint8_t colors[5][4] = {
        {0, 0, 30, 0},       // Dark blue, no white
        {100, 30, 10, 50},   // Deep orange/blue, adding white
        {200, 80, 20, 100},  // Orange/amber, more white
        {255, 180, 50, 150}, // Warm yellow with some white
        {255, 180, 50, 255}  // Full white at the very end
    };

    for (int step = 0; step <= steps; ++step)
    {
        float progress = static_cast<float>(step) / steps;

        // Depending on whether it's sunrise or sunset, reverse the progress
        float adjusted_progress = is_sunrise ? progress : 1.0f - progress;
        int stage = static_cast<int>(adjusted_progress * 4); // Adjusted for 5 stages
        float stage_progress = (adjusted_progress * 4) - stage;

        // Interpolate each RGBW component
        uint8_t red = gamma_correct(colors[stage][0] * (1 - stage_progress) + colors[stage + 1][0] * stage_progress);
        uint8_t green = gamma_correct(colors[stage][1] * (1 - stage_progress) + colors[stage + 1][1] * stage_progress);
        uint8_t blue = gamma_correct(colors[stage][2] * (1 - stage_progress) + colors[stage + 1][2] * stage_progress);
        uint8_t white = gamma_correct(colors[stage][3] * (1 - stage_progress) + colors[stage + 1][3] * stage_progress);

        set_all_leds(red, green, blue, white);
        sleep_us((duration_ms * 1000) / steps);
    }
}

void LEDController::sunrise(int duration_ms)
{
    transition(duration_ms, true); // Sunrise: transition forwards
}

void LEDController::sunset(int duration_ms)
{
    transition(duration_ms, false); // Sunset: transition backwards
}
