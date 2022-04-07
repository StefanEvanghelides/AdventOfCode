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
#include <filesystem>
namespace fs = std::filesystem;

// Forward declaration
class Node;
class Graph;

using SpNode = std::shared_ptr<Node>;
using Tuple = std::tuple<uint64_t, SpNode>;
//using PrioQueue = std::priority_queue<Tuple, std::vector<Tuple>, std::greater<Tuple>>;
using Vector = std::vector<Tuple>;

class Node
{
public:
  Node() = default;
  ~Node() = default;

  static uint64_t STATIC_IDX;
  uint64_t _idx;

  Node(const uint64_t idx, const uint64_t v)
    : index(idx), value(v), cost(-1), pos({})
  {
    _idx = STATIC_IDX;
    STATIC_IDX++;
  }
  
  Node(const uint64_t idx, const uint64_t v, uint64_t i, uint64_t j)
    : index(idx), value(v), cost(-1), pos({i,j})
  {
    _idx = STATIC_IDX;
    STATIC_IDX++;
  }

  uint64_t getIndex() const { return index; }
  void setIndex(const uint64_t idx) { index = idx; }

  uint64_t getValue() const { return value; }
  void setValue(const uint64_t val) { value = val; }

  std::pair<uint64_t,uint64_t> getPos() const { return pos; }
  void setPos(const std::pair<uint64_t,uint64_t> p) { pos = p; }

  void addAdjacentNode(const SpNode& n) { adjacencies.emplace_back(n); }
  std::vector<SpNode> getAdjacencies() const { return adjacencies; }
  void setAdjancecies(const std::vector<SpNode> adj) { adjacencies = adj; }


  int64_t getCost() const { return cost; }
  void setCost(const int64_t c) { cost = c; }
  bool isVisited() const { return cost != -1; }
  void reset() { cost = -1; }


// Data fields
private:
  uint64_t index;
  uint64_t value;
  std::pair<uint64_t,uint64_t> pos;
  std::vector<SpNode> adjacencies;

  int64_t cost;
};

uint64_t Node::STATIC_IDX = 0;

std::ostream& operator<<(std::ostream& os, const SpNode& n)
{
  os << "Node: " << n->getValue()
    << " - pos=(" << n->getPos().first << "," << n->getPos().second
    << ") (idx=" << n->getIndex() << " - INTERNAL IDX=" << n->_idx
    << ") (visited=" << n->isVisited() << " - cost=" << n->getCost()
    << ") -> adjs: ";
  for (const auto& adj : n->getAdjacencies())
  {
    os << adj->getValue() << " (idx=" << adj->getIndex() << ") ";
  }
  return os;
}

void sort(Vector& v)
{
  std::sort(v.begin(), v.end(), [](const Tuple& a, const Tuple&b)
  {
    if (std::get<0>(a) == std::get<0>(b))
    {
      return std::get<1>(a)->getIndex() < std::get<1>(b)->getIndex();
    }
    return std::get<0>(a) < std::get<0>(b);
  });
}

void pop(Vector& v)
{
  v.erase(v.begin());
}

void show(const Vector& v)
{
  for (const auto& t : v)
  {
    std::cout << "  Cost=" << std::get<0>(t) << " -> " << std::get<1>(t) << std::endl;
  }
}

void show (const std::vector<std::vector<uint64_t>>& values, const bool useSep = false, const uint64_t sepDistX = 1, const uint64_t sepDistY = 1)
{
  for (uint64_t i = 0; i < values.size(); ++i)
  {
    if (useSep)
    {
      if (i > 0 && i % sepDistX == 0)
      {
        std::cout << std::endl;
      }
    }
    for (uint64_t j = 0; j < values[0].size(); ++j)
    {
      if (useSep)
      {
        if (j > 0 && j % sepDistY == 0)
        {
          std::cout << " ";
        }
      }
      std::cout << values[i][j];
    }
    std::cout << std::endl;
  }
}

class Graph
{
public:
  Graph() = default;
  ~Graph() = default;

  Graph(const std::vector<std::vector<uint64_t>>& values)
  {
    // First add the nodes in 2D array
    std::vector<std::vector<SpNode>> n;
    uint64_t idx = 0;
    for (uint64_t i = 0; i < values.size(); ++i)
    {
      std::vector<SpNode> v;
      for (uint64_t j = 0; j < values[i].size(); ++j)
      {
        const uint64_t val = values[i][j];
        v.push_back(std::make_shared<Node>(idx, val, i, j));
        ++idx;
      }
      n.push_back(v);
    }

    // Then add the adjacency values.
    for (uint64_t i = 0; i < n.size(); ++i)
    {
      for (uint64_t j = 0; j < n[0].size(); ++j)
      {
        std::vector<SpNode> adjs;
        if (i > 0) adjs.push_back(n[i-1][j]);
        if (j > 0) adjs.push_back(n[i][j-1]);
        if (i < n.size()-1) adjs.push_back(n[i+1][j]);
        if (j < n[0].size()-1) adjs.push_back(n[i][j+1]);

        n[i][j]->setAdjancecies(adjs);
      }
    }

    // Now flatten the 2D array into the main nodes array
    for (const auto& vec : n)
    {
      for (const auto& v : vec)
      {
        nodes.push_back(v);
      }
    }
  }

  std::vector<SpNode> getNodes() const { return nodes; }
  void setNodes(const std::vector<SpNode>& n) { nodes = n; }

  uint64_t computeLowestRisk()
  {
    resetNodes();
    uint64_t lowestSum = aStar(nodes[0], nodes.back());
    return lowestSum;
  }


// Functions
private:
  int64_t isPresent(const Vector& vector, const SpNode& node)
  {
    for (int64_t i = 0; i < vector.size(); ++i)
    {
      const Tuple& elem = vector[i];
      const SpNode& nodeElem = std::get<1>(elem);
      if (nodeElem->getIndex() == node->getIndex())
      {
        return i;
      }
    }

    return -1;
  }
  
  uint64_t distance(const SpNode& node1, const SpNode& node2)
  {
    // Let's try Manhattan distance first.
    const uint64_t deltaX = (uint64_t)abs((int64_t)(node1->getPos().first - node2->getPos().first));
    const uint64_t deltaY = (uint64_t)abs((int64_t)(node1->getPos().second - node2->getPos().second));
    return deltaX + deltaY;
  }

  uint64_t visitedNodes()
  {
    return std::count_if(nodes.begin(), nodes.end(), [](const SpNode& node) {
      return node->isVisited();
    });
  }

  // Uses the Manhattan distance as the heuristic function.
  uint64_t aStar(const SpNode& start, const SpNode& end)
  {
    // Use a priority queue to check open nodes
    Vector open;

    // Add starting node as the top of the queue
    open.emplace_back(0, start);
    uint64_t visitedNodes = 0;
    while(!open.empty() && visitedNodes < nodes.size())
    {
      // Get the top element position
      const Tuple& top = open.front();
      const uint64_t cost = std::get<0>(top);
      const SpNode node = std::get<1>(top);

      // If we reached the end then print the cost.
      if (node->getIndex() == end->getIndex())
      {
        return cost;
      }

      // Remove top element.
      pop(open);

      // If node was already visited, then simply return.
      // The prio queue guarantees that the node has the lowest cost already.
      if (node->isVisited())
      {
        continue;
      }

      node->setCost(cost);
      visitedNodes++;

      //std::cout << "Queue size: " << open.size() << " TopNode cost=" << cost << " TopNode: " << node << std::endl;
      std::cout << "Queue size: " << open.size() << " VisitedNodes=" << visitedNodes << " (max= " << nodes.size() << ")" << std::endl;

      // Check the node's neighbours now to add to the queue.
      for (const SpNode& adj : node->getAdjacencies())
      {
        const uint64_t newCost = cost + adj->getValue();
        if (adj->getIndex() == end->getIndex())
        {
          return newCost;
        }

        if (!adj->isVisited())
        {
          open.push_back(std::make_tuple(newCost, adj));
          sort(open);
        }
      }
      
      //show(open);
    }

    return 0;
  }

  void resetNodes()
  {
    for (const auto& node : nodes)
    {
      node->reset();
    }
  }

// Data fields
private:
  std::vector<SpNode> nodes;
};

std::ostream& operator<<(std::ostream& os, const Graph& g)
{
  os << "Graph:" << std::endl;
  for (const auto& node : g.getNodes())
  {
    os << node << std::endl;
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

  std::vector<std::vector<uint64_t>> values;
  for (const std::string& line : rawInput)
  {
    std::vector<uint64_t> vec;
    for (const char& c : line)
    {
      const uint64_t digit = c - '0';
      vec.push_back(digit);
    }
    values.push_back(vec);
  }
  const uint64_t sizeX = values.size();
  const uint64_t sizeY = values[0].size();

  // Now for part b, extend this input into a 5x5 matrix.
  // Also process the new values.
  const uint64_t MAX_I = 5;
  const uint64_t MAX_J = 5;
  std::vector<std::vector<std::vector<std::vector<uint64_t>>>> fullValues;
  for (uint64_t i = 0; i < MAX_I; ++i)
  {
    std::vector<std::vector<std::vector<uint64_t>>> almostFullValues;
    for (uint64_t j = 0; j < MAX_J; ++j)
    {
      std::vector<std::vector<uint64_t>> newValues = values;
      for (uint64_t x = 0; x < sizeX; ++x)
      {
        for (uint64_t y = 0; y < sizeY; ++y)
        {
          newValues[x][y] = (newValues[x][y] + i + j - 1) % 9 + 1;
        }
      }
      almostFullValues.push_back(newValues);
    }
    fullValues.push_back(almostFullValues);
  }

  // Now collapse the 4D vectors into 2D.
  std::vector<std::vector<uint64_t>> newValues;
  for (uint64_t i = 0; i < MAX_I * sizeX; ++i)
  {
    std::vector<uint64_t> partialValues;
    for (uint64_t j = 0; j < MAX_J * sizeY; ++j)
    {
      const uint64_t group_i = i / sizeX;
      const uint64_t group_j = j / sizeY;
      const uint64_t relative_i = i % sizeX;
      const uint64_t relative_j = j % sizeY;
      const uint64_t val = fullValues[group_i][group_j][relative_i][relative_j];
      partialValues.push_back(val);
    }
    newValues.push_back(partialValues);
  }

  //// Now show the new input fields
  // show(values);
  // std::cout << " ------------------- " << std::endl;
  // show(newValues, true, sizeX, sizeY);

  Graph graph(newValues);

  // std::cout << graph << std::endl;

  const uint64_t lowestSum = graph.computeLowestRisk();
  std::cout << "Lowest risk: " << lowestSum << std::endl;

  //std::cout << graph << std::endl;

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