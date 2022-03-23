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

  Numbers seats;
  for (const std::string& input : rawInput)
  {
    if (input.size() != 10)
    {
      std::cout << "INPUT DOES NOT HAVE 10 ELEMENTS!" << std::endl;
      return;
    }

    // compute the row
    std::string rowStr;
    for (uint64_t i = 0; i < 7; ++i)
    {
      rowStr += (input[i] == 'B' ? '1' : '0');
    }
    const uint64_t row = std::stoull(rowStr, 0, 2);

    // compute the seatID
    std::string seatIDstr;
    for (uint64_t i = 7; i < input.size(); ++i)
    {
      seatIDstr += (input[i] == 'R' ? '1' : '0');
    }
    const uint64_t seatID = std::stoull(seatIDstr, 0, 2);

    // comput the seat
    const uint64_t seat = row * 8 + seatID;

    // Add the seat
    seats.push_back(seat);
  }

  // Show the highest number
  sort(seats.begin(), seats.end());

  // show the seats
  std::cout << "Showing seats:" << std::endl;
  for (const auto& s : seats) std::cout << s << std::endl;

  // binary search for the missing seat.
  uint64_t mySeat = seats.size() / 2;

  if (seats[mySeat+1] - seats[mySeat] != 2)
  {
    // check downwards until 5% of places
    for (uint64_t i = mySeat / 2; i > 20; --i)
    {
      if (seats[i+1] - seats[i] == 2)
      {
        mySeat = i;
        break;
      }
    }
  }
  if (seats[mySeat+1] - seats[mySeat] != 2)
  {
    // check upwards until 5% of places
    for (uint64_t i = mySeat / 2; i < seats.size() - 20; ++i)
    {
      if (seats[i+1] - seats[i] == 2)
      {
        mySeat = i;
        break;
      }
    }
  }

  std::cout << "My seat: " << seats[mySeat]+1 << std::endl;

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