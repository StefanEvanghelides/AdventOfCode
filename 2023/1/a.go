package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"unicode"
)

func solve(lines []string) {
	var total int64

	for _, line := range lines {
		var first int64 = -1
		var last int64 = -1

		for _, char := range line {
			if unicode.IsDigit(char) {
				if first == -1 {
					first = int64(char - '0')
				} else {
					last = int64(char - '0')
				}
			}
		}

		if last == -1 {
			last = first
		}

		number := first * 10 + last
		total += number
	}

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
	lines := readLines("input.txt")
	// showLines(lines)

	solve(lines)
}