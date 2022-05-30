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
  int64_t x, y, z;

  Point()
    : x(0LL), y(0LL), z(0LL)
  {}
  Point(const int64_t& a, const int64_t& b, const int64_t& c)
    : x(a), y(b), z(c)
  {}
  ~Point() = default;
};

struct Rect
{
  Point topLeftNear, bottomRightFar;

  Rect() = default;
  Rect(const Point& tln, const Point& brf)
    : topLeftNear(tln), bottomRightFar(brf)
  {}
  ~Rect() = default;
};

struct Board
{
  std::vector<std::vector<std::vector<bool>>> grid;
  int64_t minX, maxX;
  int64_t minY, maxY;
  int64_t minZ, maxZ;

  Board() = default;
  Board(const int64_t& mix, const int64_t& mx,
        const int64_t& miy, const int64_t& my,
        const int64_t& miz, const int64_t& mz,
        const bool initValue = false)
    : minX(mix), maxX(mx), minY(miy), maxY(my), minZ(miz), maxZ(mz)
  {
    const std::vector<bool> vecZ(abs(maxZ - minZ + 1), initValue);
    const std::vector<std::vector<bool>> vecY(abs(maxY - minY + 1), vecZ);
    const std::vector<std::vector<std::vector<bool>>> vecX(abs(maxX - minX + 1), vecY);
    grid = vecX;
  }
  ~Board() = default;

  void turn(const Rect& r, const bool turnType)
  {
    for (int64_t i = r.topLeftNear.x; i <= r.bottomRightFar.x; ++i)
    {
      // Guard for I
      if (i < minX || i > maxX) continue;

      for (int64_t j = r.topLeftNear.y; j <= r.bottomRightFar.y; ++j)
      {
        // Guard for J
        if (j < minY || j > maxY) continue;

        for (int64_t k = r.topLeftNear.z; k <= r.bottomRightFar.z; ++k)
        {
          // Guard for K
          if (k < minZ || k > maxZ) continue;

          grid[i-minX][j-minY][k-minZ] = turnType;
        }
      }
    }
  }

  uint64_t countOn() const
  {
    uint64_t count = 0;

    for (uint64_t i = 0; i < abs(maxX - minX + 1); ++i)
    {
      for (uint64_t j = 0; j < abs(maxY - minY + 1); ++j)
      {
        for (uint64_t k = 0; k < abs(maxZ - minZ + 1); ++k)
        {
          if (grid[i][j][k])
          {
            count++;
          }
        }
      }
    }

    return count;
  }
};

std::ostream& operator<<(std::ostream& os, const Rect& r)
{
  os << "(" << r.topLeftNear.x << "; " << r.topLeftNear.y << "; " << r.topLeftNear.z << ")  ";
  os << "(" << r.bottomRightFar.x << "; " << r.bottomRightFar.y << "; " << r.bottomRightFar.z << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
  os << "minX, maxX = " << b.minX << " " << b.maxX << "; minY, maxY = " << b.minY << " " << b.maxY << "; minZ, maxZ = " << b.minZ << " " << b.maxZ << std::endl;
  for (uint64_t i = 0; i < abs(b.maxX - b.minX + 1); ++i)
  {
    for (uint64_t j = 0; j < abs(b.maxY - b.minY + 1); ++j)
    {
      for (uint64_t k = 0; k < abs(b.maxZ - b.minZ + 1); ++k)
      {
        os << (int)b.grid[i][j][k];
      }
      os << std::endl;
    }
    os << std::endl;
  }
  return os;
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

  Board b (-50, 50, -50, 50, -50, 50);

  for (const auto& line : rawInput)
  {
    // Guards
    if (line.size() < 5)
    {
      std::cout << "LINE HAS LESS THAN 5 CHARS! Skipping..." << std::endl;
      continue;
    }

    // First split each line on the comma.
    const std::vector<std::string>& groups = split(line, ",");

    if (groups.size() != 3)
    {
      std::cout << "Group does not have 3 elements! Line: " << line << std::endl;
      continue;
    }

    // Expect 3 groups, split them on the equal sign.
    const std::vector<std::string>& xRanges = split(groups[0], "=");
    const std::vector<std::string>& yRanges = split(groups[1], "=");
    const std::vector<std::string>& zRanges = split(groups[2], "=");

    // Now take the 2nd part, this is the range.
    const std::string& xRangeStr = xRanges[1];
    const std::string& yRangeStr = yRanges[1];
    const std::string& zRangeStr = zRanges[1];

    // Now split them again on the ".." -> these are now the 2 numbers.
    const std::vector<std::string>& xRange = split(xRangeStr, "..");
    const std::vector<std::string>& yRange = split(yRangeStr, "..");
    const std::vector<std::string>& zRange = split(zRangeStr, "..");

    // Now fetch and convert the numbers
    int64_t x1 = std::stoll(xRange[0]);
    int64_t x2 = std::stoll(xRange[1]);
    int64_t y1 = std::stoll(yRange[0]);
    int64_t y2 = std::stoll(yRange[1]);
    int64_t z1 = std::stoll(zRange[0]);
    int64_t z2 = std::stoll(zRange[1]);

    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
    if (z1 > z2) std::swap(z1, z2);

    const Point tln(x1, y1, z1);
    const Point brf(x2, y2, z2);
    const Rect rect(tln, brf);

    // std::cout << "Line: " << line << std::endl;
    // std::cout << "  x1, x2: " << x1 << " " << x2 << std::endl;
    // std::cout << "  y1, y2: " << y1 << " " << y2 << std::endl;
    // std::cout << "  z1, z2: " << z1 << " " << z2 << std::endl;
    // std::cout << "Rect: " << rect << std::endl;

    // Check if we turn on
    const bool turnOn = line[1] == 'n';
    b.turn(rect, turnOn);

    //std::cout << "Board: " << std::endl << b << std::endl;

  }

  const uint64_t countOn = b.countOn();
  std::cout << "Lights ON: " << countOn << std::endl;

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