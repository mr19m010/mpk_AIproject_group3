# mpk_AIproject_group3
Rubiks Mastermind Cube Project
Students Project

#Installation:

``
./server
ctrl + shift + t
./client
``


#Layout

        *********
        * 0 1 2 *
   0 -->* 0 1 2 *
        * 0 1 2 *
*****************************************
*       *       *       *       *       *
*   1   *   2   *   3   *   4   *   5   *
*       *       *       *       *       *
*****************************************
        *       *
        *   6   *
        *       *
        *********

Array[6][3][3] --> 1. Cube side; 2. Row; 3. Column;

# How does it work?

1. Enter a number from range [2-53]
2. Be surprised



# Project Description:

The project is a combination of the Rubik's cube puzzle and the Mastermind puzzle.
1) At the beginning a "server" will create a solvable Rubik's cube by applying x random
(parameter given to the server program e.g.: 100) turns (see here) on all possible
cube-sides on a cube that will have 6 different colors (red, green, blue, white, yellow, orange (not necessarily a standard "cube")) and create a client.
2) The "client" will be able to connect to the server and ask the server questions in the form of a "cube"-question
(giving colors at positions (max c colors (c<<6) and max n positions (n<<9*6)))
3) The "client" will also be able to give commands to the server on how to turn the cube or cube sides in a standardized fashion (see here).
4) The "server" will answer the "cube"-question in a "MasterMind" fashion. Black (1) is returned (in an unsorted array)
 if a color is given at the correct position. White (0) is returned (in an unsorted array) if a color is given at the correct side of the cube.
 Nothing (2) is returned (in an unsorted array) if a color is neither given at the correct position nor on the correct side.
 The positions of the 0,1,2 in the array won't give any hint about the positions of the colors nor the sides.
5) The "server" will additionally answer the "client" if the Rubik's cube is solved or not solved - by returning 0 (not solved) or 1 (solved) and then exiting.

The "client" has to solve the Rubik's cube in an efficient way.
Trivial solutions won't be allowed (e.g. first guess the colors (pure MasterMind) and then solve the cube (pure DFS, A* or RBFS-Algorithm)). The solving can and has to already start before all the colors-positions are proven.
You'll have to find arguments/strategies on "when" to start solving (turning) the cubes-sides.
Due to the structure of this problem you'll have to use heuristic functions to implement the strategies and change of strategies (From a pure Masterproblem to a Rubik's cube problem)

Rubik's cube solver example
Mastermind solver example or (here)

You have to deliver a C/C++ program (gnu C++ in Linux using a makefile in the automake environment), which is able to solve all the given problems (on the given homepage of the specific problem) in a reasonable time.
The project has to be compilable under the linux distribution currently used by the FHTW.
The Makefile has to include a "make test" ability - that calls the executable and tests if the program works like intended.
Any openly available libraries can be used.
You have to analyze your algorithm concerning time and memory usage (gprof and Valgrind) - without this analysis no points will be given!
At the end the group will have to give a presentation (pptx) describing all the used strategies to solve the given problem.
