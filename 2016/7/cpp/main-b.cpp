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


std::vector<std::string> split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
{
  std::vector<std::string> tokens;

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

std::vector<std::string> getAllABA(const std::string& str)
{
  if (str.size() < 3) return {};

  std::vector<std::string> abas;

  for (uint64_t i = 2; i < str.size(); ++i)
  {
    if (str[i] == str[i-2] &&
        str[i] != str[i-1])
    {
      const std::string str2 {str[i-2], str[i-1], str[i]};
      abas.push_back(str2);
    }
  }
  
  return abas;
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  uint64_t counter = 0;
  for (const std::string& input : rawInput)
  {
    const std::vector<std::string>& inputs = split(input, " ");

    if (inputs.size() % 2 == 0)
    {
      std::cout << "INPUT SIZE IS EVEN! Input: " << input << std::endl;
    }

    // check ABA's outside
    std::vector<std::string> allABAS;
    for (uint64_t i = 0; i < inputs.size(); i = i + 2)
    {
      const auto& abas = getAllABA(inputs[i]);
      allABAS.insert(allABAS.end(), abas.begin(), abas.end());
    }

    // check BAB inside
    bool isValid = allABAS.size() > 0;
    if (isValid)
    {
      // compute reverse BAB
      std::vector<std::string> allBABs;
      for (const std::string& aba : allABAS)
      {
        std::string bab {aba[1], aba[0], aba[1]};
        allBABs.push_back(bab);
      }

      // Now check if any babs are inside the square brackets
      isValid = false;
      for (uint64_t i = 1; i < inputs.size(); i = i + 2)
      {
        std::vector<std::string> babs = getAllABA(inputs[i]);
        for (const std::string bab : allBABs)
        {
          if (std::find(babs.begin(), babs.end(), bab) != babs.end())
          {
            isValid = true;
            break;
          }
        }
        if (isValid) break;
      }
    }

    if (isValid) counter++;
  }

  std::cout << "Valid IPs: " << counter << std::endl;

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
  std::vector<std::string> rawInput;

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