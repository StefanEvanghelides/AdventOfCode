import argparse

def solve(raw_input):
  input = raw_input.split()
  vowels = 'aeiou'
  strings = ['ab', 'cd', 'pq', 'xy']
  count = 0
  for s in input:
    if any(x in s for x in strings) or len(s) < 1:
      continue

    has_double = False
    vowels_count = 0 + (s[0] in vowels)
    for i in range(1, len(s)):
      l_prev = s[i-1]
      l = s[i]
      if l in vowels:
        vowels_count += 1

      if l == l_prev:
        has_double = True

    #print(f"{s=}")
    #print(f"{has_double=} {vowels_count=}")

    count += has_double * (vowels_count > 2)

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