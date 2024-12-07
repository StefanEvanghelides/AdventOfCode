import argparse

# Update: Here I solved another problem!
# The task for (a) assumes that the initial direction is kept!

def searchLetter(lines: list[str], i: int, j: int, n: int, m: int, letter: chr) -> list[(int, int)]:
	# Search in the 8 direction for the `letter`.
	results: list[(int, int)] = []
	
	# 1. top
	if i > 0 and lines[i-1][j] == letter:
		results.append((i-1, j))

	# 2. top right
	if i > 0 and j < m - 1 and lines[i-1][j+1] == letter:
		results.append((i-1, j+1))

	# 3. right
	if j < m - 1 and lines[i][j+1] == letter:
		results.append((i, j+1))

	# 4. bottom right
	if i < n - 1 and j < m - 1 and lines[i+1][j+1] == letter:
		results.append((i+1, j+1))

	# 5. bottom
	if i < n - 1 and lines[i+1][j] == letter:
		results.append((i+1, j))

	# 6. bottom left
	if i < n - 1 and j > 0 and lines[i+1][j-1] == letter:
		results.append((i+1, j-1))

	# 7. left
	if j > 0 and lines[i][j-1] == letter:
		results.append((i, j-1))

	# 8. top left
	if i > 0 and j > 0 and lines[i-1][j-1] == letter:
		results.append((i-1, j-1))

	return results

def searchM(lines: list[str], i: int, j: int, n: int, m: int) -> list[(int, int)]:
	return searchLetter(lines, i, j, n, m, 'M')

def searchA(lines: list[str], i: int, j: int, n: int, m: int) -> list[(int, int)]:
	return searchLetter(lines, i, j, n, m, 'A')

def searchS(lines: list[str], i: int, j: int, n: int, m: int) -> list[(int, int)]:
	return searchLetter(lines, i, j, n, m, 'S')

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	total = 0
	n: int = len(lines)
	m: int = len(lines[0])
	for i in range(n):
		for j in range(m):
			# This is the beginning of a word. Search for the rest of the letters
			if lines[i][j] == "X":
				all_m: list[(int, int)] = searchM(lines, i, j, n, m)
				for m_pos in all_m:
					m_i: int = m_pos[0]
					m_j: int = m_pos[1]
					all_a: list[(int, int)] = searchA(lines, m_i, m_j, n, m)
					for a_pos in all_a:
						a_i: int = a_pos[0]
						a_j: int = a_pos[1]
						all_s: list[(int, int)] = searchS(lines, a_i, a_j, n, m)
						total += len(all_s)

	print(f"{total=}")


def main(filename: str):
	# Read file and convert to ints
	input = []
	with open(filename) as file:
		input = file.read()
		solve(input)

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