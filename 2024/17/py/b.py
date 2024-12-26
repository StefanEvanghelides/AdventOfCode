import argparse
from tqdm import tqdm
import copy
import functools
import heapq

# This part (b) requires debugging the run_program to produce an output equivalent to the input program.

def run_program(regA: int, regB: int, regC: int) -> list[str]:
	output: list[str] = []
	while regA > 0:
		regB = (regA & 7) ^ 1
		regC = regA // (2 ** regB)
		regB = (regB ^ regC) ^ 4
		regA = regA >> 3;
		output.append(str(regB & 7))

		print(f"  after: {regA=} ({str(bin(regA))[2:]})  {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]}) -> {output=}")

	return output


def run_program_simplified(regA: int, regB: int, regC: int) -> list[str]:
	output: list[str] = []
	iteration: int = 0
	while regA > 0:
		iteration += 1
		regC = regA // (2 ** ((regA & 7) ^ 1))
		regB = (regA & 7) ^ 1 ^ (regA // (2 ** ((regA & 7) ^ 1))) ^ 4

		print(f"  curr:           {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]})")

		regA = regA >> 3;
		output.append(str(regB & 7))

		print(f"  after (i={iteration}): {regA=} ({str(bin(regA))[2:]})  {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]}) -> {output=}")

	return output



def run_program_simplified2(regA: int, regB: int, regC: int) -> list[str]:
	output: list[str] = []
	iteration: int = 0
	while regA > 0:
		iteration += 1

		part: int = (regA & 7) ^ 1
		regC = regA // (2 ** part)
		regB = (regA & 7) ^ 5 ^ (regA // (2 ** part))
		# So essentially here: we change bits 1 and 3, keep bit 2 the same.
		# Then we xor with regC (which is this smaller number)

		print(f"  curr:           {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]})")

		regA = regA >> 3;
		output.append(str(regB & 7))

		print(f"  after (i={iteration}): {regA=} ({str(bin(regA))[2:]})  {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]}) -> {output=}")

	return output


def run_program_simplified3(regA: int, regB: int, regC: int) -> list[str]:
	output: list[str] = []
	iteration: int = 0

	print(f"{regA=} ({str(bin(regA))[2:]})")

	while regA > 0:
		iteration += 1

		part: int = regA & 7 ^ 1;
		regC = regA // (2 ** part)
		regB = (regA // (2 ** part)) ^ (regA & 7) ^ 5
		# So essentially here: we change bits 1 and 3, keep bit 2 the same.
		# Then we xor with regC (which is this smaller number)

		print(f"  curr:      {part=} ({str(bin(part))[2:]})   {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]})")

		regA = regA >> 3;
		output.append(str(regB & 7))

		print(f"  after (i={iteration}): {regA=} ({str(bin(regA))[2:]})  {regB=} ({str(bin(regB))[2:]})  {regC=} ({str(bin(regC))[2:]}) -> {output=}")

	return output



def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	regA: int = int(lines[0].split(": ")[1])
	regB: int = int(lines[1].split(": ")[1])
	regC: int = int(lines[2].split(": ")[1])

	programs: list[int] = [int(x) for x in lines[4].split(": ")[1].split(",")]

	print(f"Initial {regA=} ({str(bin(regA))[2:]})  {regB=}  {regC=}")

	output = run_program_simplified3(regA, regB, regC)
	expected_output = ['7', '4', '2', '0', '5', '0', '5', '3', '7']

	print(f"{programs=}")
	print(f"{output=}")


	if expected_output == output:
		print(f"CORRECT OUTPUT SO FAR")
	else:
		print(f"No match yet! Try again!")

	if programs == output:
		print(f"THEY MATCH! RegA={regA}")
	else:
		print(f"No match yet! Try again!")


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