package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
	// "unicode"
	"strconv"
)

func solve(lines []string) {
	var total int64

	for _, game := range lines {
		sets := strings.Split(game, ";")
		var maxBlue int64 = 0
		var maxGreen int64 = 0
		var maxRed int64 = 0
		for _, set := range sets {
			cubes := strings.Split(set, ",")
			for _, cube := range cubes {
				values := strings.Split(cube, " ")
				// Here we expect exactly 2 values
				if len(values) != 2 {
					os.Exit(1)
				}

				numberI, _ := strconv.Atoi(values[0])
				number := int64(numberI)
				if values[1] == "b" && number > maxBlue {
					maxBlue = number
				}
				if values[1] == "r" && number > maxRed {
					maxRed = number
				}
				if values[1] == "g" && number > maxGreen {
					maxGreen = number
				}
			}
		}
		total += maxBlue * maxRed * maxGreen
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