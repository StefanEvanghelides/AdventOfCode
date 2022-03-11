#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

void printVec(const std::vector<std::string>& vec, const std::string& delim = "\n")
{
  for (const std::string& v : vec)
  {
    std::cout << v << delim;
  }
  if (delim != "\n") std::cout << std::endl;
}

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

  std::string gamma;
  std::vector<std::string> input = rawInput;
  const size_t inputSize = input[0].size();
  for (size_t j = 0; j < inputSize; ++j)
  {
    int count = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
      count += (input[i][j] == '1') - (input[i][j] == '0');
    }
    char bit = '0' * (count < 0) + '1' * (count >= 0);

    input.erase(std::remove_if(input.begin(), input.end(), [bit, j](std::string s) {
      return s[j] != bit;
    }), input.end());

    if (input.size() == 1)
    {
      gamma = input[0];
      break;
    }
  }

  std::string not_gamma;
  input = rawInput;
  for (size_t j = 0; j < inputSize; ++j)
  {
    int count = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
      count += (input[i][j] == '1') - (input[i][j] == '0');
    }
    char bit = '1' * (count < 0) + '0' * (count >= 0);

    input.erase(std::remove_if(input.begin(), input.end(), [bit, j](std::string s) {
      return s[j] != bit;
    }), input.end());

    if (input.size() == 1)
    {
      not_gamma = input[0];
      break;
    }
  }

  const int gamma_int = std::stoi(gamma, 0, 2);
  const int not_gamma_int = std::stoi(not_gamma, 0, 2);
  const int power = gamma_int * not_gamma_int;

  std::cout << "Gamma: " << gamma << std::endl;
  std::cout << "not_gamma: " << not_gamma << std::endl;
  std::cout << "Power: " << power << std::endl;
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