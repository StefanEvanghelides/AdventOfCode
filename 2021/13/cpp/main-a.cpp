#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

constexpr char START[] { "start" };
constexpr char END[] { "end" };


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

  std::set<std::pair<int64_t, int64_t>> paper;
  std::vector<std::pair<uint64_t, uint64_t>> foldCommands;
  int64_t minX = 0, maxX = 0;
  int64_t minY = 0, maxY = 0;
  for (const std::string& line : rawInput)
  {
    if (line[0] == 'f')
    {
      // Fold commands
      const std::vector<std::string>& tokens = split(line, " ");
      const std::vector<std::string>& axes = split(tokens[2], "=");
      const uint64_t axis = axes[0][0] - 'x';
      const uint64_t value = std::stoi(axes[1]);
      foldCommands.push_back(std::make_pair(axis, value));
    }
    else
    {
      // Paper positions
      const std::vector<std::string>& tokens = split(line, ",");
      const int64_t x = std::stoi(tokens[0]);
      const int64_t y = std::stoi(tokens[1]);
      paper.insert(std::make_pair(x, y));
      maxX = x * (x > maxX) + maxX * (x <= maxX);
      maxY = y * (y > maxY) + maxY * (y <= maxY);
    }
  }

  std::cout << "Initial maxX=" << maxX << "; maxY=" << maxY << std::endl;

  // Quick and dirty first fold fix
  for (const auto& command : foldCommands)
  {
    const uint64_t axis = command.first;
    const uint64_t value = command.second;

    if (axis == 0) maxX = value;
    else maxY = value;

    for (const auto& p : paper)
    {
      const int64_t& x = p.first;
      const int64_t& y = p.second;
      if (x > maxX)
      {
        const int64_t newX = 2 * maxX - x;
        const auto newPair = std::make_pair(newX, y);
        paper.insert(newPair);
      }
      if (y > maxY)
      {
        const int64_t newY = 2 * maxY - y;
        const auto newPair = std::make_pair(x, newY);
        paper.insert(newPair);
      }
    }

    
    std::cout << " - current maxX=" << maxX << "; maxY=" << maxY << std::endl;

    uint64_t visibleDots = 0;
    for (const auto& p : paper)
    {
      const int64_t x = p.first;
      const int64_t y = p.second;

      visibleDots += (x <= maxX && y <= maxY);
    }

    std::cout << "VisibleDots=" << visibleDots << std::endl;

    // for the first part, we need only the first command.
    break;
  }

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