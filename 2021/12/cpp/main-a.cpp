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

constexpr char START[] { "start" };
constexpr char END[] { "end" };


class Graph
{
public:
  Graph() = default;
  ~Graph() = default;

  void addNode(const std::string& node)
  {
    if (!nodeExists(node))
    {
      nodes[node] = std::vector<std::string>();
    }
  }

  void addConnection(const std::string& node1, const std::string& node2)
  {
    if (nodeExists(node1))
    {
      nodes[node1].push_back(node2);
    }
    if (nodeExists(node2))
    {
      nodes[node2].push_back(node1);
    }
  }

  uint64_t traverse()
  {
    const uint64_t paths = dfs();
    return paths;
  }

// Functions
private:
  bool nodeExists(const std::string& node)
  {
    return nodes.find(node) != nodes.end();
  }

  bool nodeVisited(const std::string& node, const std::vector<std::string> visited)
  {
    return std::find(visited.begin(), visited.end(), node) != visited.end();
  }

  bool isSmall(const std::string& node)
  {
    return node.size() > 0 && std::islower(node[0]);
  }

  uint64_t dfs(const std::string& node = START, std::vector<std::string> visited = {})
  {
    if (node == END)
    {
      return 1;
    }

    if (nodeVisited(node, visited))
    {
      // Node already visited, just return.
      return 0;
    }

    if (isSmall(node))
    {
      visited.push_back(node);
    }

    uint64_t paths = 0;
    for (const auto& connection : nodes[node])
    {
      if (!nodeVisited(connection, visited) || !isSmall(connection))
      {
        paths += dfs(connection, visited);
      }
    }

    return paths;
  }

// Data fields
private:
  std::unordered_map<std::string, std::vector<std::string>> nodes;
};

std::ostream& operator<<(std::ostream& os, const Graph& graph)
{

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

  // Read input first to determine the connections
  Graph graph;
  for (const std::string& line : rawInput)
  {
    const std::vector<std::string>& connections = split(line, "-");
    const std::string& node1 = connections[0];
    const std::string& node2 = connections[1];
    graph.addNode(node1);
    graph.addNode(node2);
    graph.addConnection(node1, node2);
  }

  std::cout << "Initial graph:" << std::endl << std::flush;
  std::cout << graph << std::endl << std::flush;

  const uint64_t paths = graph.traverse();
  std::cout << "Number paths: " << paths << std::endl;
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