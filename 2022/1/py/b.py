import argparse

MAX_VALS = 3

def solve(raw_input):
  inp = raw_input.split('\n')
  sum = 0
  max_sum = 3 * [0]
  for i in inp:
    if not i or len(i) < 1:
      sum = 0
      continue

    nr = int(i)
    sum = sum + nr
    if sum > max_sum[MAX_VALS-1]:
      max_sum[MAX_VALS-1] = sum
  
    for i in range(MAX_VALS-2,-1,-1):
      if sum > max_sum[i]:
        max_sum[i+1] = max_sum[i]
        max_sum[i] = sum

  combined = 0;
  for mx in max_sum:
    combined = combined + mx

  print("combined: " + str(combined))
  

def main(filename):
  # Read file and convert to ints
  input = []
  with open(filename) as file:
    input = file.read()
    solve(input)

if __name__ == '__main__':

  # Simple argparse for the input
  parser = argparse.ArgumentParser()
  parser.add_argument("filename", type=str, nargs="?", default="input",
        help="Enter the name of the file (default = input)")
  parser.add_argument("-i", "--interactive", default="false", action="store_true", dest="interactive",
        help="If flag is set, the input is interactive")
  args = parser.parse_args()

  # Run the program
  main(args.filename)