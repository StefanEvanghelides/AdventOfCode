import argparse
from tqdm import tqdm
import copy


# Tree structure, where the first elements are the 1st level.
# Then in the end we run a simple BFS on the leaves.

class Tree:
	def __init__(self, datas: int = -1):
		self.data: int = datas
		self.children: list[Tree] = None

	def __repr__(self):
		children_str: str = ""
		if self.children == None or len(self.children) < 1:
			children_str = "[]"
		else:
			children_str = self.children.__repr__()
		return f"({self.data}) -> {children_str}"


def find_leaves(roots: list[Tree]):
	leaves: list[Tree] = []

	for root in roots:
		if root.children == None or len(root.children) < 1:
			leaves.append(root)
		else:
			leaves += root.children

	return leaves


def blink(leaves: list[Tree]):
	# We have the leaves, now let's apply the rules
	for leaf in leaves:
		if leaf.data == 0:
			# Rule #1: 0 -> 1
			leaf.data = 1
		else:
			data_str: str = str(leaf.data)
			sz: int = len(data_str)
			if sz % 2 == 0:
				# Rule #2: even number of digits, then split
				half_idx: int = sz // 2
				first_half: int = int(data_str[:half_idx])
				second_half: int = int(data_str[half_idx:])
				leaf.children = []
				leaf.children.append(Tree(first_half))
				leaf.children.append(Tree(second_half))
			else:
				# Rule #3: multiply by 2024
				leaf.data *= 2024
	

def count_leaves(root: Tree):
	if root == None or root.children == None:
		# Leaf node
		return 1

	total: int = 0
	if root.children != None:
		for child_node in root.children:
			total += count_leaves(child_node)

	return total

def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	numbers: list[int] = [int(x) for x in lines[0].split(' ')]
	n: int = len(numbers)
	print(f"{numbers=}")

	root: Tree = Tree()
	root.children = [Tree() for _ in range(n)]
	for i, num in enumerate(numbers):
		root.children[i].data = num

	print(f"{root=}")

	leaves: list[Tree] = [root]

	# Task "blink 25 times"
	for i in tqdm(range(25)):
		leaves = find_leaves(leaves)
		#print(f"{leaves=}")
		blink(leaves)

	# Count leaves
	print(f"Leaves: {count_leaves(root)}")



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