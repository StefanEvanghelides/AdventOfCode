import argparse
from tqdm import tqdm
import copy
import functools

# For part (b) we want to print the grid positions of all robots

# Test grid. Note that x/y are reversed
NX: int = 7
NY: int = 11

# Real grid
NX = 103
NY = 101

# Need to wait for more.. maybe about 10k?
SECONDS: int = 10000

# 7857 too low -> But I think that's because I missed 1 when computing the number of seconds.

def compute_positions(x: int, y: int, dx: int, dy: int, nx: int, ny: int, seconds: int) -> (int,int):
	# Use modulo to compute positions
	# It's easier to do so when values are already positive

	if dx < 0:
		dx = nx + dx

	if dy < 0:
		dy = ny + dy

	rx: int = (x + seconds * dx) % nx
	ry: int = (y + seconds * dy) % ny

	return rx, ry
	

def count_robots(robots: list[(int,int,int,int)], i1: int, i2: int, j1: int, j2: int) -> int:
	count: int = 0

	for x, y, _, _ in robots:
		if i1 <= x < i2 and j1 <= y < j2:
			count += 1

	return count

def plot(robots: list[(int,int,int,int)], iter: int):
	grid: list[list[int]] = []
	for _ in range (NX):
		grid.append([0] * NY)

	for x, y, _, _ in robots:
		grid[x][y] += 1

	print(f"\n Iteration {iter}")
	for i in range (NX):
		for j in range (NY):
			if grid[i][j] == 0:
				print(" ", end="")
			else:
				print(f"#", end="")
		print("\n", end="")


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	robots: list[(int,int,int,int)] = []
	for line in lines:
		parts = line.split(" v=")
		starts = parts[0].split("=")[1].split(',')
		x = int(starts[1])
		y = int(starts[0])
		velos = parts[1].split(',')
		dx = int(velos[1])
		if dx < 0:
			dx = NX + dx
		dy = int(velos[0])
		if dy < 0:
			dy = NY + dy
		robots.append((x, y, dx, dy))

	# Needed a higher range
	# Heuristic: try and check in batches of 1k-2k...
	# Then I searched for many "#############" grouped together.
	for i in range(10000):
		# Move robots once
		robots = [((x + dx) % NX, (y + dy) % NY, dx, dy) for x, y, dx, dy in robots]
		if i > 7000:
			plot(robots, i+1)

	# Check quadrants
	mid_x = NX // 2
	mid_y = NY // 2
	count_top_left: int = count_robots(robots, 0, mid_x, 0, mid_y)
	count_top_right = count_robots(robots, 0, mid_x, mid_y+1, NY)
	count_bottom_left = count_robots(robots, mid_x+1, NX, 0, mid_y)
	count_bottom_right = count_robots(robots, mid_x+1, NX, mid_y+1, NY)

	print(f"{count_top_left=}  {count_top_right=}  {count_bottom_left=}  {count_bottom_right=}")

	total_safety = count_top_left * count_top_right * count_bottom_left * count_bottom_right
	print(f"{total_safety=}")


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