import argparse
import copy
from tqdm import tqdm

# dd = [up, right, down, left]
dd: list((int,int)) = [(-1,0), (0,1), (1,0), (0,-1)]

# 2023 too high! It was 2022, I forgot to avoid placing the obstruction on the starting position!

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	# We can already operate on the lines, this is the grid.
	grid: list[list[str]] = [list(x) for x in lines]
	n: int = len(grid)
	m: int = len(grid[0])

	# Find the starting carret
	i: int = 0
	j: int = 0
	for i in range(n):
		found = False
		for j in range(m):
			if grid[i][j] == '^':
				found = True
				break
		if found:
			break

	start_i: int = i
	start_j: int = j

	dir: int = 0
	seen: set((int, int)) = set()
	while True:
		seen.add((i, j))

		next_i = i + dd[dir][0]
		next_j = j + dd[dir][1]

		if not (0 <= next_i < n and 0 <= next_j < m):
			# Out of bounds
			#print(f"Out of bounds {next_i=}  {next_j=}")
			break

		if grid[next_i][next_j] == '#':
			#print(f"Turning at {next_i=}  {next_j=}")
			dir = (dir + 1) % 4

		else:
			i, j = next_i, next_j

	# Rerun everything, but add a new obstruction along the ways.
	total: int = 0
	for obs in tqdm(seen):
		if start_i == obs[0] and start_j == obs[1]:
			continue

		grid_copy: list[list[str]] = copy.deepcopy(grid)		
		grid_copy[obs[0]][obs[1]] = '#'

		i, j = start_i, start_j
		dir = 0
		seen: set((int, int, int)) = set()
		while True:
			tup: (int, int, int) = (i, j, dir)
			if tup in seen:
				total += 1
				break

			seen.add(tup)

			next_i = i + dd[dir][0]
			next_j = j + dd[dir][1]

			if not (0 <= next_i < n and 0 <= next_j < m):
				# Out of bounds
				#print(f"Out of bounds {next_i=}  {next_j=}")
				break

			if grid_copy[next_i][next_j] == '#':
				#print(f"Turning at {next_i=}  {next_j=}")
				dir = (dir + 1) % 4

			else:
				i, j = next_i, next_j


	print(f"Total unique: {total}")


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