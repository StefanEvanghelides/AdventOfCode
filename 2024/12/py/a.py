import argparse
from tqdm import tqdm
import copy

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


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	# Find connected components
	# Run DFS

	seen: set((int,int)) = set()
	components: list[list[(int,int)]] = []
	total: int = 0
	for i in range(n):
		for j in range(n):
			if (i,j) in seen:
				continue

			# New component to process
			components.append([(i,j)])

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
					components[-1].append((i2,j2))

			# Component added, now check the perimeter and area.
			area: int = len(components[-1])
			perimeter: int = compute_perimeter(components[-1], lines, n)

			#print(f"{area=}  {perimeter=}  {lines[i][j]}={components[-1]}")

			total += area * perimeter
	

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