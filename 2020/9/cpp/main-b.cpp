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
using Numbers = std::vector<int64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Bools3 = std::vector<Bools2>;
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


constexpr uint64_t PREAMBLE = 25;

Numbers transformInputs(const Strings& rawInput)
{
  Numbers nrs;
  for (const std::string& str : rawInput) nrs.push_back(std::stoll(str));
  return nrs;
}

bool checkNumber(const Numbers& nrs, const int64_t& idx)
{
  if (idx < PREAMBLE || idx >= nrs.size()) return false;

  for (int64_t i = idx-PREAMBLE; i < idx-1; ++i)
  {
    for (int64_t j = i + 1; j < idx; ++j)
    {
      if (nrs[i] + nrs[j] == nrs[idx]) return true;
    }
  }

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

  Numbers inputs = transformInputs(rawInput);

  // Now check the numbers
  int64_t invalidNr = 0, invalidIdx = 0;
  for (uint64_t i = PREAMBLE; i < rawInput.size(); ++i)
  {
    const int64_t nr = std::stoll(rawInput[i]);

    // Now check the number
    if (!checkNumber(inputs, i))
    {
      std::cout << "Number does not check: " << nr << std::endl;
      invalidNr = inputs[i];
      invalidIdx = i;
      break;
    }
  }

  // Now find a contiguous list.
  // Strategy: if amount is less, add to the right;
  //           if amount is higher, subtract left.
  int64_t idx1 = 0, idx2 = 1;
  int64_t sum = inputs[idx1] + inputs[idx2];
  while (idx2 < invalidIdx && sum != invalidNr)
  {
    if (sum < invalidNr)
    {
      idx2++;
      sum += inputs[idx2];
    }
    else
    {
      sum -= inputs[idx1];
      idx1++;
    }
  }

  // std::cout << "Equal: " << (sum == invalidNr ? "YES" : "no") << ";  idx1=" << idx1 << ";  idx2=" << idx2 << std::endl;

  int64_t low = 1e10, high = -1;
  for (int64_t i = idx1; i <= idx2; ++i)
  {
    if (inputs[i] < low) low = inputs[i];
    if (inputs[i] > high) high = inputs[i];
  }
  std::cout << "low-high sum: " << low + high << std::endl;

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