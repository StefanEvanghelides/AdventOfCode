import argparse

from dataclasses import dataclass

@dataclass
class Field:
  nr: int
  checked: bool

  def __init__(self, nr: int):
    self.nr = nr
    self.checked = False

  def __str__(self):
    return f"{self.nr}({self.checked})"

  def __repr__(self):
    return f"{self.nr}({self.checked})"

  def check(self, nr: int):
    if (nr == self.nr):
      self.checked = True


@dataclass
class Board:
  rows: list[list[Field]]
  idx: int

  def __init__(self, idx, rows: list[list[Field]]):
    self.rows = rows
    self.idx = idx

  def __str__(self):
    str = f"\nBoard ({self.idx})\n"
    for row in self.rows:
      for field in row:
        str += field.__str__() + "  "
      str += "\n"
    return str

  def __repr__(self):
    str = f"\nBoard ({self.idx})\n"
    for row in self.rows:
      for field in row:
        str += field.__str__() + "  "
      str += "\n"
    return str

  def check_nr(self, nr: int):
    for row in self.rows:
      for field in row:
        field.check(nr)

  def check_bingo(self) -> bool:
    # First check each row
    for row in self.rows:
      bingo = True
      for field in row:
        if not field.checked:
          bingo = False
          break

      if bingo:
        return True


    # Now check columns
    for j in range(0,len(self.rows[0])):
      bingo = True
      for i in range(0,len(self.rows)):
        f = self.rows[i][j]
        if not f.checked:
          bingo = False
          break
      
      if bingo:
        return True

    return False

  def sum_unchecked(self) -> int:
    sum_unchecked = 0
    for row in self.rows:
      for field in row:
        if not field.checked:
          sum_unchecked += field.nr

    return sum_unchecked


def solve(raw_input):
  input = raw_input.split('\n')
  numbers = [int(x) for x in input[0].split(',')]
  boards = []
  b = []
  idx = 0
  for i in range(1,len(input)):
    x = input[i]
    if not x:
      if b:
        board = Board(idx, b)
        boards.append(board)
        idx += 1
      b = []
      continue

    b.append([Field(int(nr)) for nr in  x.split()])

    if i == len(input)-1:
      board = Board(idx, b)
      boards.append(board)
      break

  for nr in numbers:
    bingo = False
    bingos = []
    for board in boards:
      board.check_nr(nr)
      if board.check_bingo():
        bingo = True
        board_sum = board.sum_unchecked()
        bingos.append((board.idx, board_sum))

    if bingo:
      print(f" ")
      for bin in bingos:
        board_nr, board_sum = bin
        final_score = board_sum * nr
        print(f"{final_score=}")
        boards = [board for board in boards if board.idx != board_nr]

    if not boards:
      break

def main(filename):
  # Read file and convert to ints
  input = []
  with open(filename) as file:
    input = file.read()
    solve(input)

if __name__ == '__main__':

  # Simple argparse for the input
  parser = argparse.ArgumentParser()
  parser.add_argument("filename", type=str, nargs="?", default="input.txt",
        help="Enter the name of the file (default = input.txt)")
  parser.add_argument("-i", "--interactive", default="false", action="store_true", dest="interactive",
        help="If flag is set, the input is interactive")
  args = parser.parse_args()

  # Run the program
  main(args.filename)