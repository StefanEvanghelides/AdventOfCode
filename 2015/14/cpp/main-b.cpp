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

std::string nextString(const std::string& pass)
{
  std::string newPass = pass;

  for (uint64_t i = newPass.size()-1; i >= 0; --i)
  {
    newPass[i]++;
    // If this is still within boundaries, then we're done
    if (newPass[i] >= 'a' && newPass[i] <= 'z')
    {
      break;
    }
    newPass[i] = 'a';
  }

  return newPass;
}

Strings2 transformInput(const Strings& rawInput)
{
  Strings2 inputs;

  for (std::string inp : rawInput)
  {
    const Strings& tokens = split(inp, " ");
    inputs.emplace_back();
    Strings& lastInput = inputs.back();

    // We care about only 3 tokens: speed, duration, reset timer.
    lastInput.push_back(tokens[3]);
    lastInput.push_back(tokens[6]);
    lastInput.push_back(tokens[13]);
  }

  return inputs;
}

class Reindeer
{
public:
  Reindeer() = default;
  Reindeer(const uint64_t& s, const uint64_t& d, const uint64_t& r)
    : speed(s), duration(d), rest(r), remainingDuration(d),
      remainingRest(r),run(true), distance(0), points(0)
  {}
  ~Reindeer() = default;

  void nextMove()
  {
    if (run)
    {
      distance += speed;
      if (remainingDuration == 1)
      {
        run = false;
        remainingDuration = duration;
      }
      else remainingDuration--;
    }
    else
    {
      if (remainingRest == 1)
      {
        run = true;
        remainingRest = rest;
      }
      else remainingRest--;
    }
  }

  uint64_t getDistance() const { return distance; }
  uint64_t getPoints() const { return points; }

  void incrementPoints() { points++; }

private:
  uint64_t speed;
  uint64_t duration;
  uint64_t rest;

  uint64_t remainingDuration;
  uint64_t remainingRest;
  bool run;
  uint64_t distance;
  uint64_t points;
};

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

  
  // The input is very verbose, we must transform it.
  const Strings2& inputs = transformInput(rawInput);

  // create a vector of reindeers
  std::vector<Reindeer> reindeers;
  for (const Strings& input : inputs)
  {
    const uint64_t speed = std::stoull(input[0]);
    const uint64_t duration = std::stoull(input[1]);
    const uint64_t rest = std::stoull(input[2]);
    reindeers.emplace_back(speed, duration, rest);
  }

  constexpr uint64_t TIMER = 2503;

  uint64_t time = 0;
  while (time++ < TIMER)
  {
    // Make the next move for each reindeer
    for (Reindeer& r : reindeers) r.nextMove();

    // Now check distances and assign points.
    uint64_t maxDist = 0;
    for (const Reindeer& r : reindeers)
    {
      if (r.getDistance() > maxDist) maxDist = r.getDistance();
    }

    // Assign a new point to this reindeer.
    for (Reindeer& r : reindeers)
    {
      if (r.getDistance() == maxDist) r.incrementPoints();
    }
  }

  // At the end, check the maximum number of points.
  uint64_t maxPoints = 0;
  for (const Reindeer& r : reindeers)
  {
    if (r.getPoints() > maxPoints) maxPoints = r.getPoints();
  }
  std::cout << "Max points: " << maxPoints << std::endl;
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