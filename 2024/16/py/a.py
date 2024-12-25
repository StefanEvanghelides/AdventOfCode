import argparse
from tqdm import tqdm
import copy
import functools
import heapq


DD: list[(int,int)] = [(0,1), (1,0), (0,-1), (-1,0)]


def print_grid(grid: list[list[str]], message: str = ''):
	vals: list[str] = [''.join(line) for line in grid]
	print(f"{message}{'\n'.join(vals)}")

def is_valid(grid: list[list[str]], point: (int,int), dir: (int,int), seen: set[((int,int),(int,int))]) -> bool:
	x, y = point[0], point[1]
	return grid[x][y] != '#' and (point, dir) not in seen


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	grid: list[list[str]] = []
	start = None
	end = None
	for i in range(n):
		grid.append([])
		for j in range(n):
			if lines[i][j] == '.':
				grid[-1].append(' ')
			else:
				grid[-1].append(lines[i][j])

			if lines[i][j] == 'S':
				start = (i,j)
			elif lines[i][j] == 'E':
				end = (i,j)

	print_grid(grid, "initial grid\n")
	print(f"{start=}  {end=}")

	seen: set((int,int),(int,int)) = set()

	start_dir = DD[0] # We start east
	pq = [(0, start, start_dir)]
	while pq:
		score, point, dir = heapq.heappop(pq)

		if point == end:
			print(f"End reached, {score=}")
			break

		if (point, dir) in seen:
			# We have seen this point and dir before, so skip
			continue

		# We haven't seen it yet
		seen.add((point, dir))

		for dd in DD:
			new_point = (point[0] + dd[0], point[1] + dd[1])
			if is_valid(grid, new_point, dd, seen):
				new_score = score + 1
				if dd != dir:
					new_score += 1000
				heapq.heappush(pq, (new_score, new_point, dd))


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