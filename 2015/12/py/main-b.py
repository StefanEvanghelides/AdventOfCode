import argparse
import json


def sumNumbers(jsonObj: json, redVal: str) -> int:
  sum: int = 0
  if type(jsonObj) == int:
    return jsonObj
  elif type(jsonObj) == dict:
    # First determine if there is a redVal in the dict
    for key in jsonObj:
      if type(jsonObj[key]) == str and jsonObj[key] == redVal:
        return 0

    for key in jsonObj:
      sum += sumNumbers(jsonObj[key], redVal)
  elif type(jsonObj) == list:
    for key in jsonObj:
      sum += sumNumbers(key, redVal)

  return sum


def solve(raw_input) -> None:
  inputs = raw_input.split('\n')

  # I have multiple test inputs
  for inp in inputs:
    # Parse the json object
    jsonObj = json.loads(inp)

    # Sum the numbers
    print(f"Numbers sum: {sumNumbers(jsonObj, 'red')}")


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