package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	// "math"
	// "strconv"
	// "sort"
)

func encode(node string) int {
	nr := 0
	for i := 0; i < len(node); i++ {
		nr *= 100
		nr += int(rune(node[i]) - 'A')
	}
	return nr
}

func isValidEnd(starts []int) bool {
	for _, start := range starts {
		if start % 100 != 25 {
			return false
		}
	}
	return true
}

// greatest common divisor (GCD) via Euclidean algorithm
func GCD(a, b int64) int64 {
	for b != 0 {
		t := b
		b = a % b
		a = t
	}
	return a
}

// find Least Common Multiple (LCM) via GCD
func LCM(array []int64) int64 {
	a := array[0]
	b := array[1]
	result := a * b / GCD(a, b)

	for i := 2; i < len(array); i++ {
		arr := make([]int64, 2)
		arr[0] = result
		arr[1] = array[i]
		result = LCM(arr)
	}

	return result
}

func solve(lines []string) {
	// Process inputs.
	// For part (b) we need to identify all starting points.
	MAX_SIZE := 252525
	leftNodes := make([]int, MAX_SIZE+1)
	rightNodes := make([]int, MAX_SIZE+1)
	input := ""
	var starts []int
	for i := 0; i < len(lines); i++ {
		if i == 0 {
			// This is the instructions line
			tokens := strings.Fields(lines[i])
			input = tokens[0]
			// fmt.Println("Tokens:",tokens)
			i++
			continue
		}
		// Now we just have the nodes
		firstParts := strings.Split(lines[i], " = (")
		secondParts := strings.Split(firstParts[1], ", ")
		thirdParts := strings.Split(secondParts[1], ")")

		// Now process all elements.
		node := firstParts[0]
		left := secondParts[0]
		right := thirdParts[0]

		// generate the arrays of elements
		encodedNode := encode(node)
		leftNodes[encodedNode] = encode(left)
		rightNodes[encodedNode] = encode(right)

		// Check if this is a starting point
		if rune(node[len(node)-1]) == 'A' {
			starts = append(starts, encodedNode)
		}
	}
	// fmt.Println(leftNodes)
	// fmt.Println(rightNodes)
	fmt.Println(starts)

	// Now follow the inputs
	sizeStarts := len(starts)
	var counters []int64
	inputSize := len(input)
	// Find counters for all start points.
	for i := 0; i < sizeStarts; i++ {
		var counter int64 = 0
		pos := starts[i]
		for pos % 100 != 25 {
			idx := int(counter % int64(inputSize))
			counter++
			if rune(input[idx]) == 'L' {
				// Move left
				pos = leftNodes[pos]
			} else {
				// Move right
				pos = rightNodes[pos]
			}
		}
		counters = append(counters, counter)
	}
	fmt.Println(counters)

	// Now find the least common multiplier between these numbers.
	total := LCM(counters)
	fmt.Println(total)
}

func readLines(filename string) ([]string) {
	file, err := os.Open(filename)
	if err !=nil {
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