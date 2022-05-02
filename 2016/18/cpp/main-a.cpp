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

constexpr uint64_t MAX_ROWS = 40;

void print(const Bools2& rows, const uint64_t& size = MAX_ROWS)
{
  for (uint64_t i = 0; i < size; ++i)
  {
    if (i >= rows.size()) break;
    for (uint64_t j = 0; j < rows[i].size(); ++j)
    {
      std::cout << (rows[i][j] ? '^' : '.');
    }
    std::cout << std::endl;
  }
}

uint64_t count(const Bools2& rows, const uint64_t& size = MAX_ROWS)
{
  uint64_t counter = 0;
  for (uint64_t i = 0; i < size; ++i)
  {
    if (i >= rows.size()) break;
    for (uint64_t j = 0; j < rows[i].size(); ++j)
    {
      counter += !rows[i][j];
    }
  }
  return counter;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // For loop because I have multiple test scenarios.
  for (const std::string& inp : rawInput)
  {
    const uint64_t& size = inp.size();
    Bools2 rows;
    rows.emplace_back(size, false);
    for (uint64_t i = 0; i < size; ++i)
      rows.back()[i] = inp[i] == '^';
    uint64_t iter = 1;

    while (iter++ < MAX_ROWS)
    {
      const Bools last = rows.back();
      rows.emplace_back(size, false);
      for (uint64_t i = 0; i < size; ++i)
      {
        const bool left = (i == 0 ? false : last[i-1]);
        const bool right = (i == size-1 ? false : last[i+1]);
        rows.back()[i] = left ^ right;
      }
    }

    // Now show some rows: only the first 10 we are interested in.
    print(rows, 10);
    std::cout << "Count safe (max 10 rows): " << count(rows, 10) << std::endl;
    std::cout << "Full count safe: " << count(rows) << std::endl;
    std::cout << std::endl;
  }

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