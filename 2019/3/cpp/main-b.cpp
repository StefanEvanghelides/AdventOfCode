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
using Points = std::vector<Point>;
using PointsMap = std::map<Point,uint64_t>;
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

uint64_t pointsDist(const Point& p1, const Point& p2)
{
  return uint64_t(abs(p2.first - p1.first) + abs(p2.second - p1.second));
}

void parseInput(PointsMap& line, const Strings& lineStr, const int64_t& x0, const int64_t& y0, const bool optimize = false)
{
  int64_t x = x0;
  int64_t y = y0;
  // Always include the origin.
  line[{x,y}] = 0;
  uint64_t totalSteps = 0;
  for (const std::string& elem : lineStr)
  {
    if (elem.size() < 2)
    {
      std::cout << "ELEM IS LESS THAN 2! elem=" << elem << std::endl;
    }
    const char& c = elem[0];
    const std::string& str = elem.substr(1);
    const int64_t& nr = std::stoull(str);
    if (c == 'R')
    {
      // Move to the right.
      for (int64_t i = y+1; i <= y+nr; ++i)
      {
        totalSteps++;
        const Point p {x,i};
          // Avoid duplicates, just use the previous total steps.
        if (optimize && line.contains(p)) totalSteps = line[p];
        else line[{x,i}] = totalSteps;
      }
      y += nr;
    }
    else if (c == 'L')
    {
      // Move to the left.
      for (int64_t i = y-1; i >= y-nr; --i)
      {
        totalSteps++;
        const Point p {x,i};
        // Avoid duplicates, just use the previous total steps.
        if (optimize && line.contains(p)) totalSteps = line[p];
        else line[{x,i}] = totalSteps;
      }
      y -= nr;
    }
    else if (c == 'U')
    {
      // Move up.
      for (int64_t i = x+1; i <= x+nr; ++i)
      {
        totalSteps++;
        const Point p {i,y};
        // Avoid duplicates, just use the previous total steps.
        if (optimize && line.contains(p)) totalSteps = line[p];
        else line[{i,y}] = totalSteps;
      }
      x += nr;
    }
    else if (c == 'D')
    {
      // Move down.
      for (int64_t i = x-1; i >= x-nr; --i)
      {
        totalSteps++;
        const Point p {i,y};
        // Avoid duplicates, just use the previous total steps.
        if (optimize && line.contains(p)) totalSteps = line[p];
        else line[{i,y}] = totalSteps;
      }
      x -= nr;
    }
    else
    {
      std::cout << "Incorrect element! elem: " << elem << std::endl;
    }
  }
}

void showLine(const PointsMap& line)
{
  // We want to display the map based on the distance travelled,
  // i.e. based on the second value. Use a helper vector for this.

  std::cout << "Line size: " << line.size() << std::endl;
  std::vector<std::pair<Point,int64_t>> points;
  for (const auto& [k,v] : line) points.emplace_back(k,v);

  sort(points.begin(), points.end(),
    [](const std::pair<Point,int64_t>& p1, const std::pair<Point,int64_t>& p2) {
      return p1.second < p2.second;
    });

  for (const auto& [point,dist] : points)
  {
    std::cout << " - p: (" << point.first << "; " << point.second  << ");  dist: " << dist << std::endl;
  }

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

  // split the 2 inputs
  const Strings& lineStr1 = split(rawInput[0], ",");
  const Strings& lineStr2 = split(rawInput[1], ",");

  // Origin
  const int64_t x0 = 0;
  const int64_t y0 = 0;
  const Point origin{x0,y0};

  // Parse the 2 inputs.
  // Cycles/duplicate points are handled by switching OPTIMIZATION flag on.
  // Note that for this exercise, we should NOT optimize.
  constexpr bool OPTIMIZATION = false;
  PointsMap line1, line2;
  parseInput(line1, lineStr1, x0, y0, OPTIMIZATION);
  parseInput(line2, lineStr2, x0, y0, OPTIMIZATION);

  // showLine(line1);
  // showLine(line2);

  // Now check the common points, except for the origin.
  line1.erase({x0, y0});
  line2.erase({x0, y0});
  uint64_t minDist = 1e10;
  uint64_t closestIntersection = 1e10;
  for (const auto& [point,dist1] : line1)
  {
    // Check if this is in line2, if so, check the distances
    if (line2.contains(point))
    {
      // We have an intersection, check if it's the best one.
      const int64_t& dist2 = line2[point];
      if (dist1 + dist2 < minDist) minDist = dist1 + dist2;

      // Check the closest distance
      const uint64_t& originDist = pointsDist(origin, point);
      if (originDist < closestIntersection) closestIntersection = originDist;
    }
  }

  std::cout << "Min dist: " << minDist << std::endl;
  std::cout << "Closest Intersection: " << closestIntersection << std::endl;

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