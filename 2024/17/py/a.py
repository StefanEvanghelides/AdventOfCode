import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# TODO: Not solved!
# Part (b) requires us to do debugging and working backwards.
# Essentially find a value for RegA such that the output is the same as the program input.

def get_combo_operand(regA: int, regB: int, regC: int, operand: int) -> int:
	if operand == 4:
		return regA
	elif operand == 5:
		return regB
	elif operand == 6:
		return regC
	
	# Operands 0-3 (also 7 as it's invalid)
	return operand


def run_program(regA: int, regB: int, regC: int, programs: list[int]):
	n: int = len(programs)
	output: list[str] = []
	i: int = 0
	iter: int = 0
	while i < n-1:
		iter += 1
		opcode = programs[i]
		operand = programs[i+1]

		print(f"{iter=} - {i=} -> ({opcode=}  {operand=}): before {regA=} ({str(bin(regA))[2:]})  {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]})")

		# Run program
		if opcode == 0:
			combo_operand: int = get_combo_operand(regA, regB, regC, operand)
			val: int = 2 ** combo_operand
			regA = regA // val
		elif opcode == 1:
			# This doesn't use a combo operand, but the literal value itself
			regB = regB ^ operand
		elif opcode == 2:
			combo_operand: int = get_combo_operand(regA, regB, regC, operand)
			regB = combo_operand % 8
		elif opcode == 3:
			# This is valid only if regA != 0
			if regA != 0:
				i = operand
				# Do not increase the pointer value, hence continue loop here
				continue
		elif opcode == 4:
			# Operand is irrelevant here
			regB = regB ^ regC
		elif opcode == 5:
			combo_operand: int = get_combo_operand(regA, regB, regC, operand)
			output.append(str(combo_operand % 8))
		elif opcode == 6:
			# Works just like A, but we store result in regB
			combo_operand: int = get_combo_operand(regA, regB, regC, operand)
			val: int = 2 ** combo_operand
			regB = regA // val
		else:
			# Works just like A, but we store result in regC
			combo_operand: int = get_combo_operand(regA, regB, regC, operand)
			val: int = 2 ** combo_operand
			regC = regA // val

		# Go to the next opcode (skip the operand)
		i += 2

		print(f"  after: {regA=}  {regB=}  {regC=} -> {output=}")


	print(f"Final: {regA=}  {regB=}  {regC=} -> {iter=}")
	print(f"{output=}")
	print(f"{','.join(output)=}")
	print(f"Len prog: {len(programs)}  len output: {len(output)}")


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	regA: int = int(lines[0].split(": ")[1])
	regB: int = int(lines[1].split(": ")[1])
	regC: int = int(lines[2].split(": ")[1])

	programs: list[int] = [int(x) for x in lines[4].split(": ")[1].split(",")]

	print(f"{regA=}  {regB=}  {regC=}")
	print(f"{programs=}")

	run_program(regA, regB, regC, programs)


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