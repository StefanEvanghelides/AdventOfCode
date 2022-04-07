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
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;


uint64_t computePolymerSize(const std::unordered_map<std::string, int64_t>& polymer)
{
  std::vector<uint64_t> hist('Z'-'A'+1, 0);
  for (const auto& rulePair : polymer)
  {
    // Take just the first char of any pair.
    const char& c = rulePair.first[1];
    hist[c-'A'] += rulePair.second;
  }
  uint64_t size = 0;
  for (const uint64_t& nr : hist)
  {
    size += nr;
  }

  return size;
}

void printPolymer(const std::unordered_map<std::string, int64_t>& polymer)
{
  for (const auto& p : polymer)
  {
    if (p.second > 0)
    {
      std::cout << "{" << p.first << " - " << p.second << "}" << std::endl << std::flush;
    }
  }
}

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

  const std::string& startingPolymer = rawInput[0];
  std::unordered_map<std::string, char> rules;
  for (uint64_t i = 1; i < rawInput.size(); ++i)
  {
    const std::string& line = rawInput[i];
    const std::vector<std::string>& tokens = split(line, " -> ");
    const std::string& rule = tokens[0];
    const char& ch = tokens[1][0];
    rules[rule] = ch;
  }

  // Construct the polymer map from the polymer string
  std::unordered_map<std::string, int64_t> polymer;
  for (uint64_t i = 1; i < startingPolymer.size(); ++i)
  {
    const std::string& str = startingPolymer.substr(i-1, 2);
    polymer[str]++;
  }

  std::cout << "Initial polymer" << std::endl;
  printPolymer(polymer);

  constexpr uint64_t MAX_ITERATIONS { 40 };
  uint64_t iter = 0;
  while (iter < MAX_ITERATIONS)
  {
    iter++;
    std::unordered_map<std::string, int64_t> newPolymer = polymer;
    for (const auto& rulePair : rules)
    {
      const std::string& rule = rulePair.first;
      const char& ruleChar = rulePair.second;

      if (polymer.contains(rule) && polymer[rule] > 0)
      {
        // New generate the new 2 rules and increase them by the existing rule.
        const std::string rule1 = { rule[0], ruleChar };
        const std::string rule2 = { ruleChar, rule[1] };
        newPolymer[rule1] += polymer[rule];
        newPolymer[rule2] += polymer[rule];

        // Reset the existing rule to 0.
        newPolymer[rule] -= polymer[rule];

        // std::cout << "rule=" << rule << " -> rule1=" << rule1 << " - rule2=" << rule2 << std::endl;
      }
    }
    polymer = newPolymer;

    const uint64_t polymerSize = computePolymerSize(polymer);

    std::cout << "After iteration=" << iter << " polymersize=" << polymerSize << std::endl;
    // printPolymer(polymer);

  }

  // printPolymer(polymer);

  // Compute a histogram
  std::vector<uint64_t> hist('Z'-'A'+1, 0);
  for (const auto& rulePair : polymer)
  {
    // Take just the second char of any pair.
    const char& c = rulePair.first[1];
    hist[c-'A'] += rulePair.second;
  }
  // Now don't forget to append the first char
  const char& c = startingPolymer[0];
  hist[c-'A']++;

  // We don't need to remember the letters, hence we can sort this hist for faster computations.
  std::sort(hist.begin(), hist.end());

  const uint64_t maxElement = hist.back();
  const uint64_t minElement = hist[std::lower_bound(hist.begin(), hist.end(), 1) - hist.begin()];
  std::cout << "MaxElement=" << maxElement << "  MinElement=" << minElement << "  Diff=" << (maxElement - minElement) << std::endl << std::flush;

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