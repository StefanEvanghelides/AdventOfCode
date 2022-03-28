import argparse

def permutations(lst: list[int]):
  import itertools
  return itertools.permutations(lst)


def computeHappiness(arrangements: dict[str, dict[str,int]], indices: dict[int,str], perm: list[int]) -> int:
  happiness: int = 0
  for i in range(len(perm)):
    p: int = perm[i]
    leftIdx: int = perm[i-1] if i > 0 else perm[-1]
    rightIdx: int = perm[i+1] if i < len(perm)-1 else perm[0]

    pers: str = indices[p]
    left: str = indices[leftIdx]
    right: str = indices[rightIdx]

    # Find the happiness of the current person for the 2 neighbours.
    happiness += arrangements[pers][left]
    happiness += arrangements[pers][right]

  return happiness


def optimalArrangement(arrangements: dict[str, dict[str,int]]) -> int:
  # Basically find all combinations and see which is the best.

  # Give index numbers to the persons.
  indices: dict[int,str] = {}
  idx: int = 0
  for key in arrangements:
    indices[idx] = key
    idx += 1

  start: list[int] = [x for x in range(idx)]
  # print(f"{start=}")
  
  optimal: int = 0
  for perm in permutations(start):
    # print(f"{perm=}")
    happiness: int = computeHappiness(arrangements, indices, perm)
    if happiness > optimal:
      optimal = happiness

  return optimal


def solve(raw_input) -> None:
  inputs = raw_input.split('\n')

  arrangements: dict[str, dict[str,int]] = {}
  for inp in inputs:
    tokens = inp.split(' ')
    # Guard: There should always be 11 elements.
    if len(tokens) != 11:
      print(f"There are not 11 elements, but {len(tokens)}!")
      continue

    # Determine the sign.
    sign: int = (tokens[2][0] == 'g') - (tokens[2][0] == 'l')

    # Now fetch the numbers.
    currentPerson: str = tokens[0]
    otherPerson: str = tokens[10][:-1] # Remove the '.' at the end.
    unit: int = int(tokens[3])

    # Add the numbers to the arrangement.
    if currentPerson not in arrangements:
      arrangements[currentPerson] = {}

    arrangements[currentPerson][otherPerson] = unit * sign

  # print(f"{arrangements=}")

  # Now search for the optimal arrangement.
  optimal: int = optimalArrangement(arrangements)
  print(f"Optimal arrangement happiness: {optimal}")


def main(filename):
  # Read file and convert to ints.
  input = []
  with open(filename) as file:
    input = file.read()
    solve(input)

if __name__ == '__main__':

  # Simple argparse for the input.
  parser = argparse.ArgumentParser()
  parser.add_argument("filename", type=str, nargs="?", default="input.txt",
        help="Enter the name of the file (default = input.txt)")
  parser.add_argument("-i", "--interactive", default="false", action="store_true", dest="interactive",
        help="If flag is set, the input is interactive")
  args = parser.parse_args()

  # Run the program.
  main(args.filename)