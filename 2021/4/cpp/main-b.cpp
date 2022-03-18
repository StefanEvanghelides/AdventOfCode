#include <iostream>
#include <fstream>
#include <string_view>
#include <string>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

class Field
{
public:
  Field(const size_t& n)
    : nr(n), checked(false)
  {}
  ~Field() = default;

  void check(const size_t& n)
  {
    if (nr == n)
      checked = true;
  }

  bool isChecked() const { return checked; }
  size_t getNr() const { return nr; }

private:
  size_t nr;
  bool checked;
};

class Board
{
public:
  Board(const std::vector<std::vector<Field>> r, const size_t& i = 0)
    : rows(r), idx(i)
  {}
  ~Board() = default;

  std::vector<std::vector<Field>> getRows() const { return rows; }
  size_t getIdx() const { return idx; }

  void check_nr(const size_t& n)
  {
    for (auto& row : rows)
    {
      for (auto& field : row)
      {
        field.check(n);
      }
    }
  }

  bool check_bingo()
  {
    // First check each row
    for (const auto& row : rows)
    {
      bool bingo = true;
      for (const auto& field : row)
      {
        if (!field.isChecked())
        {
          bingo = false;
          break;
        }
      }

      if (bingo)
      {
        return true;
      }
    }

    // Now check columns
    for (size_t j = 0; j < rows[0].size(); j++)
    {
      bool bingo = true;
      for (size_t i = 0; i < rows.size(); i++)
      {
        const Field& f = rows[i][j];
        if (!f.isChecked())
        {
          bingo = false;
          break;
        }
      }

      if (bingo)
      {
        return true;
      }
    }

    return false;
  }

  size_t sumUnchecked()
  {
    size_t sum = 0;
    for (const auto& row : rows)
    {
      for (const auto& field : row)
      {
        if (!field.isChecked())
        {
          sum += field.getNr();
        }
      }
    }

    return sum;
  }

private:
  size_t idx;
  std::vector<std::vector<Field>> rows;
};

std::ostream& operator<<(std::ostream& os, const Field& f)
{
  os << f.getNr() << " (" << f.isChecked() << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
  os << "Board:" << std::endl;
  for (const auto& row : b.getRows())
  {
    for (const auto& field : row)
    {
      os << field << " ";
    }
    os << std::endl;
  }
  return os;
}

std::string trimmed(const std::string& s)
{
  auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

std::vector<std::string> split(const std::string& s, const std::string& delim = " ")
{
  std::vector<std::string> tokens;

  size_t start = 0;
  size_t end = 0;
  while((end = s.find(delim, start)) != std::string::npos)
  {
    tokens.emplace_back(s.substr(start, end-start));
    start = end + delim.size();
    while (s.substr(start, delim.size()) == delim)
    {
      start += delim.size();
    }
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

  // First line is the input numbers.
  const std::vector<std::string>& numbersStr = split(rawInput[0], ",");
  std::vector<size_t> numbers;
  for (const std::string& s : numbersStr)
  {
    numbers.push_back(std::stoi(s));
  }

  // Now we expect groups of 5 lines for the boards.
  std::vector<Board> boards;
  for (size_t i = 1; i < rawInput.size(); i += 5)
  {
    std::vector<std::vector<Field>> rows;
    for (size_t j = 0; j < 5; j++)
    {
      const std::vector<std::string>& line = split(rawInput[i+j]);
      std::vector<Field> row;
      for (const std::string& nr : line)
      {
        row.push_back(Field(std::stoi(nr)));
      }
      rows.push_back(row);
    }
    boards.push_back(Board(rows, i/5));
  }

  // Now run the numbers
  for (const size_t& n : numbers)
  {
    bool bingo = false;
    std::unordered_map<size_t, size_t> bingos;
    for (Board& b : boards)
    {
      b.check_nr(n);
      if (b.check_bingo())
      {
        bingo = true;
        const size_t boardSum = b.sumUnchecked();
        bingos[b.getIdx()] = boardSum;
      }
    }

    if (bingo)
    {
      std::cout << std::endl;
      for (const auto& bin : bingos)
      {
        const size_t boardNr = bin.first;
        const size_t boardSum = bin.second;
        const size_t finalScore = boardSum * n;
        std::cout << "Final score: " << finalScore << std::endl;

        // Remove current board from the list
        boards.erase(
          std::remove_if(boards.begin(), boards.end(), [boardNr](const Board& board) -> bool {
            return board.getIdx() == boardNr;
          }),
          boards.end());
      }
    }

    if (boards.empty())
    {
      break;
    }
  }
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