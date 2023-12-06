// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* shows movement on Y axis (North(1) and South(-1)) and X axis (East(1), West(-1)) */
    int Y[] = {1,-1,0,0};
    int X[] = {0,0,-1,1};
    // show every possible movement for cur
    for (int i = 0; i < 4; i++) {
        int updtX = cur.row + X[i];
        int updtY = cur.row + Y[i];
        // used to check if the updated X and Y values lie within the maze.
        if (updtX >= 0 && updtX < maze.numRows() && updtY >= 0 && updtY < maze.numCols()) {
            // checks to see if updated cur doesn't run into a wall
            if (maze[updtX][updtY] == true) {
                // update the cur into the neighbor set after it has been approved
                GridLocation neighbor(updtX, updtY);
                neighbors.add(neighbor);
            }
        }
    }
    return neighbors;
}

// TODO: validatePath takes a maze and a path and makes sure the path is valid in order to lead to the solution of the maze, if not, it will return an error with a simple explanation on why it failed.
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty()) {
        error("Path is empty!");
    }
    if ((path[0].row, path[0].col) != (0,0)) {
        error("The path does not start at the entry coordinates");
    }

    if ((path[path.size() -1].row, path[path.size() -1].col)!= (maze.numRows() - 1, maze.numCols() - 1)) {
            error("The path does not end at the exit coordinates");
    }
    // records past locations
     Set<GridLocation> pastlocations;
     for (int i = 0; i < path.size() - 1; i++) {
        if (!generateValidMoves(maze, path[i]).contains(path[i+1])) { // verifies connection between paths
            error("Path goes through a wall");
        }
        if (pastlocations.contains(path[i+1])) { //Checks if this is a past location
            error("Path overlaps with a previously visited grid");
        }
            pastlocations.add(path[i]);
    }
}




// SolveMazeBFS works by taking a maze and searching for the exit. This is done by using queues of different paths to explore until exit is found.
Vector<GridLocation> solveMazeBFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Queue<Vector<GridLocation>> allPaths;

    drawMaze(maze);

    Set<GridLocation> pastlocations;
    GridLocation exit(maze.numRows() -1, maze.numCols() -1);

    allPaths.enqueue({{0,0}});
    while (allPaths.size() > 0) {
        Vector<GridLocation> Cur = allPaths.dequeue();
        GridLocation final = Cur[Cur.size() - 1]; // updates the final position of the Cur
        pastlocations.add(final);
        if (final == exit) {
            return Cur;
        }
    for (GridLocation neighbor : generateValidMoves(maze, final)) {
        Vector<GridLocation> newPath = Cur;
            if (!pastlocations.contains(neighbor)) {
                newPath.add(neighbor);
                allPaths.enqueue(newPath);
            }
        }
    }
    return path;
}

// TODO: This function takes a maze and tries to solve the maze by expanding outward from the starting position. it uses a stack of paths that pops the ones that arent the answer. When answer is found, it return the current path.
Vector<GridLocation> solveMazeDFS(Grid<bool>& maze) {
    Vector<GridLocation> path;
    Stack<Vector<GridLocation>> allPaths;

    drawMaze(maze);
    Set<GridLocation> visitedPaths;
    GridLocation exit(maze.numRows() - 1, maze.numCols() - 1);

    allPaths.push({{0,0}});
    while (allPaths.size() > 0) {
        Vector<GridLocation> Cur = allPaths.pop();
        GridLocation final = Cur[Cur.size() -1]; // updates final position of Cur
        if (final == exit) { // is used to see if the exit is in the final
            return Cur;
        }
        for (GridLocation neighbor : generateValidMoves(maze, final)) {
            Vector<GridLocation> newPath = Cur;
            if (!visitedPaths.contains(neighbor)) {
                newPath.add(neighbor);
                allPaths.push(newPath);
            }
        }
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
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

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln = solveMazeBFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln = solveMazeDFS(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves without any valid moves") {
    Grid<bool> maze = {{true, false, true},
                       {false, true, false},
                       {true, false, true}};
    GridLocation Cur = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, Cur), expected);
}
STUDENT_TEST("generateValidMoves on a 1x1 grid") {
    Grid<bool> maze = {{true}};
    GridLocation Cur = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, Cur), expected);
}
STUDENT_TEST("generateValidMoves in center with one neighbor free") {
    Grid<bool> maze = {{true, true},
                       {false, true},
                       {true, false}};
    GridLocation Cur = {1, 1};
    Set<GridLocation> expected = { {0, 1} };

    EXPECT_EQUAL(generateValidMoves(maze, Cur), expected);
}
STUDENT_TEST("generateValidMoves on a 3x3 with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation Cur = {0, 0};
    Set<GridLocation> expected = { {0, 1}, {1, 0} };

    EXPECT_EQUAL(generateValidMoves(maze, Cur), expected);
}

STUDENT_TEST("validatePath on various invalid paths should raise error") {
    Grid<bool> maze = {{true, true, false},
                       {false, true, false},
                       {false, true, true}};

    Vector<GridLocation> notExit = { {0, 0}, {2, 0}, {1, 0} };
    Vector<GridLocation> noPath = {};
    Vector<GridLocation> inwall = { {1 ,0}, {2, 0}, {0, 2}, {1, 2} };
    Vector<GridLocation> wrongBeginning = { {0, 1}, {2,0}, {1,2} };
    Vector<GridLocation> pastlocations = { {0, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 1}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, notExit));
    EXPECT_ERROR(validatePath(maze, noPath));
    EXPECT_ERROR(validatePath(maze, inwall));
    EXPECT_ERROR(validatePath(maze, wrongBeginning));
    EXPECT_ERROR(validatePath(maze, pastlocations));
}
