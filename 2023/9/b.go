package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	// "math"
	// "sort"
)

// Verify if the number are the same: if so, then return true.
func canStop(numbers []int64) bool {
	for i := 1; i < len(numbers); i++ {
		if numbers[i-1] != numbers[i] {
			return false
		}
	}
	return true
}

func prependInt64(x []int64, y int64) []int64 {
	x = append(x, 0)
	copy(x[1:], x)
	x[0] = y
	return x
}

/*
 * We will have to iteratively compute the next value
 * Hence we'll use a 2D array.
 * Note: As an optimization, we don't actually need to save all histories, just the first number of each of them.
 * PART B) We now extrapolate backwards! But the same principle applies. Hence we now have FIRST_VALUES instead of LAST_VALUES
 */
func computeNextNumber(numbers []int64) int64 {
	// Guard: If there are no numbers, return -1
	if len(numbers) == 0 {
		return -1
	}
	// Guard: if there's just 1 number, return that one
	if len(numbers) == 1 {
		return numbers[0]
	}
	// Guard: if there are 2 numbers, we can already compute the next one
	if len(numbers) == 2 {
		return 2*numbers[0] - numbers[1]
	}

	// Initially the current history is the numbers list
	var currentHist []int64 = numbers
	// Init the array of first values. Initially it has the first element of the numbers
	var firstValues []int64
	firstValues = append(firstValues, numbers[0])

	// Main loop
	for !canStop(currentHist) {
		// Here we want to generate the next history.
		// Then take the last value of that and add it to our numbers.
		var newHist []int64
		for i := 1; i < len(currentHist); i++ {
			newHist = append(newHist, currentHist[i]-currentHist[i-1])
		}
		firstValues = prependInt64(firstValues, newHist[0])

		// fmt.Println("newHist:", newHist)
		currentHist = newHist
	}
	// fmt.Println("firstValues:", firstValues)

	// Loop is finished, we should now be able to generate our number easily, by means of repeated subtractions
	// But we need to start from the end now.
	result := firstValues[0]
	for i := 1; i < len(firstValues); i++ {
		result = firstValues[i] - result
	}
	return result
}

func mapStringsToInts(strs []string) []int64 {
	var numbers []int64
	for _, str := range strs {
		nr, _ := strconv.Atoi(str)
		numbers = append(numbers, int64(nr))
	}
	return numbers
}

func solve(lines []string) {
	// Process inputs.
	var total int64 = 0
	for i := 0; i < len(lines); i++ {
		var tokens []string = strings.Fields(lines[i])
		// fmt.Println(tokens)
		// Get an array of numbers
		var numbers []int64 = mapStringsToInts(tokens)
		// fmt.Println(numbers)

		// Solve now for the current nubmers
		result := computeNextNumber(numbers)
		total += result
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
