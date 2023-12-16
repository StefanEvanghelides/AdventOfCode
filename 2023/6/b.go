package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	"strconv"
	// "math"
)

func countWins(time int, dist int) int {
	// Check the first value greater than dist.
	i := 0
	for i * (time-i) <= dist {
		i++
	}
	// Now "i" reached the first value that beats the distance.
	counter := ((time - 1) / 2 - i + 1)

	// Double everything now
	counter *= 2

	// Even-timed runs will have 1 extra value.
	if time % 2 == 0 {
		counter++
	}

	return counter
}

func solve(lines []string) {

	// Process inputs for times and distances.
	var times []string
	var distances []string
	for idx, line := range lines {
		tokens := strings.Fields(line)
		// Skip the first token
		for i := 1; i < len(tokens); i++ {
			if idx == 0 {
				// This is the "times" array
				times = append(times, tokens[i])
			} else {
				// This is the "distances" array
				distances = append(distances, tokens[i])
			}
		}
	}
	// fmt.Println(times)
	// fmt.Println(distances)

	// For part (b) we want to collapse these times.
	finalTime, _ := strconv.Atoi(strings.Join(times, ""))
	finalDistance, _ := strconv.Atoi(strings.Join(distances, ""))

	fmt.Println("finalTime:",finalTime)
	fmt.Println("finalDistance:",finalDistance)

	// For each combination of time x distance, calculate the number of wins.
	// Multiply the results at the end.
	fmt.Println("Final result:", countWins(finalTime, finalDistance))
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