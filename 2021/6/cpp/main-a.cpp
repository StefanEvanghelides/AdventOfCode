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

  const size_t MAX_ITERATIONS = 80;
  size_t i = 0;

  std::vector<int> fish;
  const std::vector<std::string> input = split(rawInput[0], ",");
  for (const std::string& e : input)
  {
    const int nr = std::stoi(e);
    fish.push_back(nr);
  }
  while (i < MAX_ITERATIONS)
  {
    ++i;
    size_t countAdd = 0;
    for (int& f : fish)
    {
      if (f == 0)
      {
        f = 6;
        countAdd++;
      }
      else
      {
        f--;
      }
    }
    for (size_t i = 0; i < countAdd; ++i)
    {
      fish.push_back(8);
    }

    // std::cout << "After " << i << " days (size=" << fish.size() <<"): ";
    // for (const int& f : fish)
    //   std::cout << f << ",";
    // std::cout << std::endl;
  }

  std::cout << "Final count: " << fish.size() << std::endl;
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