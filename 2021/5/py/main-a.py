import argparse

#from dataclasses import dataclass

def solve(raw_input):
  field = {}
  input = raw_input.split('\n')
  for line in input:
    coords = line.split(' -> ')
    startCoords = coords[0].split(',')
    endCoords = coords[1].split(',')

    # Movement on X
    if startCoords[1] == endCoords[1]:
      startX = int(startCoords[0])
      endX = int(endCoords[0])
      if startX > endX:
        # swap numbers
        startX = startX ^ endX
        endX = startX ^ endX
        startX = startX ^ endX

      for i in range(startX, endX+1):
        newX = str(i)
        fullCoord = newX + '*' + startCoords[1]
        if fullCoord in field:
          field[fullCoord] += 1
        else:
          field[fullCoord] = 1

    # Movement on Y
    if startCoords[0] == endCoords[0]:
      startY = int(startCoords[1])
      endY = int(endCoords[1])
      if startY > endY:
        # swap numbers
        startY = startY ^ endY
        endY = startY ^ endY
        startY = startY ^ endY

      for i in range(startY, endY+1):
        newY = str(i)
        fullCoord = startCoords[0] + '*' + newY
        if fullCoord in field:
          field[fullCoord] += 1
        else:
          field[fullCoord] = 1

  count = 0
  for key in field:
    count += (field[key] > 1)

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