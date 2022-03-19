import argparse

#from dataclasses import dataclass

def solve(raw_input):
  MAX_ITERATIONS = 256
  fish = {}
  for i in range(9):
    fish[i] = 0

  input = [int(x) for x in raw_input.split(',')]

  for i in input:
    fish[i] += 1

  iter = 0
  while iter < MAX_ITERATIONS:
    iter += 1
    fish0 = fish[0]

    for i in range(8):
      fish[i] = fish[i+1]

    fish[8] = fish0
    fish[6] += fish0

  count = 0
  for i in range(9):
    count += fish[i]

  print(f"{count=}")


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