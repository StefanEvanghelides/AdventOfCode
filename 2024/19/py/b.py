import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# This is solved best with a Trie -> we should index the patterns in a trie to facilitate searching.

# Part (b) doesn't just want yes/no. It wants a count of all possibilities.

# Simple cache. For part (b) the value is not a `bool`, but an `int`.
seen: dict[str, int] = {}

def design_possible(patterns: set[str], design: str, max_sz: int, depth: int = 0) -> int:
	if design in seen:
		return seen[design]

	# Base case: design was possible
	if design == '':
		return 1

	count: int = 0
	for i in range(1,max_sz+1):
		prefix: str = design[:i]
		if prefix in patterns and i <= len(design):
			# Check if we have a positive value here
			subdesign: str = design[i:]
			count += design_possible(patterns, subdesign, max_sz, depth+1)

	seen[design] = count
	return count

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	patterns: set[str] = set(lines[0].split(", "))
	designs: list[str] = lines[2:]

	max_sz: int = max([len(x) for x in patterns])

	possible: int = 0
	total_count: int = 0
	for i in tqdm(range(len(designs))):
		design: str = designs[i]
		count: int = design_possible(patterns, design, max_sz)
		if count > 0:
			possible += 1
			total_count += count

		# break

	print(f"{possible=} -> {total_count=}")


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