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

uint64_t computeBinaryNumber(const std::vector<std::string>& image, const uint64_t& i, const uint64_t& j)
{
  std::string binaryStr(9, '0');
  binaryStr[0] = image[i-1][j-1];
  binaryStr[1] = image[i-1][j];
  binaryStr[2] = image[i-1][j+1];
  binaryStr[3] = image[i][j-1];
  binaryStr[4] = image[i][j];
  binaryStr[5] = image[i][j+1];
  binaryStr[6] = image[i+1][j-1];
  binaryStr[7] = image[i+1][j];
  binaryStr[8] = image[i+1][j+1];

  // Convert it into an actual number
  const uint64_t& binaryNr = std::stoll(binaryStr, 0, 2);

  return binaryNr;
}

// Padding should be done outside this function.
void enhanceImage(const std::string& algorithm, std::vector<std::string>& image)
{
  const uint64_t N = image.size();
  std::vector<std::string> img = image;
  for (uint64_t i = 1; i < N - 1; ++i)
  {
    for (uint64_t j = 1; j < N - 1; ++j)
    {
      const uint64_t& binaryNr = computeBinaryNumber(image, i, j);

      // Fetch the new character
      const char& c = algorithm[binaryNr];

      // Apply it to the IMG copy of the input image.
      img[i][j] = c;
    }
  }
  image = img;
}

void pad(std::vector<std::string>& image)
{
  // Add top and bottom rows.
  const std::string emptyLine(image.size(), '0');
  image.insert(image.begin(), emptyLine);
  image.push_back(emptyLine);

  for (std::string& line : image)
  {
    line.insert(line.begin(), '0');
    line.push_back('0');
  }
}

void unpad(std::vector<std::string>& image)
{
  // Remove top and bottom rows
  image.pop_back();
  image.erase(image.begin());

  for (std::string& line : image)
  {
    line.pop_back();
    line.erase(0, 1);
  }
}

void print(const std::vector<std::string>& image)
{
  for (const std::string& line : image)
  {
    std::cout << "IMG: " << line << std::endl;
  }
}

void solve(const std::vector<std::string>& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  // Transform inputs from '.' and '#' into '0' and '1'
  std::vector<std::string> copyRawInput = rawInput;
  for (std::string& line : copyRawInput)
  {
    for (char& c : line)
    {
      if (c == '.') c = '0';
      else if (c == '#') c = '1';
    }
  }

  //print(copyRawInput);

  const std::string& algorithm = copyRawInput[0];
  std::vector<std::string> image(copyRawInput.begin()+1, copyRawInput.end());

  //print(image);

  const uint64_t MAX_PADS = 100;
  for (uint64_t i = 0 ; i < MAX_PADS; ++i) pad(image);


  const uint64_t MAX_ITERATIONS = 50;
  for (uint64_t i = 0; i < MAX_ITERATIONS; ++i)
  {
    enhanceImage(algorithm, image);

    // std::cout << "Showing image: " << std::endl;
    // print(image);
    // std::cout << std::endl;
  }

  // Unpad the image back
  for (uint64_t i = 0; i < MAX_PADS - MAX_ITERATIONS; ++i) unpad(image);

  // std::cout << "Final image: " << std::endl;
  // print(image);
  // std::cout << std::endl;

  // Count lit pixels now
  uint64_t count = 0;
  for (const std::string& line : image)
  {
    for (const char& c : line)
    {
      count += (c - '0');
    }
  }
  std::cout << "Lit up pixels are: " << count << std::endl;

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