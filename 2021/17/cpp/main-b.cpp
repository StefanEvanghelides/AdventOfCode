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
#include <filesystem>
namespace fs = std::filesystem;

struct Point
{
  int64_t x, y;

  Point() = default;
  ~Point() = default;

  Point(const int64_t& xs, const int64_t& ys)
    : x(xs), y(ys)
  {}
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{
  os << "(" << p.x << "," << p.y << ")";
  return os;
}

struct Rect
{
  Point topLeft, bottomRight;

  Rect() = default;
  ~Rect() = default;

  Rect(const Point& tl, const Point& br)
    : topLeft(tl), bottomRight(br)
  {}
  Rect(const int64_t& x1, const int64_t& y1, const int64_t& x2, const int64_t& y2)
    : topLeft({x1, y1}), bottomRight({x2, y2})
  {}
};

std::ostream& operator<<(std::ostream& os, const Rect& r)
{
  os << "TopLeft: " << r.topLeft << " - BottomRight: " << r.bottomRight;
  return os;
}

bool insideRect(const Point& p, const Rect& r)
{
  return p.x >= r.topLeft.x && p.x <= r.bottomRight.x &&
        p.y <= r.topLeft.y && p.y >= r.bottomRight.y;
}

bool tooFar(const Point& p, const Point& dir, const Rect& r)
{
  return p.x > r.bottomRight.x || p.y < r.bottomRight.y ||
         dir.x == 0 && p.x < r.topLeft.x;
}

bool hitsTarget(const Point& startDir, const Rect& target, const uint64_t& maxIterations)
{
  uint64_t i = 0;
  Point p = startDir;
  Point dir = startDir;
  while (i++ < maxIterations && !tooFar(p, dir, target))
  {
    if (insideRect(p, target))
    {
      return true;
    }
    dir.x = std::max(0LL, dir.x - 1);
    dir.y--;

    p.x += dir.x;
    p.y += dir.y;
  }

  return false;
}

std::vector<std::string> split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
{
  std::vector<std::string> tokens;

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

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  //// SOLUTION WAS PURELY MATHEMATICAL!
  //// Knowing these rules, we had to make sure that the sum N(N+1)/2 is always within the input
  //// and that X = abs(min(y)+1). Since the min Y = -107 -> then the starting Y value = 106
  //// We then apply the 106*107/2 = 5671.
  ////
  //// Quite easy and fun.


  const std::vector<std::string>& input = split(rawInput[0], ": ");
  const std::vector<std::string>& sides = split(input[1], ", ");
  
  // First side
  const std::vector<std::string>& numbersSide1 = split(sides[0], "=");
  const std::vector<std::string>& topLeftNumbers = split(numbersSide1[1], "..");
  const int64_t& x1 = std::stoll(topLeftNumbers[0]);
  const int64_t& x2 = std::stoll(topLeftNumbers[1]);

  // Second side
  const std::vector<std::string>& numbersSide2 = split(sides[1], "=");
  const std::vector<std::string>& bottomRightNumbers = split(numbersSide2[1], "..");
  // Note the inversion between y1 and y2!
  const int64_t& y1 = std::stoll(bottomRightNumbers[1]);
  const int64_t& y2 = std::stoll(bottomRightNumbers[0]);

  // Retrieve the rectangle from the input
  const Rect target(x1, y1, x2, y2);
  //std::cout << "Target: " << target << std::endl;

  // Use the computed value from part a) as the max iterations possible.
  // Also set some boundaries
  const int64_t& minY = target.bottomRight.y;
  const int64_t& maxY = abs(minY + 1);
  const int64_t& minX = 0;
  const int64_t& maxX = target.bottomRight.x;

  const int64_t MAX_ITERATIONS = maxY * (maxY + 1) * 0.5;

  std::cout << "minX: " << minX << "; " << "maxX: " << maxX << "; " << "minY: " << minY << "; " << "maxY: " << maxY << std::endl;
  std::cout << "Max iterations: " << MAX_ITERATIONS << std::endl;

  uint64_t count = 0;
  for (int64_t i = minX; i <= maxX; ++i)
  {
    for (int64_t j = minY; j <= maxY; ++j)
    {
      const Point p(i,j);
      if (hitsTarget(p, target, MAX_ITERATIONS))
      {
        count++;
      }
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
  std::vector<std::string> rawInput;

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