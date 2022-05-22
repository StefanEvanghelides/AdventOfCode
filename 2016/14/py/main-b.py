import argparse
import pprint
import hashlib


MAX_KEYS = 64
MAX_ITERATIONS = 1_00_000
LOOKAHEAD = 1_000
NR_REHASHES = 2016


def matchThree(value):
  for idx in range(len(value) - 2):
    if len({value[idx + i] for i in range(3)}) == 1:
      return value[idx]
  return None

def matchFive(value):
  for idx in range(len(value) - 4):
    if len({value[idx + i] for i in range(5)}) == 1:
      return value[idx]
  return None

def solve(raw_input: str):
  triples: dict[str,list] = {}
  keys: dict[int,str] = {}
  for i in range(0,MAX_ITERATIONS):
    inp: str = raw_input + str(i)
    digest: str = hashlib.md5(inp.encode()).hexdigest()
    # part (b): repeat many times this operation
    for _ in range(NR_REHASHES):
      digest = hashlib.md5(digest.encode()).hexdigest()

    match = matchFive(digest)
    if match and match in triples:
      for (idx,hx) in triples[match]:
        if i - idx <= LOOKAHEAD:
          # We finally have a key!
          keys[idx] = hx

    match = matchThree(digest)
    if match:
      if match not in triples:
        triples[match] = [(i,digest)]
      else:
        triples[match] += [(i,digest)]

    if len(keys) >= MAX_KEYS:
      break

  counter: int = 0
  for key,val in sorted(keys.items()):
    counter += 1
    print(f"{counter=} -> {key=}  {val=}")


def main(filename: str):
  # Read file and convert to ints
  with open(filename) as file:
    inp: str = file.read()
    solve(inp)

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