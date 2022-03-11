import argparse

def solve(raw_input):
  input = raw_input.split()
  gamma = ''
  for j in range(0, len(input[0])):
    count = 0
    for i in range(0, len(input)):
      count+= (input[i][j] == '1') - (input[i][j] == '0')

    gamma += '0' * (count < 0) + '1' * (count >= 0)

  not_gamma = "".join([str(1 - int(x)) for x in gamma])
  power = int(gamma,2) * int(not_gamma,2)

  print(f"{gamma=}")
  print(f"{not_gamma=}")
  print(f"{power=}")


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