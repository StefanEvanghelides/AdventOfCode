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

	for idx, game := range lines {
		sets := strings.Split(game, ";")
		isGameOk := true
		for _, set := range sets {
			cubes := strings.Split(set, ",")
			isSetOk := true
			for _, cube := range cubes {
				values := strings.Split(cube, " ")
				// Here we expect exactly 2 values
				if len(values) != 2 {
					os.Exit(1)
				}

				number, _ := strconv.Atoi(values[0])
				if values[1] == "b" && number > 14 ||
					values[1] == "r" && number > 12 ||
					values[1] == "g" && number > 13 {
						isSetOk = false
						break
					}
			}
			if !isSetOk {
				isGameOk = false
				break
			}
		}
		if isGameOk {
			// fmt.Printf("gameOk idx=%d\n", idx+1)
			total += int64(idx+1)
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
	lines := readLines("input.txt")
	// showLines(lines)

	solve(lines)
}