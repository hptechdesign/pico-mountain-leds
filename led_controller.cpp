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
    const int steps = 10000; // Increased steps for smoother transition
    const float max_brightness = 255.0f;

    for (int step = 0; step <= steps; ++step)
    {
        float progress = static_cast<float>(step) / steps;
        float brightness = pow(progress, 1.5f) * max_brightness; // Using a power function for a more natural ramp-up
        uint8_t red = gamma_correct(brightness);
        uint8_t green = gamma_correct(brightness * 0.5f); // Half-brightness for green to give a warm effect
        uint8_t blue = 0;                                 // No blue in sunrise

        set_all_leds(red, green, blue);
        sleep_us((duration_ms * 1000) / steps); // Microsecond delay for smoother transition
    }
}

void LEDController::sunset(int duration_ms)
{
    const int steps = 10000; // Increased steps for smoother transition
    const float max_brightness = 255.0f;

    for (int step = steps; step >= 0; --step)
    {
        float progress = static_cast<float>(step) / steps;
        float brightness = pow(progress, 1.5f) * max_brightness; // Using a power function for a smoother ramp-down
        uint8_t red = gamma_correct(brightness);
        uint8_t green = gamma_correct(brightness * 0.5f);
        uint8_t blue = 0;

        set_all_leds(red, green, blue);
        sleep_us((duration_ms * 1000) / steps); // Microsecond delay for smoother transition
    }
}
