package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func computeHash(str string) int {
	hash := 0

	for _, ch := range str {
		hash += int(ch)
		hash *= 17
		hash %= 256
	}

	return hash
}

func solve(lines []string) {
	total := 0
	// Separate input line into grops
	groups := strings.Split(lines[0], ",")
	// For each group compute the hash function
	for _, group := range groups {
		hash := computeHash(group)
		total += hash
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
