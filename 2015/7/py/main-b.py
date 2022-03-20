import argparse

MAX_ITERATIONS: int = 1000

def is_number(s: str) -> bool:
  if isinstance(s,int):
    return True

  if len(s) < 1:
    return False

  if s[0] == '-' and len(s) > 1:
    return s.lstrip('-').isnumeric()

  return s.isnumeric()


def check_solved(book: dict) -> bool:
  for _,v in book.items():
    if len(v) > 1 or not is_number(v[0]):
      return False;

  return True;

def solve_book(original_book: dict) -> dict:
  book: dict = original_book.copy()

  it: int = 0
  while not check_solved(book) and it < MAX_ITERATIONS:
    # Check the values, see if they can be evaluated
    for key,values in book.items():
      # Expect here either a number of a variable with a number
      if len(values) == 1:
        if not is_number(values[0]) and \
            len(book[values[0]]) == 1 and \
            is_number(book[values[0]][0]):
          book[key] = book[values[0]]

      elif len(values) == 2:
        # This is the NOT operation
        value1 = values[1]
        if not is_number(value1) and \
            len(book[value1]) == 1 and \
            is_number(book[value1][0]):
          value1 = book[value1][0]

        if is_number(value1):
          book[key] = [65535 ^ value1]

      elif len(values) == 3:
        # This can be a OR, AND, LSHIFT or RSHIFT operation.
        op: str = values[1]
        value0 = values[0]
        value2 = values[2]

        if not is_number(value0) and \
            len(book[value0]) == 1 and \
            is_number(book[value0][0]):
          value0 = book[value0][0]

        if not is_number(value2) and \
            len(book[value2]) == 1 and \
            is_number(book[value2][0]):
          value2 = book[value2][0]

        if is_number(value0) and is_number(value2):
          result: int = 0
          if op == "AND":
            result = value0 & value2
          elif op == "OR":
            result = value0 | value2
          elif op == "LSHIFT":
            result = value0 << value2
          elif op == "RSHIFT":
            result = value0 >> value2
          else:
            print(f"WRONG OPERATION! {op=}")

          book[key] = [result]

      else:
        print(f"WRONG VALUES SIZE! {values=}")

    it += 1
    #print(f" - {key=}  {book[key]=}")

  return book

def solve(raw_input):
  input = raw_input.split('\n')

  # Now create the equation book
  book = {}
  for line in input:
    inputs, output = line.split(' -> ')
    tokens = [int(inp) if is_number(inp) else inp for inp in inputs.split(' ')]
    book[output] = tokens

  #print(f"{len(book)=}")
  #print(f"{book=}")

  # Solve the book directly from the map
  new_book = solve_book(book)

  # For part b) now assign [a] to [b] and rerun
  book["b"] = new_book["a"]

  new_book = solve_book(book)

  print(f"{book=}")
  print(f"{new_book=}")
  print(f"{new_book['a']=}")


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