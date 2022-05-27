#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <string_view>
#include <string>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<uint_fast32_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Pairs = std::vector<std::pair<int64_t,int64_t>>;
using Pairs2 = std::vector<Pairs>;
using Line = std::pair<std::pair<int64_t,int64_t>,std::pair<int64_t,int64_t>>;
using Lines = std::vector<Line>;

static void print(const Numbers& nrs, const std::string& delim = " ")
{
  for (const auto& nr : nrs) std::cout << nr << delim;
  std::cout << std::endl;
}

static void print(const Numbers2& nrs, const std::string& delim = " ")
{
  for (const auto& nr : nrs) print(nr);
  //std::cout << std::endl;
}

static Strings split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
{
  Strings tokens;

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

static std::string trimmed(const std::string& s)
{
  auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}


struct Graph
{
  Graph() = default;
  Graph(const Numbers2& verts) : vertices(verts) {}
  ~Graph() = default;

  Numbers2 connectedComponents()
  {
    Numbers2 components;
    Bools visited(vertices.size(), false);
    for (uint_fast32_t i = 0; i < vertices.size(); ++i)
    {
      if (!visited[i])
      {
        Numbers& comp = components.emplace_back();
        std::queue<uint_fast32_t> q;
        q.emplace(i);
        visited[i] = true;
        while (!q.empty())
        {
          const uint_fast32_t w = q.front();
          comp.push_back(w);
          q.pop();
          for (const auto& neighbour : vertices[w])
          {
            if (!visited[neighbour] ) {
              // We’ve found another node in this component.
              visited[neighbour] = true;
              q.emplace(neighbour);
            }
          }
        }
      }
    }
    
    return components;
  }

  Numbers2 getVertices() const { return vertices; }
  void setVertices(const Numbers2& verts) { vertices = verts; }

  void printGraph()
  {
    print(vertices);
  }

private:
  Numbers2 vertices;
};


static void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  if (rawInput.size() < 1)
  {
    std::cout << "RAW INPUT HAS NO ELEMENTS!" << std::endl;
    return;
  }

  // This is a basic graph problem, the input is an adjacency list.
  // Part a) wants to find which vertices are connected to the 0.
  // So this can be solved by determining the number of connected components.

  Numbers2 vertices;
  for (const std::string& inp : rawInput)
  {
    const Strings& tokens = split(inp, " <-> ");
    if (tokens.size() != 2)
    {
      std::cout << "Incorrect splitting! inp: " << inp << std::endl;
      continue;
    }
    const uint_fast32_t& node = std::stoul(tokens[0]);
    const Strings& neighboursStr = split(tokens[1], ", ");
    Numbers neighbours;
    for (const std::string& ns : neighboursStr)
    {
      neighbours.emplace_back(std::stoul(ns));
    }

    vertices.emplace_back(std::move(neighbours));
  }

  Graph g(vertices);
  //g.printGraph();

  const Numbers2 components = g.connectedComponents();
  //print(components);

  // Find which component has the starting node.
  constexpr uint_fast32_t START_NODE = 0;

  uint_fast32_t connected = 0;
  for (const Numbers& comp : components)
  {
    if (std::find(comp.begin(), comp.end(), START_NODE) != comp.end())
    {
      // This component contains the starting node!
      connected = comp.size();
      break;
    }
  }

  std::cout << "Connected elements with 0: " << connected << std::endl;

}

static void run(std::string_view filename)
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
  Strings rawInput;

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