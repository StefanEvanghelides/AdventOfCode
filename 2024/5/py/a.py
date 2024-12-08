import argparse

class RulesBook:
	rulesLeft: dict[int, set[int]] = {}
	rulesRight: dict[int, set[int]] = {}

	def __init__(self, lines: list[str], sz: int):
		for i in range(sz):
			elems = lines[i].split('|')
			left: int = int(elems[0])
			right: int = int(elems[1])
			if left not in self.rulesLeft:
				self.rulesLeft[left] = set()
			self.rulesLeft[left].add(right)
			if right not in self.rulesRight:
				self.rulesRight[right] = set()
			self.rulesRight[right].add(left)

		# print(f"Final {self.rulesLeft=}")
		# print(f"Final {self.rulesRight=}")

	def testValues(self, left: int, right: int) -> bool:
		# print(f"{left=}  {right=}")

		# Check left and right rules
		return left in self.rulesLeft and right in self.rulesLeft[left] and \
			right in self.rulesRight and left in self.rulesRight[right]

	def testAllValues(self, numbers: list[int], idx: int, sz: int) -> bool:
		# Guard: if we reached the end, then there's nothing to compare
		if idx >= sz - 1:
			return False

		number: int  = numbers[idx]
		for i in range(idx+1, sz):
			rightVal: int = numbers[i]
			if not self.testValues(number, rightVal):
				return False

		# All checks passed
		return True



def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	# Split input in 2 parts: the rules and the test

	# Part 1: build the rules
	idx: int = lines.index('')
	rulesBook: RulesBook = RulesBook(lines, idx)

	# Part 2: test rules and count total
	total = 0
	sz: int = len(lines)
	for i in range(idx+1, sz):
		elems: list[str] = lines[i].split(',')
		numbers: list[int] = [int(x) for x in elems]
		nsz: int = len(numbers)
		isCorrect: bool = True

		# Check all numbers (O(N^2)). Transitivity is not enough here.
		for j in range(nsz-1):
			number: int = numbers[j]
			if not rulesBook.testAllValues(numbers, j, nsz):
				isCorrect = False
				break

		if isCorrect:
			mid: int = int(numbers[nsz // 2])
			total += mid

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