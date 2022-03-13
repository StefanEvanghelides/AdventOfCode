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
using NumberPairs = std::vector<std::pair<uint64_t,uint64_t>>;
using NumberPairs2 = std::vector<NumberPairs>;


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

Strings transformInput(const Strings& rawInput)
{
  Strings input;

  for (std::string line : rawInput)
  {
    auto it = line.find(':');
    line.erase(it, 1);
    it = line.find('@');
    line = line.substr(it + 2);
    
    input.push_back(line);
  }

  return input;
}

struct Rect
{
  uint64_t x0, y0, x1, y1;

  Rect()
    : x0(0), y0(0), x1(0), y1(0)
  {}

  Rect(const uint64_t& x00, const uint64_t& y00, const uint64_t& x01, const uint64_t& y01)
    : x0(x00), y0(y00), x1(x01), y1(y01)
  {}
};

bool checkOverlap(const Rect& r1, const Rect& r2)
{
  return (r1.x0 < r2.x1 && r1.x1 > r2.x0 &&
          r1.y1 > r2.y0 && r1.y0 < r2.y1); 
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }
 
  const Strings& input = transformInput(rawInput);

  const uint64_t MAX_SIZE = 1000;

  NumberPairs2 board(MAX_SIZE, NumberPairs(MAX_SIZE, {0,0}));
  std::vector<Rect> rectangles;
  for (uint64_t idx = 0; idx < input.size(); ++idx)
  {
    const std::string& line = input[idx];
    const Strings& rects = split(line, " ");
    if (rects.size() < 2)
    {
      std::cout << "WRONG INPUT LINE: " << line << std::endl;
      continue;
    }

    // the first part is the top-left corner
    const Strings& pos = split(rects[0], ",");
    if (pos.size() < 2)
    {
      std::cout << "Positions have less than 2 elements, line: " << line << std::endl;
      continue;
    }
    const uint64_t& x = std::stoull(pos[0]);
    const uint64_t& y = std::stoull(pos[1]);

    // the second part is the size of the rect
    const Strings& sizes = split(rects[1], "x");
    if (sizes.size() < 2)
    {
      std::cout << "Sizes have less than 2 elements, line: " << line << std::endl;
      continue;
    }
    const uint64_t& width = std::stoull(sizes[0]);
    const uint64_t& height = std::stoull(sizes[1]);

    rectangles.emplace_back(x, y, x+width, y+height);

  }

  // Check overlaps
  bool found = false;
  for (uint64_t i = 0; i < rectangles.size(); ++i)
  {
    bool overlap = false;
    for (uint64_t j = 0; j < rectangles.size(); ++j)
    {
      if (i == j) continue;

      const Rect& r1 = rectangles[i];
      const Rect& r2 = rectangles[j];

      if (checkOverlap(r1, r2))
      {
        overlap = true;
        break;
      }
    }
    if (!overlap)
    {
      std::cout << "Non-overlapping rectangle: " << i+1 << std::endl;
      break;
    }
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