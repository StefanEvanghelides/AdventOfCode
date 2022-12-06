import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class A {

    public static int overlaps(int x1, int x2, int y1, int y2) {
        // x overlaps y
        if (x1 <= y1 && x2 >= y2) return 1;
        // y overlaps x
        if (y1 <= x1 && y2 >= x2) return 1;
        return 0;
    }

    // O(N) time and O(1) space. No need to construct arrays, just range comparison here.
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        int total = 0;
        for (String rawPairs : rawInput) {
            String[] pairs = rawPairs.split(",");
            assert(pairs != null && pairs.length == 2);
            String[] xs = pairs[0].split("-");
            assert(xs != null && xs.length == 2);
            String[] ys = pairs[1].split("-");
            assert(ys != null && ys.length == 2);

            int x1 = Integer.parseInt(xs[0]);
            int x2 = Integer.parseInt(xs[1]);
            int y1 = Integer.parseInt(ys[0]);
            int y2 = Integer.parseInt(ys[1]);

            total += overlaps(x1, x2, y1, y2);
            
        }

        System.out.println("Total: " + total);
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