#include <cctype>
#include <iostream>
#include <string>

#include "rm_a0/a0_04_vowel_count.hpp"

int main() {
  std::string line;
  if (!std::getline(std::cin, line)) {
    return 0;
  }

  std::cout << rm_a0::CountVowels(line) << "\n";

  return 0;
}

std::size_t rm_a0::CountVowels(const std::string &input) {
    std::size_t count = 0;
    char lower_c;
    for (char c : input) {
        lower_c = std::tolower(c);
        if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' ||
                lower_c == 'o' || lower_c == 'u') {
            count++;
                }
    }
    return count;
}