import argparse

def solve(raw_input):
  input = raw_input.split('\n')
  horizontal = 0
  vertical = 0
  for x in input:
    cmd, val = x.split()
    val = int(val)

    horizontal += val * (cmd[0] == 'f')
    vertical += val * (cmd[0] == 'd') - val * (cmd[0] == 'u')

  print(f"Total: {horizontal=}")
  print(f"Total: {vertical=}")
  print(f"Total: {horizontal*vertical=}")

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