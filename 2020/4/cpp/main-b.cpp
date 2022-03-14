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

bool checkIsValid(std::map<std::string,std::string> passport)
{
  if (!passport.contains("byr")) return false;
  if (!passport.contains("iyr")) return false;
  if (!passport.contains("eyr")) return false;
  if (!passport.contains("hgt")) return false;
  if (!passport.contains("hcl")) return false;
  if (!passport.contains("ecl")) return false;
  if (!passport.contains("pid")) return false;

  // Now add extra checks
  if (passport["byr"] < "1920" || passport["byr"] > "2002") return false;
  if (passport["iyr"] < "2010" || passport["iyr"] > "2020") return false;
  if (passport["eyr"] < "2020" || passport["eyr"] > "2030") return false;

  if (passport["hgt"].size() == 4 || passport["hgt"].size() == 5)
  {
    // check the 4th idx
    if (passport["hgt"][2] == 'i' && passport["hgt"][3] == 'n')
    {
      // height in inch
      const std::string height = passport["hgt"].substr(0,2);
      if (height < "59" || height > "76") return false;
    }
    else if (passport["hgt"][3] == 'c' && passport["hgt"][4] == 'm')
    {
      // height in cm
      const std::string height = passport["hgt"].substr(0,3);
      if (height < "150" || height > "193") return false;
    }
    else return false;
  }
  else return false;
  
  if (passport["hcl"].size() == 7) 
  {
    if (passport["hcl"][0] != '#') return false;
    for (uint64_t i = 1; i < 7; ++i)
    {
      if (passport["hcl"][i] >= '0' && passport["hcl"][i] <= '9' ||
          passport["hcl"][i] >= 'a' && passport["hcl"][i] <= 'f')
      {}
      else return false;
    }
  }
  else return false;

  if (passport["ecl"] != "amb" &&
      passport["ecl"] != "blu" &&
      passport["ecl"] != "brn" &&
      passport["ecl"] != "gry" &&
      passport["ecl"] != "grn" &&
      passport["ecl"] != "hzl" &&
      passport["ecl"] != "oth") return false;

  if (passport["pid"].size() != 9) return false;

  return true;
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

  uint64_t idx = 0;
  uint64_t valid = 0;
  while (idx < rawInput.size())
  {
    // Check groups of inputs
    uint64_t idx2 = idx;
    while (!rawInput[idx2].empty() && idx2 < rawInput.size()) idx2++;

    // Current passport between [idx, idx2).
    std::map<std::string, std::string> passport;
    for (uint64_t i = idx; i < idx2; ++i)
    {
      const std::string& line = rawInput[i];

      // split current line
      const Strings& inputs = split(line, " ");
      if (inputs.size() < 1)
      {
        std::cout << "INPUTS HAS NO ELEMENTS!" << std::endl;
        return;
      }

      // now check each field
      for (const std::string& inp : inputs)
      {
        const Strings& fields = split(inp, ":");
        if (fields.size() != 2)
        {
          std::cout << "fields DOES NOT HAVE 2 ELEMENTS!" << std::endl;
          return;
        }

        passport[fields[0]] = fields[1];
      }
    }

    // Now check the fields
    valid += checkIsValid(passport);

    // Reassign the idx
    idx = idx2 + 1;
  }

  std::cout << "Valid passports: " << valid << std::endl;

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