import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# Part (b): run a search on the input to determine when the input is broken.
# We need to ensure that when the path is fully blocked, we do get the INF result.

N: int = 71
LIMIT: int = 1024

START: (int,int) = (0,0)
END: (int,int) = (N-1,N-1)
INF: int = N*N+1
DD: list[(int,int)] = [(1,0), (0,1), (-1,0), (0,-1)]

def print_grid(grid: list[list[str]]):
	for i in range(N):
		for j in range(N):
			print(grid[i][j], end='')
		print()

def is_valid(distances: list[list[int]], x: int, y: int, dist: int) -> bool:
	return 0 <= x < N and 0 <= y < N and distances[x][y] > dist

def min_dist(grid: list[list[str]]) -> int:
	distances: list[list[int]] = [[(INF)] * N for _ in range(N)]
	for i in range(N):
		for j in range(N):
			if grid[i][j] == '#':
				distances[i][j] = -1

	pq = [(0, START)]
	while pq:
		dist, (x,y) = heapq.heappop(pq)

		if distances[x][y] <= dist:
			# Current value is worse than what we've seen so far, no need to continue this one
			continue

		# If we reach here, it means that the current value is better than what we've seen, so re-assign.
		distances[x][y] = dist

		for dd in DD:
			x2, y2 = x + dd[0], y + dd[1]
			if is_valid(distances, x2, y2, dist):
				heapq.heappush(pq, (dist+1, (x2,y2)))

	return distances[END[0]][END[1]]

def add_next_byte(grid: list[list[str]], idx: int, lines: list[str]):
	# Inverted coords
	line: str = lines[idx]
	x: int = int(line.split(',')[1])
	y: int = int(line.split(',')[0])
	grid[x][y] = '#'


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	grid: list[list[str]] = [['.'] * N for _ in range(N)]
	for i in range(LIMIT):
		add_next_byte(grid, i, lines)

	print_grid(grid)

	dist = min_dist(grid)
	print(f"{dist=}")

	while dist < INF:
		i += 1
		print(f"{i=}")
		add_next_byte(grid, i, lines)
		dist = min_dist(grid)
		if dist >= INF:
			break

	print(f"{i} -> {lines[i]}")


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