import argparse

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	# We can already operate on the lines, this is the grid.
	grid: list[str] = lines
	n: int = len(grid)
	m: int = len(grid[0])

	# dir = [up, right, down, left]
	dir = 0 # default = up
	dd: list((int,int)) = [(-1,0), (0,1), (1,0), (0,-1)]

	# Find the starting carret
	i: int = 0
	j: int = 0
	for i in range(n):
		found = False
		for j in range(m):
			if grid[i][j] == '^':
				found = True
				break
		if found:
			break

	seen: set((int, int)) = set()
	while True:
		seen.add((i, j))

		next_i = i + dd[dir][0]
		next_j = j + dd[dir][1]

		if not (0 <= next_i < n and 0 <= next_j < m):
			# Out of bounds
			#print(f"Out of bounds {next_i=}  {next_j=}")
			break

		if grid[next_i][next_j] == '#':
			#print(f"Turning at {next_i=}  {next_j=}")
			dir = (dir + 1) % 4

		else:
			i, j = next_i, next_j


	print(f"Total unique: {len(seen)}")


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