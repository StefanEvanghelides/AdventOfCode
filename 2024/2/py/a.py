import argparse

def isSafe(record: list[int]) -> bool:
	isAscending: bool = False
	isDescending: bool = False
	for i in range(1, len(record)):
		# Guard: if diff == 0, then it's unsafe
		diff: int = record[i-1] - record[i]
		if diff == 0:
			return False

		if diff < 0:
			isAscending = True
			if diff <= -4:
				# Diff too large
				return False
		
		if diff > 0:
			isDescending = True
			if diff >= 4:
				# Diff too large
				return False

		# Records that have numbers both ascending and descending are not safe
		if isAscending and isDescending:
			return False
	
	# Loop processed, the record is safe is either is safe
	return isAscending or isDescending


def solve(raw_input: list[str]):
	inp_pairs: list[str] = raw_input.split('\n')
	totalSafe = 0
	for inp_pair in inp_pairs:
		elems: list[str] = inp_pair.split()
		record: list[int] = [int(x) for x in elems]
		if (isSafe(record)):
			#print(f"safe {record=}")
			totalSafe += 1

	print(f"{totalSafe=}")


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