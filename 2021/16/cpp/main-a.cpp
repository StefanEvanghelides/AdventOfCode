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


std::unordered_map<char, std::string> DECODER {
  {'0', "0000"},
  {'1', "0001"},
  {'2', "0010"},
  {'3', "0011"},
  {'4', "0100"},
  {'5', "0101"},
  {'6', "0110"},
  {'7', "0111"},
  {'8', "1000"},
  {'9', "1001"},
  {'A', "1010"},
  {'B', "1011"},
  {'C', "1100"},
  {'D', "1101"},
  {'E', "1110"},
  {'F', "1111"},
};

std::unordered_map<std::string, uint64_t> ENCODER {
  {"000", 0},
  {"001", 1},
  {"010", 2},
  {"011", 3},
  {"100", 4},
  {"101", 5},
  {"110", 6},
  {"111", 7},
};

struct State
{
  std::string numberStr;
  uint64_t totalBits;
  uint64_t sumVersions;

  State()
    : numberStr(""), totalBits(0), sumVersions(0)
  {}
  ~State() = default;
};

State parseString(const std::string& input, const uint64_t& start, const uint64_t& size, const uint64_t& depth)
{
  State state;
  const std::string decoded = input.substr(start, size);
  const std::string v_bits = decoded.substr(0, 3);
  const std::string t_bits = decoded.substr(3, 3);

  const uint64_t& packetVersion = ENCODER[v_bits];
  const uint64_t& packetTypeID = ENCODER[t_bits];

  state.sumVersions = packetVersion;

  // Easy case: Just take the strings A, B, C.
  state.totalBits = 6;
  char startBit = decoded[state.totalBits];

  // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
  // std::cout << "Decoded=" << decoded << "; packet type: " << packetTypeID << "; packetVersion=" << packetVersion << "; startBit=" << startBit << std::endl;

  if (packetTypeID == 4)
  {
    do
    {
      startBit = decoded[state.totalBits];
      const std::string bits = decoded.substr(state.totalBits+1, 4);
      state.numberStr += bits;
      state.totalBits += 5;
    }
    while (startBit == '1' && state.totalBits < decoded.size());

    return state;
  }

  // More processing is necessary now.
  if (startBit == '0')
  {
    // next 15 bits are the total length (in bits) of sub-packets.
    const std::string lengthStr = decoded.substr(7, 15);
    int64_t length = std::stoi(lengthStr, 0, 2);
    state.totalBits = 22;

    // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
    // std::cout << "Bit 0: length=" << length << "; curr total bits: " << state.totalBits << std::endl;

    while (length > 0)
    {
      // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
      // std::cout << "Iteration Bit 0 BEFORE: length=" << length << "; curr total bits: " << state.totalBits << std::endl;

      const State& currentState = parseString(decoded, state.totalBits, size, depth+1);
      length -= currentState.totalBits;
      state.numberStr += currentState.numberStr;
      state.totalBits += currentState.totalBits;
      state.sumVersions += currentState.sumVersions;

      // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
      // std::cout << "Iteration Bit 0 AFTER: length=" << length << "; curr total bits: " << state.totalBits << "; sumVers: " << state.sumVersions << std::endl;
    }
  }
  else
  {
    // next 11 bits are the number of sub-packets.
    const std::string numberSubpacketsStr = decoded.substr(7, 11);
    const uint64_t numberSubpackets = std::stoi(numberSubpacketsStr, 0, 2);

    // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
    // std::cout << "Bit 1: numberSubpackets=" << numberSubpackets << std::endl;

    state.totalBits = 18;
    for (uint64_t i = 0; i < numberSubpackets; ++i)
    {
      // for (uint64_t i = 0; i < depth; ++i) std::cout << "  ";
      // std::cout << "Iteration Bit 1: start=" << state.totalBits << std::endl;

      const State& currentState = parseString(decoded, state.totalBits, size, depth+1);

      state.numberStr += currentState.numberStr;
      state.totalBits += currentState.totalBits;
      state.sumVersions += currentState.sumVersions;
    }
  }

  return state;
}

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

  // First, decode the hexa into bits.
  for (const std::string& line : rawInput)
  {
    std::string decoded;
    for (const char& c : line)
    {
      decoded += DECODER[c];
    }

    std::cout << std::endl << "Line:" << line << std::endl;
    std::cout << "Initial decoded string=" << decoded << std::endl;

    const State& state = parseString(decoded, 0, decoded.size(), 0);

    // const std::string& numberStr = state.numberStr;
    // if (!numberStr.empty())
    // {
    //   const uint64_t number = std::stoi(numberStr, 0, 2);
    //   std::cout << "Number=" << number << std::endl;
    // }
    // else
    // {
    //   std::cout << "Number is empty!" << std::endl;
    // }
    
    const uint64_t& sumVersions = state.sumVersions;
    std::cout << "SumVersions=" << sumVersions << std::endl;
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