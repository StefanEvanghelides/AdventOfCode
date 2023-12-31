package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strings"
)

func isEmptyLine(line string) bool {
	for _, ch := range line {
		if ch != '.' {
			return false
		}
	}
	return true
}

/* Compute the free space in horizontal dimension only. */
func computeFreeSpace(space []string) []int {
	var res []int
	// Check if the line is empty. If so, add the index in the RES array
	for idx, str := range space {
		// Check if the string contains only "." space.
		if isEmptyLine(str) {
			res = append(res, idx)
		}
	}
	return res
}

/* Simple matrix transpozition. */
func transposeSpace(a []string) []string {
	newArr := make([][]rune, len(a))
	for i := 0; i < len(a); i++ {
		for j := 0; j < len(a[0]); j++ {
			newArr[j] = append(newArr[j], rune(a[i][j]))
		}
	}

	// Now convert 2D rune matrix into array of strings
	res := make([]string, len(a))
	for i := 0; i < len(newArr); i++ {
		res[i] = string(newArr[i])
	}
	return res
}

func computeSpaceWarp1Dimension(p1, p2, warpValue int, warp []int) int {
	dist := 0
	diff := int(math.Abs(float64(p1 - p2)))
	dist += diff
	for _, idx := range warp {
		// Check if the index of horizontals is between x1 and x2.
		diff1 := int(math.Abs(float64(p1 - idx)))
		diff2 := int(math.Abs(float64(p2 - idx)))
		if diff1+diff2 == diff {
			// We got a space warp, add 1 to the distance
			dist += warpValue
		}
	}
	return dist
}

func computeDistance(x1, y1, x2, y2, warpValue int, horizontals, verticals []int) int {
	dist := 0
	// First handle horizontals
	dist += computeSpaceWarp1Dimension(x1, x2, warpValue, horizontals)

	// Now handle verticals
	dist += computeSpaceWarp1Dimension(y1, y2, warpValue, verticals)

	return dist
}

func solve(lines []string) {
	// Initialize the 2D space.
	// This is basically just a string array.
	// The input LINES is already in the right format!
	space := lines

	// Now search the space for those horizontal and vertical lines
	// Save their indices in specific arrays.
	var horizontals []int = computeFreeSpace(lines)
	// For the verticals, we need to transpose the input
	var transpose []string = transposeSpace(space)
	var verticals []int = computeFreeSpace(transpose)

	// fmt.Println("Lines:")
	// showLines(lines)
	// fmt.Println("Transpose:")
	// showLines(transpose)
	// fmt.Println(horizontals)
	// fmt.Println(verticals)

	// Now identify all points.
	var pointsX []int
	var pointsY []int
	for i := 0; i < len(space); i++ {
		for j := 0; j < len(space[0]); j++ {
			if space[i][j] == '#' {
				pointsX = append(pointsX, i)
				pointsY = append(pointsY, j)
			}
		}
	}
	// fmt.Println(pointsX)
	// fmt.Println(pointsY)

	// Compute distance between them, taking into account the space warp.
	// Sum up all of them.
	/* FOR PART B) WE DON'T ADD 1, WE ADD 999'999. Easy. */
	size := len(pointsX)
	total := 0
	for i := 0; i < size-1; i++ {
		for j := i + 1; j < size; j++ {
			dist := computeDistance(pointsX[i], pointsY[i], pointsX[j], pointsY[j], 999999, horizontals, verticals)
			// fmt.Println("Dist:", dist)
			total += dist
		}
	}
	fmt.Println(total)
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
