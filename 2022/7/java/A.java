import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class A {

    private static final int MAX_AMOUNT = 100000;

    // Simple generic tree class, implemented as a struct.
    private static class Tree {
    
        public String fileDirName;
        public int size;
        public boolean isDir;
        public Tree parent;
        public List<Tree> children;

        public Tree(String fdName, int sz, Tree par) {
            parent = par;
            fileDirName = fdName;
            size = sz;
            isDir = false;
            children = new ArrayList<>();
        }

        public Tree(String fdName, Tree par) {
            this(fdName, 0, par);
            isDir = true;
        }

        public Tree(String fdName) {
            this(fdName, null);
        }

        public Tree findChild(String name) {
            for (Tree child : children) {
                if (child.fileDirName.equals(name))
                    return child;
            }
            // Should not reach this!
            throw new IllegalStateException("No child found!");
        }

        public void print() {
            printDepth(0);
        }

        public void printDepth(int depth) {
            StringBuilder sb = new StringBuilder();
            // Add empty spaces for depth
            sb.append(new String(new char[depth]).replace('\0', ' '));
            sb.append("Name=" + fileDirName);
            if (isDir) {
                sb.append(" (dir)");
            }
            sb.append("  size=" + size);
            System.out.println(sb.toString());
            for (Tree child : children) {
                if (child != null)
                    child.printDepth(depth+1);
            }
        }

        public int computeSum() {
            if (!isDir) {
                return size;
            }

            for (Tree child : children) {
                if (child != null)
                    size += child.computeSum();
            }
            return size;
        }

        public int sumDirs() {
            if (!isDir) {
                return 0;
            }

            int total = size < MAX_AMOUNT ? size : 0;
            for (Tree child : children) {
                if (child != null)
                    total += child.sumDirs();
            }
            return total;
        }
    
    }

    // O() time and O() space
    private static void solve(String[] rawInput) {
        // Guard
        if (rawInput == null || rawInput.length < 1) {
            throw new Error("Raw input is NULL!");
        }

        // System.out.println("testing input...");
        // for (String inp : rawInput) {
        //     System.out.println("Line: " + inp);
        // }

        // First pass to construct the tree structure.
        Tree files = new Tree("/");
        Tree currNode = files;
        // Skip the first one, that is root.
        for (int i = 1; i < rawInput.length; ++i) {
            String inp = rawInput[i];
            // Split the input into tokens.
             String[] tokens = inp.split(" ");
            assert(tokens != null && tokens.length > 1);

            if (tokens[0].equals("$")) {
                // This is a command: can be either "cd" or "ls".
                if (tokens[1].equals("ls")) {
                    // "ls" command shows the objects. These will be the children.
                    // Do nothing more here
                } else {
                    // Navigate to either a child or a parent.
                    if (tokens[2].equals("..")) {
                        // Navigate to parent
                        currNode = currNode.parent;
                    } else {
                        currNode = currNode.findChild(tokens[2]);
                    }
                }

                continue;
            }

            // Special cases over. Now we have either files or directories.
            if (tokens[0].equals("dir")) {
                // We have a dir.
                currNode.children.add(new Tree(tokens[1], currNode));
            } else {
                // We have a file
                Integer size = Integer.parseInt(tokens[0]);
                currNode.children.add(new Tree(tokens[1], size, currNode));
            }

        }

        // files.print();

        // Compute the sums for all levels.
        System.out.println("Total sum: " + files.computeSum());

        // files.print();

        // counting the sum of directories less than MAX_SIZE is basically applying
        // DFS on this tree, or a simple preorder traversal.
        System.out.println("Dirs sum: " + files.sumDirs());

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