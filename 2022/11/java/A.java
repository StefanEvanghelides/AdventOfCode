import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.function.Function;
import javax.script.*;


// NOTE: I transformed the input a bit, check out the test and input files!
public class A {

    private static class ScriptFunction {
        public static Function<Object,Object> getFunctionFromString(String func) throws Exception {
            ScriptEngine engine = new ScriptEngineManager().getEngineByName("nashorn");
            @SuppressWarnings("unchecked")
            Function<Object,Object> f = (Function<Object,Object>)engine.eval(
                String.format("new java.util.function.Function(%s)", func));

            return f;
        }
        private ScriptFunction(){};
    }
    
    private static class Monkey {
        public int idx;
        public ArrayList<Integer> items;
        public Function<Object,Object> operation;
        public int divisibleBy;
        public int trueIdx;
        public int falseIdx;
        public int inspectedItems;

        @Override
        public String toString() {
            return "idx=" + idx + " inspected items=" + inspectedItems + " div by=" + divisibleBy + " trueIdx=" + trueIdx + " falseIdx=" + falseIdx + " items=" + items.toString();
        }
    }

    private static ArrayList<Monkey> parseInput(String[] rawInput) throws Exception {
        ArrayList<Monkey> monkeys = new ArrayList<Monkey>();
        Monkey monkey = null;
        for (String inp : rawInput) {
            String[] tokens = inp.split(" ");
            if (tokens[0].equals("Monkey")) {
                monkey = new Monkey();
                monkeys.add(monkey);
                monkey.idx = tokens[1].charAt(0) - '0'; 
                monkey.inspectedItems = 0;
            } else if (tokens[0].equals("Starting")) {
                monkey.items = new ArrayList<Integer>();
                for (int i = 2; i < tokens.length; ++i) {
                    Integer item = Integer.parseInt(tokens[i]);
                    monkey.items.add(item);
                }
            } else if (tokens[0].equals("Operation:")) {
                String op = "function(old) " + inp.substring(17);
                monkey.operation = ScriptFunction.getFunctionFromString(op);
            } else if (tokens[0].equals("Test:")) {
                int val = Integer.parseInt(tokens[3]);
                monkey.divisibleBy = val;
            } else if (tokens[0].equals("If")) {
                int otherIdx = Integer.parseInt(tokens[5]);
                if (tokens[1].equals("true:")) {
                    monkey.trueIdx = otherIdx;
                } else {
                    monkey.falseIdx = otherIdx;
                }
            }
        }
        return monkeys;
    }

    private static final int MAX_ROUNDS = 20;
    private static final int WORRY_LEVEL = 3;

    private static void play(ArrayList<Monkey> monkeys) {
        System.out.println("Start:");
        for (Monkey monk : monkeys) {
            System.out.println("Monkey: " + monk.toString());
        }
        for (int i = 0; i < MAX_ROUNDS; ++i) {
            playRound(monkeys);
        }
        System.out.println("Final:");
        for (Monkey monk : monkeys) {
            System.out.println("New Monkey: " + monk.toString());
        }
        
        // Check which 2 monkeys inspected the most items.
        int max1 = 0;
        int max2 = 0;
        for (Monkey monkey : monkeys) {
            if (monkey.inspectedItems >= max1) {
                max2 = max1;
                max1 = monkey.inspectedItems;
            } else if (monkey.inspectedItems > max2) {
                max2 = monkey.inspectedItems;
            }
        }
        System.out.println("Total: " + (max1 * max2));
    }

    private static void playRound(ArrayList<Monkey> monkeys) {
        for (Monkey monkey : monkeys) {
            monkey.inspectedItems += monkey.items.size();
            for (Integer item : monkey.items) {
                int val = ((Double)monkey.operation.apply(item)).intValue();
                val /= WORRY_LEVEL;
                int otherIdx = 0;
                otherIdx = val % monkey.divisibleBy == 0 ? monkey.trueIdx : monkey.falseIdx;
                monkeys.get(otherIdx).items.add(val);
            }
            monkey.items.clear();
        }
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
        ArrayList<Monkey> monkeys = parseInput(rawInput);

        play(monkeys);

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