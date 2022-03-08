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

  const std::vector<std::string> inputs = split(rawInput[0], ", ");

  enum class Dir
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

  using Location = std::pair<int64_t, int64_t>;
  std::map<Location, uint64_t> locations;
  locations[{0,0}] = 1; // starting point must already be there.

  Dir curr = Dir::UP;
  int64_t hor = 0, vert = 0;
  int64_t found_hor = 0, found_vert = 0;
  bool found = false;
  for (const std::string& input : inputs)
  {
    // parse the input
    const bool left = input[0] == 'L';
    const int64_t& value = std::stoll(input.substr(1));

    // Now update the value and the direction
    switch (curr)
    {
      case Dir::UP:
        if (left)
        {
          curr = Dir::LEFT;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert, hor-i}]++;

            // Check for double
            if (locations[{vert, hor-i}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor-i;
              found_vert = vert;
              break;
            }
          }
          hor -= value;
        }
        else
        {
          curr = Dir::RIGHT;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert, hor+i}]++;

            // Check for double
            if (locations[{vert, hor+i}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor+i;
              found_vert = vert;
              break;
            }
          }
          hor += value;
        }
        break;
      
      case Dir::DOWN:
        if (left)
        {
          curr = Dir::RIGHT;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert, hor+i}]++;

            // Check for double
            if (locations[{vert, hor+i}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor+i;
              found_vert = vert;
              break;
            }
          }
          hor += value;
        }
        else
        {
          curr = Dir::LEFT;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert, hor-i}]++;

            // Check for double
            if (locations[{vert, hor-i}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor-i;
              found_vert = vert;
              break;
            }
          }
          hor -= value;
        }
        break;
      
      case Dir::LEFT:
        if (left)
        {
          curr = Dir::DOWN;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert-i, hor}]++;

            // Check for double
            if (locations[{vert-i, hor}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor;
              found_vert = vert-i;
              break;
            }
          }
          vert -= value;
        }
        else
        {
          curr = Dir::UP;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert+i, hor}]++;

            // Check for double
            if (locations[{vert+i, hor}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor;
              found_vert = vert+i;
              break;
            }
          }
          vert += value;
        }
        break;
      
      case Dir::RIGHT:
        if (left)
        {
          curr = Dir::UP;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert+i, hor}]++;

            // Check for double
            if (locations[{vert+i, hor}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor;
              found_vert = vert+i;
              break;
            }
          }
          vert += value;
        }
        else
        {
          curr = Dir::DOWN;
          for (int64_t i = 1; i <= value; ++i)
          {
            // add new locations to the map
            locations[{vert-i, hor}]++;

            // Check for double
            if (locations[{vert-i, hor}] > 1)
            {
              // we found our double spot
              found = true;
              found_hor = hor;
              found_vert = vert-i;
              break;
            }
          }
          vert -= value;
        }
        break;
      
      default:
      std::cout << "ERROR, MISSING DIRECTION!" << std::endl;
    }


    if (found) break;
  }

  std::cout << "First location visited twice: " << abs(found_hor) + abs(found_vert) << std::endl;

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