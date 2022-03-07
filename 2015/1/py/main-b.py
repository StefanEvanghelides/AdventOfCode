import argparse

def solve(raw_input):
  lvl = 0
  for i in range(0, len(raw_input)):
    x = raw_input[i]
    lvl = lvl + (x == '(') - (x==')')
    if lvl < 0:
      print(f"Basement at position {i+1=}")
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