#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
#include <map>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

class HeatMap
{
public:
  HeatMap() = default;
  ~HeatMap() = default;

  HeatMap(const std::vector<std::vector<size_t>> r)
    : rows(r)
  {
    computeLocalMinima();
  }

  std::vector<std::vector<size_t>> getRows() const { return rows; }
  std::map<std::pair<size_t,size_t>,size_t> getLocalMinima() const { return localMinima; }

// Private functions
private:
  void computeLocalMinima()
  {
    localMinima.clear();
    for (size_t i = 0; i < rows.size(); ++i)
    {
      for (size_t j = 0; j < rows[i].size(); ++j)
      {
        // Check above if possible
        if (i > 0 && rows[i][j] >= rows[i-1][j]) continue;

        // Check below if possible
        if (i < rows.size()-1 && rows[i][j] >= rows[i+1][j]) continue;

        // Check left if possible
        if (j > 0 && rows[i][j] >= rows[i][j-1]) continue;

        // Check right if possible
        if (j < rows[i].size()-1 && rows[i][j] >= rows[i][j+1]) continue;

        // All checks passed, we have a new local minima
        localMinima[std::make_pair(i,j)] = rows[i][j];
      }
    }
  }

// Private data fields
private:
  std::vector<std::vector<size_t>> rows;
  std::map<std::pair<size_t,size_t>,size_t> localMinima;
};

std::ostream& operator<<(std::ostream& os, const HeatMap& hp)
{
  const auto& rows = hp.getRows();
  const auto& localMinima = hp.getLocalMinima();

  for (size_t i = 0; i < rows.size(); ++i)
  {
    for (size_t j = 0; j < rows[i].size(); ++j)
    {
      // Check if this value should be formatted.
      bool shouldFormat = false;
      for (const auto& l : localMinima)
      {
        const auto& p = l.first;
        if (p.first == i && p.second == j)
        {
          shouldFormat = true;
          break;
        }
      }

      if (shouldFormat)
      {
        os << "\x1B[1;31m" << rows[i][j] << "\x1B[0m";
      }
      else
      {
        os << rows[i][j];
      }
    }
    os << std::endl;
  }

  return os;
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

  std::vector<std::vector<size_t>> rows;
  for (const std::string& line : rawInput)
  {
    std::vector<size_t> row;
    for (const char& c : line)
    {
      row.push_back(c - '0');
    }
    rows.push_back(row);
  }

  const HeatMap hp(rows);

  //std::cout << hp << std::endl;

  const auto& lm = hp.getLocalMinima();

  size_t sum = lm.size();
  for (const auto& l : lm)
  {
    sum += l.second;
  }

  std::cout << "\x1b[0mSum: " << sum << std::endl;
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