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

uint64_t decompressLength(const std::string& input)
{
  uint64_t idx = 0;
  uint64_t counter = 0;
  while (idx < input.size())
  {
    // std::cout << "Initial idx=" << idx << " ;  initial counter=" << counter << std::endl;
    if (input[idx] == '(')
    {
      // Parse the marker
      uint64_t jdx = idx;
      while (input[jdx] != ')') jdx++;
      const uint64_t sizeIdx = jdx - idx + 1;
      idx++;
      jdx--;

      // std::cout <<"MARKER idx=" << idx << "  jdx=" << jdx << std::endl;

      // Now the marker is between idx and jdx. we can split it on X.
      const std::string chunk = input.substr(idx, sizeIdx);
      const std::vector<std::string> splitChunk = split(chunk, "x");
      if (splitChunk.size() < 2)
      {
        std::cout << "ISSUE! INCORRECT CHUNK!" << std::endl;
        idx = jdx;
      }
      const uint64_t nrChars = std::stoull(splitChunk[0]);
      const uint64_t repetitions = std::stoull(splitChunk[1]);

      // std::cout << " - nrChars=" << nrChars << " ; repetitions=" << repetitions << std::endl;
      // std::cout << "  --- before counter=" << counter << ";  idx=" << idx << std::endl;

      counter = counter + nrChars * repetitions;
      idx = jdx + 1 + nrChars;

      // std::cout << "  --- after counter=" << counter << " ; idx=" << idx << std::endl;
    }
    else
    {
      // Nothing, just increase idx and counter
      counter++;
    }
    idx++;
  }

  return counter;
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // Loop because of multiple test scenarios
  for (const std::string& input : rawInput)
  {
    std::cout << std::endl;
    std::cout << "Input: " << input << std::endl;
    std::cout << "Size: " << input.size() << std::endl;
    std::cout << "Total size: " << decompressLength(input) << std::endl;
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