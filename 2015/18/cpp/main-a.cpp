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


constexpr uint64_t MAX_ITERATIONS = 100;


uint64_t countNeighbours(const Bools2& grid, const uint64_t& x, const uint64_t& y)
{
  uint64_t count = 0;
  const int64_t& ix = (int64_t)x;
  const int64_t& iy = (int64_t)y;
  for (int64_t i = ix-1; i <= ix+1; ++i)
  {
    for (int64_t j = iy-1; j <= iy+1; ++j)
    {
      // Conditions
      if (i < 0 || i >= grid.size() ||
          j < 0 || j >= grid[0].size() ||
          i == ix && j == iy) continue;

      count += grid[i][j];
    }
  }

  return count;
}


Bools2 next(const Bools2& grid)
{
  // Default lights to off.
  Bools2 copy (grid.size(), Bools(grid[0].size()));

  for (uint64_t i = 0; i < grid.size(); ++i)
  {
    for (uint64_t j = 0; j < grid[i].size(); ++j)
    {
      // Count the neighbours.
      const uint64_t& count = countNeighbours(grid, i, j);

      // By default lights are off. Check conditions for turning on.
      copy[i][j] = (count == 3) || (count == 2 && grid[i][j]);
    }
  }

  return copy;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // Read the grid.
  Bools2 grid;
  for (uint64_t i = 0; i < rawInput.size(); ++i)
  {
    Bools row;
    for (uint64_t j = 0; j < rawInput[i].size(); ++j)
    {
      row.push_back(rawInput[i][j] == '#');
    }
    grid.push_back(row);
  }

  // Now perform iterations.
  uint64_t iter = 0;
  while (iter++ < MAX_ITERATIONS)
  {
    grid = next(grid);
  }

  // Count the number of lights on.
  uint64_t count = 0;
  for (const Bools& row : grid)
    for (const bool& b : row)
      count += b;

  std::cout << "Lights on: " << count << std::endl;

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