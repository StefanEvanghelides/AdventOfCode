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
	if y >= len(lines)-1 {
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

func findNeighbour(start point, size int, lines []string) point {
	if start.x > 0 && goesUp(start.x-1, start.y, lines) {
		return point{start.x - 1, start.y}
	} else if start.y < size-1 && goesRight(start.x, start.y+1, lines) {
		return point{start.x, start.y + 1}
	} else if start.y > 0 && goesLeft(start.x, start.y-1, lines) {
		return point{start.x, start.y - 1}
	}
	// Go down by default
	return point{start.x + 1, start.y}
}

// Find the contour and navigate around it.
func navigate(start point, size int, lines []string) int {
	// Find a continuation from the starting point. Check the 4 neighbours.
	cur := findNeighbour(start, size, lines)
	delta := point{cur.x - start.x, cur.y - start.y}

	// Now traveres the maze until we find the start again
	count := 1
	for cur != start {
		count = count + 1
		cur, delta = findNext(delta, cur, lines)
		// fmt.Println("- Cur:", cur)
		// fmt.Println("--- Delta:", delta)
	}

	return count / 2
}

func solve(lines []string) {
	// Initialize the 2D space.
	// This is basically just a string array.
	// The input LINES is already in the right format!

	// Find starting point
	start := point{-1, -1}
	for idx, line := range lines {
		for idx2, ch := range line {
			if ch == 'S' {
				start.x = idx
				start.y = idx2
				break
			}
		}
		if start.x > -1 {
			break
		}
	}
	fmt.Println("Start:", start)

	// Cache the size, it will be used everywhere.
	size := len(lines)
	total := navigate(start, size, lines)
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
