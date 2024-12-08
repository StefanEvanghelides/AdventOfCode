import argparse

UP = '^'
DOWN = 'v'
LEFT = '<'
RIGHT = '>'
OBSTRUCTION = -1

class Board:
	_sz: int = 0
	_board: list[list[int]] = []
	_obstructions: list[(int,int)] = []
	_x: int = 0
	_y: int = 0
	_dir: str = UP

	def __init__(self, lines: list[str]):
		# Init board size
		self._sz = len(lines)
		self._board = [[0] * self._sz for _ in range(self._sz)]

		for i in range(self._sz):
			for j in range(self._sz):
				if lines[i][j] == '^':
					# This is the start point
					self._x = i
					self._y = j
					self._board[i][j] = 1
					continue

				if lines[i][j] == '#':
					# Obstruction!
					self._board[i][j] = OBSTRUCTION
					self._obstructions.append((i, j))

		# print(f"Final {self.board=}")

	def isOnEdge(self) -> bool:
		return self._x == 0 or self._x == self._sz-1 \
			or self._y == 0 or self._y == self._sz-1

	def moveUp(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestTop: int = -1
		for obstr in self._obstructions:
			if obstr[1] == self._y and obstr[0] < self._x and obstr[0] > closestTop:
				closestTop = obstr[0]

		# Walk the board
		dist: int = self._x - closestTop
		for i in range(dist-1):
			self._board[self._x-i-1][self._y] += 1

		# Update the pointer
		self._x = closestTop+1

	def moveDown(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestBottom: int = self._sz
		for obstr in self._obstructions:
			if obstr[1] == self._y and obstr[0] > self._x and obstr[0] < closestBottom:
				closestBottom = obstr[0]

		# Walk the board
		dist: int = closestBottom - self._x
		for i in range(dist-1):
			self._board[closestBottom-i-1][self._y] += 1

		# Update the pointer
		self._x = closestBottom-1

	def moveLeft(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestLeft: int = -1
		for obstr in self._obstructions:
			if obstr[0] == self._x and obstr[1] < self._y and obstr[1] > closestLeft:
				closestLeft = obstr[1]

		# Walk the board
		dist: int = self._y - closestLeft
		for i in range(dist-1):
			self._board[self._x][self._y-i-1] += 1

		# Update the pointer
		self._y = closestLeft+1

	def moveRight(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestRight: int = self._sz
		for obstr in self._obstructions:
			if obstr[0] == self._x and obstr[1] > self._y and obstr[1] < closestRight:
				closestRight = obstr[1]

		# Walk the board
		dist: int = closestRight - self._y
		for i in range(dist-1):
			self._board[self._x][self._y+i+1] += 1

		# Update the pointer
		self._y = closestRight-1

	def walk(self):
		while not self.isOnEdge():
			if self._dir == UP:
				self.moveUp()
				self._dir = RIGHT
			elif self._dir == DOWN:
				self.moveDown()
				self._dir = LEFT
			elif self._dir == LEFT:
				self.moveLeft()
				self._dir = UP
			elif self._dir == RIGHT:
				self.moveRight()
				self._dir = DOWN
		

	def countUnique(self) -> int:
		unique: int = 0
		for row in self._board:
			for col in row:
				if col > 0:
					unique += 1

		return unique

	def __repr__(self) -> str:
		def computeRowStr(row: list[int]) -> str:
			rowStr: str = ''
			for col in row:
				colStr : str = str(col)
				if col == OBSTRUCTION:
					colStr = '#'
				val: str = colStr.rjust(2, ' ')
				rowStr += val
			return rowStr

		def computeBoardStr() -> str:
			boardStr: str = '\n'.join([computeRowStr(row) for row in self._board])
			return boardStr

		return f"Board=\n{computeBoardStr()}\n{self._obstructions=}\npointer=({self._x},{self._y})\n{self._dir=}"


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	board: RulesBookBoard = Board(lines)

	# Walk along the board until exit through the edge
	board.walk()

	# Count unique
	total: int = board.countUnique()

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