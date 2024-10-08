#include "command_parser.hpp"

CommandParser::CommandParser(LEDController &controller) : led_controller(controller)
{

    // Register commands
    register_command("all_on", [this](const std::vector<std::string> &args)
                     {
            if (args.size() != 4) {
                std::cerr << "Error: all_on() expects 4 arguments (r, g, b, w)." << std::endl;
                return;
            }
            int r = std::stoi(args[0]);
            int g = std::stoi(args[1]);
            int b = std::stoi(args[2]);
            int w = std::stoi(args[3]);
            led_controller.all_on(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(w)); });

    register_command("all_off", [this](const std::vector<std::string> &args)
                     { led_controller.all_off(); });

    register_command("sunrise", [this](const std::vector<std::string> &args)
                     {
            if (args.size() != 1) {
                std::cerr << "Error: sunrise() expects 1 argument (duration)." << std::endl;
                return;
            }
            int duration = std::stoi(args[0]);
            led_controller.sunrise(duration); });

    register_command("sunset", [this](const std::vector<std::string> &args)
                     {
            if (args.size() != 1) {
                std::cerr << "Error: sunset() expects 1 argument (duration)." << std::endl;
                return;
            }
            int duration = std::stoi(args[0]);
            led_controller.sunset(duration); });
}

void CommandParser::register_command(const std::string &name, std::function<void(const std::vector<std::string> &)> func)
{
    commands[name] = func;
}

void CommandParser::parse_and_execute(const std::string &input)
{
    std::cout << "Executing command: " << input << std::endl; // Echo command
    size_t pos = input.find('(');
    if (pos == std::string::npos || input.back() != ')')
    {
        std::cerr << "Error: Invalid command syntax." << std::endl;
        return;
    }

    std::string command_name = input.substr(0, pos);
    std::string args_string = input.substr(pos + 1, input.length() - pos - 2); // Remove parentheses
    std::vector<std::string> args;

    // Split the arguments by comma
    size_t start = 0, end;
    while ((end = args_string.find(',', start)) != std::string::npos)
    {
        args.push_back(args_string.substr(start, end - start));
        start = end + 1;
    }
    args.push_back(args_string.substr(start)); // Add the last argument

    // Check if the command exists and execute
    auto command_iter = commands.find(command_name);
    if (command_iter != commands.end())
    {
        command_iter->second(args);
    }
    else
    {
        std::cerr << "Error: Unknown command '" << command_name << "'." << std::endl;
    }
}
