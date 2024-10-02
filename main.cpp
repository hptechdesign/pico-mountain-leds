
#include "led_controller.hpp"
#include "command_parser.hpp"

int main()
{
    LEDController controller(60); // Adjust the number of LEDs as needed
    controller.init();

    CommandParser parser(controller);

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
