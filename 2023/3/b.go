package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"unicode"
	"strconv"
	"math"
)

func isValidGear(lines []string, idx int, idx2 int) (bool, int64) {
	var numbers []int64
	// First part
	if idx > 0 {
		var number int64 = 0
		// Check top left corner, going left.
		start := idx2
		for start-1 >= 0 && unicode.IsDigit(rune(lines[idx-1][start-1])) {
			start--;
		}
		if start < idx2 {
			number = int64(computeNumber(lines[idx-1], start, idx2-1))
		}
		// We don't want to stop here, we also want to check top-middle part
		isMidValid := false
		if unicode.IsDigit(rune(lines[idx-1][idx2])) {
			// Valid digit in the middle part!
			isMidValid = true
			number *= 10
			number += int64(rune(lines[idx-1][idx2]) - '0')
		}
		// Check if mid was actually a gap
		if number > 0 && !isMidValid {
			// We have a gap in the middle, so we have to store the current number
			numbers = append(numbers, number)
			number = 0 // reset current number
		}
		// Now check elements on the top right.
		end := idx2
		for end+1 < len(lines[idx-1]) && unicode.IsDigit(rune(lines[idx-1][end+1])) {
			end++;
		}
		if end > idx2 {
			// New number found
			newNumber := int64(computeNumber(lines[idx-1], idx2+1, end))
			if isMidValid {
				number *= int64(math.Pow10(end - idx2))
				number += newNumber
			} else {
				number = newNumber
			}
			numbers = append(numbers, number)
		} else if isMidValid {
			numbers = append(numbers, number)
		}
	}

	// Middle part
	start := idx2
	for start-1 >= 0 && unicode.IsDigit(rune(lines[idx][start-1])) {
		start--;
	}
	if start < idx2 {
		numbers = append(numbers, int64(computeNumber(lines[idx], start, idx2-1)))
	}
	end := idx2
	for end+1 < len(lines[idx]) && unicode.IsDigit(rune(lines[idx][end+1])) {
		end++;
	}
	if end > idx2 {
		numbers = append(numbers, int64(computeNumber(lines[idx], idx2+1, end)))
	}

	// Final part
	if idx < len(lines)-1 {
		var number int64 = 0
		// Check bottom left corner, going left.
		start := idx2
		for start-1 >= 0 && unicode.IsDigit(rune(lines[idx+1][start-1])) {
			start--;
		}
		if start < idx2 {
			number = int64(computeNumber(lines[idx+1], start, idx2-1))
		}
		// We don't want to stop here, we also want to check bottom-middle part
		isMidValid := false
		if unicode.IsDigit(rune(lines[idx+1][idx2])) {
			// Valid digit in the middle part!
			isMidValid = true
			number *= 10
			number += int64(rune(lines[idx+1][idx2]) - '0')
		}
		// Check if mid was actually a gap
		if number > 0 && !isMidValid {
			// We have a gap in the middle, so we have to store the current number
			numbers = append(numbers, number)
			number = 0 // reset current number
		}
		// Now check elements on the bottom right.
		end := idx2
		for end+1 < len(lines[idx+1]) && unicode.IsDigit(rune(lines[idx+1][end+1])) {
			end++;
		}
		if end > idx2 {
			// New number found
			newNumber := int64(computeNumber(lines[idx+1], idx2+1, end))
			if isMidValid {
				number *= int64(math.Pow10(end - idx2))
				number += newNumber
			} else {
				number = newNumber
			}
			numbers = append(numbers, number)
		} else if isMidValid {
			numbers = append(numbers, number)
		}
	}

	if len(numbers) == 2 {
		gearRatio := numbers[0] * numbers[1]
		return true, gearRatio
	}
	return false, 0
}

func computeNumber(row string, start int, end int) int {
	// +1 because end of the slice is exclusive.
	number, _ := strconv.Atoi(row[start:end+1])
	return number
}

func solve(lines []string) {
	var total int64 = 0

	for idx, row := range lines {
		for idx2 := 0; idx2 < len(row); idx2++ {
			// Check if we detect a '*'
			if rune(row[idx2]) == '*' {
				// Now we have to search for potential numbers in all directions.
				isOk, gearRatio := isValidGear(lines, idx, idx2)
				if isOk {
					total += gearRatio
				}
			}
		}
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
	// Check if we have arg "t" -> then use the test input
	filename := "input.txt"
	if len(os.Args) > 1 && os.Args[1] == "t" {
		filename = "test.txt"
	}
	lines := readLines(filename)
	// showLines(lines)

	solve(lines)
}