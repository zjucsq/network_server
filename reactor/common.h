#include <iostream>
#include <source_location>
#include <string_view>

void log(std::string_view message, const std::source_location& location = std::source_location::current());