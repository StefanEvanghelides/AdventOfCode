import argparse
from tqdm import tqdm
import copy


# For (b), blink 75 instead of 25 times.
# For this solution we cannot use a Tree-structure anymore. The trick here is to use a simple map to count frequencies.
# A lot of these numbers will repeat themselves. We don't actually care about the order of these elements.

def blink(previous_cache: dict[int,int]) -> dict[int,int]:
	new_cache: dict[int,int] = {}
	for nr, count in previous_cache.items():
		if nr == 0:
			# Rule #1: 0 -> 1
			if 1 not in new_cache:
				new_cache[1] = 0
			new_cache[1] += count
		else:
			data_str: str = str(nr)
			sz: int = len(data_str)
			if sz % 2 == 0:
				# Rule #2: even number of digits, then split
				half_idx: int = sz // 2
				first_half: int = int(data_str[:half_idx])
				second_half: int = int(data_str[half_idx:])
				if first_half not in new_cache:
					new_cache[first_half] = 0
				new_cache[first_half] += count
				if second_half not in new_cache:
					new_cache[second_half] = 0
				new_cache[second_half] += count
			else:
				# Rule #3: multiply by 2024
				val: int = nr * 2024
				if val not in new_cache:
					new_cache[val] = 0
				new_cache[val] += count

	return new_cache


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	numbers: list[int] = [int(x) for x in lines[0].split(' ')]
	print(f"{numbers=}")

	# Init cache
	cache: dict[int,int] = {}
	for n in numbers:
		cache[n] = 1

	# Task for (b) "blink 75 times"
	for _ in tqdm(range(75)):
		cache = blink(cache)

	# Count leaves
	print(f"Total: {sum(cache.values())}")



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