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

func solve(lines []string) {
	// Process inputs.
	MAX_SIZE := 252525
	leftNodes := make([]int, MAX_SIZE+1)
	rightNodes := make([]int, MAX_SIZE+1)
	input := ""
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
		leftNodes[encode(node)] = encode(left)
		rightNodes[encode(node)] = encode(right)
	}
	// fmt.Println(leftNodes)
	// fmt.Println(rightNodes)

	// Now follow the inputs
	pos := 0
	var counter int64 = 0
	inputSize := len(input)
	for pos != MAX_SIZE {
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

	fmt.Println(counter)
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