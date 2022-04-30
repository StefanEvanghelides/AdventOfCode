import argparse
import hashlib


SIZE = 4


def check(dir_str: str, pos: tuple, size: int = SIZE) -> bool:
  return  (
    dir_str == 'UP' and pos[1] > 0 or
    dir_str == 'DOWN' and pos[1] < size-1 or
    dir_str == 'LEFT' and pos[0] > 0 or
    dir_str == 'RIGHT' and pos[0] < size-1
  )


def find_end(curr_pos: tuple, end_pos: tuple, inp: str, curr_path: str = "") -> list[str]:
  if curr_pos == end_pos:
    return [curr_path]

  paths: list[str] = []

  hashcode: str = hashlib.md5(inp.encode()).hexdigest()[0:4]

  # Up, down, left, right
  open_dir: list[bool] = [x >= 'b' and x <= 'f' for x in hashcode]

  # Now try all possible combinations.
  # Do this in a smart way: check if any new position would reach the end already.
  sel_pos_letter: str = ''
  if open_dir[0] and check('UP', curr_pos):
    new_pos: tuple = (curr_pos[0], curr_pos[1]-1)
    if new_pos == end_pos:
      sel_pos_letter = 'U'
  if open_dir[1] and check('DOWN', curr_pos):
    new_pos: tuple = (curr_pos[0], curr_pos[1]+1)
    if new_pos == end_pos:
      sel_pos_letter = 'D'
  if open_dir[2] and check('LEFT', curr_pos):
    new_pos: tuple = (curr_pos[0]-1, curr_pos[1])
    if new_pos == end_pos:
      sel_pos_letter = 'L'
  if open_dir[3] and check('RIGHT', curr_pos):
    new_pos: tuple = (curr_pos[0]+1, curr_pos[1])
    if new_pos == end_pos:
      sel_pos_letter = 'R'

  if len(sel_pos_letter) > 0:
    return [curr_path + sel_pos_letter]

  # End is not immediat, so check any combination.
  if open_dir[0] and check('UP', curr_pos):
    new_pos: tuple = (curr_pos[0], curr_pos[1]-1)
    paths += find_end(new_pos, end_pos, inp+'U', curr_path+'U')
  if open_dir[1] and check('DOWN', curr_pos):
    new_pos: tuple = (curr_pos[0], curr_pos[1]+1)
    paths += find_end(new_pos, end_pos, inp+'D', curr_path+'D')
  if open_dir[2] and check('LEFT', curr_pos):
    new_pos: tuple = (curr_pos[0]-1, curr_pos[1])
    paths += find_end(new_pos, end_pos, inp+'L', curr_path+'L')
  if open_dir[3] and check('RIGHT', curr_pos):
    new_pos: tuple = (curr_pos[0]+1, curr_pos[1])
    paths += find_end(new_pos, end_pos, inp+'R', curr_path+'R')

  return paths


def solve(raw_input):

  # for loop because I have multiple lines for testing.
  for inp in raw_input.split():

    # Quickly check for some comments
    if len(inp) < 1 or inp[0] == '#':
      continue

    start_pos: tuple = (0, 0)
    end_pos: tuple = (SIZE-1, SIZE-1)

    paths: list[str] = find_end(start_pos, end_pos, inp)

    if len(paths) > 0:
      shortest_path: str = sorted(paths, key=len)[0]
      print(f"{shortest_path=}")
    else:
      print(f"No path found for input {inp}")


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