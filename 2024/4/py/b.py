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


def countPlusFormat(lines: list[str], i: int, j: int, n: int, m: int) -> bool:
	# Check top-south-left-right. NOTE: This is not actually needed!
	return False


def countXFormat(lines: list[str], i: int, j: int, n: int, m: int) -> bool:
	# Check diagonal corners
	all_m: list[(int,int)] = []
	all_s: list[(int,int)] = []

	# top-left corner
	if lines[i-1][j-1] == 'M':
		all_m.append((i-1, j-1))
	elif lines[i-1][j-1] == 'S':
		all_s.append((i-1, j-1))

	# top-right corner
	if lines[i-1][j+1] == 'M':
		all_m.append((i-1, j+1))
	elif lines[i-1][j+1] == 'S':
		all_s.append((i-1, j+1))

	# bottom-left corner
	if lines[i+1][j-1] == 'M':
		all_m.append((i+1, j-1))
	elif lines[i+1][j-1] == 'S':
		all_s.append((i+1, j-1))

	# bottom-right corner
	if lines[i+1][j+1] == 'M':
		all_m.append((i+1, j+1))
	elif lines[i+1][j+1] == 'S':
		all_s.append((i+1, j+1))

	# Guard: ensure that we have 2 and 2
	if len(all_m) != 2 or len(all_s) != 2:
		return False

	# Guard: ensure letters are next to each other. It's sufficient to check M's.
	m1_i: int = all_m[0][0]
	m1_j: int = all_m[0][1]
	m2_i: int = all_m[1][0]
	m2_j: int = all_m[1][1]
	if m1_i + m2_i == i * 2 and m1_j + m2_j == j * 2:
		return False
	

	# All checks passed
	return True


def countA(lines: list[str], i: int, j: int, n: int, m: int) -> int:
	count: int = 0

	# Guards: A cannot be on edge
	if i == 0 or j == 0 or i >= n-1 or j >= m-1 or lines[i][j] != 'A':
		return count

	count += countPlusFormat(lines, i, j, n, m)
	count += countXFormat(lines, i, j, n, m)

	return count

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	total = 0
	n: int = len(lines)
	m: int = len(lines[0])
	for i in range(n):
		for j in range(m):
			# This is the beginning of a word. Search for the rest of the letters
			if lines[i][j] == "A":
				count: int = countA(lines, i, j, n, m)
				
				#print(f"A_i={i}  A_j={j}  {count=}")
				
				total += count

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