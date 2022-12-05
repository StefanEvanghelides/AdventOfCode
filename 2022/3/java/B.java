import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;

public class B {

    private static int computePriority(Character c) {
        if (c >= 'a' && c <= 'z') return c - 'a' + 1;
        return c - 'A' + 27;
    }

    // O(N*M) time, O(M) space. N = nr lines, M = length of line.
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        int sum = 0;
        int total = rawInput.length;
        for (int r = 0; r < total; r += 3) {
            String rucksack0 = rawInput[r];
            String rucksack1 = rawInput[r+1];
            String rucksack2 = rawInput[r+2];

            // Algo:
            // 1. Put all chars of the 1st rucksack in a HashSet.
            // 2. Create a 2nd HashSet that holds the common ones between them.
            // 3. Now check which character from the 3rd rucksack is present in the 2nd HashSet.

            // 1.
            int length = rucksack0.length();
            HashSet<Character> bag = new HashSet<>();
            for (int i = 0; i < length; ++i) {
                bag.add(rucksack0.charAt(i));
            }
            // 2.
            length = rucksack1.length();
            HashSet<Character> bag2 = new HashSet<>();
            for (int i = 0; i < length; ++i) {
                Character c = rucksack1.charAt(i);
                if (bag.contains(c)) {
                    // We found a common char.
                    bag2.add(c);
                }
            }
            // 3.
            length = rucksack2.length();
            for (int i = 0; i < length; ++i) {
                Character c = rucksack2.charAt(i);
                if (bag2.contains(c)) {
                    // We found the one!
                    sum += computePriority(c);
                    break;
                }
            }
        }
        System.out.println("Sum: " + sum);
    }

    private static void run(String filename) {
        // Guard
        if (filename == null || filename.isEmpty()) {
            throw new Error("Filename is NULL!");
        }

        // Open file and read it.
        String content = null;
        try {
            content = Files.readString(Paths.get(filename));
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        // Now split this in multiple rows, if exist.
        String[] rawInput = content.split(System.lineSeparator());

        solve(rawInput);
    }

    private static final String DEFAULT_INPUT_FILENAME = "input";

    public static void main(String[] args) {
        // Parse argument.
        String filename = null;
        filename = args != null && args.length > 0 ? args[0] : DEFAULT_INPUT_FILENAME;

        // Run the code.
        run(filename);
    }
}