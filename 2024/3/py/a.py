import argparse


def isMul(line: str, idx: int, sz: int) -> bool:
	# Guard: going out of bounds
	if idx + 3 >= sz:
		return False

	return line[idx] == 'm' and line[idx+1] == 'u' and line[idx+2] == 'l'


def isValidParenthesis(line: str, idx: int, sz: int) -> int:
	# Guard: going out of bounds
	if idx >= sz:
		return 0
	
	if line[idx] != '(':
		return 0


	idx2: int = line.find(')', idx+1)
	if idx2 > -1:
		# Found our value, now check the numbers.
		values: list[str] = line[idx+1:idx2].split(',')
		#print(f"{values=}")
		if len(values) != 2:
			return 0
		
		if values[0].isdigit() and values[1].isdigit():
			return idx2 + 1

	# If we reached this, then number is invalid
	return 0


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')
	total = 0
	for line in lines:
		# Parse string based on chars
		sz: int = len(line)
		for i in range(sz):
			# Potentially the beginning of `mul`
			if isMul(line, i, sz):
				i += 3
				# Check numbers between 2 parentheses: j is the index of final parentheses
				j: int = isValidParenthesis(line, i, sz)
				if j > 0:
					# Valid numbers, multiply them!
					# i+1 and j-1 to remove parentheses
					substr: str = line[i+1 : j-1]
					values: list[str] = substr.split(',')
					total += int(values[0]) * int(values[1])
					i += j

	print(f"{total=}")


def main(filename: str):
	# Read file and convert to ints
	input = []
	with open(filename) as file:
		input = file.read()
		solve(input)

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