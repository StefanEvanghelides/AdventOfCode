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

uint64_t countBags(const std::string& bag, std::map<std::string, std::map<std::string, uint64_t>> bags)
{
  if (!bags.contains(bag) || bags[bag].size() < 1) return 1;

  uint64_t count = 1;
  for (const auto& [b,quant] : bags[bag])
  {
    count += quant * countBags(b, bags);
  }
  return std::max(1ULL, count);
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

  // Map <bag -> map <bag -> quantity>>
  std::map<std::string, std::map<std::string, uint64_t>> bags;

  const std::string MY_BAG = "shiny gold";

  // Process the bags in a structure.
  for (const std::string& line : rawInput)
  {
    const Strings& inputs = split(line, " contain ");
    if (inputs.size() != 2)
    {
      std::cout << "INPUTS DOES NOT HAVE 2 ELEMENTS! Inputs size=" << inputs.size() << std::endl;
      return;
    }

    const std::string& inputsFirst = inputs[0];
    const std::string& inputsSecond = inputs[1];

    // Process the current bag, it's the first input.
    Strings bag = split(inputsFirst, " ");
    if (!bag.empty()) bag.pop_back();
    if (bag.empty())
    {
      std::cout << "BAG IS NOW EMPTY! Line: " << line << std::endl;
      return;
    }
    std::string bagStr;
    for (uint64_t i = 0; i < bag.size(); ++i)
    {
      bagStr += bag[i];
      if (i < bag.size() - 1) bagStr += " ";
    }

    // Now check if there are bags inside this one.
    std::map<std::string, uint64_t> otherBagsMap;
    const Strings& otherBags = split(inputsSecond, ", ");
    for (const std::string& otherBag : otherBags)
    {
      Strings ob = split(otherBag, " ");
      if (ob.size() < 2)
      {
        std::cout << "WRONG OTHER BAG SECOND PART! LINE: " << line << std::endl;
        return;
      }

      if (ob.size() == 3 && otherBags.size() == 1 && ob[0] == "no")
      {
        // this will be then an empty bag, do nothing
        break;
      }

      // remove the "bags" word
      ob.pop_back();

      // Now the 1st element is the quantity, the rest is the bag
      if (ob.size() < 3)
      {
        std::cout << "WRONG OB! OtherBag: " << otherBag << std::endl;
        return;
      }

      const uint64_t quant = std::stoull(ob[0]);
      std::string obStr;
      for (uint64_t i = 1; i < ob.size(); ++i)
      {
        obStr += ob[i];
        if (i < ob.size() - 1) obStr += " ";
      }

      otherBagsMap[obStr] = quant;
    }

    bags[bagStr] = otherBagsMap;
  }

  std::cout << "Bag count dark olive: " << bags["faded blue"].size() << std::endl;
  std::cout << "Bag count vibrant plum: " << bags["dotted black"].size() << std::endl;

  // Now perform the counting of the bags.
  // std::cout << "Count bags: " << bags.size() << std::endl;
  // std::cout << "Count bags shiny: " << bags[MY_BAG].size() << std::endl; 
  std::cout << "Count: " << countBags(MY_BAG, bags) - 1 << std::endl;

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