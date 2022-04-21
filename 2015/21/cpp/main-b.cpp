#define NOMINMAX
#include <windows.h> // for nice formatted output
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences?redirectedfrom=MSDN

#include <limits.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <cmath>
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
using Numbers = std::vector<uint64_t>;
using Numbers2 = std::vector<Numbers>;
using Bools = std::vector<bool>;
using Bools2 = std::vector<Bools>;
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

struct Item
{
  int64_t cost;
  int64_t damage;
  int64_t armor;

  Item() = default;
  ~Item() = default;
  Item(const int64_t& c, const int64_t& d, const int64_t& a)
    : cost(c), damage(d), armor(a)
  {}
};

struct Weapon : Item
{
  Weapon() = default;
  ~Weapon() = default;
  Weapon(const int64_t& c, const int64_t& d)
    : Item(c, d, 0)
  {}
};

struct Armor : Item
{
  Armor() = default;
  ~Armor() = default;
  Armor(const int64_t& c, const int64_t& a)
    : Item(c, 0, a)
  {}
};

struct Ring : Item
{
  Ring() = default;
  ~Ring() = default;
  Ring(const int64_t& c, const int64_t& d, const int64_t& a)
    : Item(c, d, a)
  {}
};

struct Player
{
  int64_t hitpoints;
  int64_t damage;
  int64_t armor;
  int64_t gold;

  Player() = default;
  ~Player() = default;
  Player(const int64_t& h, const int64_t& d, const int64_t& a)
    : hitpoints(h), damage(d), armor(a), gold(0)
  {}

  Player applyItems(std::vector<Item> items) const
  {
    Player copy = *this;
    for (const Item& i : items)
    {
      copy.damage += i.damage;
      copy.armor += i.armor;
      copy.gold += i.cost;
    }
    return copy;
  }
};

std::ostream& operator<<(std::ostream& os, const Player& p)
{
  os << "(Hitpoints: " << p.hitpoints << "  damage: " << p.damage << "  armor: " << p.armor << ")";
  return os;
}

constexpr uint64_t MAX_HITS = 1000;

// Returns true if the player wins.
bool playGame(Player p, Player opponent)
{
  bool turn = false;
  uint64_t iter = 0;
  while (p.hitpoints > 0 && opponent.hitpoints > 0 && iter++ < MAX_HITS)
  {
    turn = !turn;
    if (turn)
    {
      // It's our turn! Inflict damage to the opponent.
      const int64_t value = std::max(0LL, p.damage - opponent.armor);
      opponent.hitpoints -= value;
    }
    else
    {
      // Opponent's turn!
      const int64_t value = std::max(0LL, opponent.damage - p.armor);
      p.hitpoints -= value;
    }
  }

  return opponent.hitpoints <= 0;
}

std::vector<std::vector<Item>> generatePermutations(const std::vector<Weapon>& weapons, const std::vector<Armor>& armors, const std::vector<Ring>& rings)
{
  std::vector<std::vector<Item>> permutations;

  // Currently, the game has the following constraints:
  // - exactly 1 weapon
  // - armor is optional, at most 1.
  // - rings optional, at most 2.
  for (const auto& w : weapons)
  {
    std::vector<Item> perm;

    // All combinations of just 1 weapon.
    perm = {w};
    permutations.push_back(perm);

    for (const auto& a : armors)
    {
      // all combinations of weapon-armors-0rings.
      perm = {w, a};
      permutations.push_back(perm);

      // all combinations of weapon-armors-1ring.
      for (uint64_t i = 0; i < rings.size()-1; ++i)
      {
        const Ring& r1 = rings[i];
        perm = {w, a, r1};
        permutations.push_back(perm);

        // all combinations of weapon-armors-2rings.
        for (uint64_t j = i+1; j < rings.size(); ++j)
        {
          const Ring& r2 = rings[j];
          perm = {w, a, r1, r2};
          permutations.push_back(perm);
        }
      }
    }

    // Then there are also all combinations of weapon-rings.
    for (uint64_t i = 0; i < rings.size()-1; ++i)
    {
      const Ring& r1 = rings[i];
      perm = {w, r1};
      permutations.push_back(perm);

      // all combinations of weapon-armors-2rings.
      for (uint64_t j = i+1; j < rings.size(); ++j)
      {
        const Ring& r2 = rings[j];
        perm = {w, r1, r2};
        permutations.push_back(perm);
      }
    }
  }

  return permutations;
}

void solve(const Strings& rawInput)
{
  // std::cout << "Printing the input inside solution():" << std::endl;
  // for (auto line : rawInput) {
  //   std::cout << "Line: " << line << std::endl;
  // }

  Player p {100, 0, 0};
  Player opponent;
  for (const std::string& inp : rawInput)
  {
    const Strings& tokens = split(inp, ": ");
    if (tokens.size() != 2)
    {
      std::cout << "Incorrect input line! Line: " << inp << std::endl;
      continue;
    }

    // The value is on the second item.
    const int64_t& value = std::stoll(tokens[1]);
    if (tokens[0][0] == 'H') opponent.hitpoints = value;
    if (tokens[0][0] == 'D') opponent.damage = value;
    if (tokens[0][0] == 'A') opponent.armor = value;
  }

  // Now let's add some items.
  // Normally these would be separate objects, but there's no point in doing it here.

  // Weapons
  std::vector<Weapon> weapons;
  weapons.emplace_back(8, 4);
  weapons.emplace_back(10, 5);
  weapons.emplace_back(25, 6);
  weapons.emplace_back(40, 7);
  weapons.emplace_back(74, 8);

  // Armors
  std::vector<Armor> armors;
  armors.emplace_back(13, 1);
  armors.emplace_back(31, 2);
  armors.emplace_back(53, 3);
  armors.emplace_back(75, 4);
  armors.emplace_back(102, 5);

  // Rings
  std::vector<Ring> rings;
  rings.emplace_back(25, 1, 0);
  rings.emplace_back(50, 2, 0);
  rings.emplace_back(100, 3, 0);
  rings.emplace_back(20, 0, 1);
  rings.emplace_back(40, 0, 2);
  rings.emplace_back(80, 0, 3);

  // Now generate all possible permutations of items
  std::vector<std::vector<Item>> permutations = generatePermutations(weapons, armors, rings);
  std::cout << "Total permutations: " << permutations.size() << std::endl;

  // For part b) this is an easy change: most amount the gold and still lose.
  int64_t maxCost = 0;
  for (const std::vector<Item> perm : permutations)
  {
    Player updated = p.applyItems(perm);
    if (updated.gold > maxCost && !playGame(updated, opponent))
    {
      maxCost = updated.gold;
    }
  }
  std::cout << "Max cost: " << maxCost << std::endl;
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