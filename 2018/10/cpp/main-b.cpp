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
#include <optional>
#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;

using Strings = std::vector<std::string>;
using Strings2 = std::vector<Strings>;
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
using Point = std::pair<int64_t,int64_t>;
using Pairs = std::vector<std::pair<int64_t,int64_t>>;
using Pairs2 = std::vector<Pairs>;
using Line = std::pair<std::pair<int64_t,int64_t>,std::pair<int64_t,int64_t>>;
using Lines = std::vector<Line>;


Strings split(const std::string& s, const std::string& delim = " ", const bool keepSplitting = true)
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

std::string trimmed(const std::string& s)
{
  auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

struct Node
{
  Node() = default;
  ~Node() = default;

  Point point;
  Point velocity;

  void print() const
  {
    std::cout << "Point (" << point.first << "; " << point.second << ");  velo (" << velocity.first << "; " << velocity.second << ")" << std::endl;
  }
};

using Nodes = std::vector<Node>;

std::optional<Node> parseInputLine(const std::string& input)
{
  Node node;
  const Strings& parts = split(input, "> velocity=<");

  if (parts.size() != 2)
  {
    std::cout << "Input does not have 2 parts! " << input << std::endl;
    return {};
  }
  const Strings& positionParts = split(parts[0], "=<");
  if (positionParts.size() != 2)
  {
    std::cout << "Position parts does not have 2 parts! " << parts[0] << std::endl;
    return {};
  }
  // Here we only care about the 2nd part.
  const Strings& positions = split(positionParts[1], ",");
  if (positions.size() != 2)
  {
    std::cout << "Positions does not have 2 parts! " << positionParts[1] << std::endl;
    return {};
  }
  // Note: Axes are reversed.
  node.point = Point{std::stoll(positions[1]), std::stoll(positions[0])};

  // Now the velocity
  const Strings& velocityParts = split(parts[1], ">");
  // Here we care only about the first part
  if (velocityParts.size() < 1)
  {
    std::cout << "Velocity parts should have at least 1 element! " << parts[1] << std::endl;
    return {};
  }
  const Strings& velocities = split(velocityParts[0], ", ");
  if (velocities.size() != 2)
  {
    std::cout << "Velocities does not have 2 parts! " << velocityParts[0] << std::endl;
    return {};
  }
  // Note: Axes are reversed.
  node.velocity = Point{std::stoll(velocities[1]), std::stoll(velocities[0])};

  return node;
}


constexpr uint64_t MAX_ITERATIONS = 100000;
constexpr uint64_t WIDTH = 80;
constexpr uint64_t HEIGHT = 10;

void print(const Nodes& nodes, const int64_t& i0 = 0, const int64_t& i1 = HEIGHT, const int64_t& j0 = 0, const int64_t& j1 = WIDTH)
{
  const int64_t maxHeight = i1 - i0 + 1;
  const int64_t maxWidth = j1 - j0 + 1;
  if (maxHeight <= 0 || maxWidth <= 0)
  {
    std::cout << "Invalid inputs, width or height are not positive!" << std::endl;
    return;
  }

  Bools2 output(maxHeight, Bools(maxWidth, false));

  // Fill in the points.
  for (const Node& n : nodes)
  {
    const Point& p = n.point;
    const int64_t& x = p.first;
    const int64_t& y = p.second;
    if (x >= i0 && x <= i1 && y >= j0 && y <= j1)
    {
      output[x-i0][y-j0] = true;
    }
  }

  // Now print the box
  for (const Bools& row : output)
  {
    for (const bool&  b : row) std::cout << (b ? '#' : '.');
    std::cout << std::endl;
  }
}

void next(Nodes& nodes)
{
  // For each node, change the position by the velocity.
  for (Node& n : nodes)
  {
    n.point.first += n.velocity.first;
    n.point.second += n.velocity.second;
  }
}

uint64_t pointsArea(const Nodes& nodes)
{
  // Find the area with the most points, use the WIDTH, HEIGHT as mask windows.
  // First, find the extremes of the points.
  int64_t minX = 1e10, minY = 1e10;
  int64_t maxX = -1e10, maxY = -1e10;

  for (const Node& n : nodes)
  {
    const int64_t& x = n.point.first;
    const int64_t& y = n.point.second;
    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;
  }

  const uint64_t dx = uint64_t(maxX - minX);
  const uint64_t dy = uint64_t(maxY - minY);
  return dx * dy;
}


void solve(const Strings& rawInput)
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

  Nodes nodes;
  for (const std::string& input : rawInput)
  {
    const auto& nodeOpt = parseInputLine(input);
    if (!nodeOpt.has_value())
    {
      std::cout << "Node has no value! Skipping.." << std::endl;
      continue;
    }

    const Node node = nodeOpt.value();
    // node.print();

    nodes.push_back(node);
  }

  // Now it's time for some magic: we need to see when do we actually see a message.
  // The input has number ranging -50k ... 50k. We want a small box for perhaps 50 x 10.
  uint64_t iter = 0;
  uint64_t minArea = 1e10;
  Nodes minNodes;
  uint64_t minIter;
  while (iter++ < MAX_ITERATIONS)
  {
    // During each iterations, move nodes, then print them.
    next(nodes);
    // std::cout << "Iteration " << iter << std::endl;
    // print(nodes);

    const uint64_t& area = pointsArea(nodes);
    if (area < minArea)
    {
      minArea = area;
      minNodes = nodes;
      minIter = iter;
    }
    // std::cout << "Area: " << area << std::endl;

    if (area > 2 * minArea)
    {
      // std::cout << "Area enlarged too much to continue" << std::endl;
      break;
    }

  }

  // Now show the min nodes
  int64_t minX = 1e10, minY = 1e10;
  int64_t maxX = -1e10, maxY = -1e10;
  for (const Node& n : minNodes)
  {
    const int64_t& x = n.point.first;
    const int64_t& y = n.point.second;
    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;
  }
  print(minNodes, minX, maxX, minY, maxY);
  std::cout << "Message appeared after " << minIter << " seconds" << std::endl;
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