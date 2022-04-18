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

void print(const std::map<std::string, std::pair<uint64_t, Strings>>& towers)
{
  for (const auto& [key,value] : towers)
  {
    std::cout << "Base: " << key << ";  weight: " << value.first << ";  children size: " << value.second.size() << std::endl;
    if (value.second.size() > 0)
    {
      std::cout << " - ";
      for (const auto& child : value.second)
        std::cout << child << "  ";
      std::cout << std::endl;
    }
  }
}

// Some quick cache
std::unordered_map<std::string, uint64_t> cache;

constexpr uint64_t MAX_ITERATIONS = 1000;

uint64_t computeWeight(std::map<std::string, std::pair<uint64_t, Strings>> towers, const std::string& root)
{
  // Check if the value was already cached
  if (cache.contains(root))
    return cache[root];

  // Value not cached. Compute it and add it to the cache
  uint64_t weight = towers[root].first;
  const Strings& children = towers[root].second;
  if (children.size() > 0)
  {
    for (const std::string& child : children)
    {
      weight += computeWeight(towers, child);
    }
  }

  cache[root] = weight;

  return weight;
}

std::map<std::string, std::pair<uint64_t, std::pair<uint64_t, Strings>>>
computeTotalWeights(const std::map<std::string, std::pair<uint64_t, Strings>>& towers, const std::string& root)
{
  std::map<std::string, std::pair<uint64_t, std::pair<uint64_t, Strings>>> fullWeightTowers;

  for (const auto& [base,values] : towers)
  {
    const uint64_t& weight = values.first;
    const uint64_t& totalWeight = computeWeight(towers, base);
    const Strings& children = values.second;
    // Include the current tower's weight in the total.
    fullWeightTowers[base] = {totalWeight, {weight, children}};
  }

  return fullWeightTowers;
}

uint64_t findCorrectWeight(const std::map<std::string, std::pair<uint64_t, Strings>>& towers, const std::string& root)
{
  // We need to find the total weights of all (sub)towers.
  // Structure <base, <totalWeight, <weight,children>>>
  std::map<std::string, std::pair<uint64_t, std::pair<uint64_t, Strings>>> fullWeightTowers = computeTotalWeights(towers, root);

  // We know there are at least 3 children on the root tower.
  // This means that we can directly find the difference between them, also the sign.
  Strings children = fullWeightTowers[root].second.second;
  if (children.size() < 3)
  {
    std::cout << "Root node does not have at least 3 children!" << std::endl;
  }
  const uint64_t& childWeight0 = fullWeightTowers[children[0]].first;
  int64_t weightSame = 0;
  int64_t delta = 0;
  std::string oddChild = children[0];
  std::cout << "root child[0]: " << children[0] << ";  weight: " << childWeight0 << std::endl;
  for (uint64_t i = 1; i < children.size(); ++i)
  {
    uint64_t childWeight = fullWeightTowers[children[i]].first;
    std::cout << "root child[" << i << "]: " << children[i] << ";  weight: " << childWeight << std::endl;
    if (childWeight0 == childWeight || i > 1 && 
      fullWeightTowers[children[i]].first == fullWeightTowers[children[i-1]].first)
    {
      weightSame = childWeight;
    }
    if (weightSame > 0)
    {
      // Now find the odd child and delta.
      if (i < 2)
      {
        // The first 2 children had the same weight. Now look for the odd one out.
        for (uint64_t j = 2; j < children.size(); ++j)
        {
          childWeight = fullWeightTowers[children[j]].first;
          if (childWeight != weightSame)
          {
            // Found it!
            oddChild = children[j];
            delta = childWeight - weightSame;
            break;
          }
        }
      }
      else
      {
        // The odd child is part of what we've seen already.
        // Check backwards now.
        for (uint64_t j = 0; j <= i; ++j)
        {
          childWeight = fullWeightTowers[children[j]].first;
          if (childWeight != weightSame)
          {
            // Found it!
            oddChild = children[j];
            delta = childWeight - weightSame;
            break;
          }
        }
      }
      break;
    }
  }
  if (weightSame == 0)
  {
    // Now value found, then the first child is the odd one out.
    // Use then the 2nd child. The delta value is already correct.
    weightSame = fullWeightTowers[children[1]].first;
  }
  if (delta == 0)
  {
    // This is a huge mistake, it means there's nothing to correct
    std::cout << "NOTHING TO CORRECT!" << std::endl;
    return 0;
  }
  // // Now find the odd child
  // for (const std::string& child : children)
  // {
  //   if (fullWeightTowers[child].first != weightSame)
  //   {
  //     oddChild = child;
  //     break;
  //   }
  // }
  // if (oddChild.empty())
  // {
  //   // If there's no odd child, then this is another huge mistake.
  //   std::cout << "NO ODD CHILD DETECTED!" << std::endl;
  //   return 0;
  // }

  std::cout << "Weight same: " << weightSame << std::endl;
  std::cout << "delta: " << delta << std::endl;
  std::cout << "oddChild: " << oddChild << ";  weight: " << weightSame + delta << std::endl;
  
  // Now we want to go deeper into the children's of the odd tower to fix it.
  uint64_t iter = 0;
  while(iter < MAX_ITERATIONS)
  {
    std::cout << " - iter: " << iter << std::endl;

    // Check if the tower has any children.
    children = fullWeightTowers[oddChild].second.second;
    std::cout << "children size: " << children.size() << std::endl;
    if (children.size() < 2)
    {
      // No children, then simply update the towers weight.
      return uint64_t(int64_t(fullWeightTowers[oddChild].first) - delta);
    }

    // Check if the children are equal: if they are, then
    // simply correct the tower's weight.
    bool areEqual = true;
    const uint64_t& childWeight0 = fullWeightTowers[children[0]].first;
    std::cout << "child0: " << children[0] << ";  childweight0: " << childWeight0 << std::endl;
    uint64_t oddChildWeight = 0;
    for (uint64_t i = 1; i < children.size(); ++i)
    {
      const uint64_t& childWeight = fullWeightTowers[children[i]].first;
      std::cout << "child[" << i << "=" << children[i] << "] weight: " << childWeight << std::endl;
      if (childWeight != childWeight0)
      {
        areEqual = false;
        // We can also check here which should be the next oddChild
        oddChild = childWeight == childWeight0 + delta ? children[i] : children[0];
        oddChildWeight = childWeight == childWeight0 + delta ? childWeight : childWeight0;
        break;
      }
    }
    std::cout << "areEqual: " << areEqual << ";  oddChild: " << oddChild << ";  weight: " << oddChildWeight << std::endl;
    if (areEqual)
    {
      return uint64_t(int64_t(fullWeightTowers[oddChild].first) - int64_t(children.size() * childWeight0) - delta);
    }

    // At this point it means that there is a difference in the children's weight.
    // We already have the new odd child, so just move on to the next iteration.
    iter++;
  }

  // We should technically not reach this.
  std::cout << "Strange, we reached the end! Iterations: " << iter << std::endl;
  return 0;
}


void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::map<std::string, std::pair<uint64_t, Strings>> towers;
  for (const std::string& input : rawInput)
  {
    const Strings& parts = split(input, " -> ");
    if (parts.size() < 1 || parts.size() > 2)
    {
      std::cout << "WRONG INPUT LINE! Line: " << input << std::endl;
      continue;
    }

    // First, get the base tower and the weight
    const Strings& baseWeight = split(parts[0], " ");
    const std::string& base = baseWeight[0];
    const uint64_t& weight = std::stoull(baseWeight[1].substr(1,baseWeight[1].size()-2));

    // If there are 2 parts, then get the children
    Strings children;
    if (parts.size() > 1)
    {
      children = split(parts[1], ", ");
    }
    towers[base] = {weight, children};
  }

  // print(towers);

  // To get the name of the base tower, simply check which children are not being named.
  std::set<std::string> bottoms;
  for (const auto& [base,_] : towers)
  {
    bottoms.insert(base);
  }
  for (const auto& [base,values] : towers)
  {
    if (values.second.size() > 0)
    {
      for (const std::string& child : values.second)
      {
        bottoms.erase(child);
      }
    }
    if (bottoms.size() == 1) break;
  }

  // Now show the bottom tower
  const std::string& root = *bottoms.begin();
  std::cout << "Bottom tower: " << root << std::endl;

  // For part b) there is only 1 tower off-balance: find it
  const uint64_t& correctWeight = findCorrectWeight(towers, root);
  std::cout << "Correct weight: " << correctWeight << std::endl;
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