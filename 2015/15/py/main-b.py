import argparse
import numpy as np

MAX_SUM: int = 100
MAX_CALORIES: int = 500

def generate_all_combinations(list_size: int, results: list[list], result: list[int] = []) -> None:
  if len(result) == list_size:
    if sum(result) == MAX_SUM:
      results += [result]
    return

  max_range = MAX_SUM-list_size+1
  for i in range(1,max_range+1):
    generate_all_combinations(list_size, results, result+[i])


def next_ingredients(ingredients_quants: dict[str,int], ingr_list: list[int]) -> None:
  """
    Important: The elements in the dictionary should not change their position
    If so, this would be a disaster!
  """

  it: int = 0
  for key,_ in ingredients_quants.items():
    ingredients_quants[key] = ingr_list[it]
    it += 1


def compute_total_scores(ingredients: dict[str,dict[str,int]], ingredients_quants: dict[str,int]) -> list[int]:
  total_cap: int = 0
  total_dur: int = 0
  total_fla: int = 0
  total_tex: int = 0
  total_cal: int = 0

  for key,quant in ingredients_quants.items():
    total_cap += quant * ingredients[key]["capacity"]
    total_dur += quant * ingredients[key]["durability"]
    total_fla += quant * ingredients[key]["flavor"]
    total_tex += quant * ingredients[key]["texture"]
    total_cal += quant * ingredients[key]["calories"]

  total_cap = 0 if total_cap < 0 else total_cap
  total_dur = 0 if total_dur < 0 else total_dur
  total_fla = 0 if total_fla < 0 else total_fla
  total_tex = 0 if total_tex < 0 else total_tex
  total_cal = 0 if total_cal < 0 else total_cal

  return [total_cap, total_dur, total_fla, total_tex, total_cal]


def solve(raw_input):
  inputs = raw_input.split('\n')

  ingredients: dict[str,dict[str,int]] = {}
  ingredients_quants: dict[str,int] = {}
  for inp in inputs:
    key, values = inp.split(': ')
    ingredients_quants[key] = 0
    ingredients[key] = {}
    tokens = values.split(', ')
    for tok in tokens:
      tok_key, tok_val = tok.split(' ')
      ingredients[key][tok_key] = int(tok_val)

  combinations: list[list[int]] = []
  generate_all_combinations(len(ingredients), combinations)
  print(f"{len(combinations)=}")

  max_score: int = 0
  for combination in combinations:
    next_ingredients(ingredients_quants, combination)
    total_scores: list[int] = compute_total_scores(ingredients, ingredients_quants)
    total_calories = total_scores[-1]
    if total_calories == MAX_CALORIES:
      score: int = np.prod(total_scores[:-1])
      max_score = score if score > max_score else max_score

  print(f"{max_score=}")


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