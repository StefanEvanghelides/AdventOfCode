#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
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

  std::unordered_map<size_t, size_t> signalNumbers;
  signalNumbers[0] = 6;
  signalNumbers[1] = 2;
  signalNumbers[2] = 5;
  signalNumbers[3] = 5;
  signalNumbers[4] = 4;
  signalNumbers[5] = 5;
  signalNumbers[6] = 6;
  signalNumbers[7] = 3;
  signalNumbers[8] = 7;
  signalNumbers[9] = 6;

  size_t count = 0;
  for (const std::string& rawLine : rawInput)
  {
    const std::vector<std::string> sides = split(rawLine, "|");
    const std::vector<std::string> secondPart = split(trimmed(sides[1]));
    for (const std::string& s : secondPart)
    {
      count += (s.size() == signalNumbers[1])
             + (s.size() == signalNumbers[4])
             + (s.size() == signalNumbers[7])
             + (s.size() == signalNumbers[8]);
    }
  }

  std::cout << "Occurrences for 1, 4, 7 and 8: " << count << std::endl;
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