import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class B {

    private static final int MAX_POSITIONS = 240;
    private static final int WIDTH = 40;

    // O() time and O() space.
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
        char[] crt = new char[MAX_POSITIONS+5];
        for (String inp : rawInput) {
            String[] tokens = inp.split(" ");
            assert(tokens != null && tokens.length > 0);
            cycle++;
            if (cycle % WIDTH == x || cycle % WIDTH == x-1 || cycle % WIDTH == x+1) {
                crt[cycle] = '#';
                // System.out.println("-cycle #" + cycle + " x= " + x + " => " + (cycle * x));
            } else
                crt[cycle] = '.';
            if (tokens[0].equals("addx")) {
                cycle++;
                x += Integer.parseInt(tokens[1]);
                if (cycle % WIDTH == x || cycle % WIDTH == x-1 || cycle % WIDTH == x +1) {
                    crt[cycle] = '#';
                    // System.out.println("+cycle #" + cycle + " x= " + x + " => " + (cycle * x));
                } else
                    crt[cycle] = '.';
            }
        }
        for (int i = 1; i <= MAX_POSITIONS; ++i) {
            System.out.print(crt[i]);
            if (i % WIDTH == 0)
                System.out.println();
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