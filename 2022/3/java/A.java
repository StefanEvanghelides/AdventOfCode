import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;

public class A {

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
        for (String rucksack : rawInput) {
            int length = rucksack.length();
            int mid = length/2;
            HashSet<Character> bag = new HashSet<>();
            for (int i = 0; i < mid; ++i) {
                bag.add(rucksack.charAt(i));
            }
            for (int i = mid; i < length; ++i) {
                Character c = rucksack.charAt(i);
                if (bag.contains(c)) {
                    // We found the common one
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