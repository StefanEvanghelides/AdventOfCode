package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"strconv"
	// "math"
)

func countWinning(winning map[int]bool, myNumbers map[int]bool) int {
	count := 0

	for val, _ := range winning {
		if _, ok := myNumbers[val]; ok {
			count++
		}
	}

	return count
}

func convertToMap(strs []string) map[int]bool {
	numbers := map[int]bool{}
	for _, str := range strs {
		number, _ := strconv.Atoi(str)
		numbers[number] = true
	}
	return numbers
}

func solve(lines []string) {
	var total int64 = 0
	cards := map[int]int{}

	// First pre-populate the map based on the lines
	for i := 0; i < len(lines); i++ {
		cards[i] = 1
	}

	for cardIdx, card := range lines {
		// First remove the "Card number" and colon.
		removed := strings.Split(card, ": ")
		if len(removed) != 2 {
			os.Exit(1)
		}

		// Split card in the 2 parts
		parts := strings.Split(removed[1], " | ")
		if len(parts) != 2 {
			os.Exit(1)
		}

		// Get the list of winning numbers
		winning := convertToMap(strings.Fields(parts[0]))
		myNumbers := convertToMap(strings.Fields(parts[1]))

		// Count how many numbers are actually winning.
		count := countWinning(winning, myNumbers)

		// This will now generate copies on the next cards
		for i := 0; i < count; i++ {
			if cardIdx+i+1 < len(cards) {
				cards[cardIdx+i+1] += cards[cardIdx]
			}
		}
	}

	// Compute total by summing up all numbers in the map
	for _, count := range cards {
		total += int64(count)
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