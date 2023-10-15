// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;



Vector<GridLocation> generateValidMovesHelper(GridLocation cur) {
    Vector<GridLocation> allMoves;
    allMoves.add(cur.row+1);
    allMoves.add(cur.row-1);
    allMoves.add(cur.col+1);
    allMoves.add(cur.col-1);
    return allMoves;
}
Set<GridLocation> generateValidMovesSet(GridLocation cur) {
    Set<GridLocation> allMoves;
    allMoves.add(cur.row+1);
    allMoves.add(cur.row-1);
    allMoves.add(cur.col+1);
    allMoves.add(cur.col-1);
    return allMoves;
}
/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    Vector<GridLocation> allCardinals = generateValidMovesHelper(cur);
    for (GridLocation elem : allCardinals) {
        if (maze.inBounds(elem) && elem == true) {
            neighbors.add(elem);
        }
    }
    return neighbors;
}

void reverse(Queue<int>& q) {
    Stack<int> s;
    while (!q.isEmpty()) {
        int val = q.dequeue();
        s.push(val);
    }
    while (!s.isEmpty()) {
        int val = s.pop();
        q.enqueue(val);
    }
}
Stack<GridLocation> reverseStack(Stack<GridLocation> s) {
    Queue<GridLocation> q;
    Stack<GridLocation> reversedCopy;
    while (!s.isEmpty()) {
        GridLocation temp = s.pop();
        q.enqueue(temp);
    }
    while (!q.isEmpty()) {
        GridLocation temp = q.dequeue();
        s.push(temp);
    }
    return reversedCopy;
}
/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    Stack<GridLocation> getEntrance = reverseStack(path);
    GridLocation properEntrance = {0,0};
    if (getEntrance.pop() != properEntrance) {
        error("path does not start at the Proper Entrance.");
    }
    int elements = path.size();
    for (int i = 0; i < elements; i++) {
        Set<GridLocation> visitedSteps;
        GridLocation curStep = path.pop();
        if (!visitedSteps.contains(curStep)) {
            visitedSteps.add(curStep);
        } else {error("repeated location");}
        if (curStep == false || !maze.inBounds(curStep)) {
            error("path steps out of bounds");
        }
        Set<GridLocation>nextStep = generateValidMovesSet(curStep);
        if (nextStep.contains(path.peek())) {
            error("step took diagonal or leap");
        }
    }
}

Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Queue<Stack<GridLocation>> pathProcessor;
    Stack<GridLocation> Start;
    GridLocation mazeExit = {0,0};
    Start.push({0,0});
    pathProcessor.enqueue(Start);
    while (!pathProcessor.isEmpty()){
        Stack<GridLocation> curPath = pathProcessor.dequeue();
        MazeGraphics::highlightPath(curPath, "blue");
        if (curPath.peek() == mazeExit) {
            return curPath;
        } else {
            Set<GridLocation> nextMoves = generateValidMoves(maze, curPath.peek());
            for (GridLocation elem : nextMoves) {
                Stack<GridLocation> newPath = curPath;
                newPath.add(elem);
                pathProcessor.enqueue(newPath);
            }
        }
    }
    return pathProcessor.dequeue();
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * However, there are various malformed inputs which it does not
 * correctly handle. At the very end of this assignment, you will
 * update this code to have complete error-checking behavior to make
 * the function fully robust. Then, update this header comment to
 * indicate which error checking cases you added. See the writeup
 * for more details.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    int area = maze.size();
    if ((maze.width() * maze.height()) != area) {
        error("maze is not a square");
    }

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("improper character found in maze");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}


PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}
