import argparse
from tqdm import tqdm
import copy
import functools

# Part b: we just do the math here

# Given input: pushing button A costs 3 tokens.
# Pushing button B costs 1 token
# Also buttons should not be pressed more than 100 times
COST_A: int = 3
COST_B: int = 1
MAX_BUTTON_PRESSES: int = 100

INF: int = 900000000000000

EXTRA_VALUE = 10000000000000

def parse_grouping(lines: list[str], i) -> (int, int, int, int, int, int):
	# Each grouping uses 3 lines

	# Line #1 = button A
	values: list[str] = lines[i].split(": ")[1].split(", ")
	ax: int = int(values[0].split('+')[1])
	ay: int = int(values[1].split('+')[1])

	# Line #2 = button B
	values = lines[i+1].split(": ")[1].split(", ")
	bx: int = int(values[0].split('+')[1])
	by: int = int(values[1].split('+')[1])

	# Line #3 = prize
	values = lines[i+2].split(": ")[1].split(", ")
	tx = int(values[0].split('=')[1]) + EXTRA_VALUE
	ty = int(values[1].split('=')[1]) + EXTRA_VALUE

	return ax, ay, bx, by, tx, ty
	
def solve_equation(ax: int, ay: int, bx: int, by: int, tx: int, ty: int) -> (int, int):
	i: int = (by * tx - ty * bx) // (by * ax - ay * bx)
	j: int = (ty - ay * i) // by
	return (i, j)

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	total_cost: int = 0
	idx: int = 0
	while idx < n:
		ax, ay, bx, by, tx, ty = parse_grouping(lines, idx)
		# print(f"{ax=}  {ay=}  {bx=}  {by=}  {tx=}  {ty=}")

		# Solve the 2 linear equations.
		# There can always be only 1 solution here.
		i, j = solve_equation(ax, ay, bx, by, tx, ty)
		if i > 0 and j > 0 and ax * i + bx * j == tx and ay * i + by * j == ty:
			total_cost += COST_A * i + COST_B * j

		# Next grouping
		idx += 4

	print(f"{total_cost=}")



def main(filename: str):
	# Read file and convert to ints
	inputValues = []
	with open(filename) as file:
		inputValues = file.read()
		solve(inputValues)

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