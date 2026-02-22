#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rm_a0/a0_01_temperature.hpp"

int main() {
  double celsius = 0.0;
  if (!(std::cin >> celsius)) {
    return 0;
  }
  std::cout << rm_a0::FormatFahrenheit(rm_a0::CelsiusToFahrenheit(celsius));
  return 0;
}

double rm_a0::CelsiusToFahrenheit(double celsius) {
    return celsius * 1.8 + 32;
}

std::string rm_a0::FormatFahrenheit(double fahrenheit) {
    std::ostringstream oss;
    oss << std::fixed<<std::setprecision(2) <<fahrenheit;
    return oss.str();
}

std::string SolveTemperature(const std::string &input, bool &ok) {
    std::istringstream iss(input);
    double celsius = 0.0;
    if (iss >> celsius) {
        ok = true;
        double fahrenheit = rm_a0::CelsiusToFahrenheit(celsius);
        std::string result = rm_a0::FormatFahrenheit(fahrenheit);
        result+="\n";
        return result;
    }
    ok = false;
    return "";
}
