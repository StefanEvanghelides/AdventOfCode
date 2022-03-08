import argparse

def solve(raw_input):
  santa_x = 0
  santa_y = 0
  robot_x = 0
  robot_y = 0
  location = str(santa_x) + '*' + str(santa_y)

  houses = {}
  houses[location] = 2
  count = 0

  for i in range(0, len(raw_input), 2):
    santa, robot = raw_input[i:i+2]

    santa_x += (santa == '^') - (santa == 'v')
    santa_y += (santa == '>') - (santa == '<')
    location_santa = str(santa_x) + '*' + str(santa_y)

    if location_santa in houses:
      houses[location_santa] += 1
    else:
      houses[location_santa] = 1

    robot_x += (robot == '^') - (robot == 'v')
    robot_y += (robot == '>') - (robot == '<')
    location_robot = str(robot_x) + '*' + str(robot_y)

    if location_robot in houses:
      houses[location_robot] += 1
    else:
      houses[location_robot] = 1

  print(f"{len(houses)=}")


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