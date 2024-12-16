import argparse
from tqdm import tqdm
import copy

cache: dict[(int,int), set((int,int))] = {}

def is_next(grid: list[list[int]], n: int, m: int, i: int, j: int, i2: int, j2: int) -> bool:
	if not (0 <= i2 < n and 0 <= j2 < m):
		return False

	return grid[i][j] == (grid[i2][j2] - 1)


def compute_trail(grid: list[list[int]], n: int, m: int, start: (int,int), point: (int,int)) -> int:
	i, j = point[0], point[1]
	#print(f"{start=}  {point=}  {grid[i][j]=}")
	if grid[i][j] == 9:
		# Found a trail!
		if start not in cache:
			cache[start] = set()
		cache[start].add((i,j))
		return 1

	# Check all other paths until it leads to a god one.
	# There are 4 directions.
	total: int = 0

	# Check if top is an option
	i2, j2 = i-1, j
	if is_next(grid, n, m, i, j, i2, j2):
		total += compute_trail(grid, n, m, start, (i2,j2))

	# Check if bottom is an option
	i2, j2 = i+1, j
	if is_next(grid, n, m, i, j, i2, j2):
		total += compute_trail(grid, n, m, start, (i2,j2))

	# Check if left is an option
	i2, j2 = i, j-1
	if is_next(grid, n, m, i, j, i2, j2):
		total += compute_trail(grid, n, m, start, (i2,j2))

	# Check if right is an option
	i2, j2 = i, j+1
	if is_next(grid, n, m, i, j, i2, j2):
		total += compute_trail(grid, n, m, start, (i2,j2))

	return total


def find_trails(grid: list[list[int]], n: int, m: int, starts: list[(int,int)]) -> list[int]:
	sz: int = len(starts)
	trails: list[int] = [0] * sz

	for i, start in enumerate(starts):
		#print(f"{i=}  {start=}")
		trail: int = compute_trail(grid, n, m, start, start)
		trails.append(trail)

	return trails


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)
	m: int = len(lines[0])
	grid: list[list[int]] = [[0] * m for _ in range(n)]
	starts: list[(int,int)] = []
	for i in range(n):
		for j in range(m):
			grid[i][j] = -1 if lines[i][j] == '.' else int(lines[i][j])
			if grid[i][j] == 0:
				starts.append((i,j))

	print(f"{grid=}")

	trails: list[int] = find_trails(grid, n, m, starts)

	total: int = sum(trails)
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