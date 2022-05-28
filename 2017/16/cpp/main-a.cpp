#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <cmath>
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
using Numbers = std::vector<int_fast32_t>;
using Numbers2 = std::vector<Numbers>;
using Numbers3 = std::vector<Numbers2>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Pairs = std::vector<std::pair<int64_t,int64_t>>;
using Pairs2 = std::vector<Pairs>;
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

constexpr char INPUT_STRING[] {"abcdefghijklmnop"};

void applyCommand(const std::string& cmd, std::string& inp)
{
  // First tokenize the command
  if (cmd.size() < 1)
  {
    std::cout << "Empty command line!" << std::endl;
    return;
  }

  const std::string rem = cmd.substr(1);
  if (cmd[0] == 's')
  {
    // Spin command: perhaps rotation to the right.
    const uint_fast32_t steps = std::stoul(rem);
    std::rotate(inp.begin(), inp.begin()+inp.size()-steps, inp.end());
  }
  else if (cmd[0] == 'x')
  {
    // Swap at positions.
    const Strings& tokens = split(rem, "/");
    if (tokens.size() != 2)
    {
      std::cout << "Incorrect tokens size: " << tokens.size() << std::endl;
      return;
    }
    const uint_fast32_t idx1 = std::stoul(tokens[0]);
    const uint_fast32_t idx2 = std::stoul(tokens[1]);
    std::swap(inp[idx1], inp[idx2]);

  }
  else if (cmd[0] == 'p')
  {
    const char c1 = cmd[1];
    const char c2 = cmd[3];
    for (uint_fast32_t i = 0; i < inp.size(); ++i)
    {
      char& c = inp[i];
      if (c == c1) c = c2;
      else if (c == c2) c = c1;
    }
  }
  else
  {
    std::cout << "Incorrect cmd! cmd: " << cmd << std::endl;
  }

}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::string inp { INPUT_STRING };
  for (const std::string& rawinp : rawInput)
  {
    const Strings& commands = split(rawinp, ",");
    for (const std::string& cmd : commands)
    {
      applyCommand(cmd, inp);
    }
  }
  std::cout << "Final string: " << inp << std::endl;

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