import argparse
from tqdm import tqdm
import copy

# For part (b) we don't use perimeter, but we use number of sides

dd: list[(int,int)] = [(-1, 0), (1, 0), (0, -1), (0, 1)]

def is_inside(i: int, j: int, n: int) -> bool:
	return 0 <= i < n and 0 <= j < n

def compute_perimeter(component: list[(int,int)], lines: list[str], n: int) -> int:
	perimeter: int = 0
	for i,j in component:
		# Check the 4 neighbours for diffs / out of bounds.
		for di, dj in dd:
			i2, j2 = i + di, j + dj
			if not is_inside(i2, j2, n) or \
					lines[i2][j2] != lines[i][j]:
				perimeter += 1
	return perimeter

def compute_number_sides(component: set((int,int)), n: int) -> int:
	# Use the 2x2 method to determine corners
	corners_count: int = 0

	# Reduce the search area: find the min/max values of the shape
	mini = minj = n
	maxi = maxj = 0
	for i,j in component:
		mini = min(mini,i)
		maxi = max(maxi,i)
		minj = min(minj,j)
		maxj = max(maxj,j)

	# Use a 2x2 sliding window that also checks the out-of-bounds
	# Out of bounds will be counted as "outside".
	# The (i,j) is the top-left corner for this 2x2 box.
	for i in range(mini-1, maxi+1):
		for j in range(minj-1, maxj+1):
			box = [(i,j,), (i,j+1), (i+1,j), (i+1,j+1)]
			outsides = count_outsides(component, box)
			if outsides == 1 or outsides == 3:
				corners_count += 1
			elif outsides == 2 and are_opposite(component, box):
				# If there are 2 pieces opposite, these count as 2 separate corners
				corners_count += 2

	return corners_count

def count_outsides(component: set((int,int)), box: list[(int,int)]) -> int:
	outsides: int = 0
	for i,j in box:
		if (i,j) not in component:
			outsides += 1

	return outsides

def are_opposite(component: set((int,int)), box: list[(int,int)]) -> bool:
	diag1 = box[1] in component and box[2] in component
	diag2 = box[0] in component and box[3] in component
	return diag1 or diag2

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	# Find connected components
	# Run DFS

	seen: set((int,int)) = set()
	components: list[set((int,int))] = []
	total: int = 0
	for i in range(n):
		for j in range(n):
			if (i,j) in seen:
				continue

			# New component to process
			components.append(set())
			components[-1].add((i,j))

			stack: list[(int,int)] = [(i,j)]
			seen.add((i,j))

			while len(stack) > 0:
				si, sj = stack.pop()
				# Check all 4 directions
				for di, dj in dd:
					i2, j2 = si + di, sj + dj
					if not is_inside(i2, j2, n) or \
							(i2, j2) in seen or \
							lines[i2][j2] != lines[si][sj]:
						continue
					seen.add((i2,j2))
					stack.append((i2,j2))
					components[-1].add((i2,j2))

			# Component added, now check the perimeter and area.
			area: int = len(components[-1])
			number_sides: int = compute_number_sides(components[-1], n)

			print(f"COMPONENT: {area=}  {number_sides=}  {lines[i][j]}={components[-1]}")

			total += area * number_sides

	

	print(f"Price {total=}")



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