import argparse
from tqdm import tqdm
import copy
import functools

DD: dict[str,(int,int)] = {'^': (-1, 0), 'v': (1, 0), '<': (0, -1), '>': (0, 1)}

def double_grid(grid: list[list[str]], sz: int) -> list[list[str]]:
	new_grid: list[list[str]] = []

	for i in range(sz):
		new_grid.append([])
		for j in range(sz):
			if grid[i][j] == '#':
				new_grid[-1].append("#")
				new_grid[-1].append("#")
			elif grid[i][j] == 'O':
				new_grid[-1].append("[")
				new_grid[-1].append("]")
			elif grid[i][j] == '.':
				new_grid[-1].append(".")
				new_grid[-1].append(".")
			elif grid[i][j] == '@':
				new_grid[-1].append("@")
				new_grid[-1].append(".")

	return new_grid

def print_grid(grid: list[list[str]], message: str = ''):
	vals: list[str] = [''.join(line) for line in grid]
	print(f"{message}{'\n'.join(vals)}")

def is_box(grid: list[list[str]], i: int, j: int) -> bool:
	return grid[i][j] == '[' or grid[i][j] == ']'

def contain_boxes(grid: list[list[str]], area: set[(int,int)]) -> bool:
	# Here we are interesting in finding at least 1 box, the rest can be white spaces
	if len(area) < 1:
		return False

	at_least_1_box: bool = False
	for i,j in area:
		if area_has_walls(grid, [(i,j)]):
			return False
		if is_box(grid, i, j):
			at_least_1_box = True
	return at_least_1_box


def complete_box(grid: list[list[str]], i: int, j: int) -> int:
	if grid[i][j] == '[':
		return j+1
	
	return j-1


def get_next_area(grid: list[list[str]], area: set[(int,int)], di: int) -> set[(int,int)]:
	# Check the boxes above the current area
	new_area: set[(int,int)] = set()
	for i,j in area:
		i2 = i+di
		if is_box(grid, i2, j):
			new_area.add((i2,j))
			new_area.add((i2,complete_box(grid, i2, j)))
		elif grid[i2][j] == '#':
			new_area.add((i2,j))
	return new_area


def area_has_walls(grid: list[list[str]], area: set[(int,int)]) -> bool:
	for i,j in area:
		if grid[i][j] == '#':
			return True

	return False


def move_boxes(grid: list[list[str]], areas: list[list[(int,int)]], di: int):
	# Iterate in reverse order
	n: int = len(areas)
	for k in range(n-1, -1, -1):
		area: set[(int,int)] = areas[k]
		for i,j in area:
			grid[i+di][j] = grid[i][j]
			# Set the current row to empty
			grid[i][j] = '.'


def find_box(grid: list[list[str]], i: int, j: int) -> list[(int,int)]:
	# We know that (i,j) is part of a box, now find the other part
	area: set[(int,int)] = set()
	area.add((i,j))
	area.add((i,complete_box(grid,i,j)))
	return area


def do_moves(grid: list[list[str]], moves: list[str], x: int, y: int) -> list[list[str]]:
	for move in moves:
		di, dj = DD[move]
		i, j = x + di, y + dj

		#print(f"{move=}  {di=}  {dj=}  {grid[i][j]=}")

		# We hit a wall, do nothing
		if grid[i][j] == '#':
			continue

		# Empty space, move forward
		if grid[i][j] == '.':
			grid[i][j] = '@'
			grid[x][y] = '.'
			x, y = i, j
			#print_grid(grid, f"grid after move {move}: \n")
			continue

		# Here we hit a block, this can potentially move.
		# Check more values in the direction of the block.

		# For part (b) this is a bit trickier actually
		# Because now we shall move a 2x1 box, we need to be smarter.

		# We can split the logic in 2 parts:
		# - if move is horizontal, then act like in part (a)
		# - if move is vertical, then we need to be smarter and use area

		if abs(di) == 0:
			# Horizontal move: essentially we want to move in steps of 2 here

			j2 = j
			while is_box(grid, i, j2):
				j2 = j2 + 2*dj

			#print(f"{i=}  {j=}  {j2=}  {grid[i][j]=}  {grid[i][j2]=}")

			# Now check what we hit
			if grid[i][j2] == '#':
				# This was a continuous wall, so still can't move
				continue

			# At this point it means we can move the boxes
			while j2 != j:
				grid[i][j2] = grid[i][j2-dj]
				j2 -= dj

		else:

			# Vertical move: use areas
			area: set[(int,int)] = set()
			area.add((i,j))
			area.add((i,complete_box(grid,i,j)))
			areas: list[set[(int,int)]] = []
			while True:
				areas.append(area)
				area = get_next_area(grid, area, di)

				#print(f"Next {area=}")

				if not contain_boxes(grid, area):
					break

			#print(f"{areas=}")

			# Check if the new area has any walls
			if area_has_walls(grid, area):
				continue

			# Here it means we can move boxes
			move_boxes(grid, areas, di)

		# Regardless of horizontal/vertical, if we reach this step, then it means we moved boxes
		# So update the robot location
		grid[i][j] = '@'
		grid[x][y] = '.'
		x, y = i, j

		#print_grid(grid, f"grid after move {move}: \n")

	return grid

def calculate_total(grid: list[list[str]], sz: int) -> int:
	total: int = 0
	for i in range(1, sz-1):
		for j in range(1, 2*sz-1):
			if grid[i][j] == '[':
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

	# Part (b): double grid
	grid = double_grid(grid, sz)
	y *= 2
	print_grid(grid, "after doubling\n")
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