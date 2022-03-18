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

  std::unordered_map<std::string, int> field;
  for (const std::string& line : rawInput)
  {
    const std::vector<std::string>& coords = split(line, " -> ");
    const std::vector<std::string>& startCoords = split(coords[0], ",");
    const std::vector<std::string>& endCoords = split(coords[1], ",");

    size_t startX = std::stoi(startCoords[0]);
    size_t startY = std::stoi(startCoords[1]);
    const size_t endX = std::stoi(endCoords[0]);
    const size_t endY = std::stoi(endCoords[1]);

    const int facX = (startX < endX) - (startX > endX);
    const int facY = (startY < endY) - (startY > endY);

    startX -= facX;
    startY -= facY;
    do
    {
      startX += facX;
      startY += facY;

      const std::string fullCoord = std::to_string(startX) + "*" + std::to_string(startY);
      if (field.find(fullCoord) == field.end())
      {
        field[fullCoord] = 1;
      }
      else
      {
        field[fullCoord]++;
      }

    } while (startX != endX || startY != endY);
  }

  size_t count = 0;
  for (const auto& [_, value] : field)
  {
    count += (value > 1);
  }

  std::cout << "Final count: " <<  count << std::endl;
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