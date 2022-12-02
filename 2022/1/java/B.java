import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;

public class B {

    private static final int MAX_VALS = 3;

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
        int[] maxSum = new int[MAX_VALS];
        Arrays.fill(maxSum, 0);
        for (String inp : rawInput) {
            if (inp == null || inp.isEmpty()) {
                sum = 0;
                continue;
            }

            int nr = Integer.parseInt(inp);
            sum += nr;
            if (sum > maxSum[MAX_VALS-1]) maxSum[MAX_VALS-1] = sum;
            for (int i = MAX_VALS-2; i >= 0; --i) {
                if (sum > maxSum[i]) {
                    maxSum[i+1] = maxSum[i];
                    maxSum[i] = sum;
                }
            }
        }

        int combined = 0;
        for (int i = 0; i < MAX_VALS; ++i) {
            combined += maxSum[i];
        }

        System.out.println("Combined: " + combined);
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