#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <string>
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

  std::unordered_map<size_t, std::string> signals;
  signals[0] = "abcefg";
  signals[1] = "cf";
  signals[2] = "acdeg";
  signals[3] = "acdfg";
  signals[4] = "bcdf";
  signals[5] = "abdfg";
  signals[6] = "abdefg";
  signals[7] = "acf";
  signals[8] = "abcdefg";
  signals[9] = "abcdfg";

  std::uint64_t sum = 0;
  for (const std::string& rawLine : rawInput)
  {
    std::unordered_map<size_t, std::string> signalPossibilities;
    for (size_t i = 0; i < 7; ++i)
    {
      signalPossibilities[i] = "abcdefg";
    }

    const std::vector<std::string> sides = split(rawLine, "|");
    const std::vector<std::string> firstPart = split(trimmed(sides[0]));
    const std::vector<std::string> secondPart = split(trimmed(sides[1]));

    // Determine top part: check 7 (3nrs) and 1 (2nrs)
    std::string one, four, seven;
    for (size_t i = 0; i < firstPart.size(); ++i)
    {
      const std::string current = firstPart[i];
      if (current.size() == 2)
      {
        one = current;
      }
      else if (current.size() == 3)
      {
        seven = current;
      }
      else if (current.size() == 4)
      {
        four = current;
      }
    }
    char topChar = 0;
    char char1 = 0 , char2 = 0;
    for (const char& c : seven)
    {
      if (one.find(c) == std::string::npos)
      {
        topChar = c;
      }
      else if (char1 == 0)
      {
        char1 = c;
      }
      else
      {
        char2 = c;
      }
    }
    // TOP & RIGHT: Update all positions now. We are sure that there is a topChar.
    for (auto& sig : signalPossibilities)
    {
      auto& v = sig.second;
      v.erase(std::remove(v.begin(), v.end(), topChar), v.end());
      v.erase(std::remove(v.begin(), v.end(), char1), v.end());
      v.erase(std::remove(v.begin(), v.end(), char2), v.end());
    }
    signalPossibilities[0] = topChar;
    signalPossibilities[2] = {char1, char2};
    signalPossibilities[5] = {char1, char2};

    // We can do a similar thing for 2, 3, 5 -> they have 3 common sides
    std::string two, three, five;
    for (const std::string& s : firstPart)
    {
      if (s.size() == 5)
      {
        if (two.empty()) two = s;
        else if (three.empty()) three = s;
        else five = s;
      }
    }
    // Now find the 3 common chars in these 3 strings.
    // We also know that top char is part of them, so we can eliminate it.
    char1 = 0; char2 = 0;
    for (const char& c : two)
    {
      if (c == topChar) continue;
      if (three.find(c) != std::string::npos &&
          five.find(c) != std::string::npos)
      {
        if (char1 == 0) char1 = c;
        else char2 = c;
      }
    }

    for (auto& sig : signalPossibilities)
    {
      auto& v = sig.second;
      v.erase(std::remove(v.begin(), v.end(), char1), v.end());
      v.erase(std::remove(v.begin(), v.end(), char2), v.end());
    }
    signalPossibilities[3] = {char1, char2};
    signalPossibilities[6] = {char1, char2};

    
    // Now look at 4 again. We know one of the middle lines from two,three,five.
    // The other one will be our b(1).
    char char3 = 0, char4 = 0;
    for (const char& c : four)
    {
      if (c == char1 || c == char2) char3 = c;

      if (c != char1 && c != char2 && c != char3 &&
          signalPossibilities[1].find(c) != std::string::npos)
      {
        char4 = c;
      }
    }

    // we found b(1) and d(3). The complement will be filled in automatically.
    for (auto& sig : signalPossibilities)
    {
      auto& v = sig.second;
      v.erase(std::remove(v.begin(), v.end(), char3), v.end());
      v.erase(std::remove(v.begin(), v.end(), char4), v.end());
    }
    signalPossibilities[1] = char4;
    signalPossibilities[3] = char3;
    
    for (auto& sig : signalPossibilities)
    {
      auto& v = sig.second;
      v.erase(std::remove(v.begin(), v.end(), char3), v.end());
      v.erase(std::remove(v.begin(), v.end(), char4), v.end());
    }
    signalPossibilities[3] = char3;
    signalPossibilities[1] = char4;


    // Almost there: now use b(1) = char4 to find the Five.
    std::string actualFive;
    if (two.find(char4) != std::string::npos) actualFive = two;
    if (three.find(char4) != std::string::npos) actualFive = three;
    if (five.find(char4) != std::string::npos) actualFive = five;
    // Now we can finally determine f(5).
    char f = 0;
    for (const char& c : actualFive)
    {
      if (signalPossibilities[5].find(c) != std::string::npos)
      {
        f = c;
        break;
      }
    }
    for (auto& sig : signalPossibilities)
    {
      auto& v = sig.second;
      v.erase(std::remove(v.begin(), v.end(), f), v.end());
    }
    signalPossibilities[5] = f;

    // for (const auto& s : signalPossibilities)
    // {
    //   std::cout << "POS: " << s.first << " - " << s.second << std::endl;
    // }
    std::cout << "Configuration: ";
    for (size_t i = 0; i < signalPossibilities.size(); ++i) std::cout << signalPossibilities[i];
    std::cout << std::endl;

    // Compute map for quick computations.
    std::unordered_map<char, char> correctSignals;
    for (const auto& s : signalPossibilities)
    {
      correctSignals[s.second[0]] = 'a' + s.first;
    }

    // Now compute the numbers on the second part.
    int nr = 0;
    for (const std::string& str : secondPart)
    {
      std::string decodedString;
      for (const char& c : str)
      {
        decodedString += correctSignals[c];
      }

      std::cout << "STR=" << str << " (" << str.size() << ") - decoded=" << decodedString << std::endl;

      for (const auto& possibility : signals)
      {
        const std::string& s = possibility.second;
        if (s.size() != str.size()) continue;
        bool found = true;
        for (const char& c : decodedString)
        {

          if (s.find(c) == std::string::npos)
          {
            found = false;
            break;
          }
        }

        //std::cout << " - Checked possibility: " << possibility.first << " - " << s << " -> found=" << found << std::endl;

        if (found)
        {
          nr *= 10;
          nr += possibility.first;
          break;
        }
      }
    }
    std::cout << "NUMBER=" << nr << std::endl;
    sum += nr;
  }

  std::cout << "Sum: " << sum << std::endl;
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