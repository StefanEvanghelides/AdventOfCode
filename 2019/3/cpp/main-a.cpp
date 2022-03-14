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
#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
using Pairs = std::vector<std::pair<int64_t,int64_t>>;
using Pairs2 = std::vector<Pairs>;
using Point = std::pair<int64_t,int64_t>;
using Line = std::pair<Point,Point>;
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

void parseInput(Lines& lines, const Strings& lineStr, const int64_t& x0, const int64_t& y0)
{
  int64_t x = x0;
  int64_t y = y0;
  for (const std::string& elem : lineStr)
  {
    if (elem.size() < 2)
    {
      std::cout << "ELEM IS LESS THAN 2! elem=" << elem << std::endl;
    }
    const char& c = elem[0];
    const std::string& str = elem.substr(1);
    const int64_t& nr = std::stoull(str);
    // std::cout << "c=" << c << ";  nr=" << nr << std::endl;
    if (c == 'R')
    {
      // move to the right
      lines.push_back({{x, y}, {x, y+nr}});
      y += nr;
    }
    else if (c == 'L')
    {
      // move to the left
      lines.push_back({{x, y-nr}, {x, y}});
      y -= nr;
    }
    else if (c == 'U')
    {
      // move up
      lines.push_back({{x, y}, {x+nr, y}});
      x += nr;
    }
    else if (c == 'D')
    {
      // move down
      lines.push_back({{x-nr, y}, {x, y}});
      x -= nr;
    }
    else
    {
      std::cout << "Incorrect element! elem: " << elem << std::endl;
    }
    // std::cout << "  x1=" << lines.back().first.first << "; y1=" << lines.back().first.second << ";  x2=" << lines.back().second.first << "; y2=" << lines.back().second.second << std::endl;
  }
}

std::optional<Point> linesCross(const Line& line1, const Line& line2)
{
  // Now we know that lines are perpendicular, check their positions
  if (line1.first.second == line1.second.second && 
      line2.first.first == line2.second.first)
  {
    // Line one is on X, line 2 is on Y
    const int64_t diff1 = line1.first.second - line2.first.second;
    const int64_t diff2 = line1.first.second - line2.second.second;
    const int64_t diff3 = line2.first.first - line1.first.first;
    const int64_t diff4 = line2.first.first - line1.second.first;
    if (diff1 * diff2 < 0 && diff3 * diff4 < 0)
    {
      // intersection point
      const Point p{line2.first.first, line1.first.second};
      // std::cout << "Intersection(1): x=" << p.first << "; y=" << p.second << std::endl;
      // std::cout << "    Line 1: p1: x=" << line1.first.first << "; y=" << line1.first.second << ":  p2: x=" << line1.second.first << "; y=" << line1.second.second << std::endl;
      // std::cout << "    Line 2: p1: x=" << line2.first.first << "; y=" << line2.first.second << ":  p2: x=" << line2.second.first << "; y=" << line2.second.second << std::endl;
      return {p};
    }
  }
  else if (line1.first.first == line1.second.first &&
           line2.first.second == line2.second.second)
  {
    // Line one is on Y, line 2 is on X
    const int64_t diff1 = line1.first.first - line2.first.first;
    const int64_t diff2 = line1.first.first - line2.second.first;
    const int64_t diff3 = line2.first.second - line1.first.second;
    const int64_t diff4 = line2.first.second - line1.second.second;
    if (diff1 * diff2 < 0 && diff3 * diff4 < 0)
    {
      // intersection point
      const Point p{line1.first.first, line2.first.second};
      // std::cout << "Intersection(2): x=" << p.first << "; y=" << p.second << std::endl;
      // std::cout << "    Line 1: p1: x=" << line1.first.first << "; y=" << line1.first.second << ":  p2: x=" << line1.second.first << "; y=" << line1.second.second << std::endl;
      // std::cout << "    Line 2: p1: x=" << line2.first.first << "; y=" << line2.first.second << ":  p2: x=" << line2.second.first << "; y=" << line2.second.second << std::endl;
      return {p};
    }
  } 

  return {};
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

  Lines lines1;
  Lines lines2;

  // split the 2 inputs
  const Strings& lineStr1 = split(rawInput[0], ",");
  const Strings& lineStr2 = split(rawInput[1], ",");

  // Origin
  const int64_t x0 = 0;
  const int64_t y0 = 0;

  // Parse the 2 inputs.
  parseInput(lines1, lineStr1, x0, y0);
  parseInput(lines2, lineStr2, x0, y0);

  // Now find the closest intersection to the origin.
  uint64_t minDist = 1000000;
  for (uint64_t i = 0; i < lines1.size(); ++i)
  {
    for (uint64_t j = 0; j < lines2.size(); ++j)
    {
      const Line& line1 = lines1[i];
      const Line& line2 = lines2[j];
      const auto& result = linesCross(line1, line2);
      if (result.has_value())
      {
        // check the distance
        const Point& intersection = result.value();
        const uint64_t dist = uint64_t(abs(intersection.first) + abs(intersection.second));
        if (dist < minDist) minDist = dist;
      }
    }
  }

  std::cout << "Min dist: " << minDist << std::endl;

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