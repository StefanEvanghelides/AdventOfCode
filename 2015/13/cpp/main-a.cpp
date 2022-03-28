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

using Arrangements = std::map<std::string, std::map<std::string, int64_t>>;

Numbers2 permutations(const Numbers& start)
{
  // Generate all permutations. Python has a built-in itertools.permutations().
  // For C++, we need to use std::next_permutation
  Numbers2 perms;
  Numbers lst = start;
  std::sort(lst.begin(), lst.end());
  do
  {
    perms.push_back(lst);
  }
  while (std::next_permutation(lst.begin(),lst.end()));
  return perms;
}

int64_t computeHappiness(const Arrangements& arrangements,
        const std::map<uint64_t, std::string>& indices, const Numbers& perm)
{
  int64_t happiness = 0;

  for (uint64_t i = 0; i < perm.size(); ++i)
  {
    const uint64_t& p = perm[i];
    const uint64_t& leftIdx = (i > 0 ? perm[i-1] : perm[perm.size()-1]);
    const uint64_t& rightIdx = (i < perm.size() - 1 ? perm[i+1] : perm[0]);

    const std::string& pers = indices.at(p);
    const std::string& left = indices.at(leftIdx);
    const std::string& right = indices.at(rightIdx);

    // Find the happiness of hte current person for the 2 neighbours.
    happiness += arrangements.at(pers).at(left);
    happiness += arrangements.at(pers).at(right);
  }

  return happiness;
}

int64_t optimalArrangement(const Arrangements& arrangements)
{
  // Basically find all combinations and see which is the best.

  // Give index numbers to the persons.
  std::map<uint64_t, std::string> indices;
  uint64_t idx = 0;
  for (const auto& [key,_] : arrangements)
  {
    indices[idx] = key;
    idx++;
  }

  Numbers start(idx);
  std::iota(start.begin(), start.end(), 0);

  int64_t optimal = 0;
  for (const Numbers& perm : permutations(start))
  {
    // std::cout << "Permutation: ";
    // for (const auto& x : perm) std::cout << x << "  ";
    // std::cout << std::endl;

    const int64_t& happiness = computeHappiness(arrangements, indices, perm);
    if (happiness > optimal) optimal = happiness;
  }
  
  return optimal;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  Arrangements arrangements;
  for (const std::string& inp : rawInput)
  {
    const Strings& tokens = split(inp, " ");
    // Guard: There should always be 11 elements.
    if (tokens.size() != 11)
    {
      std::cout << "There are not 11 elements, but " << tokens.size() << std::endl;
      continue;
    }

    // Determine the sign.
    const int64_t sign = (tokens[2][0] == 'g') - (tokens[2][0] == 'l');

    // Now fetch the numbers.
    const std::string& currentPerson = tokens[0];
    const std::string otherPerson = split(tokens[10], ".")[0];
    const int64_t& unit = std::stoll(tokens[3]);

    // Add the numbers to the arrangement.
    if (!arrangements.contains(currentPerson))
      arrangements[currentPerson] = {};

    arrangements[currentPerson][otherPerson] = unit * sign;
  }

  // // Show the arrangements.
  // std::cout << "Show arrangements:" << std::endl;
  // for (const auto& [key,val] : arrangements)
  // {
  //   std::cout << key << ": ";
  //   for (const auto& [k,v] : val)
  //   {
  //     std::cout << "(" << k << ":" << v << ")  ";
  //   }
  //   std::cout << std::endl;
  // }

  // Now search for the optimal arrangement.
  const int64_t& optimal = optimalArrangement(arrangements);
  std::cout << "Optimal arrangement happiness: " << optimal << std::endl;

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