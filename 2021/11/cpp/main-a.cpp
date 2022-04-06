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

class Octopi
{
public:
  Octopi() = default;
  ~Octopi() = default;

  Octopi(const std::vector<std::vector<uint64_t>> o)
    : os(o)
  {}

  std::vector<std::vector<uint64_t>> getOctopi() const { return os; }

  void nextIteration()
  {
    pad();

    std::vector<std::pair<uint64_t,uint64_t>> increases;

    // First, increase everything by 1.
    for (uint64_t i = 1; i < os.size()-1; ++i)
    {
      for (uint64_t j = 1; j < os[i].size()-1; ++j)
      {
        os[i][j]++;
        if (os[i][j] == 10)
        {
          // Now increase the 8 neighbours by 1, without mod.
          increases.emplace_back(i, j);
        }
      }
    }

    // Then, increase all neighbours of flashing octopi.
    // Note that everything is padded.
    for (const auto& inc : increases)
    {
      const uint64_t& i = inc.first;
      const uint64_t& j = inc.second;
      increaseNeighbours(i, j);
    }

    // Finally, reset values to 0 if higher than 9
    reset();

    unpad();
  }

  uint64_t computeFlashes() const
  {
    uint64_t flashes = 0;

    for (const auto& op : os)
    {
      for (const auto& o : op)
      {
        flashes += (o == 0);
      }
    }

    return flashes;
  }

// Functions
private:
  void increaseNeighbours(const uint64_t& i, const uint64_t& j, const uint64_t& depth = 0)
  {

    // std::cout << "INCREASE NEIGHBOURS I=" << i << " J=" << j << " DEPTH=" << depth << std::endl;

    if (i < 1 || i > os.size() - 2 ||
        j < 1 || j > os[0].size() - 2)
    {
      return;
    }

    std::vector<std::pair<uint64_t,uint64_t>> increases;
    for (uint64_t a = i-1; a <= i+1; ++a)
    {
      for (uint64_t b = j-1; b <= j+1; ++b)
      {
        if (a == i && b == j) continue;

        os[a][b]++;
        if (os[a][b] == 10)
        {
          increases.emplace_back(a, b);
        }
      }
    }

    // Recursively retest the neighbours.
    for (const auto& inc : increases)
    {
      const uint64_t& a = inc.first;
      const uint64_t& b = inc.second;
      increaseNeighbours(a, b, depth+1);
    }
  }

  void reset()
  {
    for (auto& op : os)
    {
      for (auto& o : op)
      {
        o = o > 9 ? 0 : o;
      }
    }
  }

  void pad(const uint64_t& n = 0)
  {
    // Add top and bottom rows.
    std::vector<uint64_t> o(os[0].size(), n);
    os.insert(os.begin(), o);
    os.push_back(o);

    for (auto& op : os)
    {
      op.insert(op.begin(), n);
      op.push_back(n);
    }
  }

  void unpad()
  {
    // Remove top and bottom rows
    os.pop_back();
    os.erase(os.begin());
    for (auto& op : os)
    {
      op.pop_back();
      op.erase(op.begin());
    }
  }

  void display()
  {
    for (const auto& octo : os)
    {
      for (const auto& oc : octo)
      {
        std::cout << (oc == 0 ? "\x1B[1;31m" : "") << oc << "\x1B[0m ";
      }
      std::cout << std::endl;
    }
  }

// Data fields
private:
  std::vector<std::vector<uint64_t>> os;
};

std::ostream& operator<<(std::ostream& os, Octopi o)
{
  const auto& octos = o.getOctopi();

  for (const auto& octo : octos)
  {
    for (const auto& oc : octo)
    {
      os << (oc == 0 ? "\x1B[1;31m" : "") << oc << "\x1B[0m ";
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

  std::vector<std::vector<uint64_t>> os;
  for (const std::string& line : rawInput)
  {
    std::vector<uint64_t> o;
    for (const char& c : line)
    {
      o.push_back(c - '0');
    }
    os.push_back(o);
  }
  Octopi octopi (os);
  
  uint64_t flashes = 0;
  const uint64_t MAX_ITERATIONS = 100;
  for (uint64_t i = 0; i < MAX_ITERATIONS; ++i)
  {
    octopi.nextIteration();
    const uint64_t flash = octopi.computeFlashes();
    flashes += flash;

    // std::cout << "After iteration " << i+1 << std::endl;
    // std::cout << octopi << std::endl;
  }
  std::cout << "Flashes=" << flashes << std::endl;
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