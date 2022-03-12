import argparse

def solve(raw_input):
  input = raw_input.split()
  count = 0
  for s in input:
    #print(f"{s=}")
    ok = False
    for i in range(0, len(s)-3):
      a = s[i]
      b = s[i+1]
      for j in range(i+2,len(s)-1):
        c = s[j]
        d = s[j+1]
        if a == c and b == d:
          ok = True
          break
      if ok:
        break

    if not ok:
      continue

    ok = False
    for i in range(0, len(s)-2):
      a = s[i]
      c = s[i+2]
      if a == c:
        ok = True
        break

    if not ok:
      continue

    count += 1

  print(f"{count=}")

# 51 is wrong
# 52 is wrong
# 53 is wrong
# 59 is wrong

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