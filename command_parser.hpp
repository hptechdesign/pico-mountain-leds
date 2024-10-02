#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include "led_controller.hpp"

class CommandParser
{
private:
    LEDController &led_controller;
    std::map<std::string, std::function<void(const std::vector<std::string> &)>> commands;

public:
    CommandParser(LEDController &controller);
    void register_command(const std::string &name, std::function<void(const std::vector<std::string> &)> func);
    void parse_and_execute(const std::string &input);
};

#endif // COMMAND_PARSER_HPP
