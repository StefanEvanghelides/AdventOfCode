#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <optional>
#include <filesystem>
namespace fs = std::filesystem;


void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  size_t countIncreases = 0;
  for (size_t idx = 0; idx < rawInput.size() - 3; ++idx)
  {
    const int sumCurrent = std::stoi(rawInput[idx]) + std::stoi(rawInput[idx+1]) + std::stoi(rawInput[idx+2]);
    const int sumNext = std::stoi(rawInput[idx+1]) + std::stoi(rawInput[idx+2]) + std::stoi(rawInput[idx+3]);

    countIncreases += sumCurrent < sumNext;
  }

  std::cout << "Increase: " << countIncreases << std::endl;
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
    rawInput.emplace_back(line);
  }

  solve(rawInput);
}

int main(int argc, char** argv)
{
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