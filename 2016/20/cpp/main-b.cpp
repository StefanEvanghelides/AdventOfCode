#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

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
#include <bits/stdc++.h>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Pairs = std::vector<std::pair<uint64_t,uint64_t>>;
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

void print(const std::map<int64_t, int64_t>& ranges)
{
  for (const auto& [key,val] : ranges)
  {
    std::cout << "(" << key << "," << val << ")" << std::endl;
  }
  std::cout << " - total size: " << ranges.size() << std::endl;
}

int64_t findFirstValue(const std::map<int64_t, int64_t>& ranges)
{
  int64_t firstVal = 0;

  while(ranges.contains(firstVal))
  {
    firstVal = ranges.at(firstVal) + 1;
  }

  return firstVal;
}

int64_t count(const std::map<int64_t, int64_t>& ranges)
{
  int64_t count = 0;

  for (const auto& [start,end] : ranges)
  {
    count += end - start + 1;
  }

  return count;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::map<int64_t, int64_t> ranges;
  for (const std::string& inp : rawInput)
  {
    const Strings& tokens = split(inp, "-");
    if (tokens.size() != 2)
    {
      std::cout << "INCORRECT INPUT: " << inp << std::endl;
      continue;
    }

    const int64_t& start = std::stoll(tokens[0]);
    const int64_t& end = std::stoll(tokens[1]);
    ranges[start] = end;
  }

  // Now that ranges are sorted, create the trimmed ranges map.
  std::map<int64_t, int64_t> trimmedRanges;
  int64_t start = -1, end = -1;
  for (const auto& [rstart,rend] : ranges)
  {
    if (end < rstart)
    {
      if (end > -1) trimmedRanges[start] = end;
      start = rstart;
      end = rend;
      if (end > -1) continue;
    }

    if (end < rend) end = rend;
  }
  trimmedRanges[start] = end;

  // print(ranges);
  // print(trimmedRanges);
  std::cout << "Cound: " << 4294967296 - count(trimmedRanges) << std::endl;
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