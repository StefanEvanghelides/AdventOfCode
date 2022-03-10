import argparse

def solve(raw_input):

  import hashlib

  nr = ''
  max_iterations = 100000000
  for i in range(1,max_iterations):
    nr = str(i)
    input = raw_input + nr
    hash = hashlib.md5(input.encode())
    digest = hash.hexdigest()
    if digest.startswith('000000'):
      break

  print(f"{nr=}")


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