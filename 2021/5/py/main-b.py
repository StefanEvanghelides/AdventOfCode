import argparse

#from dataclasses import dataclass

def solve(raw_input):
  field = {}
  input = raw_input.split('\n')
  for line in input:
    coords = line.split(' -> ')
    startCoords = coords[0].split(',')
    endCoords = coords[1].split(',')

    startX = int(startCoords[0])
    startY = int(startCoords[1])
    endX = int(endCoords[0])
    endY = int(endCoords[1])

    facX = (startX < endX) - (startX > endX)
    facY = (startY < endY) - (startY > endY)

    startX -= facX
    startY -= facY
    while (True):
      startX += facX
      startY += facY
      fullCoord = str(startX) + '*' + str(startY)
      if fullCoord in field:
        field[fullCoord] += 1
      else:
        field[fullCoord] = 1

      if startX == endX and startY == endY:
        break

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