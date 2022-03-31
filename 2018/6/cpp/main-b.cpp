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
using Pairs = std::vector<std::pair<uint64_t,uint64_t>>;
using Pairs2 = std::vector<Pairs>;


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

  const uint64_t MARGIN = 1000;
  const uint64_t AREA = 10000;

  uint64_t x0 = 100000, x1 = 0, y0 = 100000, y1 = 0;
  Pairs pairs;
  for (const std::string& line : rawInput)
  {
    const Strings& elems = split(line, ", ");
    if (elems.size() < 2)
    {
      std::cout << "ELEMS HAS LESS THAN 2 ELEMENTS! Line: " << line << std::endl;
      continue; 
    }

    const uint64_t x = std::stoull(elems[0]);
    const uint64_t y = std::stoull(elems[1]);
    pairs.emplace_back(x, y);

    if (x < x0) x0 = x;
    if (x > x1) x1 = x;
    if (y < y0) y0 = y;
    if (y > y1) y1 = y;
  }

  std::cout << "x0=" << x0 << ";  x1=" << x1 << ";  y0=" << y0 << ";  y1=" << y1 << std::endl;

  x1 = x1 - x0 + 2*MARGIN;
  y1 = y1 - y0 + 2*MARGIN;
  for (auto& [x,y] : pairs)
  {
    x = x - x0 + MARGIN;
    y = y - y0 + MARGIN;

    std::cout << " - x=" << x << "  y=" << y << std::endl;
  }
  x0 = MARGIN;
  y0 = MARGIN;

  std::cout << "AGAIN: x0=" << x0 << ";  x1=" << x1 << ";  y0=" << y0 << ";  y1=" << y1 << std::endl;

  Numbers2 board(x1, Numbers(y1, 0));
  uint64_t count = 0;
  for (uint64_t i = 0; i < board.size(); ++i)
  {
    for (uint64_t j = 0; j < board[i].size(); ++j)
    {
      uint64_t totalDistance = 0;
      for (uint64_t k = 0; k < pairs.size(); ++k)
      {
        const auto& [x,y] = pairs[k];
        totalDistance += (uint64_t)abs((int64_t)x - (int64_t)i) + (uint64_t)abs((int64_t)y - (int64_t)j);
      }
      count += totalDistance < AREA;
    }
  }

  std::cout << "Count: " << count << std::endl;

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