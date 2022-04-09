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

bool conditionMet(const int64_t& regVal, const std::string& cond, const int64_t& condVal)
{
  if (cond[0] == '<')
  {
    if (cond == "<=")
    {
      return regVal <= condVal;
    }
    else
    {
      return regVal < condVal;
    }
  }
  else if (cond[0] == '>')
  {
    if (cond == ">=")
    {
      return regVal >= condVal;
    }
    else
    {
      return regVal > condVal;
    }
  }
  else if (cond == "==")
  {
    return regVal == condVal;
  }
  else if (cond == "!=")
  {
    return regVal != condVal;
  }
  
  // We should not reach here
  std::cout << "Condition invalid! cond: " << cond << std::endl;
  return false;
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

  // tokenize the inputs and fetch the registers.
  Strings2 inputs;
  std::map<std::string, int64_t> registers;
  for (const std::string& input : rawInput)
  {
    // split each line into tokens, keep only the first one.
    const Strings& tokens = split(input, " ");
    if (tokens.size() != 7)
    {
      std::cout << "WRONG INPUT LINE: " << input << std::endl;
      continue;
    }
    inputs.push_back(tokens);
    const std::string& reg = tokens[0];
    registers[reg] = 0;
  }

  // Now we can parse the inputs and check the values.
  // Also keep track of the highest value ever tracked.
  int64_t maxVal = -1e10;
  for (const Strings& tokens : inputs)
  {
    const std::string& reg = tokens[0];
    const int64_t sign = (tokens[1][0] == 'i') - (tokens[1][0] == 'd');
    const int64_t& val = std::stoll(tokens[2]);
    const std::string& condReg = tokens[4];
    const std::string& cond = tokens[5];
    const int64_t& condVal = std::stoll(tokens[6]);

    // First check the condition
    if (conditionMet(registers[condReg], cond, condVal))
    {
      registers[reg] += sign * val;
      if (registers[reg] > maxVal) maxVal = registers[reg];
    }
  }

  std::cout << "Maximum value: " << maxVal << std::endl;

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
    // if (!trimmedLine.empty() && trimmedLine != "" && trimmedLine != "\n")
    // {
      rawInput.emplace_back(trimmedLine);
    // }
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