import argparse

def in_boundaries(antinode: (int,int), n: int) -> bool:
	x: int = antinode[0]
	y: int = antinode[1]
	return 0 <= x < n and 0 <= y < n


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	grid: list[list[str]] = [list(x) for x in lines]

	antennas_map: dict[str, list((int,int))] = {}
	n: int = len(grid)
	for i in range(n):
		for j in range(n):
			if grid[i][j] != '.':
				# Antenna found
				ant: str = grid[i][j]
				if ant not in antennas_map:
					antennas_map[ant] = []

				antennas_map[ant].append((i,j))

	# Iterate over each antenna
	antinodes: set((int,int)) = set()
	for antenna, antennas in antennas_map.items():
		m: int = len(antennas)
		for i in range(m-1):
			for j in range(i+1,m):
				# Compute positions of the new antinodes
				antenna1: (int,int) = antennas[i]
				antenna2: (int,int) = antennas[j]
				dx: int = antenna1[0] - antenna2[0]
				dy: int = antenna1[1] - antenna2[1]
				antinode1: (int,int) = (antenna1[0] + dx, antenna1[1] + dy)
				antinode2: (int,int) = (antenna2[0] - dx, antenna2[1] - dy)
				if in_boundaries(antinode1, n):
					antinodes.add(antinode1)
				if in_boundaries(antinode2, n):
					antinodes.add(antinode2)

	print(f"Total: {len(antinodes)}")


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