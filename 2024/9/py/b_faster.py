import argparse
from tqdm import tqdm
import copy
from collections import OrderedDict

# 6335973512055 too high.
# 8528125309857 (too high, not posted).


# Blocks = dict[block_id, (start_idx, sz)]
def compute_block(line: list[int], n: int) -> (list[int], OrderedDict):
	block: list[int] = [None] * n

	block_idx: int = 0
	block_nr: int = 0
	blocks: dict[int,int] = {}
	for i, val in enumerate(line):
		val: int = line[i]
		if i % 2 == 0:
			for j in range(val):
				block[block_idx+j] = block_nr
			blocks[block_nr] = (block_idx, val)
			block_nr += 1
		block_idx += val

	return (block, blocks)

def compute_empties(block: list[int], n: int) -> OrderedDict:
	empties = OrderedDict()

	i: int = 0
	while i < n:
		if block[i] != None:
			# No while so that we can re-check the top condition
			i += 1
			continue

		start: int = i
		end: int = i
		while block[end] == None:
			end += 1

		count: int = end - start
		empties[start] = count
		i += count

	return empties

def compute_checksum(block: list[int]) -> int:
	checksum: int = 0

	for i, val in enumerate(block):
		if val != None:
			checksum += i * val

	return checksum


def move(block: list[int], n: int, empties: OrderedDict, blocks: dict[int,int]):
	# Use the left-right indices strategy to move blocks.
	sz: int = len(blocks)
	block_nr: int = sz - 1
	print(f"{block_nr=}  {sz=}")
	for block_idx in tqdm(range(sz-1, 0, -1)):
		file_idx, file_sz = blocks[block_idx]
		for i, empty_sz in empties.items():
			if empty_sz >= file_sz:
				# We can move block!
				remaining_empty_sz: int = empty_sz - file_sz
				if remaining_empty_sz > 0:
					# Update emptiness and resort.
					empties[i+file_sz] = remaining_empty_sz
					empties = OrderedDict(sorted(empties.items()))

				# Remove current empty idx and resort them.
				empties.pop(i, None)

				# Update blocks
				for j in range(file_sz):
					block[i+j] = block[file_idx+j]
					block[file_idx+j] = None

				break

	return block


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	line: list[int] = [int(x) for x in lines[0]]
	n: int = sum(line)
	block, blocks = compute_block(line, n)
	empties = compute_empties(block, n)

	print(f"initial {block=}\ninitial {empties=}\ninitial {blocks=}")

	move(block, n, empties, blocks)

	print(f"final {block=}\nfinal {empties=}\ninitial {blocks=}")

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