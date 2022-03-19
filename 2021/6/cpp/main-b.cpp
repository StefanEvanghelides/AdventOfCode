#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;


#include <unordered_map>


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

  const size_t MAX_ITERATIONS = 256;
  size_t i = 0;

  std::unordered_map<int, long long> fish;
  for (size_t i = 0; i < 9; ++i)
    fish[i] = 0;

  const std::vector<std::string> input = split(rawInput[0], ",");
  for (const std::string& e : input)
  {
    const int nr = std::stoi(e);
    fish[nr]++;
  }
  while (i < MAX_ITERATIONS)
  {
    ++i;
    const long long fish0 = fish[0];

    for (size_t j = 0; j < 9-1; ++j)
    {
      fish[j] = fish[j+1];
    }
    fish[8] = fish0;
    fish[6] += fish0;

    // std::cout << "After " << i << " days (size=" << fish.size() <<"): ";
    // for (const int& f : fish)
    //   std::cout << f << ",";
    // std::cout << std::endl;
  }

  long long count = 0;
  for (size_t i = 0; i < 9; ++i)
    count += fish[i];
  std::cout << "Final count: " << count << std::endl;
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