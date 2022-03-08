import argparse

def solve(raw_input):
  start_x = 0
  start_y = 0
  location = str(start_x) + '*' + str(start_y)

  houses = {}
  houses[location] = 1
  count = 0

  for x in raw_input:
    start_x += (x == '^') - (x == 'v')
    start_y += (x == '>') - (x == '<')
    location = str(start_x) + '*' + str(start_y)

    #print(f"{x=} {start_x=} {start_y=} {location=} ")

    if location in houses:
      houses[location] += 1
    else:
      houses[location] = 1

  print(f"{len(houses)=}")


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