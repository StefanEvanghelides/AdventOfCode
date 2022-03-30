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


std::string transformLine(const std::string& line)
{
  std::string newLine;

  for (const char& c : line)
  {
    if (c == ':' || c == ',') continue;
    newLine.push_back(c);
  }

  return newLine;
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

  std::map<std::string, uint64_t> REQUIREMENTS {
    {"children", 3},
    {"cats", 7},
    {"samoyeds", 2},
    {"pomeranians", 3},
    {"akitas", 0},
    {"vizslas", 0},
    {"goldfish", 5},
    {"trees", 3},
    {"cars", 2},
    {"perfumes", 1},
  };

  uint64_t maxNumberOfMatches = 0;
  uint64_t maxAuntIdx = 0;
  for (const std::string& inp : rawInput)
  {
    // Parse input. We need to transform each line.
    const std::string& input = transformLine(inp);

    // The input consists only of words (or numbers) and spaces now, split them.
    const Strings& tokens = split(input, " ");
    if (tokens.size() < 6)
    {
      std::cout << "There are not enough tokens! Skipping..." << std::endl;
      continue;
    }

    // Now fetch the values and check how many genes match.
    uint64_t countGenes = 0;

    // The aunt index is always the second one.
    const uint64_t& auntIdx = std::stoull(tokens[1]);

    // Now we can check the genes.
    for (uint64_t i = 2; i < tokens.size(); i += 2)
    {
      const std::string& gene = tokens[i];
      if (!REQUIREMENTS.contains(gene)) continue;

      if (i+1 < tokens.size())
      {
        const uint64_t& quant = std::stoull(tokens[i+1]);
        if (gene == "cats" || gene == "trees")
        {
          if (REQUIREMENTS[gene] < quant) countGenes++;
        }
        else if (gene == "pomeranians" || gene == "goldfish")
        {
          if (REQUIREMENTS[gene] > quant) countGenes++;
        }
        else
        {
          if (REQUIREMENTS[gene] == quant) countGenes++;
        }
      }
    }

    if (countGenes >= maxNumberOfMatches)
    {
      maxNumberOfMatches = countGenes;
      maxAuntIdx = auntIdx;
    }
  }

  std::cout << "Aunt with most genes: " << maxAuntIdx << " (genes: " << maxNumberOfMatches << ")" << std::endl;
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