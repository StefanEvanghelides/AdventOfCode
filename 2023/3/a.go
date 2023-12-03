package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"unicode"
	"strconv"
)

// Valid Range is:
//	[idx-1,start-1] -> [idx-1,end+1]
//  [idx,start-1] & [idx,end+1]
//  [idx+1,start-1] -> [idx+1,end+1]
func isValid(lines []string, idx int, start int, end int) (bool) {
	p1 := start - 1
	if p1 < 0 {
		p1 = 0
	}
	p2 := end + 1
	if p2 > len(lines[0]) - 1 {
		p2 = len(lines[0]) - 1
	}
	// First part: [idx-1,start-1] -> [idx-1,end+1]
	if (idx > 0) {
		row := lines[idx-1]
		for i := p1; i <= p2; i++ {
			if rune(row[i]) != '.' && !unicode.IsDigit(rune(row[i])) {
				// special character found above our number
				return true
			}
		}
	}
	// Middle part: [idx,start-1] & [idx,end+1]
	if p1 < start && rune(lines[idx][p1]) != '.' && !unicode.IsDigit(rune(lines[idx][p1])) {
		// special character found to the left of our number
		return true
	}
	if p2 > end && rune(lines[idx][p2]) != '.' && !unicode.IsDigit(rune(lines[idx][p2])) {
		// special character found to the right of our number
		return true
	}
	// Final part: [idx+1,start-1] -> [idx+1,end+1]
	if (idx < len(lines) - 1) {
		row := lines[idx+1]
		for i := p1; i <= p2; i++ {
			if rune(row[i]) != '.' && !unicode.IsDigit(rune(row[i])) {
				// special character found above our number
				return true
			}
		}
	}

	// All other cases failed
	return false
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
			// Check if we detect a digit
			if (unicode.IsDigit(rune(row[idx2]))) {
				// Now try to move idx2 forward until the end of the number
				start := idx2
				for idx2 < len(row) && unicode.IsDigit(rune(row[idx2])) {
					idx2++
				}
				end := idx2-1
				// Now we want to check the neighbours for this range for any chars.
				// Range is:
				//	[idx-1,start-1] -> [idx-1,end+1]
				//  [idx,start-1] & [idx,end+1]
				//  [idx+1,start-1] -> [idx+1,end+1]
				if isValid(lines, idx, start, end) {
					// Now compute the number and add it to the total
					number := int64(computeNumber(row, start, end))
					// fmt.Printf("Valid idx=%d  idx2=%d  number=%d\n", idx, idx2, number)
					total += number
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