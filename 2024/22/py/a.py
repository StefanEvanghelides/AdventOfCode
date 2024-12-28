import argparse
from collections import OrderedDict
from tqdm import tqdm
import copy
import functools
import heapq

def compute_secret(val: int, iterations: int) -> int:
	res: int = val

	for _ in range(iterations):
		tmp = res * 64
		res = (res ^ tmp ) % 16777216
		tmp = res // 32
		res = (res ^ tmp ) % 16777216
		tmp = res * 2048
		res = (res ^ tmp ) % 16777216

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