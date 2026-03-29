#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rm_a0/a0_05_score_stats.hpp"
#include <vector>
#include <algorithm>
int main() {
  std::ostringstream oss;
  oss << std::cin.rdbuf();

  bool ok = false;
  std::string out = rm_a0::SolveScoreStats(oss.str(), ok);
  if (!ok) {
    return 0;
  }

  std::cout << out;

  return 0;
}
namespace rm_a0 {
    ScoreStatsResult ComputeScoreStats(const std::string &input, bool &ok) {
        ok = true;
        std::istringstream iss(input);
        int n;
        iss >> n;
        std::vector<std::pair<std::string, int>> scores;
        scores.resize(n);
        for (int i = 0; i < n; i++) {
            iss>>scores[i].first;
            iss>>scores[i].second;
        }
        std::stable_sort(scores.begin(), scores.end(),[](const auto& a, const auto& b) {  return a.second > b.second;});
        double avg = 0,sum = 0;
        for (int i = 0; i < n; i++) {
            sum += scores[i].second;
            avg = sum / n;
        }
        ScoreStatsResult result;
        result.top_score = scores[0].second;
        result.top_name = scores[0].first;
        result.avg = avg;
        return result;
    }
    std::string SolveScoreStats(const std::string &input, bool &ok) {
        ScoreStatsResult result;
        result = ComputeScoreStats(input, ok);
		std::string out;
		std::ostringstream oss;
		oss << "top=" << result.top_name << " " <<result.top_score << "\n" << "avg=" <<std::fixed << std::setprecision(2) << result.avg << std::endl;
		out = oss.str();
		return out;
    }
}