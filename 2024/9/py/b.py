import argparse
from tqdm import tqdm
import copy

# 6335973512055 too high.
# 8528125309857 (too high, not posted).

def compute_checksum(block: list[int], n: int) -> int:
	checksum: int = 0

	for i in range(n):
		if block[i] == -1:
			# Skip empty, this would be treated as 0 anyways.
			continue
		checksum += i * block[i]

	return checksum

def find_first_empty_group(block: list[int], previous_first_empty: int, last_val: int) -> (int, int):
	# Find first empty value
	i: int = previous_first_empty
	while i <= last_val and block[i] != -1:
		i += 1

	# Now also find the next empty value
	j: int = i
	while j < last_val and block[j] == -1:
		j += 1

	return (i, j)

def find_last_val_group(block: list[int], previous_last1: int, last_block_nr: int) -> (int, int):
	last1: int = previous_last1-1
	while last1 >= 0 and (block[last1] == -1 or block[last1] >= last_block_nr):
		last1 -= 1

	last2: int = last1
	while last2 >= 0 and block[last2] == block[last1]:
		last2 -= 1

	return (last1, last2)


def process_block(block: list[int], last1: int, last2: int, last_block_nr: int) -> int:
	# Find first (set of) empty spot(s). Note: empty1 is already known
	empty1, empty2 = find_first_empty_group(block, 0, last2)

	# Guard: time for some magic!
	if empty1 >= last1 or empty2 >= last1:
		print(f"MAGIC ENDS IN PROCESS_BLOCK: {empty1=}  {empty2=}  {last1=}  {last2=}")
		return -1

	delta_empty: int = abs(empty1 - empty2)
	delta_last: int = abs(last1 - last2)

	#print(f"Processing: {empty1=}  {empty2=}  {last1=}  {last2=}  {delta_empty=}  {delta_last=}")
	#print(f"Processing block:\n{block}")

	# We now want to ensure that we can fit delta_last into delta_empty.
	# If not, then find the next set of empty1-2 until something fits.
	# Note: If nothing fits, then update the last1 value.
	while delta_empty < delta_last:
		# Find the new empty1-2 values
		empty1, empty2 = find_first_empty_group(block, empty2, last2)

		# Time for magic! Last values are less than the empty1-2 values
		if empty1 >= last2 or empty2 >= last2:
			#print(f"WHILE - MAGIC ENDS IN PROCESS_BLOCK: {empty1=}  {empty2=}  {last1=}  {last2=}")
			return last2

		# Recompute the delta_last
		delta_empty = abs(empty1 - empty2)

		#print(f"WHILE - Processing: {empty1=}  {empty2=}  {last1=}  {last2=}  {delta_empty=}  {delta_last=}")


	# Here use delta_last because the new delta_empty >= delta_last
	for i in range(delta_last):
		block[empty1+i] = block[last1-i]
		block[last1-i] = -1

	# Iteration over, this means that the current group finished,
	# so return last2 to move on the the next group.
	return last2


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

	last1, last2 = find_last_val_group(block, n, sz)
	last_block_nr: int = block[last1]

	# Slice block to make it easier to work with
	block = block[:last1+1]
	n = len(block)

	print(f"initial: {last1=}  {last2=}  {last_block_nr=}")

	while True:
		last2 = process_block(block, last1, last2, last_block_nr)
		if last2 < 1:
			# MAGIC! We need to stop here, we  simply can't continue
			break

		# Ensure that we are not stuck on empty spot.
		last1, last2 = find_last_val_group(block, last2+1, last_block_nr)
		last_block_nr = block[last1]

		print(f"new {last1=}  new {last2=} new {last_block_nr=}")
		#print(f"block:\n{block}")

	# Now do the checksum for the full block
	print(f"Checksum: {compute_checksum(block, n)}")


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