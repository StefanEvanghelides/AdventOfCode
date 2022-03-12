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

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // Now process 3 triangles simultaneously

  uint64_t count = 0;
  for (uint64_t i = 0; i < rawInput.size(); i += 3)
  {
    // parse inputs
    const std::vector<std::string>& sides1 = split(rawInput[i], "  ");
    const std::vector<std::string>& sides2 = split(rawInput[i+1], "  ");
    const std::vector<std::string>& sides3 = split(rawInput[i+2], "  ");

    if (sides1.size() != 3)
    {
      std::cout << "Input I resulted in different sides! Input: " << rawInput[i] << ";  sides1=" << sides1.size() << std::endl;
      for (const auto& s : sides1)
      {
        std::cout << " - side1: " << s << std::endl;
      }
    }
    if (sides2.size() != 3)
    {
      std::cout << "Input I+1 resulted in different sides! Input: " << rawInput[i+1] << ";  sides2=" << sides2.size() << std::endl;
      for (const auto& s : sides2)
      {
        std::cout << " - side2: " << s << std::endl;
      }
    }
    if (sides3.size() != 3)
    {
      std::cout << "Input I+2 resulted in different sides! Input: " << rawInput[i+2] << ";  sides3=" << sides2.size() << std::endl;
      for (const auto& s : sides3)
      {
        std::cout << " - side3: " << s << std::endl;
      }
    }

    // Process 1st triangle

    const uint64_t& a1 = std::stoull(trimmed(sides1[0]));
    const uint64_t& b1 = std::stoull(trimmed(sides2[0]));
    const uint64_t& c1 = std::stoull(trimmed(sides3[0]));
    std::vector<uint64_t> vec1 {a1, b1, c1};
    std::sort(vec1.begin(), vec1.end());
    if (vec1[0] + vec1[1] > vec1[2]) count++;

    // Process 2nd triangle

    const uint64_t& a2 = std::stoull(trimmed(sides1[1]));
    const uint64_t& b2 = std::stoull(trimmed(sides2[1]));
    const uint64_t& c2 = std::stoull(trimmed(sides3[1]));
    std::vector<uint64_t> vec2 {a2, b2, c2};
    std::sort(vec2.begin(), vec2.end());
    if (vec2[0] + vec2[1] > vec2[2]) count++;

    // Process 3rd triangle

    const uint64_t& a3 = std::stoull(trimmed(sides1[2]));
    const uint64_t& b3 = std::stoull(trimmed(sides2[2]));
    const uint64_t& c3 = std::stoull(trimmed(sides3[2]));
    std::vector<uint64_t> vec3 {a3, b3, c3};
    std::sort(vec3.begin(), vec3.end());
    if (vec3[0] + vec3[1] > vec3[2]) count++;
  }

  std::cout << "Possible triangles: " << count << std::endl;

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