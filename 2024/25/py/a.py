from collections import OrderedDict, deque
import argparse
from tqdm import tqdm
import copy
import functools
import heapq

NUMBER_PINS: int = 5
MAX_KEY_LENGTH: int = 5

def key_fits_in_lock(key: list[int], lock: list[int]) -> bool:
	for i in range(NUMBER_PINS):
		if key[i] + lock[i] > MAX_KEY_LENGTH:
			return False

	return True

def compute_all_working_combinations(keys: list[list[int]], locks: list[list[int]]) -> int:
	keys_that_fit: int = 0
	for lock in locks:
		for key in keys:
			# Check fitness
			if key_fits_in_lock(key, lock):
				keys_that_fit += 1

	return keys_that_fit

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	keys: list[list[int]] = []
	locks: list[list[int]] = []
	i: int = 0
	while i < n-6:
		# Find a grouping for either key or lock
		lines_7: list[str] = lines[i:i+7]
		if lines_7[0] == '#####':
			# it's a lock!
			lock: list[int] = [0] * NUMBER_PINS
			for x in range(1,7):
				for y in range(NUMBER_PINS):
					if lines_7[x][y] == '#':
						lock[y] += 1
			locks.append(lock)
		elif lines_7[-1] == '#####':
			# it's a key!
			key: list[int] = [0] * NUMBER_PINS
			for x in range(6):
				for y in range(NUMBER_PINS):
					if lines_7[x][y] == '#':
						key[y] += 1
			keys.append(key)

		i += 8

	# print(f"keys={len(keys)}\n {keys}\n")
	# print(f"locks={len(locks)}\n {locks}")

	combinations: int = compute_all_working_combinations(keys, locks)
	print(f"{combinations=}")


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