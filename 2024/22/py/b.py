from collections import OrderedDict, deque
import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# For part (b) we need to keep track of the last 4 elements (diffs).

# The trick here is to realize that we can already compute scores and cache per tuple.
# There are 19^4 (hence ~103k) possibilities, hence we can store all of them.
# The important detail is to note that we can compute everything in one pass and
# to be careful to stop checking the same pattern after it was seen the first time.

scores_all: dict[(int,int,int,int),int] = {}

def compute_secret(val: int, iterations: int) -> int:
	res: int = val

	prev: int = val
	prev_d: int = 0
	nums: list[int] = [0 for _ in range(3)]
	scores_first: dict[(int,int,int,int),int] = {}
	for i in range(iterations):
		tmp = res * 64
		res = (res ^ tmp ) % 16777216
		tmp = res // 32
		res = (res ^ tmp ) % 16777216
		tmp = res * 2048
		res = (res ^ tmp ) % 16777216

		curr_d: int = res % 10

		if i < 3:
			nums[i] = curr_d - prev_d
		else:

			# Compute 4th number
			num4 = curr_d - prev_d

			# Compute tuple and add it to the caches
			tup: (int,int,int,int) = (nums[0], nums[1], nums[2], num4)
			if tup not in scores_first:
				# First time seeing it, then add the value to the scores map
				scores_first[tup] = curr_d

			# Slide values by 1
			nums[0] = nums[1]
			nums[1] = nums[2]
			nums[2] = num4


		prev = res
		prev_d = prev % 10


	# Now "merge" the first scores into the whole map of scores.
	for tup, score in scores_first.items():
		if tup not in scores_all:
			scores_all[tup] = 0
		scores_all[tup] += score

	return res

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	# For part (a) we have 2k iterations
	iterations: int = 2000

	results: OrderedDict[int,int] = OrderedDict()
	for i in tqdm(range(n)):
		line: str = lines[i]
		val: int = int(line)
		res: int = compute_secret(val, iterations)
		results[val] = res

	# Compute total
	total: int = 0
	for val in results.values():
		total += val
	
	print(f"{total=}")

	# Find the largest value
	max_val: int = 0
	for tup, val in scores_all.items():
		#print(f"{tup=}  -> {val=}")
		max_val = max(max_val, val)

	print(f"{max_val=}")


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