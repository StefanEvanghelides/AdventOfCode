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
#include <optional>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<int64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Point = std::pair<int64_t,int64_t>;
using Pair = std::pair<int64_t,int64_t>;
using Pairs = std::vector<Pair>;
using Pairs2 = std::vector<Pairs>;
using Trice = std::tuple<int64_t,int64_t,int64_t>;
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

constexpr uint64_t GRID_SIZE = 301;
constexpr uint64_t MASK_SIZE = 3;

// Output <sum,x,y>
Trice computePower(const Numbers2& grid, const int64_t maskSize)
{
  int64_t maxSum = 0;
  uint64_t maxX, maxY;
  for (uint64_t y = 1; y < GRID_SIZE-maskSize+1; ++y)
  {
    for (uint64_t x = 1; x < GRID_SIZE-maskSize+1; ++x)
    {
      int64_t sum = 0;
      for (uint64_t j = 0; j < maskSize; ++j)
      {
        for (uint64_t i = 0; i < maskSize; ++i)
        {
          sum += grid[x+i][y+j];
        }
      }
      if (sum > maxSum)
      {
        maxSum = sum;
        maxX = x;
        maxY = y;
      }
    }
  }
  return {maxSum, maxX, maxY};
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

  // Input is just 1 line, but I have multiple test scenarios.
  for (const std::string& input : rawInput)
  {
    const int64_t serialNumber = std::stoll(input);

    // Compute the grid.
    Numbers2 grid(GRID_SIZE, Numbers(GRID_SIZE, 0LL));
    for (uint64_t y = 1; y < GRID_SIZE; ++y)
    {
      for (uint64_t x = 1; x < GRID_SIZE; ++x)
      {
        const int64_t rackID = x + 10;
        int64_t power = (rackID * y + serialNumber) * rackID;
        power %= 1000;
        power /= 100;
        power -= 5;
        grid[x][y] = power;
      }
    }

    // Now find the the best fitting cell size with the maximum power.
    int64_t maxSum = 0;
    int64_t maxX, maxY, maxIdx;
    for (uint64_t i = 1; i < GRID_SIZE; ++i)
    {
      const Trice& currentOption = computePower(grid, i);
      const int64_t& sum = std::get<0>(currentOption);
      if (sum > maxSum)
      {
        maxSum = sum;
        maxX = std::get<1>(currentOption);
        maxY = std::get<2>(currentOption);
        maxIdx = i;
      }
    }


    std::cout << "Largest total power: " << maxX << "," << maxY << "," << maxIdx << std::endl;
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