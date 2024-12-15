import argparse
from tqdm import tqdm
import copy

def compute_block(line: list[int], n: int, sz: int):
	block: list[int] = [None] * n

	block_idx: int = 0
	block_nr: int = 0
	for i in range(sz):
		val: int = line[i]
		if i % 2 == 0:
			for j in range(val):
				block[block_idx+j] = block_nr
			block_nr += 1
		block_idx += val

	return block

def compute_checksum(block: list[int]) -> int:
	checksum: int = 0

	for i, val in enumerate(block):
		if val != None:
			checksum += i * val

	return checksum

def move(block: list[int], n: int):
	# Use the left-right indices strategy to move blocks.

	# Compute left-right indices
	left: int = 0
	while block[left] != None:
		left += 1
	right: int = n-1
	while block[right] == None:
		right -= 1

	# Now move values
	while left < right:
		block[left] = block[right]
		block[right] = None

		# Find next left-right indices
		while block[right] == None:
			right -= 1
		while block[left] != None:
			left += 1

	return block


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	line: list[int] = [int(x) for x in lines[0]]
	sz: int = len(line)
	n: int = sum(line)
	block: list[int] = compute_block(line, n, sz)

	print(f"initial {block=}")

	move(block, n)

	print(f"final {block=}")

	# First empty value now acts as the final size
	print(f"Checksum: {compute_checksum(block)}")


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