import argparse

# 1 = +; 2 = *.
operators: list[int] = [1, 2]

def verify_equation(s: int, values: list[int], idx: int, acc: int) -> bool:
	# Base condition: we reached the end, check accumulated value
	# If accumulated value went beyond, then stop early
	if idx >= len(values) or acc > s:
		# print(f" Base condition {s=}  {acc=}  {idx=}")
		return acc == s

	# print(f"{s=}  {acc=}  {idx=}  {values[idx]=}")

	result: bool = verify_equation(s, values, idx+1, acc + values[idx]) \
		or verify_equation(s, values, idx+1, acc * values[idx])

	return result
	

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	total: int = 0
	for line in lines:
		elems: list[str] = line.split(": ")
		s: int = int(elems[0])
		values: list[int] = [int(x) for x in elems[1].split(' ')]

		ok: bool = verify_equation(s, values, 1, values[0])
		if ok:
			total += s

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