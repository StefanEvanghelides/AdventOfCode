#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
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

  const std::vector<std::string> numbersStr = split(rawInput[0], ",");
  std::vector<size_t> numbers;
  for (const std::string& s : numbersStr)
  {
    numbers.push_back(std::stoi(s));
  }

  std::sort(numbers.begin(), numbers.end(), [](const size_t& a, const size_t& b) -> bool {
    return a < b;
  });
  // const size_t median = numbers[numbers.size() / 2];


  // ---------------- METHOD 1 ----------
  // NOTE: This is not very accurate for part 2!
  // But it's fastest!
  // ------------------------------------

  const size_t sumNumbers = std::accumulate(numbers.begin(), numbers.end(), 0);
  // const size_t mean = (size_t)((float)sumNumbers / numbers.size() + 0.5); // rounded to nearest
  const size_t mean = (size_t)((float)sumNumbers / numbers.size()); // floored

  size_t diffSum = 0;
  for (const size_t& n : numbers)
  {
    size_t currDiff = (size_t)abs((int)(n - mean));
    diffSum += currDiff * (currDiff + 1) / 2;
  }
  std::cout << "Mean: " << mean << std::endl;
  std::cout << "Diff sum: " << diffSum << std::endl;


  // ---------------- METHOD 2 ----------
  // Test and determine the optimum position.
  // ------------------------------------

  // Counting sort for a histogram
  std::map<size_t, size_t> hist;
  for (size_t i = 0; i < numbers.back(); ++i)
  {
    hist[i] = 0;
  }
  for (const size_t& n : numbers)
  {
    hist[n]++;
  }

  // Now search for the optimum spot. Also print it, I am curious.
  size_t optimumPosition = 0;
  size_t minSum = -1;
  for (int i = 0; i < numbers.back()+1; ++i)
  {
    size_t sum = 0;
    for (const auto& p : hist)
    {
      const size_t pos = p.first;
      const size_t nrs = p.second;

      const size_t diff = (size_t)abs((int)(pos - i));
      sum += nrs * diff * (diff + 1) * 0.5;
    }

    if (sum < minSum)
    {
      minSum = sum;
      optimumPosition = i;
    }
  }

  std::cout << "Optimum position: " << optimumPosition << std::endl;
  std::cout << "minSum: " << minSum << std::endl;

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