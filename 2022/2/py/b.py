import argparse

# Rock, paper, scissors
OPP_ENC: int = ord('A')
ME_ENC: int = ord('X')

DRAW_SCORE: int = 3
WIN_SCORE: int = 6
LOSE_SCORE: int = 0

def findWinShape(opp: int):
  if opp == 1:
    return 2
  if opp == 2:
    return 3
  return 1

def findLossShape(opp: int):
  if opp == 2:
    return 1
  if opp == 3:
    return 2
  return 3

# Now the inputs are chars
def compute(opp: int, me: int) -> int:
  # I need to win
  if me == 3:
    return WIN_SCORE + findWinShape(opp)


  # I need to draw
  if me == 2:
    return DRAW_SCORE + opp

  # I need to lose
  return LOSE_SCORE + findLossShape(opp)


def solve(raw_input: list[str]):
  inp_pairs: list[str] = raw_input.split('\n')
  total: int = 0
  for inp_pair in inp_pairs:
    vals: list[str] = inp_pair.split()
    opp: int = ord(vals[0]) - OPP_ENC + 1
    me: int = ord(vals[1]) - ME_ENC + 1

    # Part b) has a different encoding now
    total += compute(opp, me)

  print(f"{total=}")


def main(filename: str):
  # Read file and convert to ints
  input = []
  with open(filename) as file:
    input = file.read()
    solve(input)

if __name__ == '__main__':

  # Simple argparse for the input
  parser = argparse.ArgumentParser()
  parser.add_argument("filename", type=str, nargs="?", default="input",
        help="Enter the name of the file (default = input)")
  parser.add_argument("-i", "--interactive", default="false", action="store_true", dest="interactive",
        help="If flag is set, the input is interactive")
  args = parser.parse_args()

  # Run the program
  main(args.filename)