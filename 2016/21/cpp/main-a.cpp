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

constexpr char INPUT_STRING[] {"abcdefgh"};

void applyCommand(const std::string& cmd, std::string& inp)
{
  // First tokenize the command
  const Strings& tokens = split(cmd, " ");
  if (tokens.size() < 1)
  {
    std::cout << "Incorrect CMD line: " << cmd << std::endl;
    return;
  }

  if (tokens[0][0] == 's')
  {
    // swap command, find which one.
    if (tokens.size() < 6)
    {
      std::cout << "Wrong swap line! cmd: " << cmd << std::endl;
      return;
    }
    if (tokens[1][0] == 'p')
    {
      // positional swap.
      const uint_fast32_t idx1 = std::stoul(tokens[2]);
      const uint_fast32_t idx2 = std::stoul(tokens[5]);
      std::swap(inp[idx1], inp[idx2]);
    }
    else if (tokens[1][0] == 'l')
    {
      // letter swap.
      const char l1 = tokens[2][0];
      const char l2 = tokens[5][0];
      for (char& c : inp)
      {
        if (c == l1) c = l2;
        else if (c == l2) c = l1;
      }
    }
    else
    {
      std::cout << "Invalid swap command!" << std::endl;
    }
  }
  else if (tokens[0][0] == 'm')
  {
    // move command.
    if (tokens.size() < 6)
    {
      std::cout << "Wrong move line! cmd: " << cmd << std::endl;
      return;
    }
    const uint_fast32_t idx1 = std::stoul(tokens[2]);
    const uint_fast32_t idx2 = std::stoul(tokens[5]);
    const std::string c { inp[idx1] };
    inp.erase(idx1, 1);
    if (idx2 == inp.size()) inp += c;
    else inp.insert(idx2, c);
  }
  else if (tokens[0][0] == 'r')
  {
    // rotate or reverse command.
    if (tokens[0][1] == 'e')
    {
      // reverse command
      if (tokens.size() < 5)
      {
        std::cout << "Wrong reverse line! cmd: " << cmd << std::endl;
        return;
      }
      const uint_fast32_t idx1 = std::stoul(tokens[2]);
      const uint_fast32_t idx2 = std::stoul(tokens[4]);
      const std::string start = inp.substr(0, idx1);
      const std::string end = inp.substr(idx2+1);
      std::string mid = inp.substr(idx1, idx2-idx1+1);
      std::reverse(mid.begin(), mid.end());
      inp = start + mid + end;
    }
    else if (tokens[0][1] == 'o')
    {
      // rotate command
      if (tokens.size() < 4)
      {
        std::cout << "There should be at least 4 elements in a rotate command! cmd: " << cmd << std::endl;
        return;
      }
      if (tokens[1][0] == 'b')
      {
        // rotate "based on" command. This one has 7 tokens.
        if (tokens.size() < 7)
        {
          std::cout << "Incorrect rotate based on command! cmd: " << cmd << std::endl;
          return;
        }
        const char c = tokens[6][0];
        auto idx_it = inp.find(c);
        uint_fast16_t steps = idx_it != std::string::npos ? idx_it : 0;
        uint_fast16_t extraSteps = 1 + (idx_it > 3);
        std::string newCmd = "rotate right " + std::to_string(steps) + " steps";
        applyCommand(newCmd, inp);
        newCmd = "rotate right " + std::to_string(extraSteps) + " steps";
        applyCommand(newCmd, inp);
      }
      else
      {
        const uint_fast32_t steps = std::stoul(tokens[2]);
        if (tokens[1][0] == 'l')
        {
          // rotate left
          std::rotate(inp.begin(), inp.begin()+steps, inp.end());
        }
        else if (tokens[1][0] == 'r')
        {
          // rotate right
          std::rotate(inp.begin(), inp.begin()+inp.size()-steps, inp.end());
        }
        else
        {
          std::cout << "Invalid rotation command! cmd: " << cmd << std::endl;
        }
      }
    }
    else
    {
      std::cout << "Incorrect rotate command! cmd : " << cmd << std::endl;
    }
  }
  else
  {
    std::cout << "Invalid command! cmd: " << cmd << std::endl;
  }
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::string inp { INPUT_STRING };
  for (const std::string& cmd : rawInput)
  {
    applyCommand(cmd, inp);
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