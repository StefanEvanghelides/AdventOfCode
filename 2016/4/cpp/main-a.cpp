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

using Pair = std::pair<char, uint64_t>;

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  uint64_t sumVerified = 0;
  for (const std::string& line : rawInput)
  {
    const std::vector<std::string> inputs = split(line, " ");
    if (inputs.size() != 2)
    {
      std::cout << "Error in the inputs! inputs size: " << inputs.size() << std::endl;
      for (const std::string& input : inputs)
      {
        std::cout << " - input: " << input << std::endl;
      }
    }

    // Training part
    std::map<char, uint64_t> freq;
    uint64_t sectorID = 0;
    const std::string& training = inputs[0];
    for (const char& c : training)
    {
      if (isdigit(c)) sectorID = sectorID * 10 + c - '0';
      else freq[c]++;
    }

    // Now create sorted array based on the inputs
    std::vector<Pair> vec;
    for (const Pair& p : freq) vec.push_back(p);
    sort(vec.begin(), vec.end(), [](const Pair& a, const Pair&b)
    {
      return a.second > b.second || a.second == b.second && a.first < b.first;
    });

    // Verification part
    const std::string codeSum = inputs[1];
    if (vec.size() < codeSum.size())
    {
      std::cout << "ERROR, vec is smaller than codeSum!" << std::endl;
    }
    else
    {
      bool verify = true;
      for (uint64_t i = 0; i < codeSum.size(); ++i)
      {
        if (codeSum[i] != vec[i].first)
        {
          verify = false;
          break;
        }
      }
      if (verify) sumVerified += sectorID;
    }
  }

  std::cout << "Sum verified: " << sumVerified << std::endl;
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