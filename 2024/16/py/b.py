import argparse
from tqdm import tqdm
import copy
import functools
import heapq


DD: list[(int,int)] = [(0,1), (1,0), (0,-1), (-1,0)]
INF: int = 9999999999999

def print_grid(grid: list[list[(str,dict[(int,int),int])]], message: str = ''):
	res: str = ''
	for line in grid:
		vals: list[str] = [tup[0] for tup in line]	
		res += ''.join(vals) + '\n'
	print(f"{message}{res}")


def is_valid(grid: list[list[(str,dict[(int,int),int])]], point: (int,int), dir: (int,int), seen: set[((int,int),(int,int))]) -> bool:
	x, y = point[0], point[1]
	return grid[x][y][0] != '#' and (point, dir) not in seen


def count_paths(grid: list[list[(str,dict[(int,int),int])]], end: (int,int)) -> int:
	# Find minimum value
	min_val: int = INF
	for dd in DD:
		val: int = grid[end[0]][end[1]][1][dd]
		min_val = min(min_val, val)

	print(f"{min_val=}")

	seen: set((int,int)) = set()
	pq: [(int,(int,int))] = [(min_val, end)]
	while pq:
		score, point = heapq.heappop(pq)
		seen.add(point)
		x, y = point[0], point[1]
		for dd in DD:
			x2, y2 = x + dd[0], y + dd[1]
			for dd2 in DD:
				score2: int = grid[x2][y2][1][dd2]
				if score2 < score:
					heapq.heappush(pq, (score2, (x2,y2)))

	return len(seen)

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	grid: list[list[(str,dict[(int,int),int])]] = []
	start = None
	end = None
	for i in range(n):
		grid.append([])
		for j in range(n):
			dic = {}
			for dd in DD:
				dic[dd] = INF
			if lines[i][j] == '.':
				grid[-1].append((' ', dic))
			else:
				grid[-1].append((lines[i][j], dic))

			if lines[i][j] == 'S':
				start = (i,j)
				grid[-1][-1][1][DD[0]] = 0
				grid[-1][-1][1][DD[1]] = 0
				grid[-1][-1][1][DD[2]] = 0
				grid[-1][-1][1][DD[3]] = 0
			elif lines[i][j] == 'E':
				end = (i,j)

	print_grid(grid, "initial grid\n")
	print(f"{start=}  {end=}")

	seen: set((int,int),(int,int)) = set()

	min_score: int = INF
	start_dir = DD[0] # We start east
	pq = [(0, start, start_dir)]
	while pq:
		score, point, dir = heapq.heappop(pq)

		if point == end:
			min_score = min(min_score, INF)
			print(f"End reached, {score=}")

		values: (str, dict[(int,int),int]) = grid[point[0]][point[1]]
		if values[1][dir] < score:
			# We have seen this point, the score is worse actually
			continue

		# We haven't seen it yet. Add
		values[1][dir] = score

		for dd in DD:
			new_point = (point[0] + dd[0], point[1] + dd[1])
			if is_valid(grid, new_point, dd, seen):
				new_score = score + 1
				if dd != dir:
					new_score += 1000
				heapq.heappush(pq, (new_score, new_point, dd))

	# Now check values from starting point
	total: int = count_paths(grid, end)
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