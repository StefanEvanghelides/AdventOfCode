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
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
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

constexpr uint64_t MAX_ITERATIONS = 1000000;

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  int64_t regs[4] = {0};
  uint64_t iter = 0;
  for (uint64_t i = 0; i < rawInput.size() && iter < MAX_ITERATIONS; ++i, ++iter)
  {
    const std::string& inp = rawInput[i];
    if (inp.size() < 1) continue;
    const Strings& tokens = split(inp, " ");
    switch (inp[0])
    {
      case 'c':
      {
        // Copy assignment. Expect 3 tokens.
        if (tokens.size() != 3)
        {
          std::cout << "INCORRECT COPY COMMAND: " << inp;
        }
        else
        {
          const int64_t& reg = tokens[2][0] - 'a';
          int64_t reg_val = 0;
          if (tokens[1][0] >= 'a') reg_val = regs[tokens[1][0] - 'a'];
          else reg_val = std::stoll(tokens[1]);
          regs[reg] = reg_val;
        }
      }
      break;
      
      case 'i':
      {
        // Increment. Expect 2 tokens.
        if (tokens.size() != 2)
        {
          std::cout << "INCORRECT INC COMMAND: " << inp << std::endl;
        }
        else
        {
          regs[tokens[1][0] - 'a']++;
        }
      }
      break;
      
      case 'd':
      {
        // Increment. Expect 2 tokens.
        if (tokens.size() != 2)
        {
          std::cout << "INCORRECT DEC COMMAND: " << inp << std::endl;
        }
        else
        {
          regs[tokens[1][0] - 'a']--;
        }
      }
      break;
      
      case 'j':
      {
        // Jump non-zero. Expect 3 tokens.
        if (tokens.size() != 3)
        {
          std::cout << "INCORRECT JNZ COMMAND: " << inp << std::endl;
        }
        else
        {
          int64_t reg_val = 0;
          if (tokens[1][0] >= 'a') reg_val = regs[tokens[1][0] - 'a'];
          else reg_val = std::stoll(tokens[1]);
          if (reg_val != 0)
          {
            const int64_t& val = std::stoll(tokens[2]);
            i += val - 1;
          }
        }
      }
      break;

      default:
      {
        std::cout << "Incorrect command instruction: " << inp << std::endl;
      }
    }
  }

  std::cout << "Value in registers: a=" << regs[0] << "  b=" << regs[1] << "  c=" << regs[2] << "  d=" << regs[3] << std::endl;

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