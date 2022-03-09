import argparse

def main(filename):
  # Read file and convert to ints
  input = []
  with open(filename) as file:
    input = [int(x) for x in file.read().split()]

  # Count increases
  countIncreases = 0
  for i in range(0, len(input)-1):
    #print(f"{i=}, {input[i]=}, {input[i+1]=}")
    countIncreases += input[i] < input[i+1]

  print (countIncreases)

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