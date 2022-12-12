import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.PriorityQueue;

public class B {

    static class Point {
        public int x;
        public int y;

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public String toString() {
            return "x=" + x + " y=" + y;
        }
        
        @Override
        public int hashCode() {
            // Very bad, but eficient implementation
            return 1024 * x + y;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if (obj == null)
                return false;
            if (getClass() != obj.getClass())
                return false;
            Point other = (Point) obj;
            return x == other.x && y == other.y;
        }
    }

    static class PDist {
        public Integer dist;
        public Point p;

        public PDist(Integer d, Point p) {
            this.dist = d;
            this.p = p;
        }

        @Override
        public String toString() {
            return "dist=" + dist + " p: " + p.toString();
        }
    };

    private static final int[] dx = new int[]{1, -1, 0, 0};
    private static final int[] dy = new int[]{0, 0, 1, -1};
    private static final int MAX_ITERATIONS = 100000;
    private static final int MAX_VALUE = 10000000;

    public static class Board {

        private char[][] b;
        private int n;
        private int m;
        private Point end;

        public Board(int n, int m) {
            this.b = new char[n][m];
            this.n = n;
            this.m = m;
            end = null;
        }

        public void parseLine(int i, String line) {
            int m = line.length();
            for (int j = 0; j < m; ++j) {
                char c = line.charAt(j);
                if (c == 'S') {
                    c -= 'S';
                    c += 'a';
                } else if (c == 'E') {
                    c -= 'E';
                    this.end = new Point(i, j);
                    c += 'z';
                }
                b[i][j] = c;
            }
        }

        public int walk(Point start) {
            // Apply a simple Dijkstra algo.
            // I will think later if I need a heuristics.
            HashMap<Point, Integer> visited = new HashMap<>();

            Comparator<PDist> distComparator = new Comparator<>() {
                public int compare (PDist pd1, PDist pd2) {
                    return pd1.dist - pd2.dist;
                }
            };

            PDist startPD = new PDist(0, start);
            PriorityQueue<PDist> queue = new PriorityQueue<>(distComparator);
            queue.add(startPD);
            PDist pd = null;
            int iter = 0;
            while(!queue.isEmpty() && iter++ < MAX_ITERATIONS) {
                pd = queue.poll();

                // // End node found!
                // if (pd.p.equals(this.end)) {
                //     System.out.println("Found the end! " + pd.toString());
                //     break;
                // }

                // Now check if it was already visited. If so, check its distance.
                if (visited.containsKey(pd.p)) {
                    // Check if the existing distance is worse: if so, then we still want to process this node.
                    Integer visitedDist = visited.get(pd.p);
                    if (visitedDist <= pd.dist)
                        continue;
                }

                // System.out.println("Processing current node: " + pd.toString());

                // Set this node as visited
                visited.put(pd.p, pd.dist);

                // Check the neighbours.
                int x = (int)pd.p.x;
                int y = (int)pd.p.y;
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    // Check if the new neighbour would be a valid option.
                    if (nx >= 0 && nx < this.n && ny >= 0 && ny < this.m && b[nx][ny] <= b[x][y] + 1) {
                        PDist neighbour = new PDist(pd.dist + 1, new Point(nx, ny));
                        // Valid neighbour! Add to the queue.
                        queue.add(neighbour);
                    }
                }
            }

            // Check if we reached the end
            if (!visited.containsKey(this.end))
                return MAX_VALUE;

            return visited.get(this.end);
        }

        public void print() {
            for (int i = 0; i < this.n; ++i) {
                for (int j = 0; j < this.m; ++j) {
                    System.out.print(b[i][j]);
                }
                System.out.println();
            }
        }

        // Part b) is funny: we can run the algorithm as it is,
        // but we can also speed it up a bit by only checking those 'a' near the letter 'b'.
        // This trick works because there are only a limited number of letter 'b'.
        public List<Point> determineStartingPoints() {
            List<Point> starts = new ArrayList<>();
            for (int i = 0; i < this.n; ++i) {
                for (int j = 0; j < this.m; ++j) {
                    if (this.b[i][j] == 'a') {
                        starts.add(new Point(i, j));
                    }
                }
            }
            return starts;
        }
    }

    public static Board parseInput(String[] rawInput) {
        int n = rawInput.length;
        int m = rawInput[0].length();
        Board board = new Board(n,m);
        for (int i = 0; i < n; ++i) {
            board.parseLine(i, rawInput[i]);
        }
        return board;
    }

    // O() time and O() space.
    private static void solve(String[] rawInput) throws Exception {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        // Parse input
        Board board = parseInput(rawInput);
        List<Point> starts = board.determineStartingPoints();
        int minSteps = MAX_VALUE;
        for (Point start : starts) {
            int steps = board.walk(start);
            if (steps < minSteps) {
                minSteps = steps;
            }
        }
        System.out.println("Min steps: " + minSteps);
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

        try {
            solve(rawInput);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
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