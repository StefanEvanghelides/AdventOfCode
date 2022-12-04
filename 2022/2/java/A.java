import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class A {

    private static final int OPP_ENC = 'A';
    private static final int ME_ENC = 'X';

    private static final int DRAW_SCORE = 3;
    private static final int WIN_SCORE = 6;
    private static final int LOSE_SCORE = 0;

    private static int compute(int opp, int me) {
        // Draw
        if (opp == me) return DRAW_SCORE + me;

        // Win cases
        if (opp == 1 && me == 2 ||
            opp == 2 && me == 3 ||
            opp == 3 && me == 1)
          return WIN_SCORE + me;

        // All other cases lose
        return LOSE_SCORE + me;
    }

    // O(N) time, O(1) space
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
        for (String inp : rawInput) {
            // Further split the input by space.
            String[] vals = inp.split(" ");
            int opp = vals[0].charAt(0) - OPP_ENC + 1;
            int me = vals[1].charAt(0) - ME_ENC + 1;
            total += compute(opp, me);
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