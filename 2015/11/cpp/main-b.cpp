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

std::string findNextPassword(std::string password)
{
  constexpr uint64_t MAX_ITERATIONS = 1e8;
  std::string oldPassword = password;
  bool correct = false;
  uint64_t iterations = 0;
  while (!correct && iterations++ < MAX_ITERATIONS)
  {
    // Fetch the next lexicographical string
    password = nextString(oldPassword);
    if (password == oldPassword)
    {
      std::cout << "ERROR! PASSWORD IS THE SAME!" << std::endl;
      break;
    }

    // Check if string contains the skip letters
    bool badLetters = false;
    for (const char& c : password)
    {
      if (c == 'i' || c == 'o' || c == 'l')
      {
        badLetters = true;
        break;
      }
    }
    if (badLetters)
    {
      // Move on to the next word
      oldPassword = password;
      continue;
    }

    // Now check if there are 3 consecutive increasing chars
    bool threeConsecutive = false;
    for (uint64_t i = 0; i < password.size() - 2; ++i)
    {
      if (password[i+2] == password[i+1] + 1 && password[i+1] == password[i] + 1)
      {
        threeConsecutive = true;
        break;
      }
    }
    if (!threeConsecutive)
    {
      // Move on to the next word
      oldPassword = password;
      continue;
    }

    // Now check for 2 distinct doubles
    char firstDouble = 0;
    bool twoDistinctDoubles = false;
    for (uint64_t i = 0; i < password.size() - 1; ++i)
    {
      if (password[i] == password[i+1])
      {
        if (firstDouble == 0)
        {
          // we are now setting up the first one
          firstDouble = password[i];
        }
        else if (firstDouble != password[i])
        {
          // Yes, we have 2 distinct doubles
          twoDistinctDoubles = true;
          break;
        }
      }
    }
    if (!twoDistinctDoubles)
    {
      // Move on to the next word
      oldPassword = password;
      continue;
    }

    // If we reached this, then we finally have a correct password
    correct = true;
  }
  std::cout << "Iterations: " << iterations << std::endl;
  std::cout << "Next password: " << password << std::endl;
  return password;
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

  std::string password = findNextPassword(rawInput[0]);
  password = findNextPassword(password);

  std::cout << "Second Next password: " << password << std::endl;
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