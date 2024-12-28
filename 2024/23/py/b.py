from collections import OrderedDict, deque
import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# For part (b) we want the largest cyclic graph, sorted alphabetically.

def print_cycles(cycles: set[frozenset[str]]):
	print(f"Cycles: {len(cycles)}")

	sorted_cycles: list[list[str]] = []
	for cycle in cycles:
		sorted_cycles.append(sorted(cycle))
	sorted_cycles = sorted(sorted_cycles)

	for cycle in sorted_cycles:
		print(f"  {cycle}")
	print()

def compute_all_cycles(lan: dict[str, set[str]]) -> set[frozenset[str]]:
	cycles = set()
	stack = [(node, frozenset({node})) for node in lan]
	while stack:
		node, cycle = stack.pop()
		for neighbour in lan[node] - cycle:
			if lan[neighbour] >= cycle:
				new_cycle = cycle | {neighbour}
				if new_cycle not in cycles:
					cycles.add(new_cycle)
					stack.append((neighbour, new_cycle))

	return cycles

def compute_largest_cycle(lan: dict[str, set[str]]) -> frozenset[str]:
	all_cycles: set[frozenset[str]] = compute_all_cycles(lan)
	largest_cycle: frozenset[str] = None
	sz: int = 0
	for cycle in all_cycles:
		if len(cycle) > sz:
			sz = len(cycle)
			largest_cycle = cycle

	return largest_cycle


def filter_cycles(cycles: set[set[str]]) -> int:
	count: int = 0

	for cycle in cycles:
		for node in cycle:
			if node[0] == 't':
				count += 1
				break

	return count

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	lan: dict[str, set[str]] = {}
	for line in lines:
		elems: list[str] = line.split("-")
		node1: str = elems[0]
		node2: str = elems[1]

		if node1 not in lan:
			lan[node1] = set()
		if node2 not in lan:
			lan[node2] = set()

		lan[node1].add(node2)
		lan[node2].add(node1)

	# print(f"{lan=}")

	# Compute cycles
	# Part (b) -> get the largest cycle
	largest_cycle: frozenset[str] = compute_largest_cycle(lan)
	print(f"{','.join(sorted(largest_cycle))}")



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