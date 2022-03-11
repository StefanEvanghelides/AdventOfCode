import argparse

def solve(raw_input):
  input = raw_input.split()
  gamma = ''
  input_size = len(input[0])
  copy_input = input
  for j in range(0, input_size):
    count = 0
    for i in range(0, len(input)):
      count+= (input[i][j] == '1') - (input[i][j] == '0')

    curr_bit = '0' * (count < 0) + '1' * (count >= 0)
    input = [x for x in input if x[j] == curr_bit]

    #print(f"{input=} len={len(input)}")
    
    if len(input) == 1:
      gamma = input[0]
      break

  input = copy_input
  non_gamma = ''
  for j in range(0, input_size):
    count = 0
    for i in range(0, len(input)):
      count+= (input[i][j] == '1') - (input[i][j] == '0')

    curr_bit = '0' * (count >= 0) + '1' * (count < 0)
    input = [x for x in input if x[j] == curr_bit]

    #print(f"{input=} len={len(input)}")
    
    if len(input) == 1:
      non_gamma = input[0]
      break

  rating = int(gamma,2) * int(non_gamma,2)

  print(f"{gamma=}")
  print(f"{non_gamma=}")
  print(f"{rating=}")


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