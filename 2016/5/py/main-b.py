import argparse

def solve(raw_input):

  import hashlib

  nr = ''
  max_iterations = int(1e9)
  passList = ['.'] * 8
  for i in range(1,max_iterations):
    nr = str(i)
    input = raw_input + nr
    hash = hashlib.md5(input.encode())
    digest = hash.hexdigest()
    if digest.startswith('00000') and digest[5] < '8':
      idx = int(digest[5])
      if passList[idx] == '.':
        passList[idx] = digest[6]
        print(f"Current pass: {''.join(passList)}")
        if '.' not in passList:
          break

  password = ''.join(passList)
  print(f"{password=}"   )


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