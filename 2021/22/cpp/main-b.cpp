#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <memory>
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


/*

The Point and Cube structure are nice, they could work and allow the code to be easily maintainable.
But the solution is quite cumbersome.

The proposed solution comes from Competitive Programmer Neal Wu: https://www.youtube.com/watch?v=YKpViLcTp64

The idea: create a condensed array with the inputs only.
To count the number of lights open, simply compute volumes based on the inputs.

Perhaps some other ideas: https://github.com/marcodelmastro/AdventOfCode2021/

*/

// struct Point
// {
//   int64_t x, y, z;

//   Point()
//     : x(0LL), y(0LL), z(0LL)
//   {}
//   Point(const int64_t& a, const int64_t& b, const int64_t& c)
//     : x(a), y(b), z(c)
//   {}
//   ~Point() = default;
// };

// struct Cube
// {
//   Point topLeftNear, bottomRightFar;

//   Cube() = default;
//   Cube(const Point& tln, const Point& brf)
//     : topLeftNear(tln), bottomRightFar(brf)
//   {}
//   ~Cube() = default;

//   uint64_t volume() const
//   {
//     const int64_t sizeX = bottomRightFar.x - topLeftNear.x;
//     const int64_t sizeY = bottomRightFar.y - topLeftNear.y;
//     const int64_t sizeZ = bottomRightFar.z - topLeftNear.z;
//     const uint64_t volume = (uint64_t)abs(sizeX * sizeY * sizeZ);
//     return volume;
//   }

//   bool isEmpty()
//   {
//     return topLeftNear.x == bottomRightFar.x
//         || topLeftNear.y == bottomRightFar.y
//         || topLeftNear.z == bottomRightFar.z;
//   }

//   static bool intersect1D(const int64_t& x1, const int64_t& y1, const int64_t& x2, const int64_t& y2)
//   {
//     return y1 >= x2 && x1 <= y2;
//   }

//   bool intersect(const Cube& c)
//   {
//     return intersect1D(topLeftNear.x, bottomRightFar.x, c.topLeftNear.x, c.bottomRightFar.x)
//         && intersect1D(topLeftNear.y, bottomRightFar.y, c.topLeftNear.y, c.bottomRightFar.y)
//         && intersect1D(topLeftNear.z, bottomRightFar.z, c.topLeftNear.z, c.bottomRightFar.z);
//   }

//   Cube difference(const Cube& c)
//   {
//     std::cout << "TODO: complete 3D intersection" << std::endl;
//     return c;
//   }
// };


struct State
{
  bool type;
  int64_t x1, x2, y1, y2, z1, z2;
};

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

  std::vector<int64_t> X, Y, Z;
  std::vector<State> states;

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
    const int64_t& x1 = std::stoll(xRange[0]);
    const int64_t& x2 = std::stoll(xRange[1]);
    const int64_t& y1 = std::stoll(yRange[0]);
    const int64_t& y2 = std::stoll(yRange[1]);
    const int64_t& z1 = std::stoll(zRange[0]);
    const int64_t& z2 = std::stoll(zRange[1]);

    const bool type = line[1] == 'n';

    states.emplace_back();
    State& s = states.back();
    s.type = type;
    s.x1 = x1;
    s.x2 = x2;
    s.y1 = y1;
    s.y2 = y2;
    s.z1 = z1;
    s.z2 = z2;

    s.x2++;
    s.y2++;
    s.z2++;

    X.push_back(s.x1);
    X.push_back(s.x2);
    Y.push_back(s.y1);
    Y.push_back(s.y2);
    Z.push_back(s.z1);
    Z.push_back(s.z2);
  }

  std::sort(X.begin(), X.end());
  std::sort(Y.begin(), Y.end());
  std::sort(Z.begin(), Z.end());

  const uint64_t N = X.size();

  // This is the condensed array
  std::vector<std::vector<std::vector<bool>>> grid(N, std::vector<std::vector<bool>>(N, std::vector<bool>(N, false)));

  // Simple function to get the index of an element in an array
  auto getIndex = [&](const std::vector<int64_t>& C, const int64_t& c) -> uint64_t {
    return uint64_t(std::lower_bound(C.begin(), C.end(), c) - C.begin());
  };

  for (const State& s : states)
  {
    // Fetch the indices from the condensed array
    const uint64_t& x1_idx = getIndex(X, s.x1);
    const uint64_t& x2_idx = getIndex(X, s.x2);
    const uint64_t& y1_idx = getIndex(Y, s.y1);
    const uint64_t& y2_idx = getIndex(Y, s.y2);
    const uint64_t& z1_idx = getIndex(Z, s.z1);
    const uint64_t& z2_idx = getIndex(Z, s.z2);
    const bool type = s.type;

    // Assign condensed grid values
    for (int64_t x = x1_idx; x < x2_idx; ++x)
    {
      for (int64_t y = y1_idx; y < y2_idx; ++y)
      {
        for (int64_t z = z1_idx; z < z2_idx; ++z)
        {
          grid[x][y][z] = type;
        }
      }
    }
  }

  uint64_t count = 0;

  for (uint64_t x = 0; x < N-1; ++x)
  {
    for (uint64_t y = 0; y < N-1; ++y)
    {
      for (uint64_t z = 0; z < N-1; ++z)
      {
        count += uint64_t(grid[x][y][z]) * (X[x+1] - X[x]) * (Y[y+1] - Y[y]) * (Z[z+1] - Z[z]);
      }
    }
  }

  std::cout << "Lights ON: " << count << std::endl;

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