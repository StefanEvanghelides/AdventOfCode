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

void printBoard(const std::vector<std::string>& board)
{
  for (const std::string& line : board)
  {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
}

std::vector<std::string> transformInput(const std::vector<std::string>& rawInput)
{
  std::vector<std::string> inputs;

  for (std::string line : rawInput)
  {
    // Condition
    const auto it = std::find(line.begin(), line.end(), '=');
    if (it != line.end())
    {
      line.erase(it-1);
      line.erase(it-1);
    }
    inputs.push_back(line);
  }

  return inputs;
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  const std::vector<std::string> inputs = transformInput(rawInput);

  // std::cout << "Transformed input:" << std::endl;
  // for (auto line : inputs) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  const uint64_t MAX_WIDTH = 50;
  const uint64_t MAX_HEIGHT = 6;

  std::vector<std::string> board(MAX_HEIGHT, std::string(MAX_WIDTH, '.'));

  for (const std::string& line : inputs)
  {
    if (line.size() < 2)
      {
        std::cout << "LINE size is less than 2!" << std::endl;
        continue;
      }

    if (line[1] == 'e')
    {
      // RECT line
      const std::vector<std::string> rects = split(line, " ");
      if (rects.size() < 2)
      {
        std::cout << "RECTS size is less than 2!" << std::endl;
        continue;
      }
      const std::vector<std::string> values = split(rects[1], "x");
      if (values.size() < 2)
      {
        std::cout << "VALUES size is less than 2!" << std::endl;
        continue;
      }
      const uint64_t cols = std::stoull(values[0]);
      const uint64_t rows = std::stoull(values[1]);

      for (uint64_t i = 0; i < rows; ++i)
      {
        for (uint64_t j = 0; j < cols; ++j)
        {
          board[i][j] = '#';
        }
      }
    }
    else if (line[1] == 'o')
    {
      // ROTATE line
      const std::vector<std::string>& rotates = split(line, " ");
      if (rotates.size() < 5)
      {
        std::cout << "ROTATES size is less than 2!" << std::endl;
        continue;
      }

      const bool isRow = rotates[1][0] == 'r';
      const uint64_t rowCol = std::stoull(rotates[2]);
      const uint64_t limit = (isRow ? MAX_WIDTH : MAX_HEIGHT);
      const uint64_t value = std::stoull(rotates[4]) % limit;
      // Now rotate
      if (isRow)
      {
        // Easy rotation
        std::rotate(board[rowCol].begin(), board[rowCol].begin() + limit - value, board[rowCol].end());
      }
      else
      {
        for (uint64_t i = 0; i < value; ++i)
        {
          // shift by 1
          const char tmp = board[MAX_HEIGHT-1][rowCol];
          for (uint64_t j = MAX_HEIGHT-1; j > 0; --j)
          {
            board[j][rowCol] = board[j-1][rowCol];
          }
          board[0][rowCol] = tmp;
        }
      }
    }
    else
    {
      std::cout << "INVALID LINE: " << line << std::endl;
    }

    // printBoard(board);
  }

  // Calculate lit panels
  uint64_t count = 0;
  for (const std::string& line : board)
  {
    for (const char& c : line)
    {
      if (c == '#') count++;
    }
  }

  std::cout << "Lit panels: " << count << std::endl;

  printBoard(board);

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