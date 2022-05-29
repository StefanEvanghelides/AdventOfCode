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

  const std::vector<std::string> player1Vec = split(rawInput[0], ": ");
  const std::vector<std::string> player2Vec = split(rawInput[1], ": ");

  uint64_t player1 = std::stoll(player1Vec[1]);
  uint64_t player2 = std::stoll(player2Vec[1]);

  uint64_t player1Score = 0;
  uint64_t player2Score = 0;

  bool firstTurn = true;
  uint64_t totalRolls = 0;
  while (player1Score < 1000 && player2Score < 1000)
  {
    const uint64_t dice1 = totalRolls % 100 + 1;
    const uint64_t dice2 = (totalRolls + 1) % 100 + 1;
    const uint64_t dice3 = (totalRolls + 2) % 100 + 1;

    if (firstTurn)
    {
      player1 += dice1 + dice2 + dice3;
      player1 = (player1 - 1) % 10 + 1;
      player1Score += player1;
    }
    else
    {
      player2 +=  dice1 + dice2 + dice3;
      player2 = (player2 - 1) % 10 + 1;
      player2Score += player2;
    }

    firstTurn = !firstTurn;
    totalRolls += 3;
  }

  const bool winner = player1Score >= 1000;

  std::cout << "Player 1 score: " << player1Score << "; player 2 score: " << player2Score << std::endl;
  std::cout << "Result: " << totalRolls * (winner ? player2Score : player1Score);

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