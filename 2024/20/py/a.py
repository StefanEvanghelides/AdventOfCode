import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# This is essentially the code for Day 18 - part b, just adapted a bit for the new input.


INF: int = 9999999999
DD: list[(int,int)] = [(1,0), (0,1), (-1,0), (0,-1)]

def print_grid(grid: list[list[str]]):
	n: int = len(grid)
	for i in range(n):
		for j in range(n):
			print(grid[i][j], end='')
		print()

def is_valid(distances: list[list[int]], n: int, x: int, y: int, dist: int) -> bool:
	return 0 <= x < n and 0 <= y < n and distances[x][y] > dist

def min_distance(grid: list[list[str]], start: (int,int), end: (int,int)) -> int:
	n: int = len(grid)
	distances: list[list[int]] = [[(INF)] * n for _ in range(n)]
	for i in range(n):
		for j in range(n):
			if grid[i][j] == '#':
				distances[i][j] = -1

	pq = [(0, start)]
	while pq:
		dist, (x,y) = heapq.heappop(pq)

		if distances[x][y] <= dist:
			# Current value is worse than what we've seen so far, no need to continue this one
			continue

		# If we reach here, it means that the current value is better than what we've seen, so re-assign.
		distances[x][y] = dist

		for dd in DD:
			x2, y2 = x + dd[0], y + dd[1]
			if is_valid(distances, n, x2, y2, dist):
				heapq.heappush(pq, (dist+1, (x2,y2)))

	return distances[end[0]][end[1]]

def find_single_walls(grid: list[list[str]], n: int, walls: list[(int,int)]) -> list[(int,int)]:
	single_walls: list[(int,int)] = []
	for x,y in walls:
		if 1 <= x < n-1 and 1 <= y < n-1 and \
				(grid[x][y-1] == '.' and grid[x][y+1] == '.' or \
				 grid[x-1][y] == '.' and grid[x+1][y] == '.'):
			single_walls.append((x,y))
	
	return single_walls

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	grid: list[list[str]] = [['.'] * n for _ in range(n)]
	walls: list[(int,int)] = []
	start: (int,int) = None
	end: (int,int) = None
	for i in range(n):
		for j in range(n):
			if lines[i][j] == 'S':
				start = (i,j)
				continue

			if lines[i][j] == 'E':
				end = (i,j)
				continue

			if lines[i][j] == '#':
				grid[i][j] = '#'
				walls.append((i,j))

	print(f"{start=}  {end=}")

	print_grid(grid)
	
	single_walls: list[(int,int)] = find_single_walls(grid, n, walls)
	print(f"{len(single_walls)=}")

	# First compute min dist with all walls
	total_dist: int = min_distance(grid, start, end)
	print(f"{total_dist=}")

	# Now re-run everything, this time by trying to cheat
	results: dict[int,int] = {}
	sz: int = len(single_walls)
	for i in tqdm(range(sz)):
		x,y = single_walls[i]
		grid[x][y] = '.'
		dist = min_distance(grid, start, end)
		if dist < total_dist:
			diff: int = total_dist - dist
			if diff not in results:
				results[diff] = 0
			results[diff] += 1

		# Reset wall
		grid[x][y] = '#'

	print(f"{results=}")

	# For part (a): how many cheats save at least 100 picoseconds?
	total: int = 0
	for save, cheats in results.items():
		if save >= 100:
			total += cheats

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