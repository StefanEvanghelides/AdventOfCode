#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <string_view>
#include <string>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Pairs = std::vector<std::pair<int64_t,int64_t>>;
using Pairs2 = std::vector<Pairs>;
using Line = std::pair<std::pair<int64_t,int64_t>,std::pair<int64_t,int64_t>>;
using Lines = std::vector<Line>;


Strings split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
{
  Strings tokens;

  size_t start = 0;
  size_t end = 0;
  while((end = s.find(delim, start)) != std::string::npos)
  {
    tokens.emplace_back(s.substr(start, end-start));
    start = end + delim.size();
    if (keepSplitting)
    {
      while (s.substr(start, delim.size()) == delim)
      {
        start += delim.size();
      }
    }
  }
  tokens.emplace_back(s.substr(start));

  return tokens;
}

std::string trimmed(const std::string& s)
{
  auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}


constexpr uint32_t MAX_VALS = 3;


void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  uint32_t sum = 0;
  uint32_t maxSum[MAX_VALS] = {0};
  for (std::string inp : rawInput) {
    if (inp.empty()) {
      sum = 0;
      continue;
    }

    uint32_t nr = (uint32_t)std::stoul(inp);
    sum += nr;
    if (sum > maxSum[MAX_VALS-1]) maxSum[MAX_VALS-1] = sum;
    for (int i = MAX_VALS-2; i >= 0; --i) {
      if (sum > maxSum[i]) {
        maxSum[i+1] = maxSum[i];
        maxSum[i] = sum;
      }
    }
  }

  uint32_t combined = 0;
  for (int i = 0; i < MAX_VALS; ++i) {
    combined += maxSum[i];
  }

  std::cout << "Combined sum: " << combined << std::endl;

}

void run(std::string_view filename)
{
  // Check if the file exists.
  const fs::path filepath { filename };
  if (!fs::exists(filepath))
  {
    std::cout << "File " << filepath.string() << " does not exist!" << std::endl;
    return;
  }

  std::cout << "Running the program using the input file: " << filepath.string() << std::endl;

  // Read the raw input and pass it in.
  Strings rawInput;

  std::ifstream file(filepath, std::ios::in);
  for(std::string line; std::getline(file, line); )
  {
    // Simple line
    rawInput.emplace_back(line);

    // Trimmed line
    // const std::string trimmedLine = trimmed(line);
    // if (!trimmedLine.empty() && trimmedLine != "" && trimmedLine != "\n")
    // {
    //   rawInput.emplace_back(trimmedLine);
    // }
  }

  solve(rawInput);
}

int main(int argc, char** argv)
{
  // Parse arguments.
  const char* INPUT_FILENAME = { "input" };
  std::string_view filename;
  if (argc < 2)
  {
    // std::cout << "Missing filename argument!" << std::endl;
    filename = INPUT_FILENAME;
  }
  else
  {
    filename = argv[1];
  }

  // Run the program.
  run(filename);

  return 0;
}