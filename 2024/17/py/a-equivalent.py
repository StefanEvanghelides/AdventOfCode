import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# The equivalent program essentially disregards the `program` input and just writes the code for it.


def run_program(regA: int, regB: int, regC: int):
	output: list[str] = []
	while regA > 0:
		regB = (regA & 7) ^ 1
		regC = regA // (2 ** regB)
		regB = (regB ^ regC) ^ 4
		regA = regA >> 3;
		output.append(str(regB & 7))

		print(f"  after: {regA=}  {regB=}  {regC=} -> {output=}")


	print(f"Final: {regA=}  {regB=}  {regC=}")
	print(f"{output=}")
	print(f"{','.join(output)=}")


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	regA: int = int(lines[0].split(": ")[1])
	regB: int = int(lines[1].split(": ")[1])
	regC: int = int(lines[2].split(": ")[1])

	print(f"{regA=}  {regB=}  {regC=}")

	run_program(regA, regB, regC)


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