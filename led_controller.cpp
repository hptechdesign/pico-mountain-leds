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

void LEDController::set_all_leds(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint i = 0; i < num_leds; ++i)
    {
        led_strip.set_rgb(i, r, g, b);
    }
}

void LEDController::all_on(uint8_t r, uint8_t g, uint8_t b)
{
    set_all_leds(r, g, b);
}

void LEDController::all_off()
{
    set_all_leds(0, 0, 0);
}

void LEDController::sunrise(int duration_ms)
{
    const int steps = 10000; // More steps for smoother transition
    uint8_t colors[4][3] = {
        {0, 0, 30},    // Dark blue
        {100, 30, 10}, // Deep orange/blue
        {200, 80, 20}, // Orange/amber
        {255, 180, 50} // Warm yellow
    };

    for (int step = 0; step <= steps; ++step)
    {
        float progress = static_cast<float>(step) / steps;
        int stage = static_cast<int>(progress * 3);
        float stage_progress = (progress * 3) - stage;

        // Interpolate each RGB component
        uint8_t red = gamma_correct(colors[stage][0] * (1 - stage_progress) + colors[stage + 1][0] * stage_progress);
        uint8_t green = gamma_correct(colors[stage][1] * (1 - stage_progress) + colors[stage + 1][1] * stage_progress);
        uint8_t blue = gamma_correct(colors[stage][2] * (1 - stage_progress) + colors[stage + 1][2] * stage_progress);

        set_all_leds(red, green, blue);
        sleep_us((duration_ms * 1000) / steps);
    }
}

void LEDController::sunset(int duration_ms)
{
    const int steps = 10000; // More steps for smoother transition
    uint8_t colors[4][3] = {
        {255, 180, 50}, // Warm yellow
        {200, 80, 20},  // Orange/amber
        {100, 30, 10},  // Deep orange/blue
        {0, 0, 30}      // Dark blue
    };

    for (int step = steps; step >= 0; --step)
    {
        float progress = static_cast<float>(step) / steps;
        int stage = static_cast<int>(progress * 3);
        float stage_progress = (progress * 3) - stage;

        uint8_t red = gamma_correct(colors[stage][0] * (1 - stage_progress) + colors[stage + 1][0] * stage_progress);
        uint8_t green = gamma_correct(colors[stage][1] * (1 - stage_progress) + colors[stage + 1][1] * stage_progress);
        uint8_t blue = gamma_correct(colors[stage][2] * (1 - stage_progress) + colors[stage + 1][2] * stage_progress);

        set_all_leds(red, green, blue);
        sleep_us((duration_ms * 1000) / steps);
    }
}
