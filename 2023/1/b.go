package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"unicode"
)

func IsDigit(idx int, line string) (int64) {
	// Guard: Minimum 3 chars
	if idx > len(line) - 3 {
		return 0
	}
	// Start with the 3 letter numbers
	if line[idx:idx+3] == "one" {
		return 1
	} else if line[idx:idx+3] == "two" {
		return 2
	} else if line[idx:idx+3] == "six" {
		return 6
	}

	// Now check the 4 letter numbers
	if idx > len(line) - 4 {
		return 0
	}
	if line[idx:idx+4] == "four" {
		return 4
	} else if line[idx:idx+4] == "five" {
		return 5
	} else if line[idx:idx+4] == "nine" {
		return 9
	}

	// Now check the 5 letter numbers
	if idx > len(line) - 5 {
		return 0
	}
	if line[idx:idx+5] == "three" {
		return 3
	} else if line[idx:idx+5] == "seven" {
		return 7
	} else if line[idx:idx+5] == "eight" {
		return 8
	}

	// Nothing hit
	return 0
}

func solve(lines []string) {
	var total int64

	for _, line := range lines {
		var first int64 = -1
		var last int64 = -1

		for idx, char := range line {
			if unicode.IsDigit(char) {
				if first == -1 {
					first = int64(char - '0')
				} else {
					last = int64(char - '0')
				}
			} else {
				// Account for potentil digits spelled out as strings.
				dig := IsDigit(idx, line)
				if dig > 0 {
					if first == -1 {
						first = dig
					} else {
						last = dig
					}
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