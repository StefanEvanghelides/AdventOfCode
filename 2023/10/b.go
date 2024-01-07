package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type point struct {
	x, y int
}

func goesUp(x, y int, lines []string) bool {
	// Guard
	if x < 1 {
		return false
	}
	next := rune(lines[x][y])

	// fmt.Printf("UP next: %c -> next == 'S': %t next == '|': %t; next == 'F': %t; next == '7': %t \n", next, next == 'S', next == '|', next == 'F', next == '7')

	return next == 'S' || next == '|' || next == 'F' || next == '7'
}

func goesDown(x, y int, lines []string) bool {
	// Guard
	if x >= len(lines)-1 {
		return false
	}
	next := rune(lines[x][y])
	// fmt.Printf("DOWN next: %c -> next == 'S': %t; next == '|': %t; next == 'J': %t; next == 'L': %t \n", next, next == 'S', next == '|', next == 'J', next == 'L')

	return next == 'S' || next == '|' || next == 'J' || next == 'L'
}

func goesRight(x, y int, lines []string) bool {
	// Guard
	if y >= len(lines[0])-1 {
		return false
	}
	next := rune(lines[x][y])
	// fmt.Printf("RIGHT next: %c -> next == 'S': %t; next == '-': %t; next == 'J': %t; next == '7': %t \n", next, next == 'S', next == '-', next == 'J', next == '7')

	return next == 'S' || next == '-' || next == 'J' || next == '7'
}

func goesLeft(x, y int, lines []string) bool {
	// Guard
	if y < 1 {
		return false
	}
	next := rune(lines[x][y])
	// fmt.Printf("LEFT next: %c -> next == 'S': %t; next == '-': %t; next == 'F': %t; next == 'L': %t \n", next, next == 'S', next == '-', next == 'F', next == 'L')

	return next == 'S' || next == '-' || next == 'F' || next == 'L'
}

// Check if the next position is possible.
func findNext(delta, cur point, lines []string) (point, point) {
	// fmt.Println("\ndelta:", delta, "cur:", cur)
	// fmt.Println(" - goesUp:", goesUp(curX-1, curY, lines), "- goesDown:", goesDown(curX+1, curY, lines), "- goesLeft:", goesLeft(curX, curY-1, lines), "- goesRight:", goesRight(curX, curY+1, lines))

	tile := rune(lines[cur.x][cur.y])
	if delta.y > 0 {
		// Coming from the left
		if tile == 'J' {
			// Go up
			return point{cur.x - 1, cur.y}, point{-1, 0}
		} else if tile == '-' {
			// Go to the right
			return point{cur.x, cur.y + 1}, point{0, 1}
		}
		// Go down
		return point{cur.x + 1, cur.y}, point{1, 0}
	} else if delta.y < 0 {
		// Coming from the right
		if tile == 'L' {
			// Go up
			return point{cur.x - 1, cur.y}, point{-1, 0}
		} else if tile == '-' {
			// Go to the left
			return point{cur.x, cur.y - 1}, point{0, -1}
		}
		// Go down
		return point{cur.x + 1, cur.y}, point{1, 0}
	} else if delta.x > 0 {
		// Coming from the top
		if tile == 'L' {
			// Go right
			return point{cur.x, cur.y + 1}, point{0, 1}
		} else if tile == '|' {
			// Go down
			return point{cur.x + 1, cur.y}, point{1, 0}
		}
		// Go left
		return point{cur.x, cur.y - 1}, point{0, -1}
	}
	// Else: coming from the bottom
	if tile == 'F' {
		// Go right
		return point{cur.x, cur.y + 1}, point{0, 1}
	} else if tile == '|' {
		// Go up
		return point{cur.x - 1, cur.y}, point{-1, 0}
	}
	// Else of the else: go left
	return point{cur.x, cur.y - 1}, point{0, -1}
}

func findNeighbour(start point, sizeY int, lines []string) point {
	if start.x > 0 && goesUp(start.x-1, start.y, lines) {
		return point{start.x - 1, start.y}
	} else if start.y < sizeY-1 && goesRight(start.x, start.y+1, lines) {
		return point{start.x, start.y + 1}
	} else if start.y > 0 && goesLeft(start.x, start.y-1, lines) {
		return point{start.x, start.y - 1}
	}
	// Go down by default
	return point{start.x + 1, start.y}
}

// Find the contour and navigate around it.
func navigate(start point, sizeY int, lines []string, allPoints, contour map[point]bool) int {
	// Find a continuation from the starting point. Check the 4 neighbours.
	cur := findNeighbour(start, sizeY, lines)
	delta := point{cur.x - start.x, cur.y - start.y}

	// Remove current point from the cache, add it to the contour
	contour[cur] = true
	delete(allPoints, cur)

	// Now traveres the maze until we find the start again
	count := 1
	for cur != start {
		count = count + 1
		cur, delta = findNext(delta, cur, lines)
		contour[cur] = true
		delete(allPoints, cur)
		// fmt.Println("- Cur:", cur)
		// fmt.Println("--- Delta:", delta)
	}

	return count / 2
}

// Check if intersections result in odd numbers.
func isPointInside(lines []string, sizeX, sizeY int, p point, contour map[point]bool) bool {
	isOdd := false
	// Run ray below
	count := 0
	for idx := p.x + 1; idx < sizeX; idx++ {
		if _, ok := contour[point{idx, p.y}]; ok && lines[idx][p.y] != '|' {
			count = count + 1
		}
	}
	if count == 0 {
		return false
	}
	if count%2 == 1 {
		isOdd = true
	}

	// Run ray above
	count = 0
	for idx := p.x - 1; idx >= 0; idx-- {
		if _, ok := contour[point{idx, p.y}]; ok && lines[idx][p.y] != '|' {
			count = count + 1
		}
	}
	if count == 0 {
		return false
	}
	if count%2 == 1 {
		isOdd = true
	}

	// Run ray to the right
	count = 0
	for idx := p.y + 1; idx < sizeY; idx++ {
		if _, ok := contour[point{p.x, idx}]; ok && lines[p.x][idx] != '-' {
			count = count + 1
		}
	}
	if count == 0 {
		return false
	}
	if count%2 == 1 {
		isOdd = true
	}

	// Run ray to the left
	count = 0
	for idx := p.y - 1; idx >= 0; idx-- {
		if _, ok := contour[point{p.x, idx}]; ok && lines[p.x][idx] != '-' {
			count = count + 1
		}
	}
	if count == 0 {
		return false
	}
	if count%2 == 1 {
		isOdd = true
	}

	// Check if it's odd.
	return isOdd
}

func countPointsInside(lines []string, sizeX, sizeY int, allPoints, contour map[point]bool) int {
	count := 0
	for p := range allPoints {
		if isPointInside(lines, sizeX, sizeY, p, contour) {
			count = count + 1
			fmt.Println("Point inside:", p)
		}
	}
	return count
}

// PART B) Apply simple algorithm to find if a point is enclosed in a contour:
// Shoot rays in the 4 directions and check if we get odd number of intersections:
// If so, then we are inside. Very interesting, I need to exclude '|' and '-' from this checks.
func solve(lines []string) {
	// Initialize the 2D space.
	// This is basically just a string array.
	// The input LINES is already in the right format!

	// Find starting point.
	// All fill in the cache with all points
	var allPoints = map[point]bool{}
	var contour = map[point]bool{}

	start := point{-1, -1}
	for idx, line := range lines {
		for idx2, ch := range line {
			p := point{idx, idx2}
			allPoints[p] = true
			if ch == 'S' {
				start = p
			}
		}
	}
	fmt.Println("Start:", start, " all points size:", len(allPoints))

	// Cache the size, it will be used everywhere.
	sizeX := len(lines)
	sizeY := len(lines[0])
	fmt.Println("SizeX:", sizeX, "- sizeY:", sizeY)

	total := navigate(start, sizeY, lines, allPoints, contour)
	fmt.Println("Total:", total)

	fmt.Println("allPoints size:", len(allPoints))
	fmt.Println("contour size:", len(contour))

	// Now check which remaining points from `allPoints` are enclosed inside
	total = countPointsInside(lines, sizeX, sizeY, allPoints, contour)
	fmt.Println("Total:", total)
}

func readLines(filename string) []string {
	file, err := os.Open(filename)
	if err != nil {
		os.Exit(1)
	}
	defer file.Close()

	var lines []string
	sc := bufio.NewScanner(file)
	for sc.Scan() {
		lines = append(lines, sc.Text())
	}
	return lines
}

func showLines(lines []string) {
	fmt.Println(strings.Join(lines, "\n"))
}

func main() {
	// Check if we have arg "t" -> then use the test input
	filename := "input.txt"
	if len(os.Args) > 1 && os.Args[1] == "t" {
		filename = "test.txt"
	}
	lines := readLines(filename)
	// showLines(lines)

	solve(lines)
}
