package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

var MAGIC_VALUE = 256

func computeHash(str string) int {
	hash := 0

	for _, ch := range str {
		hash += int(ch)
		hash *= 17
		hash %= MAGIC_VALUE
	}

	return hash
}

func computeTotal(boxes []map[string]int, boxIndices [][]string) int64 {
	var total int64 = 0

	for idx, box := range boxes {
		// Guard: Skip empty boxes
		if len(box) < 1 {
			continue
		}
		boxIdx := idx + 1
		// Iterate over the list of the box indices
		for idx2, key := range boxIndices[idx] {
			slot := idx2 + 1
			focal := box[key]
			subTotal := boxIdx * slot * focal
			// fmt.Println("Key:", key, "- box:", boxIdx, "- slot:", slot, "- focal:", focal, "- subTotal:", subTotal)
			total += int64(subTotal)
		}
	}

	return total
}

/*
PART B): We now want to split the groups into 2 categories: those with '=' and those with '-'.
Fast lookup: Check the last character of the group to know immediately if it's '-'.
2nd fast trick: for the '=', the next character can be 1 digit between 1-9, so it's also easy to slice.
*/
func solve(lines []string) {
	// Separate input line into grops
	groups := strings.Split(lines[0], ",")
	// Find the best structure for the boxes, also keep track of their order.
	// Essentially we will need 1 array of maps: 1 for the grouping, 1 for the index in the box.
	// - array for the 256 boxes -> we know how many they are already.
	// - map: key is the grouping (without the '=') and the value is the focal value (digit after '=')
	// Second map for the indices in the box.
	boxes := make([]map[string]int, MAGIC_VALUE)
	for idx := 0; idx < MAGIC_VALUE; idx++ {
		boxes[idx] = make(map[string]int)
	}
	boxIndices := make([][]string, MAGIC_VALUE)
	for _, group := range groups {
		if group[len(group)-1] == '-' {
			// This is a '-' grouping, take the slice up to the last element.
			key := group[:len(group)-1]
			// fmt.Println("Deleted group key:", key)
			// Remove element if it exists. If so, then update indices.
			hash := computeHash(key)
			// fmt.Println("Deleted key hash:", hash)
			delete(boxes[hash], key)
			// Find the value to be removed.
			for idx := 0; idx < len(boxIndices[hash]); idx++ {
				if boxIndices[hash][idx] == key {
					// Key found, now removal operation at this index. We can use Golang's slicing operation for this
					boxIndices[hash] = append(boxIndices[hash][:idx], boxIndices[hash][idx+1:]...)

					// We can now break the loop
					break
				}
			}
		} else {
			// This is a '=' grouping
			key := group[:len(group)-2]
			focal := int(rune(group[len(group)-1]) - rune('0'))
			// fmt.Println("New group key:", key, " - focal:", focal)
			hash := computeHash(key)
			// fmt.Println("Key:", key, "- Key hash:", hash, "- focal:", focal)
			// Check if the key is already present. If so, then don't update the index
			if _, ok := boxes[hash][key]; !ok {
				boxIndices[hash] = append(boxIndices[hash], key)
			}
			// Upsert the focal value.
			boxes[hash][key] = focal
		}
	}
	// fmt.Println("Boxes:", boxes)
	// fmt.Println("boxIndices:", boxIndices)

	// Compute total values from the box
	total := computeTotal(boxes, boxIndices)
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
