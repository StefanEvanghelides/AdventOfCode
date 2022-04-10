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

struct Tree;
using SpTree = std::shared_ptr<Tree>;

struct Tree
{
  Tree() = default;
  ~Tree() = default;

  std::vector<SpTree> children;
  std::vector<uint64_t> metadata;

  void printTree(const uint64_t& depth = 0)
  {
    for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "Tree metadata size (" << metadata.size() << "): ";
    for (const auto& m : metadata) std::cout << m << " ";
    std::cout << std::endl;
    for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "Children (" << children.size() << "):" << std::endl;
    for (const auto& child : children)
      child->printTree(depth+1);
  }
};

SpTree parseInput(const Numbers& nrs, uint64_t& idx)
{
  SpTree root = std::make_shared<Tree>();

  if (idx >= nrs.size()) return root;

  // Fetch the first 2 elements
  const uint64_t nrChildren = nrs[idx];
  idx++;
  if (idx >= nrs.size()) return root;
  const uint64_t nrMetadata = nrs[idx];
  idx++;
  if (idx >= nrs.size()) return root;

  // Now parse the children
  for (uint64_t j = 0; j < nrChildren; ++j)
  {
    root->children.push_back(parseInput(nrs, idx));
    idx++;
  }

  // Only now at the end do we have the remaining metadata
  for (uint64_t j = 0; j < nrMetadata; ++j)
  {
    if (idx >= nrs.size())
    {
      std::cout << "STRING REACHED THE END! ERROR! idx=" << idx << "; j=" << j << "; size=" << nrs.size() << std::endl;
      break;
    }
    root->metadata.push_back(nrs[idx]);
    ++idx;
  }
  --idx;

  return root;
}

uint64_t sumMetadata(const SpTree& root)
{
  uint64_t sum = 0;
  for (const auto& meta : root->metadata)
    sum += meta;

  for (const auto& child : root->children)
    sum += sumMetadata(child);

  return sum;
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

  // We expect just one line of input
  const std::string& input = rawInput[0];
  const Strings& tokens = split(input, " ");
  Numbers nrs;
  for (const std::string& tok : tokens)
    nrs.push_back(std::stoull(tok));

  // Parse the tree
  uint64_t start = 0;
  SpTree root = parseInput(nrs, start);
  //root->printTree();

  // Now traverse the tree and sum the metadata.
  const uint64_t sum = sumMetadata(root);
  std::cout << "Sum: " << sum << std::endl;
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