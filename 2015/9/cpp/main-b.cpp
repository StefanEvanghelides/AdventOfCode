#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
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

uint64_t runTSP(const Numbers2& distances, uint64_t startIdx)
{
  std::vector<uint64_t> vertex;
  for (uint64_t i = 0; i < distances.size(); ++i)
  {
    if (i != startIdx) vertex.push_back(i);
  }

  // std::cout << "startIdx: " << startIdx << std::endl;

  uint64_t maxDist = 0;
  do
  {
    // std::cout << "Perm: ";
    // for (const uint64_t& v : vertex)
    //   std::cout << v << " ";
    // std::cout << std::endl;

    // store current Path weight(cost)
    uint64_t current_pathweight = 0;

    // compute current path weight
    uint64_t k = startIdx;
    for (uint64_t i = 0; i < vertex.size(); i++)
    {
      current_pathweight += distances[k][vertex[i]];
      k = vertex[i];
    }
    // current_pathweight += distances[k][startIdx];

    // update minimum
    maxDist = std::max(maxDist, current_pathweight);
  }
  while (std::next_permutation(vertex.begin(), vertex.end()));

  return maxDist;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  if (rawInput.size() < 1)
  {
    std::cout << "RAW INPUT HAS NO ELEMENTS!" << std::endl;
    return;
  }

  std::map<std::string, uint64_t> cityIndices;
  uint64_t idx = 0;
  Strings2 inputs;
  for (const std::string& input : rawInput)
  {
    const Strings& tokens = split(input, " ");
    const std::string& city1 = tokens[0];
    const std::string& city2 = tokens[2];
    if (!cityIndices.contains(city1))
    {
      cityIndices[city1] = idx++;
    }
    if (!cityIndices.contains(city2))
    {
      cityIndices[city2] = idx++;
    }
    inputs.push_back(tokens);
  }

  // for (const auto& [key,val] : cityIndices)
  //   std::cout << "City - idx: " << key << " - " << val << std::endl;

  Numbers2 data(cityIndices.size(), Numbers(cityIndices.size(), 0));
  for (const Strings& input : inputs)
  {
    const uint64_t& cityIdx1 = cityIndices[input[0]];
    const uint64_t& cityIdx2 = cityIndices[input[2]];
    const uint64_t& dist = std::stoull(input[4]);
    data[cityIdx1][cityIdx2] = dist;
    data[cityIdx2][cityIdx1] = dist;
  }

  // for (const Numbers& dat : data)
  // {
  //   for (const uint64_t& d : dat)
  //   {
  //     std::cout << d << " ";
  //   }
  //   std::cout << std::endl;
  // }

  // Run a TSP algorithm.
  uint64_t maxDist = 0;
  for (const auto& [_,idx] : cityIndices)
  {
    const uint64_t dist = runTSP(data, idx);
    if (dist > maxDist) maxDist = dist;
  }
  std::cout << "MaxDist: " << maxDist << std::endl;
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
    // rawInput.emplace_back(line);

    const std::string trimmedLine = trimmed(line);
    if (!trimmedLine.empty() && trimmedLine != "" && trimmedLine != "\n")
    {
      rawInput.emplace_back(trimmedLine);
    }
  }

  solve(rawInput);
}

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

int main(int argc, char** argv)
{
  // Windows enable nice formatting output.
  HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD consoleMode;
  GetConsoleMode(handleOut, &consoleMode);
  consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(handleOut, consoleMode);

  // Parse arguments.
  const char* INPUT_FILENAME = { "input.txt" };
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