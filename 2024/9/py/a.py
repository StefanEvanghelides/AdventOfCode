import argparse
from tqdm import tqdm
import copy

def compute_checksum(block: list[int], n: int) -> int:
	checksum: int = 0

	for i in range(n):
		checksum += i * block[i]

	return checksum

def find_last_val_idx(block: list[int], n: int) -> int:
	last_val: int = n-1
	while last_val >= 0 and block[last_val] == -1:
		last_val -= 1

	return last_val

def find_first_empty_idx(block: list[int], previous_first_empty: int, last_val: int) -> int:
	# Find first empty value
	i: int = previous_first_empty
	while i <= last_val and block[i] != -1:
		i += 1

	return i

def is_finished(block: list[int], first_empty: int, last_val: int) -> bool:
	# Ensure that there is no other value after the first -1
	for j in range(first_empty, last_val+1):
		if block[j] > -1:
			return False

	return True

def process_block(block: list[int], empty1: int, last1: int) -> int:
	# Find first (set of) empty spot(s). Note: empty1 is already known
	empty2: int = empty1
	while empty2 < last1 and block[empty2] == -1:
		empty2 += 1

	# Find last (set of) value(s). Note: Last1 is already known
	last2: int = last1
	while last2 >= 0 and block[last2] != -1:
		last2 -= 1

	'''
	More details: assume we are in this situation:
	# 009..111...2...333.44.5555.6666.777.88889.

	empty1-2 = indices 3 -> 5 (last one is exclusive)
	last2-1 = indices n-7 -> n-2 (first one is exclusive)

	We now want to know which part can cover which first,
	so we take the deltas of the empty1-2 and last1-2
	'''

	delta_empty: int = abs(empty1 - empty2)
	delta_last: int = abs(last1 - last2)
	delta: int = delta_empty if delta_empty <= delta_last else delta_last

	#print(f"{delta_empty=}  {delta_last=}  {delta=}  {empty1=}  {empty2=} {last1=}  {last2=}")

	for i in range(delta):
		block[empty1+i] = block[last1-i]
		block[last1-i] = -1

	#print(f"{last1-delta=}  ")

	return last1 - delta


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	line: list[int] = [int(x) for x in lines[0]]
	sz: int = len(line)

	n: int = 9 * sz
	block: list[int] = [-1] * n

	block_idx: int = 0
	block_nr: int = 0
	for i in range(sz):
		val: int = line[i]
		if i % 2 == 0:
			for j in range(val):
				block[block_idx+j] = block_nr
			block_nr += 1
		block_idx += val

	#print(f"{block=}")

	first_empty: int = 0
	last_val: int = find_last_val_idx(block, n)
	while True:
		first_empty = find_first_empty_idx(block, first_empty, last_val)
		print(f"{first_empty=}")
		if is_finished(block, first_empty, last_val):
			print(f"Finished!  Block=\n{block}")
			break

		last_val = process_block(block, first_empty, last_val)

		# Ensure that we are not stuck on empty spot
		last_val = find_last_val_idx(block, last_val+1)

	# First empty value now acts as the final size
	print(f"Checksum: {compute_checksum(block, first_empty)}")


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