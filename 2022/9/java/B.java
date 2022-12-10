import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;

public class B {

    private static class Pair {
        public int p1;
        public int p2;

        Pair(int p1, int p2) {
            this.p1 = p1;
            this.p2 = p2;
        }

        Pair() {
            this(0, 0);
        }

        @Override
        public boolean equals(Object o) {
            if (o == this) return true;
            if (!(o instanceof Pair)) return false;

            Pair p = (Pair) o;
            return p1 == p.p1 && p2 == p.p2;
        }

        @Override
        public int hashCode() {
            return p1 * 100000 + p2; // quick and dirty.
        }

        @Override
        public String toString() {
            return "p1=" + p1 + "  p2=" + p2;
        }
    }

    public static void checkUpdateTail(Pair head, Pair tail, HashSet<Pair> tailPath) {
        int i1 = head.p1;
        int j1 = head.p2;
        int i2 = tail.p1;
        int j2 = tail.p2;

        // Check if tail is already in proximity.
        if (Math.abs(i1 - i2) <= 1 && Math.abs(j1 - j2) <= 1) return;

    
        // Diagonal move.
        if (Math.abs(i1 - i2) + Math.abs(j1 - j2) > 2) {
            if (i1 > i2+1) {
                i2++;
                j2 = j1;
            } else if (i2 > i1+1) {
                i2--;
                j2 = j1;
            }
            if (j1 > j2+1) {
                j2++;
                i2 = i1;
            } else if (j2 > j1+1) {
                j2--;
                i2 = i1;
            }
        } else {
            // Simply move to one of the sides.
            if (i1 > i2+1) i2++;
            else if (i2 > i1+1) i2--;
            if (j1 > j2+1) j2++;
            else if (j2 > j1+1) j2--;
        }
        tail.p1 = i2;
        tail.p2 = j2;

        tailPath.add(new Pair(i2, j2));
    }

    // 2497 too high
    // b-search min: 2331 (168 diff)
    // - 2413 too low
    // - 2455 too low -> answer for someone else
    // no more feedback, that's not the right answer.
    // - 2456 no

    // - 2476 no

    private static final int MAX_TAILS = 9;

    // O(N) time and O() space.
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        Pair head = new Pair();
        Pair[] tails = new Pair[MAX_TAILS];
        ArrayList<HashSet<Pair>> tailsPath = new ArrayList<HashSet<Pair>>(MAX_TAILS);
        for (int i = 0; i < MAX_TAILS; ++i) {
            tails[i] = new Pair();
            HashSet<Pair> path = new HashSet<>();
            path.add(new Pair());
            tailsPath.add(path);
        }
        for (String inp : rawInput) {
            String[] tokens = inp.split(" ");
            assert(tokens != null && tokens.length == 2);
            char dir = tokens[0].charAt(0);
            int val = Integer.parseInt(tokens[1]);

            // Move to the right.
            if (dir == 'R') {
                for (int i = 0; i < val; ++i) {
                    head.p2++;
                    checkUpdateTail(head, tails[0], tailsPath.get(0));
                    for (int j = 1; j < MAX_TAILS; ++j)
                        checkUpdateTail(tails[j-1], tails[j], tailsPath.get(j));
                }
            }
            // Move to the left
            if (dir == 'L') {
                for (int i = 0; i < val; ++i) {
                    head.p2--;
                    checkUpdateTail(head, tails[0], tailsPath.get(0));
                    for (int j = 1; j < MAX_TAILS; ++j)
                        checkUpdateTail(tails[j-1], tails[j], tailsPath.get(j));
                }
            }
            // Move up
            if (dir == 'U') {
                for (int i = 0; i < val; ++i) {
                    head.p1--;
                    checkUpdateTail(head, tails[0], tailsPath.get(0));
                    for (int j = 1; j < MAX_TAILS; ++j)
                        checkUpdateTail(tails[j-1], tails[j], tailsPath.get(j));
                }
            }
            // Move down
            if (dir == 'D') {
                for (int i = 0; i < val; ++i) {
                    head.p1++;
                    checkUpdateTail(head, tails[0], tailsPath.get(0));
                    for (int j = 1; j < MAX_TAILS; ++j)
                        checkUpdateTail(tails[j-1], tails[j], tailsPath.get(j));
                }
            }
        }
        System.out.println("Tails path covered:");
        for (int j = 0; j < MAX_TAILS; ++j)
            System.out.println("Tail #" + (j+1) + ": " + tailsPath.get(j).size());
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