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
using Pairs = std::vector<std::pair<uint64_t,uint64_t>>;
using Pairs2 = std::vector<Pairs>;


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

  const Strings& input = split(rawInput[0], ",");
  Numbers nrs;
  for (const std::string& str : input)
  {
    nrs.push_back(std::stoull(str));
  }
  const Numbers nrsOriginal = nrs;

  // part a) replace idx 1 with 12 and idx 2 with 2
  // nrs[1] = 12;
  // nrs[2] = 2;

  // part b) now check every combination of nrs[1] and nrs[2] between 0 and 99.
  const uint64_t EXPECTED_NR = 19690720;
  bool found = false;
  for (uint64_t x = 0; x < 100; ++x)
  {
    for (uint64_t y = 0; y < 100; ++y)
    {
      nrs = nrsOriginal;
      nrs[1] = x;
      nrs[2] = y;
      for (uint64_t i = 0; i < nrs.size(); i = i + 4)
      {
        const uint64_t& code = std::stoull(input[i]);

        if (code == 1 || code == 2)
        {
          // Fetch elements at indices i+1, i+2 and i+3.
          const uint64_t& idx1 = nrs[i+1];
          const uint64_t& idx2 = nrs[i+2];
          const uint64_t& idxoutput = nrs[i+3];

          if (code == 1)
          {
            nrs[idxoutput] = nrs[idx1] + nrs[idx2];
          }
          else
          {
            nrs[idxoutput] = nrs[idx1] * nrs[idx2];
          }
        }
        else
        {
          // halt
          break;
        }
      }

      if (nrs[0] == EXPECTED_NR)
      {
        found = true;
        break;
      }

    }
    if (found) break;
  }

  std::cout << "Noun: " << nrs[1] << std::endl;
  std::cout << "Verb: " << nrs[2] << std::endl;
  std::cout << "Output: " << 100 * nrs[1] + nrs[2] << std::endl;

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