import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;

public class A {

    private static final int MASK_SIZE = 4;

    private static boolean areUnique(char[] chars) {
        HashSet<Character> bag = new HashSet<>();

        for (char c : chars) {
            if (bag.contains(c)) return false;
            bag.add(c);
        }

        return bag.size() == chars.length;
    }

    // O(N) time and O(1) space
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        // For loop because I have multiple test scenarios.
        for (String inp : rawInput) {
            char[] res = new char[MASK_SIZE];
            char[] chars = inp.toCharArray();
            for (int i = 0; i < MASK_SIZE && i < chars.length; ++i) {
                res[i] = chars[i];
            }
            int i = MASK_SIZE;
            while (i < chars.length) {
                // Shift by 1
                for (int j = 0; j < MASK_SIZE-1; ++j) {
                    res[j] = res[j+1];
                }
                res[MASK_SIZE-1] = chars[i];

                // Now compare if the mask has unique elements
                if (areUnique(res)) {
                    System.out.println("Mask result after: " + (i+1));
                    break;                    
                }
                i++;
            }

        }

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