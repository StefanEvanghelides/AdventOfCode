package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func isComboValid(pattern string, conditions []int) bool {
	idxP := 0
	idxC := 0
	sizeP := len(pattern)
	sizeC := len(conditions)
	// First skip the initial '.'
	for ; idxP < sizeP; idxP++ {
		if rune(pattern[idxP]) == '#' {
			break
		}
	}

	// fmt.Println("pattern:", pattern, "- conditions:", conditions)

	// Now idxP points to the first # char -> check the conditions
	condValue := 0
	for ; idxP < sizeP && idxC < sizeC; idxP++ {
		ch := rune(pattern[idxP])
		// fmt.Println("ch:", string(ch), "- condValue:", condValue, "- idxP:", idxP, "- idxC:", idxC)
		if ch == '.' {
			// Pattern stopped. Move on to the next one or stop.
			// Can already check if the current grouping is correct
			if conditions[idxC] != condValue {
				return false
			}
			idxC = idxC + 1
			for ; idxP < sizeP-1; idxP++ {
				if rune(pattern[idxP+1]) == '#' {
					break
				}
			}
			condValue = 0
		} else {
			// Pattern #, continue counting.
			// Easiest is to actually decrease counters from copy.
			condValue = condValue + 1
			if condValue > conditions[idxC] {
				// Bad pattern and extra pattern
				return false
			}
		}
	}

	// Combo valid == we reached the end correctly
	// fmt.Println("- condValue:", condValue, "- idxP:", idxP, "- idxC:", idxC)

	return idxP == sizeP && (idxC == sizeC || idxC == sizeC-1 && condValue == conditions[idxC])
}

func computePossibilities(pattern string, startIdx, size, remainingUnknown int, conditions []int) int {
	// fmt.Println("startIdx:", startIdx, "- rem:", remainingUnknown, "- pattern:", pattern, "- conditions:", conditions)
	if remainingUnknown < 1 {
		if isComboValid(pattern, conditions) {
			// fmt.Println("startIdx:", startIdx, "- rem:", remainingUnknown, "- pattern:", pattern, "- conditions:", conditions)
			// fmt.Println(" - isComboValid(pattern, conditions): true")
			return 1
		} else {
			return 0
		}
	}
	possibilities := 0
	for i := startIdx; i < size; i++ {
		ch := rune(pattern[i])
		if ch == '?' {
			// Diverge: try 2 possiblities
			pattern1 := pattern[:i] + "." + pattern[i+1:]
			possibilities += computePossibilities(pattern1, i+1, size, remainingUnknown-1, conditions)
			pattern2 := pattern[:i] + "#" + pattern[i+1:]
			possibilities += computePossibilities(pattern2, i+1, size, remainingUnknown-1, conditions)
			break

		}
	}
	return possibilities
}

func solve(lines []string) {
	total := 0
	// For each group compute the hash function
	for _, line := range lines {
		// Separate input line into 2 parts
		groups := strings.Fields(line)
		var conditions []int
		for _, cond := range strings.Split(groups[1], ",") {
			nr, _ := strconv.Atoi(cond)
			conditions = append(conditions, nr)
		}
		pattern := groups[0]
		remainingUnknown := strings.Count(pattern, "?")
		possibilities := computePossibilities(pattern, 0, len(pattern), remainingUnknown, conditions)
		// fmt.Println("- possibilities:", possibilities)
		total += possibilities
	}
	fmt.Println(total)

	// Testing separate functions
	// fmt.Println(isComboValid(".###....##..", []int{3, 2, 1}))
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
