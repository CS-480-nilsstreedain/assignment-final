# Assignment 3

In this assignment, we'll hook up an LLVM-based backend (and optimizer) to the Python language frontend we've been working on throughout the term to implement a complete end-to-end compiler.  Specifically, your goal will be to generate LLVM IR representing Python source program.

There are a few major parts to this assignment, described below.  To get you started, you are provided with a Flex scanner specification in `scanner.l`, a Bison parser specification in `parser.y`, and an Abstract Syntax Tree (AST) implementation divided between several files in the `ast/` directory together with the `main()` function in `main.c`.  There is also a makefile that specifies compilation for the entire compiler.

Currently, this compiler first constructs an abstract syntax tree (AST) representing the source program and then traverses the AST to generate a GraphViz specification of the AST to enable us visualize the AST.

An AST is a tree-based representation of the source program in which each node in the tree represents a coherent semantic construct in the source program.  An AST closely resembles a parse tree but is more compact because it eliminates many of the nodes corresponding to nonterminals. Wikipedia has [a helpful article about them](https://en.wikipedia.org/wiki/Abstract_syntax_tree).

GraphViz uses a very simple notation for representing trees and graphs. The GraphViz specification is flexible, and nodes and edges can be defined in any order, with edges even possibly appearing before the corresponding nodes are declared. One important constraint on the GraphViz specification is that each node must have a unique identifier (`a`, `b`, `c`, `d`, etc. in the sample specification above). More info on additional visualization options is [available here](http://www.graphviz.org/).

To generate a GraphViz specification in a file `p1.gv` for a python source program `p1.py` (assuming you have already run `make`), invoke the compiler from the command line like so:
```
./compile < p1.py > p1.gv
```
We can now use the `dot`(https://graphviz.gitlab.io/_pages/pdf/dotguide.pdf) program to generate a PNG to visualize the AST using this command:
```
dot -T png -o p1.png p1.gv
```
You can see visualizations of example ASTs in the `example_asts/` directory.  Here is an AST `example_asts/png/p4.png` corresponding to a Python code  `example_asts/code/p4.py` (the Fibonacci function):

![AST for p4.py](example_asts/png/p4.png)

## 1. Use the AST constructed by the parser to generate LLVM IR

For the first task in this assignment, you should modify the AST classes to generate LLVM IR to represent each program construct represented by the AST.  The IR you generate should exactly reproduce the computation specified in the source program.

**For the compilation process set up in `Makefile` to work correctly, you must implement all of your LLVM generation functionality (anything that uses types or functions from the LLVM C API) in `ast/ast_llvm.c`.**  If you use parts of the LLVM API elsewhere in this project, you may have to modify `Makefile` to correctly compile the project.

All generated IR must be contained within a single function called `target()`.  Importantly, this function should return a `float` value.  Specifically, it should return (as a `float`) the last value stored in the variable named `return_value` in the Python source program.  If the Python source program does not contain a variable named `return_value`, your generated `target()` function should return 0.

For full credit, you should implement IR generation for all straight-line (i.e. non-branching) source code constructs currently recognized by the parser as well as if/else statements, while loops, and break statements. While loops may be implemented using a pattern similar to the one we explored in lecture for if/else statements. Specifically, each while loop will involve three distinct basic blocks (in addition to the block in which the while loop initially appears):
  * The **condition block**, where the loop condition is evaluated.
  * The **while block**, where the body of the while statement is executed.
  * The **continuation block**, where control continues after the loop terminates.

To correctly implement a while loop, the condition block should conditionally branch to either the while block or the continuation block, depending on whether the loop condition is satisfied, and the while block should always branch back to the condition block for re-evaluation of the loop condition.

A break statement within a while loop should simply short-circuit the loop and branch to the *immediate* loop's continuation block. (Care must be taken here, since while loops can be nested within each other.  You'll need to make sure each break statement breaks only out of the current loop).

Another important consideration when implementing break statements is that an LLVM basic block must contain *exactly one* terminating statement (e.g. a return or a branch), which *must* be the last statement in the block.  Thus, you'll need to essentially ignore any code after a break statement in the same block.  For the purposes of this assignment, you may treat a break statement that is not within a loop as a no-op.

Note that the following constructs have been removed from the parser to simplify this assignment:
  * AND and OR expressions
  * NOT expressions
  * ELIF blocks for IF statements

The most straightforward way to compute IR for each construct will likely be to add a new function in `ast/ast_llvm.c` to generate LLVM IR for each of the different subtypes of AST node defined in `ast/_ast_internal.h`.  For an example of what this might look like, refer to the way GraphViz generation is performed by the functions in `ast/ast_graphviz.c`.  In particular, the functions in that file together recursively traverse the AST to generate GraphViz specification for the tree.  A similar recursive traversal would work well to generate LLVM IR.

Once you get your IR generation code working, you should invoke it on the entire AST from the `main()` function in `main.c`, and print the complete textual representation of the generated IR (i.e. the complete LLVM module) to `stdout`.

## 2. Generate object code from your generated IR

Next, modify your compiler to output an object code file based on your generated IR.  Specifically, update your compiler so that it accepts a single, optional command-line argument.  If this argument is specified, then your compiler should output an object code file in the location specified by the argument.  The object code should be executable on the architecture of the machine on which your compiler is being run.

For example, if your compiler is invoked from the command line as in the example below, then in addition to printing the textual representation of the generated IR to `stdout`, it should output an object code file to `target.o` representing the computation specified in `tests/python/straightline_1.py`:
```
./compile target.o < tests/python/straightline_1.py
```
If your compiler is invoked from the command line without the optional argument specified as in the example below
```
./compile < tests/python/straightline_1.py
```
then it should only print the textual representation of the generated IR to `stdout` without generating an object code file.

## Testing your compiler

A makefile is included in this repo that should already be correctly set up to compile your compiler into an executable named `compile`.  All you should need to do to generate this executable is run `make`.  **For the compilation process defined in the makefile to work correctly, you must limit use of the LLVM API to the file `ast/ast_llvm.c`.**  If you use the LLVM API outside that file, you may need to modify the makefile accordingly.

Several testing Python source programs are included in `tests/python/` that you can use to test your code.  These are several different kinds of tests:
  * **`tests/python/straightline_*.py`** - These programs contain only straight-line constructs (i.e. no branches) of increasing complexity.
  * **`tests/python/ifelse_*.py`** - These programs contain both straight-line constructs and if/else statements of increasing complexity.
  * **`tests/python/while_*.py`** - These programs contain straight-line constructs, if/else statements, and while/break constructs of increasing complexity.  Two of these programs (#2 and #4) contain break statements, and the other two (#1 and #3) do not.

For your reference, correct textual IR representations of these testing Python source programs are included in corresponding files in `tests/llvm_ir/`.  For example, correct LLVM IR for the source program `tests/python/straightline_1.py` is in `tests/llvm_ir/straightline_1.ll`.  Note that your generated IR may look different than the IR in `tests/llvm_ir/` but still produce a correct computation, so don't worry about matching this IR exactly.  It is intended only for your reference.

To verify that your generated code can be successfully executed and that it correctly represents the computation specified in the source program, you can use `target.c` to run your code.  This program simply calls the function `target()` (i.e. your generated function) and prints its return value.  **You should not modify `target.c`, since the autograding tests (described below) rely on it.**

To use `target.c`, you simply need to compile it along with an object code file containing your generated `target()` function.  There are two ways to produce this object code file, described in the subsections below.

The expected return values for the source programs (i.e. the expected output of the code in `target.c`) are included in corresponding files in `tests/return_value/`.  For example, the expected output for `tests/python/straightline_1.py` is in `tests/return_value/straightline_1`.  You must make sure the values returned by your generated code **exactly match** these expected return values.

### Generating object code from textual IR

To test that your generated IR is correct, you can generate an object file from the textual representation of LLVM IR using the LLVM system compiler `llc`.  The following example demonstrates how you can use your compiler (called `compile`) to generate an LLVM IR file called `target.ll` from an input source program, pass that IR file to `llc` to generate an object code file called `target.o`, and then compile and run that object code with `target.c`:
```
./compile < tests/python/straightline_1.py > target.ll
llc -filetype=obj -o=target.o target.ll
gcc target.c target.o -o target
./target
```

### Generating object code directly from your compiler

Once you've completed step 2 above, your compiler should be able to output object code directly.  For example, after you're done with step 2, you should be able to generate an object code file named `target.o` from your compiler and then compile and run that code along with `target.c` as follows:
```
./compile target.o < tests/python/straightline_1.py
gcc target.c target.o -o target
./target
```

## Useful LLVM documentation

Here are links to a few useful pieces of LLVM documentation that might help you with the assignment:

  * LLVM C++ API tutorial: https://llvm.org/docs/tutorial/index.html
  * LLVM IR language reference: https://llvm.org/docs/LangRef.html
  * LLVM programmer's manual: https://llvm.org/docs/ProgrammersManual.html
  * Autogenerated documentation for the complete LLVM C API: https://llvm.org/doxygen/group__LLVMC.html

## Submission

We'll be using GitHub Classroom for this assignment, and you will submit your assignment via GitHub.  Make sure your completed files are committed and pushed by the assignment's deadline to the main branch of the GitHub repo that was created for you by GitHub Classroom.  A good way to check whether your files are safely submitted is to look at the main branch your assignment repo on the github.com website (i.e. https://github.com/osu-cs480-sp25/assignment-final-YourGitHubUsername/). If your changes show up there, you can consider your files submitted.

## Autograding tests

Note that this repository contains some tests that will be used for auto-grading this assignment.  These tests will be automatically executed every time you push a new commit to GitHub.  They will specifically be executed on a GitHub Actions runner with LLVM version 13 installed.

If all of the tests pass, you'll see a green checkmark on your GitHub repo page, next to the status message for the most recent commit, like the one below:

![Image of GitHub Actions indicator](https://www.dropbox.com/s/2b6l41ppcznws3p/Screenshot%202020-05-08%2020.25.38.png?raw=true)

If any of the tests fail, you'll see a red "X" instead of a green checkmark.

You can click on either the green checkmark or the red "X" to see more details about the test results.  In this details page, you'll specifically want to look under the section labeled "education/autograding" to see the results of the tests.  The messages printed during testing may be somewhat cryptic, but hopefully they'll help you debug any problems.  If you scroll all the way to the bottom of the "education/autograding" section you should see a message that lets you know how many points out of all possible 120 points your current code would earn, based on the results of the tests.  Go ahead and keep pushing code to GitHub until you earn all 120 points ;)

**To make sure your code works correctly with these auto-grading tests, it must satisfy these requirements:**
  * Your code must compile to an executable named `compile` using the provided makefile.  The starter code is set up to do this, but you might have to modify the makefile if, for example, you add new files that need to be compiled to generate your executable compiler.
  * Your compiler must read the source program from `stdin`.
  * All of your generated LLVM IR must be contained within a single function named `target()`.
  * The function `target()` must return a `float` value.  This return value must be the last value stored in the variable named `return_value` in the source program.  If the source program does not contain a variable named `return_value`, `target()` should return 0.
  * Your compiler should always output the textual representation of the generated LLVM IR to `stdout`.  Your compiler should not output anything else to `stdout`.  If you want to print debugging statements, use `stderr` for these.
  * Your compiler should accept a single, optional command-line argument.  If this argument is specified, your compiler should output object code from its generated IR to the file specified in the argument.

The tests are implemented using [Bats](https://github.com/bats-core/bats-core/).  If you want, you can follow the instructions to install Bats and run the tests locally (it should work to install Bats into your home directory on the OSU ENGR servers).  The test specifications are implemented in `tests/*.bats`.

## Running code in GitHub Codespaces

For this assignment, I've enabled a feature called [GitHub Codespaces](https://docs.github.com/en/codespaces/) that will provide you with a private online environment where you can develop and test your code for the assignment.  This environment will be centered around a browser-based version of the [VS Code](https://code.visualstudio.com/) editor.  You can access the Codespace by clicking "Create codespace on main" under the "Code" button in your assignment repository on GitHub:

![Image of GitHub Codespaces button](https://www.dropbox.com/s/wvijvh130fjuud5/Screen%20Shot%202022-05-24%20at%2011.17.58%20AM.png?raw=true)

You may use this browser-based editor as much or as little as you like, and in general, I encourage you to stick with whatever development setup already works best for you (i.e. your preferred editor running on your preferred development machine).  The reason I'm providing the Codespace is to give you a standardized environment where you can build and test your code.  Specifically, within the Codespace's, it will be very easy to use a fairly recent version of LLVM.  In fact, LLVM v10 is already installed there in the Codespace, and all you'll need to do is run the following command in the Codespace's terminal to make sure Flex and Bison are installed:
```
sudo apt-get install flex bison
```
You can access the Codespace terminal through the menu of the browser-based version of VS Code associated with the Codespace:

![Image of Codespace terminal access](https://www.dropbox.com/s/nqebudssjvcwyw5/Screen%20Shot%202022-05-24%20at%2011.45.34%20AM.png?raw=true)

Inside this terminal, you should be able to run your code as described above.

If you're editing outside the browser-based version of VS Code associated with your Codespace, and you want to test your code inside the Codespace, you'll need to make sure you use Git to pull your most recent commit(s) into the Codespace.  You can do this through the browser-based VS Code's Git integration:

![Image of VS Code Git pull command](https://www.dropbox.com/s/d4rlv954af0q6r4/Screen%20Shot%202022-05-24%20at%2011.37.23%20AM.png?raw=true)

## Grading criteria

Your grade for this assignment will be based on the points earned in the auto-grading test results on GitHub from your last commit before the assignment deadline.  In addition to checking your score from the auto-grading test results, we will also review your code to verify that you have indeed correctly implemented LLVM code generation.

This assignment is worth 120 points total, broken down as follows:
  * 50 points: Your compiler generates computationally correct LLVM IR for straight-line program constructs.
    * 10 points each for generating computationally correct IR for the testing Python source programs in `tests/python/straightline_*.py`.

  * 30 points: Your compiler generates computationally correct LLVM IR for if/else statements.
    * 6 points each for generating computationally correct IR for the testing Python source programs in `tests/python/ifelse_*.py`.

  * 20 points: Your compiler outputs computationally correct object code for straight-line and if/else constructs.
    * 2 points each for generating computationally correct object code for the testing Python source programs in `tests/python/straightline_*.py` and `tests/python/ifelse_*.py`.
      
  * 20 points: Your compiler generates computationally correct LLVM IR for while/break statements.
    * 5 points each for generating computationally correct IR for the testing Python source programs in `tests/python/while_*.py`
