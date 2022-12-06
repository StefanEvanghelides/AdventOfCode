import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class B {

    // Note: I changed the input of the cranes to be easier to parse!
    // from:
    // [N]  [P]
    //  1    2
    //
    // to:
    // 1:N,...
    // 2:P,...


    // O(N*M*Q) time and O(N*M) space, where N = nr of cranes, M = avg. number of moves for 1 crane, Q = avg. quantity of a crane
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        // First build the cranes 2D array.
        int cranesNr = Integer.parseInt(rawInput[0]);
        ArrayList<ArrayList<String>> cranes = new ArrayList<>();
        for (int i = 1; i <= cranesNr; i++) {
            String[] tokens = rawInput[i].split(",");
            assert(tokens != null && tokens.length < 1);
            ArrayList<String> crane = new ArrayList<>();
            for (String x : tokens) {
                crane.add(x);
            }
            cranes.add(crane);
        }

        // Now parse the moves.
        // For part b) moving elements remains in the same order.
        for (int i = cranesNr + 1; i < rawInput.length; i++) {
            String[] move = rawInput[i].split(" ");
            assert(move != null && move.length == 6);
            int quant = Integer.parseInt(move[1]);
            int src = Integer.parseInt(move[3]) - 1;
            int dest = Integer.parseInt(move[5]) - 1;
            ArrayList<String> craneSrc = cranes.get(src);
            ArrayList<String> craneDest = cranes.get(dest);
            for (int j = 0; j < quant; j++) {
                String elem = craneSrc.get(craneSrc.size() - quant + j);
                craneDest.add(elem);
            }
            // 2nd loop to remove elements
            for (int j = 0; j < quant; j++) {
                craneSrc.remove(craneSrc.size() - 1);
            }
            // System.out.println(cranes.toString());
        }

        // Now compute the result.
        StringBuilder result = new StringBuilder();
        for (ArrayList<String> crane : cranes) {
            if (crane.size() > 0)
                result.append(crane.get(crane.size()-1));
        }

        System.out.println("result: " + result.toString());
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