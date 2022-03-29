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

class Cell
{
public:
  Cell() = default;
  ~Cell() = default;

  Cell(const std::uint64_t& n)
    : n(n), checked(false)
  {}

  std::uint64_t getN() const { return n; }

  bool getChecked() const { return checked; }
  void setChecked(const bool check) { checked  = check; }

private:
  std::uint64_t n;
  bool checked = false;
};

class HeatMap
{
public:
  HeatMap() = default;
  ~HeatMap() = default;

  HeatMap(const std::vector<std::vector<Cell>> r)
    : rows(r)
  {
    computeLocalMinima();
  }

  void pad(const std::uint64_t n)
  {
    // Add top and bottom rows.
    std::vector<Cell> r(rows[0].size(), Cell(n));
    rows.insert(rows.begin(), r);
    rows.push_back(r);

    for (auto& row : rows)
    {
      row.insert(row.begin(), Cell(n));
      row.push_back(Cell(n));
    }

    // Recompute local minima & basins
    computeLocalMinima();
    computeBasins();
  }

  std::vector<std::vector<Cell>> getRows() const { return rows; }
  std::map<std::pair<std::uint64_t,std::uint64_t>,std::uint64_t> getLocalMinima() const { return localMinima; }
  std::vector<std::uint64_t> getBasins() const { return basins; }

// Private functions
private:
  void computeLocalMinima()
  {
    localMinima.clear();
    for (std::uint64_t i = 0; i < rows.size(); ++i)
    {
      for (std::uint64_t j = 0; j < rows[i].size(); ++j)
      {
        // Check above if possible
        if (i > 0 && rows[i][j].getN() >= rows[i-1][j].getN()) continue;

        // Check below if possible
        if (i < rows.size()-1 && rows[i][j].getN() >= rows[i+1][j].getN()) continue;

        // Check left if possible
        if (j > 0 && rows[i][j].getN() >= rows[i][j-1].getN()) continue;

        // Check right if possible
        if (j < rows[i].size()-1 && rows[i][j].getN() >= rows[i][j+1].getN()) continue;

        // All checks passed, we have a new local minima
        localMinima[std::make_pair(i,j)] = rows[i][j].getN();
      }
    }
  }

  void fillBasin(const std::uint64_t ci, const std::uint64_t cj)
  {
    if (rows[ci][cj].getChecked() || rows[ci][cj].getN() == 9) return;

    rows[ci][cj].setChecked(true);

    // Expand
    fillBasin(ci+1, cj);
    fillBasin(ci, cj+1);
    fillBasin(ci-1, cj);
    fillBasin(ci, cj-1);
  }

  std::uint64_t countCheckedCells()
  {
    std::uint64_t count = 0;
    for (const auto& row : rows)
    {
      for (const auto& cell : row)
      {
        count += cell.getChecked();
      }
    }
    return count;
  }

  void clearCells()
  {
    for (auto& row : rows)
    {
      for (auto& cell : row)
      {
        cell.setChecked(false);
      }
    }
  }

  void computeBasins()
  {
    basins.clear();

    // Use the local minima for this!
    basins = std::vector<std::uint64_t>(localMinima.size());
    
    // Idea: for each local minima, spread over and find size.
    // The maximum walls are 9 -> hence find the padding for 9's
    for (const auto& l : localMinima)
    {
      const auto& p = l.first;
      const std::uint64_t ci = p.first;
      const std::uint64_t cj = p.second;
      fillBasin(ci, cj);
      const std::uint64_t size = countCheckedCells();
      basins.push_back(size);
      clearCells();
    }
  }

// Private data fields
private:
  std::vector<std::vector<Cell>> rows;
  std::map<std::pair<std::uint64_t,std::uint64_t>,std::uint64_t> localMinima;
  std::vector<std::uint64_t> basins;
};

std::ostream& operator<<(std::ostream& os, const HeatMap& hp)
{
  const std::vector<std::vector<Cell>>& rows = hp.getRows();
  const std::map<std::pair<std::uint64_t,std::uint64_t>,std::uint64_t>& localMinima = hp.getLocalMinima();

  for (std::uint64_t i = 0; i < rows.size(); ++i)
  {
    for (std::uint64_t j = 0; j < rows[i].size(); ++j)
    {
      // Check if this value should be formatted.
      bool shouldFormat = false;
      for (const std::pair<std::pair<std::uint64_t,std::uint64_t>,std::uint64_t>& l : localMinima)
      {
        const std::pair<std::uint64_t,std::uint64_t>& p = l.first;
        if (p.first == i && p.second == j)
        {
          shouldFormat = true;
          break;
        }
      }

      if (shouldFormat)
      {
        os << "\x1B[1;31m" << rows[i][j].getN() << "\x1B[0m";
      }
      else
      {
        os << rows[i][j].getN();
      }
    }
    os << std::endl;
  }

  return os;
}

std::vector<std::string> split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
{
  std::vector<std::string> tokens;

  std::uint64_t start = 0;
  std::uint64_t end = 0;
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

  std::vector<std::vector<Cell>> rows;
  for (const std::string& line : rawInput)
  {
    std::vector<Cell> row;
    for (const char& c : line)
    {
      row.push_back(Cell(c - '0'));
    }
    rows.push_back(row);
  }

  HeatMap hp(rows);
  hp.pad(9);

  std::cout << hp << std::endl;

  std::vector<std::uint64_t> basins = hp.getBasins();
  std::sort(basins.begin(), basins.end(), std::greater<std::uint64_t>());

  std::uint64_t product = basins[0] * basins[1] * basins[2];
  std::cout << "Product: " << product << std::endl;
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