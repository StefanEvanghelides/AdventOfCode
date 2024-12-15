import argparse
from tqdm import tqdm
import copy
from collections import OrderedDict

# 6335973512055 too high.
# 8528125309857 (too high, not posted).
# 6335972980679

def compute_block(line: list[int], n: int) -> (list[int], list[int], list[int]):
	block: list[int] = [None] * n
	size = [0] * n
	file_indices = [0] * n

	block_idx: int = 0
	block_nr: int = 0
	for i, val in enumerate(line):
		val: int = line[i]
		if i % 2 == 0:
			for j in range(val):
				block[block_idx+j] = block_nr
			file_indices[block_nr] = block_idx
			size[block_nr] = val
			block_nr += 1
		block_idx += val

	return (block, size, file_indices)


def compute_checksum(block: list[int]) -> int:
	checksum: int = 0

	for i, val in enumerate(block):
		if val != None:
			checksum += i * val

	return checksum


def move_files(block: list[int], n: int, size: list[int], file_indices: list[int]) -> list[int]:
	# Find the current file to move
	block_size = 0
	while size[block_size] > 0:
		block_size += 1
	block_size -= 1

	for block_nr in tqdm(range(block_size, 0, -1)):
		# Find first free space that works
		empty_space_sz = 0
		first_empty = 0
		while first_empty < file_indices[block_nr] and empty_space_sz < size[block_nr]:
			first_empty = first_empty + empty_space_sz
			empty_space_sz = 0
			while block[first_empty] != None:
				first_empty += 1
			while first_empty + empty_space_sz < len(block) and block[first_empty + empty_space_sz] == None:
				empty_space_sz += 1
		

		#print(f"{block_nr=}  {block_size=}  {empty_space_sz=}  {first_empty=}  {file_indices[block_nr]=}  {size[block_nr]=}")

		# First empty spot is beyond the current file index, so skip this.
		if first_empty >= file_indices[block_nr]:
			continue

		# Found our indices for the file group and the empty space. Now swap them around.
		for idx in range(first_empty, first_empty + size[block_nr]):
			block[idx] = block_nr
		for idx in range(file_indices[block_nr], file_indices[block_nr] + size[block_nr]):
			block[idx] = None
		
	return block


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	line: list[int] = [int(x) for x in lines[0]]
	n: int = sum(line)
	block, size, file_indices = compute_block(line, n)

	print(f"initial {block=}\n{size=}\n{file_indices=}")

	move_files(block, n, size, file_indices)

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