import argparse

# For (b) we need to test adding a new obstruction. Then to check deadlocks.

UP = '^'
DOWN = 'v'
LEFT = '<'
RIGHT = '>'
OBSTRUCTION = -1
NEW_OBSTRUCTION = -2

class Board:
	_sz: int = 0
	_board: list[list[int]] = []
	_obstructions: list[(int,int)] = []
	_x: int = 0
	_y: int = 0
	_dir: str = UP
	_movesToNewObstruction: set[(int,int,str)] = set()
	_repeatCircle: bool = False

	# Used for final/original values
	_start: (int, int) = (0, 0)
	_originalObstructions: list[(int,int)] = []

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
					self._start = (i, j)
					self._board[i][j] = 1
					continue

				if lines[i][j] == '#':
					# Obstruction!
					self._board[i][j] = OBSTRUCTION
					self._obstructions.append((i, j))
					self._originalObstructions.append((i, j))

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

		# If closestTop is the new obstruction, then record this
		maxTop: int = closestTop+1
		if closestTop > -1 and closestTop < self._sz and self._board[closestTop][self._y] == NEW_OBSTRUCTION:
			val: (int,int,str) = (maxTop, self._y, self._dir)
			# Check if value exists
			if val in self._movesToNewObstruction:
				self._repeatCircle = True
			else:
				self._movesToNewObstruction.add(val)

		# Walk the board
		dist: int = self._x - closestTop
		for i in range(dist-1):
			self._board[self._x-i-1][self._y] += 1

		# Update the pointer
		self._x = maxTop


	def moveDown(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestBottom: int = self._sz
		for obstr in self._obstructions:
			if obstr[1] == self._y and obstr[0] > self._x and obstr[0] < closestBottom:
				closestBottom = obstr[0]

		# If closestBottom is the new obstruction, then record this
		minBottom: int = closestBottom-1
		if closestBottom > -1 and closestBottom < self._sz and self._board[closestBottom][self._y] == NEW_OBSTRUCTION:
			val: (int,int,str) = (minBottom, self._y, self._dir)
			# Check if value exists
			if val in self._movesToNewObstruction:
				self._repeatCircle = True
			else:
				self._movesToNewObstruction.add(val)

		# Walk the board
		dist: int = closestBottom - self._x
		for i in range(dist-1):
			self._board[closestBottom-i-1][self._y] += 1

		# Update the pointer
		self._x = minBottom

	def moveLeft(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestLeft: int = -1
		for obstr in self._obstructions:
			if obstr[0] == self._x and obstr[1] < self._y and obstr[1] > closestLeft:
				closestLeft = obstr[1]

		# If closestLeft is the new obstruction, then record this
		maxLeft: int = closestLeft+1
		if closestLeft > -1 and closestLeft < self._sz and self._board[self._x][closestLeft] == NEW_OBSTRUCTION:
			val: (int,int,str) = (self._x, maxLeft, self._dir)
			# Check if value exists
			if val in self._movesToNewObstruction:
				self._repeatCircle = True
			else:
				self._movesToNewObstruction.add(val)

		# Walk the board
		dist: int = self._y - closestLeft
		for i in range(dist-1):
			self._board[self._x][self._y-i-1] += 1

		# Update the pointer
		self._y = maxLeft

	def moveRight(self):
		# Check if there's a obstruction above the pointer. If so, find the closest one.
		closestRight: int = self._sz
		for obstr in self._obstructions:
			if obstr[0] == self._x and obstr[1] > self._y and obstr[1] < closestRight:
				closestRight = obstr[1]

		# If closestRight is the new obstruction, then record this
		minRight: int = closestRight-1
		if closestRight > -1 and closestRight < self._sz and self._board[self._x][closestRight] == NEW_OBSTRUCTION:
			val: (int,int,str) = (self._x, minRight, self._dir)
			# Check if value exists
			if val in self._movesToNewObstruction:
				self._repeatCircle = True
			else:
				self._movesToNewObstruction.add(val)

		# Walk the board
		dist: int = closestRight - self._y
		for i in range(dist-1):
			self._board[self._x][self._y+i+1] += 1

		# Update the pointer
		self._y = minRight

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

	def getPaths(self) -> list[(int,int)]:
		paths: list[(int,int)] = []
		for i in range(self._sz):
			for j in range(self._sz):
				if self._board[i][j] > 0:
					paths.append((i, j))
		return paths

	def reset(self):
		self._x = self._start[0]
		self._y = self._start[1]
		self._dir = UP
		self._obstructions = self._originalObstructions.copy()
		self._movesToNewObstruction = set()
		self._repeatCircle = False

		# Reconstruct the board based on the obstructions
		self._board = [[0] * self._sz for _ in range(self._sz)]
		for obstr in self._obstructions:
			self._board[obstr[0]][obstr[1]] = OBSTRUCTION

	def addObstruction(self, i: int, j: int):
		self._board[i][j] = NEW_OBSTRUCTION
		self._obstructions.append((i, j))

	def isRepeatedCircle(self) -> bool:
		# A repeated circle is when we visit the obstruction twice from the same direction.
		return self._repeatCircle

	def walkCircle(self) -> bool:
		while not self.isOnEdge() and not self.isRepeatedCircle():
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

			#print(f"{self.__repr__()}")

		return self.isRepeatedCircle()

	def __repr__(self) -> str:
		def computeRowStr(row: list[int]) -> str:
			rowStr: str = ''
			for col in row:
				colStr : str = str(col)
				if col == OBSTRUCTION:
					colStr = '#'
				elif col == NEW_OBSTRUCTION:
					colStr = 'X'
				val: str = colStr.rjust(2, ' ')
				rowStr += val
			return rowStr

		def computeBoardStr() -> str:
			boardStr: str = '\n'.join([computeRowStr(row) for row in self._board])
			return boardStr

		return f"Board=\n{computeBoardStr()}\n{self._obstructions=}\npointer=({self._x},{self._y})\n{self._dir=}\n{self._movesToNewObstruction=}\n"


def solve(raw_input: list[str]):
	lines: list[str] = raw_input.split('\n')

	board: RulesBookBoard = Board(lines)
	print(f"{board}")

	# Walk along the board until exit through the edge.
	board.walk()
	print(f"{board}")

	# Now use the path to check were we could place an extra obstruction.
	paths: list[(int,int)] = board.getPaths()
	print(f"{paths=}")

	# Count number of deadlocks we can encounter
	total: int = 0
	for path in paths:
		board.reset()
		board.addObstruction(path[0], path[1])
		#print(f"Currently testing:\n{board}")
		if board.walkCircle():
			#print(f"Full circle:\n{board}")
			total += 1
	
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