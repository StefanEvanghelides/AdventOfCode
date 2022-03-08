import argparse

def solve(raw_input):
  input = raw_input.split()
  total_area = 0
  for x in input:
    sizes = [int(i) for i in x.split('x')]
    sizes.sort()

    perimeter = sizes[0] * 2 + sizes[1] * 2
    perimeter += sizes[0] * sizes[1] * sizes[2]

    total_area += perimeter

  print(f"{total_area=}")
    
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