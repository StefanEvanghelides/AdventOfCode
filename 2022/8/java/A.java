import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class A {

    private static class Board {
        private int n;
        private int m;
        private int[][] b;
        private boolean[][] vis;

        Board(int n, int m) {
            this.n = n;
            this.m = m;
            b = new int[n][m];
            vis = new boolean[n][m];

            // Mark the outskirts visible. Rest not visible for now.
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                    vis[i][j] = (i == 0 || j == 0 || i == n-1 || j == n-1);
        }

        public void setRow(int idx, String row) {
            for (int i = 0; i < row.length(); ++i) {
                int val = row.charAt(i) - '0';
                b[idx][i] = val;
            }
        }

        public void print() {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    System.out.print(b[i][j]);
                }
                System.out.println();
            }
        }

        public void printVisible() {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    System.out.print(vis[i][j] ? 1 : 0);
                }
                System.out.println();
            }
        }

        public int countVisible() {
            markVisible();

            int total = 0;
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                    if (vis[i][j])
                        total++;

            return total;
        }

        private void markVisible() {
            // We only need to check the insides.

            // Left to right
            for (int i = 1; i < n-1; ++i) {
                int curMax = b[i][0];
                for (int j = 1; j < m-1; ++j) {
                    if (b[i][j] > curMax) {
                        curMax = b[i][j];
                        vis[i][j] |= true;
                    }
                }
            }
            // Right to left
            for (int i = 1; i < n-1; ++i) {
                int curMax = b[i][m-1];
                for (int j = m-2; j > 0; --j) {
                    if (b[i][j] > curMax) {
                        curMax = b[i][j];
                        vis[i][j] |= true;
                    }
                }
            }
            // Top to bottom
            for (int j = 1; j < m-1; ++j) {
                int curMax = b[0][j];
                for (int i = 1; i < n-1; ++i) {
                    if (b[i][j] > curMax) {
                        curMax = b[i][j];
                        vis[i][j] |= true;
                    }
                }
            }
            // Bottom up
            for (int j = 1; j < m-1; ++j) {
                int curMax = b[n-1][j];
                for (int i = n-2; i > 0; --i) {
                    if (b[i][j] > curMax) {
                        curMax = b[i][j];
                        vis[i][j] |= true;
                    }
                }
            }
        }
    
    }

    // O(N^2) time and O(N^2) space. About 4 passes on each elememnt.
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        int n = rawInput.length;
        int m = rawInput[0].length();
        Board b = new Board(n, m);
        for (int i = 0; i < n; ++i) {
            String inp = rawInput[i];
            b.setRow(i, inp);
        }

        // b.print();

        System.out.println("Count visible: " + b.countVisible());

        // b.printVisible();
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