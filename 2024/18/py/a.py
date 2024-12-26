import argparse
from tqdm import tqdm
import copy
import functools
import heapq

N: int = 71

START: (int,int) = (0,0)
END: (int,int) = (N-1,N-1)
INF: int = N*N+1
DD: list[(int,int)] = [(1,0), (0,1), (-1,0), (0,-1)]

def print_grid(grid: list[list[(str,int)]]):
	for i in range(N):
		for j in range(N):
			print(grid[i][j][0], end='')
		print()

def is_valid(grid: list[list[str]], x: int, y: int, dist: int) -> bool:
	return 0 <= x < N and 0 <= y < N and grid[x][y][1] > dist

def min_dist(grid: list[list[str]]) -> int:
	pq = [(0, START)]
	while pq:
		dist, (x,y) = heapq.heappop(pq)

		if grid[x][y][1] <= dist:
			# Current value is worse than what we've seen so far, no need to continue this one
			continue

		# If we reach here, it means that the current value is better than what we've seen, so re-assign.
		grid[x][y] = (grid[x][y][0], dist)

		for dd in DD:
			x2, y2 = x + dd[0], y + dd[1]
			if is_valid(grid, x2, y2, dist):
				heapq.heappush(pq, (dist+1, (x2,y2)))

	return grid[END[0]][END[1]][1]

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	grid: list[list[(str,int)]] = [[('.',INF)] * N for _ in range(N)]
	idx: int = 0
	for line in lines:
		idx += 1
		# Inverted coords
		x: int = int(line.split(',')[1])
		y: int = int(line.split(',')[0])
		grid[x][y] = ('#',-1)

		# For testing purposes
		if idx > 1023:
			break

	print_grid(grid)

	dist = min_dist(grid)
	print(f"{dist=}")


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