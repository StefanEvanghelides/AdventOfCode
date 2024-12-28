from collections import OrderedDict, deque
import argparse
from tqdm import tqdm
import copy
import functools
import heapq

def compute(val1: bool, val2: bool, op: str) -> bool:
	res: bool = False
	if op == "AND":
		res = val1 and val2
	elif op == "OR":
		res = val1 or val2
	else:
		# XOR
		res = val1 ^ val2

	return res

def solve_equations(values: dict[str,bool], operands1: list[str] = [], operators: list[str] = [], operands2: list[str] = [], results: list[str] = []):
	iter: int = 0
	while True:
		iter += 1
		n: int = len(operands1)
		
		if iter % 10 == 0:
			print(f"{iter=} -> remaining values: {n}")

		if n < 1:
			# Work is finally over
			break

		# Find index of the next possible equation to solve
		i: int = 0
		while i < n:
			if operands1[i] in values and operands2[i] in values:
				break
			i += 1

		# Solve equation
		values[results[i]] = compute(values[operands1[i]], values[operands2[i]], operators[i])
		
		# Remove value from the lists
		operands1.pop(i)
		operators.pop(i)
		operands2.pop(i)
		results.pop(i)


def get_z_values(values: dict[str,bool]) -> dict[str,bool]:
	z_values: dict[str,bool] = {}
	for key, val in values.items():
		if key[0] == 'z':
			z_values[key] = val
	return z_values

def compute_final_value(z_values: dict[str,bool]) -> int:
	# Sort the dict
	result: str = ''
	for val in dict(sorted(z_values.items(), reverse=True)).values():
		result += '1' if val else '0'

	return int(result, 2)

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	n: int = len(lines)

	values: dict[str,bool] = {}
	i: int = 0
	while i < n:
		line = lines[i]
		if line == '':
			i += 1
			break

		elems: list[str] = line.split(": ")
		operand: str = elems[0]
		val: bool = elems[1] == '1'
		values[operand] = val
		i += 1

	operands1: list[str] = []
	operators: list[str] = []
	operands2: list[str] = []
	results: list[str] = []
	while i < n:
		line = lines[i]
		elems: list[str] = line.split(" -> ")
		results.append(elems[1])
		parts: list[str] = elems[0].split(" ")
		operands1.append(parts[0])
		operators.append(parts[1])
		operands2.append(parts[2])
		i += 1
		
	print(f"{values=}")
	print(f"{operands1=}")
	print(f"{operators=}")
	print(f"{operands2=}")
	print(f"{results=}")

	solve_equations(values, operands1, operators, operands2, results)

	z_values: dict[str,bool] = get_z_values(values)
	print(f"{z_values=}")

	final_value: int = compute_final_value(z_values)
	print(f"{final_value=}")


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