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
#include <optional>
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


struct Rule
{
  uint64_t botIdx;
  bool botOutputLow;
  bool botOutputHigh;
  uint64_t low;
  uint64_t high;

  Rule()
    : botIdx(0), botOutputLow(false), botOutputHigh(high), low(0), high(0)
  {}

  Rule(const uint64_t& i, const bool& bol, const bool& boh, const uint64_t& l, const uint64_t& h)
    : botIdx(i), botOutputLow(bol), botOutputHigh(boh), low(l), high(h)
  {}
};


using Bots = std::map<uint64_t, Numbers>;
using Rules = std::vector<Rule>;
using Bins = std::map<uint64_t, uint64_t>;

constexpr uint64_t LOW_VALUE = 17;
constexpr uint64_t HIGH_VALUE = 61;
constexpr uint64_t MAX_ITERATIONS = 1000;


std::tuple<Bots, Rules> processInput (const Strings& input)
{
  Bots bots;
  Rules rules;
  for (const std::string& inp : input)
  {
    // Tokenize input and verify data.
    const Strings& tokens = split(inp, " ");
    if (inp.size() < 1 or tokens.size() < 1) continue;

    if (inp[0] == 'v' && tokens.size() == 6)
    {
      // Add value to a bot.
      const uint64_t& value = std::stoull(tokens[1]);
      const uint64_t botIdx = std::stoull(tokens[5]);
      if (!bots.contains(botIdx)) bots[botIdx] = Numbers();
      bots[botIdx].push_back(value);
    }
    else if (inp[0] == 'b' && tokens.size() == 12)
    {
      // This is a transfer rule.
      const uint64_t& botIdx = std::stoull(tokens[1]);
      // True for bot, False for output bin.
      const bool& botOutputLow = tokens[5][0] == 'b';
      const bool& botOutputHigh = tokens[10][0] == 'b';
      const uint64_t& low = std::stoull(tokens[6]);
      const uint64_t& high = std::stoull(tokens[11]);
      if (!bots.contains(botIdx)) bots[botIdx] = Numbers();
      if (botOutputLow && !bots.contains(low)) bots[low] = Numbers();
      if (botOutputHigh && !bots.contains(high)) bots[high] = Numbers();
      rules.emplace_back(botIdx, botOutputLow, botOutputHigh, low, high);
    }
  }
  return {bots, rules};
}


std::optional<uint64_t> checkBots(const Bots& bots, const uint64_t& low = LOW_VALUE, const uint64_t& high = HIGH_VALUE)
{
  for (const auto& [botIdx, values] : bots)
  {
    if (values.size() == 2)
    {
      if (values[0] == low && values[1] == high ||
          values[0] == high && values[1] == low)
      {
        return {botIdx};
      }
    }
  }
  return {};
}

bool applyRule(const Rule& rule, Bots& bots, Bins& bins)
{
  const uint64_t& botIdx = rule.botIdx;
  if (bots[botIdx].size() < 2) return false;

  const uint64_t& lowValue = (bots[botIdx][0] < bots[botIdx][1] ? bots[botIdx][0] : bots[botIdx][1]);
  const uint64_t& highValue = (bots[botIdx][0] > bots[botIdx][1] ? bots[botIdx][0] : bots[botIdx][1]);

  bots[botIdx].clear();

  // Now add the current values to the other bots, if necessary.
  if (rule.botOutputLow) bots[rule.low].push_back(lowValue);
  else bins[rule.low] = lowValue;

  if (rule.botOutputHigh) bots[rule.high].push_back(highValue);
  else bins[rule.high] = highValue;

  return true;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  auto [bots, rules] = processInput(rawInput);

  std::cout << "Bots size: " << bots.size() << std::endl;
  std::cout << "Rules size: " << rules.size() << std::endl;

  // First, check if a bot already has the desired values.
  auto botOption = checkBots(bots);
  if (botOption.has_value())
  {
    // Lucky case! We already got a value!
    std::cout << "Bot: " << botOption.value() << std::endl;
    return;
  }

  // Hard case: Need to work every rule and check the state.
  uint64_t iter = 0;
  Bins bins;
  while (iter++ < MAX_ITERATIONS)
  {
    bool rulesApplied = false;
    for (const Rule& rule : rules)
    {
      // Apply rule, then check again.
      if (applyRule(rule, bots, bins)) rulesApplied = true;
    }
    if (!rulesApplied) break;
  }

  if (iter == MAX_ITERATIONS) std::cout << "NO BOT FOUND!!" << std::endl;
  std::cout << "Output bins 0, 1, 2 product: " << bins[0] * bins[1] * bins[2] << std::endl;

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