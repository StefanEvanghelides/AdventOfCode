#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

std::vector<std::string> split(const std::string& s, const std::string& delim = " ")
{
  std::vector<std::string> tokens;

  size_t start = 0;
  size_t end = 0;
  while((end = s.find(delim, start)) != std::string::npos)
  {
    tokens.emplace_back(s.substr(start, end-start));
    start = end + delim.size(); 
  }
  tokens.emplace_back(s.substr(start));

  return tokens;
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  size_t horizontal = 0;
  size_t vertical = 0;
  for (size_t i = 0; i < rawInput.size(); ++i)
  {
    const std::vector<std::string>& tokens = split(rawInput[i]);
    const std::string& cmd = tokens[0];
    const int val = std::stoi(tokens[1]);

    horizontal += val * (cmd[0] == 'f');
    vertical += val * (cmd[0] == 'd') - val * (cmd[0] == 'u');
  }

  std::cout << "Total: " << horizontal * vertical << std::endl;
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