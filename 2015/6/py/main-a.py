import argparse

def solve(raw_input):
  field = {}
  # Init 1000x1000 with 0
  for i in range(0, 1000):
    for j in range (0, 1000):
      key = str(i) + '*' + str(j)
      field[key] = 0

  input = raw_input.split('\n')
  for line in input:
    tokens = line.split()
    if tokens[0] == 'toggle':
      # Flip the value for the respective numbers
      startCoords = tokens[1].split(',')
      endCoords = tokens[3].split(',')

      startX = int(startCoords[0])
      startY = int(startCoords[1])
      endX = int(endCoords[0])
      endY = int(endCoords[1])

      for i in range(startX, endX+1):
        for j in range(startY, endY+1):
          key = str(i) + '*' + str(j)
          field[key] = 1 - field[key]

    elif tokens[0] == 'turn':
      startCoords = tokens[2].split(',')
      endCoords = tokens[4].split(',')

      startX = int(startCoords[0])
      startY = int(startCoords[1])
      endX = int(endCoords[0])
      endY = int(endCoords[1])

      value = int(tokens[1] == 'on')
      for i in range(startX, endX+1):
        for j in range(startY, endY+1):
          key = str(i) + '*' + str(j)
          field[key] = value

  count = 0
  for key in field:
    count += field[key]

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