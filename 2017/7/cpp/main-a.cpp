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

void print(const std::map<std::string, std::pair<uint64_t, Strings>>& towers)
{
  for (const auto& [key,value] : towers)
  {
    std::cout << "Base: " << key << ";  weight: " << value.first << ";  children size: " << value.second.size() << std::endl;
    if (value.second.size() > 0)
    {
      std::cout << " - ";
      for (const auto& child : value.second)
        std::cout << child << "  ";
      std::cout << std::endl;
    }
  }
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::map<std::string, std::pair<uint64_t, Strings>> towers;
  for (const std::string& input : rawInput)
  {
    const Strings& parts = split(input, " -> ");
    if (parts.size() < 1 || parts.size() > 2)
    {
      std::cout << "WRONG INPUT LINE! Line: " << input << std::endl;
      continue;
    }

    // First, get the base tower and the weight
    const Strings& baseWeight = split(parts[0], " ");
    const std::string& base = baseWeight[0];
    const uint64_t& weight = std::stoull(baseWeight[1].substr(1,baseWeight[1].size()-2));

    // If there are 2 parts, then get the children
    Strings children;
    if (parts.size() > 1)
    {
      children = split(parts[1], ", ");
    }
    towers[base] = {weight, children};
  }

  // print(towers);

  // To get the name of the base tower, simply check which children are not being named.
  std::set<std::string> bottoms;
  for (const auto& [base,_] : towers)
  {
    bottoms.insert(base);
  }
  for (const auto& [base,values] : towers)
  {
    if (values.second.size() > 0)
    {
      for (const std::string& child : values.second)
      {
        bottoms.erase(child);
      }
    }
    if (bottoms.size() == 1) break;
  }

  // Now show the bottom tower
  std::cout << "Bottom tower: " << *bottoms.begin() << std::endl;

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