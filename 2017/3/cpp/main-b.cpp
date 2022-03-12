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

constexpr uint64_t SPIRAL_SIZE = 5;

uint64_t sumNeighbours(Numbers2& spiral, const int64_t& x, const int64_t& y)
{
  uint64_t sum = 0;
  for (int64_t i = x-1; i <= x+1; ++i)
  {
    if (i < 0 || i >= spiral.size()) continue;
    for (int64_t j = y-1; j <= y+1; ++j)
    {
      if (j < 0 || j > spiral[i].size()) continue;
      if (i == x && j == y) continue;
      sum += spiral[i][j];
    }
  }
  spiral[x][y] = sum;
  return sum;
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // For part b) numbers explode rather quickly,
  // so it's actually feasible to compute the nubmers spiral.

  for (const std::string& input : rawInput)
  {
    uint64_t nr = std::stoull(input);
    std::cout << std::endl << "NR=" << nr << std::endl;

    // Build the spiral on the go and check the numbers.
    Numbers2 spiral (2*SPIRAL_SIZE+1, Numbers(2*SPIRAL_SIZE+1, 0LL));
    spiral[SPIRAL_SIZE][SPIRAL_SIZE] = 1;
    int64_t dirX = 1, dirY = 0;
    int64_t cx = SPIRAL_SIZE, cy = SPIRAL_SIZE;
    uint64_t sum  = 0;
    bool found = false;
    for (int64_t i = 1; i < 2*SPIRAL_SIZE+1; ++i)
    {
      // For every turn we can do 1 X and 1 Y side,
      // starting with the X axis.

      // The X side.
      if (dirX == 1)
      {
        for (int64_t x = 1; x <= i; ++x)
        {
          cx++;
          sum = sumNeighbours(spiral, cx, cy);
          if (sum > nr)
          {
            found = true;
            std::cout << "First value larger (1): " << sum << std::endl;
            break;
          }
        }
      }
      else if (dirX == -1)
      {
        for (int64_t x = 1; x <= i; ++x)
        {
          cx--;
          sum = sumNeighbours(spiral, cx, cy);
          if (sum > nr)
          {
            found = true;
            std::cout << "First value larger (2): " << sum << std::endl;
            break;
          }
        }
      }
      if (found) break;

      // Now switch to the Y axis
      dirY = -dirX;
      dirX = 0;

      // -------------------------------

      // The Y side.
      if (dirY == 1)
      {
        for (int64_t y = 1; y <= i; ++y)
        {
          cy++;
          sum = sumNeighbours(spiral, cx, cy);
          if (sum > nr)
          {
            found = true;
            std::cout << "First value larger (3): " << sum << std::endl;
            break;
          }
        }
      }
      else if (dirY == -1)
      {
        for (int64_t y = 1; y <= i; ++y)
        {
          cy--;
          sum = sumNeighbours(spiral, cx, cy);
          if (sum > nr)
          {
            found = true;
            std::cout << "First value larger (4): " << sum << std::endl;
            break;
          }
        }
      }
      if (found) break;

      // Now switch back to the X axis
      dirX = dirY;
      dirY = 0;

      // std::cout << " -- iter " << i << "  cx=" << cx << "  cy=" << cy << "  dirX=" << dirX << "  dirY=" << dirY << std::endl;
    }

    if (!found)
    {
      std::cout << "Element not found! Increase spiral size! Current size: " << SPIRAL_SIZE << std::endl;
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