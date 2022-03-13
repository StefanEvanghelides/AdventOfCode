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

Strings transformInput(const Strings& rawInput)
{
  Strings input;

  for (std::string line : rawInput)
  {
    auto it = line.find('[');
    line.erase(it, 1); // remove the [
    it = line.find(']');
    line.erase(it+1,1); // remove space after ]

    // Now check the type of line
    if (line[it+1] == 'G')
    {
      // Fetch the id of the guard
      std::string temp = line.substr(it+1);
      Strings tokens = split(temp, " ");
      if (tokens.size() < 2)
      {
        std::cout << "CANNOT TRANSFORM THIS PART OF LINE! Temp=" << temp << std::endl;
        continue;
      }
      line = line.substr(0, it+1);
      line += tokens[1].substr(1,tokens[1].size()-1);
    }
    else if (line[it+1] == 'f')
    {
      line = line.substr(0, it+1);
      line += '#';
    }
    else if (line[it+1] == 'w')
    {
      line = line.substr(0, it+1);
      line += '.';
    }
    
    input.push_back(line);
  }

  return input;
}

void sortInput(Strings& input)
{
  sort(input.begin(), input.end());
}


void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }
 
  Strings input = transformInput(rawInput);
  sortInput(input);
  
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : input) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // structure for the actions
  Strings2 actionsGroup;
  const uint64_t ACTION_IDX = 17;
  for (const std::string& line : input)
  {
    if (line.size() < ACTION_IDX+1)
    {
      std::cout << "LINE DOES NOT HAVE 18 chars: " << line << std::endl;
      continue;
    }
    if (line[ACTION_IDX] != '#' && line[ACTION_IDX] != '.')
    {
      actionsGroup.emplace_back();
    }
    actionsGroup.back().push_back(line);
  }

  // // Checking the actionsGroup
  // std::cout << "Actions group: " << actionsGroup.size() << std::endl;
  // for (const Strings& actions : actionsGroup)
  // {
  //   std::cout << " - actions size: " << actions.size() << std::endl;
  //   for (const std::string& action : actions)
  //   {
  //     std::cout << " -   ACT: " << action << std::endl;
  //   }
  // }


  // map <guardID, map<datetime, vector<timeasleep>>>;
  std::map<uint64_t, std::map<std::string, std::vector<bool>>> guards;
  const uint64_t MAX_MINUTES = 60;
  for (const Strings& actions : actionsGroup)
  {
    uint64_t guard = 0;
    for (uint64_t i = 0; i < actions.size(); ++i)
    {
      const std::string& line = actions[i];

      // split time and action
      const Strings& tokens = split(line, "]");
      if (tokens.size() < 2)
      {
        std::cout << "TOKENS HAS LESS THAN 2 ELEMENTS, line: " << line << std::endl;
        continue;
      }

      // fetch the daytime and further split it into day and time.
      const std::string& daytime = tokens[0];
      const Strings& daytimeTokens = split(daytime, " ");
      if (daytimeTokens.size() < 2)
      {
        std::cout << "DAYTIMETOKENS HAS LESS THAN 2 ELEMENTS, line: " << line << std::endl;
        continue;
      }
      const std::string& day = daytimeTokens[0];
      const std::string& time = daytimeTokens[1];

      // fetch the action or guard id
      const std::string& action = tokens[1];
      if (line[ACTION_IDX] == '#')
      {
        if (!guards.contains(guard))
        {
          guards[guard] = std::map<std::string, std::vector<bool>>();
        }
        if (!guards[guard].contains(day))
        {
          guards[guard][day] = std::vector<bool>(MAX_MINUTES, false);
        }

        // now parse the minutes of the time
        const std::vector<std::string>& times = split(time, ":");
        if (times.size() < 2)
        {
          std::cout << "TIMES HAS LESS THAN 2 ELEMENTS! time=" << time << std::endl;
          continue;
        }

        // just take the minutes, we don't care about the hour.
        const uint64_t& minute = std::stoull(times[1]);

        // Now check if there's a waking up time, otherwise use the 60 limit.
        uint64_t limit = MAX_MINUTES;
        if (i+1 < actions.size() && actions[i+1][ACTION_IDX] == '.')
        {
          // Ok, parse the action for the waking up time
          const std::string& line2 = actions[i+1];

          // split time and action
          const Strings& tokens2 = split(line2, "]");
          if (tokens2.size() < 2)
          {
            std::cout << "TOKENS2 HAS LESS THAN 2 ELEMENTS, line: " << line2 << std::endl;
            continue;
          }

          // fetch the daytime and further split it into day and time.
          const std::string& daytime2 = tokens2[0];
          const Strings& daytimeTokens2 = split(daytime2, " ");
          if (daytimeTokens2.size() < 2)
          {
            std::cout << "DAYTIMETOKENS2 HAS LESS THAN 2 ELEMENTS, line: " << line2 << std::endl;
            continue;
          }
          const std::string& time2 = daytimeTokens2[1];
          // now parse the minutes of the time
          const std::vector<std::string>& times2 = split(time2, ":");
          if (times2.size() < 2)
          {
            std::cout << "TIMES2 HAS LESS THAN 2 ELEMENTS! time2=" << time2 << std::endl;
            continue;
          }
          limit = std::stoull(times2[1]);

          // increment the index of the action. this was already parsed.
          i++;
        }

        // Now simply set the times to sleep
        for (uint64_t j = minute; j < limit; ++j)
        {
          guards[guard][day][j] = true;
        }
      }
      else if (line[ACTION_IDX] != '.')
      {
        guard = std::stoull(action);
      }

    }

  }


  // // Now check this new structure
  // std::cout << "TOTAL GUARDS: " << guards.size() << std::endl;
  // for (const auto& [key, val] : guards)
  // {
  //   std::cout << "GUARD=" << key << ";  total days=" << val.size() << std::endl;
  //   for (const auto& [key2, val2] : val)
  //   {
  //     std::cout << " - date=" << key2 << std::endl;
  //     for (const auto& v : val2)
  //     {
  //       std::cout << (v ? "1" : "0") << " ";
  //     }
  //     std::cout << std::endl;
  //   }
  // }

  // Check which guard slept the most.
  uint64_t max_g = 0;
  uint64_t max_gi = 0;
  for (const auto& [key, val] : guards)
  {
    uint64_t curr_g = 0;
    for (const auto& [key2, val2] : val)
    {
      for (const auto& v : val2)
      {
        curr_g += v;
      }
    }
    if (curr_g > max_g)
    {
      max_g = curr_g;
      max_gi = key;
    }
  }

  std::cout << "Guard who slep the most: " << max_gi << std::endl;
  
  // Now check most common minute for this guard.

  std::vector<uint64_t> freq(MAX_MINUTES, 0);
  for (const auto& [key2, val2] : guards[max_gi])
  {
    for (uint64_t i = 0; i < val2.size(); ++i)
    {
      freq[i] += val2[i];
    }
  }

  // check the highest value
  uint64_t max_m = 0;
  uint64_t max_mi = 0;
  for (uint64_t i = 0; i < freq.size(); ++i)
  {
    if (freq[i] > max_m)
    {
      max_m = freq[i];
      max_mi = i;
    }
  }

  std::cout << "Minute most frequent: " << max_mi << std::endl;

  std::cout << "Total: " << max_gi * max_mi << std::endl;

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