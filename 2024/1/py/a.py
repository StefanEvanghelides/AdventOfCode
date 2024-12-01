import argparse

def solve(raw_input: list[str]):
	inp_pairs: list[str] = raw_input.split('\n')
	lefts: list[int] = []
	rights: list[int] = []
	for inp_pair in inp_pairs:
		elems: list[str] = inp_pair.split()
		left: int = int(elems[0])
		right: int = int(elems[1])
		lefts.append(left)
		rights.append(right)

	lefts = sorted(lefts)
	rights = sorted(rights)

	total = 0
	for i in range(len(lefts)):
		total = total + abs(lefts[i] - rights[i])

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