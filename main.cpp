
#include "led_controller.hpp"
#include "command_parser.hpp"
#include "pico/stdlib.h"

#define SWITCH_PIN 2   // GP2
#define BOOTSEL_PIN 23 // BOOTSEL button is on GPIO 23

bool get_switches(void)
{
    bool any_switch_pressed = false;
    if ((gpio_get(BOOTSEL_PIN) == 0) || (gpio_get(SWITCH_PIN) == 0))
    {
        any_switch_pressed = true;
    }

    return any_switch_pressed;
}

int main()
{
    LEDController controller(60); // Adjust the number of LEDs as needed
    controller.init();

    CommandParser parser(controller);

    // Initialize GPIO for the switch input on GP2
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN); // Set as input
    gpio_pull_up(SWITCH_PIN);          // Enable weak pull-up

    // Initialize GPIO for the BOOTSEL button (GPIO 23)
    gpio_init(BOOTSEL_PIN);
    gpio_set_dir(BOOTSEL_PIN, GPIO_IN); // Set as input
    gpio_pull_up(BOOTSEL_PIN);          // Enable pull-up resistor

    char input[128];
    unsigned int pos = 0;

    bool lights_on = false;

    while (true)
    {
        // Check for switch input (grounded when triggered)
        if (get_switches()) // Grounded state detected
        {
            sleep_ms(500); // Simple debounce delay
            if (get_switches())
            {
                printf("BOOTSEL_PIN state: %d\n", gpio_get(BOOTSEL_PIN));
                printf("SWITCH_PIN state: %d\n", gpio_get(SWITCH_PIN));
                printf("Switch triggered! Executing action...\n");
                if (lights_on)
                {
                    printf("Lights on.\n");
                    controller.sunset(1500);
                    lights_on = false;
                }
                else
                {
                    printf("Lights off.\n");
                    controller.sunrise(1500);

                    lights_on = true;
                }
            }
        }

        // UART command parsing
        int c = getchar_timeout_us(0); // 0 means immediate return if no input
        if (c != PICO_ERROR_TIMEOUT)
        {
            if (c == '\n' || c == '\r')
            {
                input[pos] = '\0';
                parser.parse_and_execute(input);
                pos = 0;
            }
            else if (pos < sizeof(input) - 1)
            {
                input[pos++] = c;
            }
        }
    }
    return 0;
}
