import argparse

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

					# Now get deltas for the direction between X -> M
					delta_i = m_i - i
					delta_j = m_j - j

					# Now check if we can also find A and S on the same directions
					next_i = m_i + delta_i
					next_j = m_j + delta_j

					#print(f"X_i={i}  X_j={j}  {m_i=}  {m_j=}  {delta_i=}  {delta_j=}  {next_i=}  {next_j=}  ")

					if next_i >= 0 and next_i < n and next_j >= 0 and next_j < m and lines[next_i][next_j] == "A":
						next_i += delta_i
						next_j += delta_j
						if next_i >= 0 and next_i < n and next_j >= 0 and next_j < m and lines[next_i][next_j] == "S":
							total += 1

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