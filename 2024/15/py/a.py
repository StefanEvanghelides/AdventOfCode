import argparse
from tqdm import tqdm
import copy
import functools

DD: dict[str,(int,int)] = {'^': (-1, 0), 'v': (1, 0), '<': (0, -1), '>': (0, 1)}

def print_grid(grid: list[list[str]], message: str = ''):
	vals: list[str] = [''.join(line) for line in grid]
	print(f"{message}{'\n'.join(vals)}")

def do_moves(grid: list[list[str]], moves: list[str], x: int, y: int) -> list[list[str]]:
	for move in moves:
		di, dj = DD[move]
		i, j = x + di, y + dj

		#print_grid(grid, f"grid before move {move}: \n")

		# We hit a wall, do nothing
		if grid[i][j] == '#':
			continue

		# Empty space, move forward
		if grid[i][j] == '.':
			grid[i][j] = '@'
			grid[x][y] = '.'
			x, y = i, j
			continue

		# Here we hit a block, this can potentially move.
		# Check more values in the direction of the block.
		i2, j2 = i, j
		while grid[i2][j2] == 'O':
			i2, j2 = i2 + di, j2 + dj

		# Now check what we hit
		if grid[i2][j2] == '#':
			# This was a continuous wall, so still can't move
			continue

		# Here it means that we can actually move
		grid[i2][j2] = 'O'
		grid[i][j] = '@'
		grid[x][y] = '.'
		x, y = i, j

	return grid

def calculate_total(grid: list[list[str]], sz: int) -> int:
	total: int = 0
	for i in range(1, sz-1):
		for j in range(1, sz-1):
			if grid[i][j] == 'O':
				total += 100 * i + j

	return total

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	sz: int = n-2
	grid: list[list[str]] = []
	x = y = 0
	for i in range(sz):
		grid.append([])
		for j in range(sz):
			grid[-1].append(lines[i][j])
			if lines[i][j] == '@':
				x, y = i, j

	print_grid(grid, "initial grid\n")
	print(f"{x=}  {y=}")

	# Moves
	moves: list[str] = [str(x) for x in lines[-1]]
	print(f"{moves=}")

	print(f"{DD=}")

	grid = do_moves(grid, moves, x, y)
	print_grid(grid, "final grid\n")

	total: int = calculate_total(grid, sz)
	print(f"{total=}")



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