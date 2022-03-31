#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
#include <map>
#include <unordered_map>
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

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  std::unordered_map<char, uint64_t> costsCorrupted;
  costsCorrupted[')'] = 3;
  costsCorrupted[']'] = 57;
  costsCorrupted['}'] = 1197;
  costsCorrupted['>'] = 25137;
  std::unordered_map<char, uint64_t> costsIncomplete;
  costsIncomplete[')'] = 1;
  costsIncomplete[']'] = 2;
  costsIncomplete['}'] = 3;
  costsIncomplete['>'] = 4;

  const std::string openChars = "([{<";

  std::vector<uint64_t> sums;
  for (const std::string& line : rawInput)
  {
    std::vector<char> chars;
    bool corrupt = false;
    for (const char& c : line)
    {
      if (openChars.find(c) != std::string::npos)
      {
        chars.push_back(c);
      }
      else
      {
        if (chars.size() < 1)
        {
          corrupt = true;
          break;
        }
        const char backChar = chars.back();
        const char expectedChar =
            ')' * (backChar == '(') + 
            ']' * (backChar == '[') + 
            '}' * (backChar == '{') + 
            '>' * (backChar == '<');
        if (c != expectedChar)
        {
          corrupt = true;
          break;
        }
        chars.pop_back();
      }
    }
    if (corrupt) continue;

    // Now the remaining lines are either correct or incomplete.
    // Simply look back on the chars vector.
    uint64_t currentSum = 0;
    while (!chars.empty())
    {
      const char c = chars.back();
      const char expectedChar =
          ')' * (c == '(') + 
          ']' * (c == '[') + 
          '}' * (c == '{') + 
          '>' * (c == '<');

      currentSum *= 5;
      currentSum += costsIncomplete[expectedChar];

      chars.pop_back();
    }
    sums.push_back(currentSum);
  }

  std::sort(sums.begin(), sums.end());

  std::cout << "Mid score: " << sums[sums.size()/2] << std::endl;
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