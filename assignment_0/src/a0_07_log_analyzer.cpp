#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rm_a0/a0_07_log_analyzer.hpp"
#include <fstream>
#include <algorithm>
int main() {
  std::string path;
  if (!std::getline(std::cin, path)) {
    return 0;
  }

  bool ok = false;
  std::string out = rm_a0::SolveLogAnalyzer(path , ok);
  std::cout << out;

  return 0;
}

namespace rm_a0 {
    LogStats AnalyzeLogFile(const std::string &path, bool &ok) {
        std::ifstream file(path);
        std::string info,warn,error,t;
        int infos = 0,warns = 0,errors = 0,ts,sum = 0;
        while (file >> t >> ts) {
            if (t=="info") {
                infos ++;
                sum += ts;
            }
            else if (t=="warn") {
                warns ++;
                sum += ts;
            }
            else if (t=="error") {
                errors ++;
                sum += ts;
            }
        }
        LogStats result;
        result.info = infos;
        result.warn = warns;
        result.error = errors;
        result.avg_ms = sum/(infos +warns +errors)*1.00;
        if (infos > warns) {
            result.max_ms = std::max(infos,errors);
        }
        else {
            result.max_ms = std::max(errors,warns);
        }
        return result;
    }
    std::string SolveLogAnalyzer(const std::string &input, bool &ok) {
        ok = true;
        LogStats result;
        result = AnalyzeLogFile(input,ok);
        std::string out;
        std::ostringstream oss;
        oss << result.info<<"\n";
        oss <<result.warn<<"\n";
        oss <<result.error<<"\n";
        oss <<std::fixed << std::setprecision(2)<<result.avg_ms<<"\n";
        oss <<result.max_ms<<"\n";
        out = oss.str();
        return out;
    }
}