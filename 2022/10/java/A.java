import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;

public class A {

    // O(N) time and O(1) space.
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        int x = 1;
        int cycle = 0;
        int total = 0;
        HashSet<Integer> CYCLES = new HashSet<Integer>();
        for (int i = 20; i <= 220; i += 40)
            CYCLES.add(i);
        for (String inp : rawInput) {
            String[] tokens = inp.split(" ");
            assert(tokens != null && tokens.length > 0);
            cycle++;
            if (CYCLES.contains(cycle)) {
                total += cycle * x;
                // System.out.println("-cycle #" + cycle + " x= " + x + " => " + (cycle * x));
            }
            if (tokens[0].equals("addx")) {
                cycle++;
                if (CYCLES.contains(cycle)) {
                    total += cycle * x;
                    // System.out.println("+cycle #" + cycle + " x= " + x + " => " + (cycle * x));
                }
                x += Integer.parseInt(tokens[1]);
            }
        }
        System.out.println("Total = " + total);
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