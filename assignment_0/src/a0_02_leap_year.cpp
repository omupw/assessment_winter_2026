#include <iostream>

#include "rm_a0/a0_02_leap_year.hpp"

int main() {
  int year = 0;
  if (!(std::cin >> year)) {
    return 0;
  }

  std::cout << rm_a0::FormatLeapYearAnswer(rm_a0::IsLeapYear(year));

  return 0;
}
std::string rm_a0::FormatLeapYearAnswer(bool is_leap_year) {
    if (is_leap_year) {
        return "YES\n";
    }
    else {
        return "NO\n";
    }
}

bool rm_a0::IsLeapYear(int year) {
    if (year % 4 == 0) {
        if (year % 100 != 0) {
            return true;
        }
    }
    else if (year % 400 == 0) {
        return true;
    }
    else {
        return false;
    }
}
