#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "Scott";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "TODO";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to find all items in the needed set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    /**/
    bool escape = false;

    /*   Loops through each char in path(moves) and verifies that it is a valid step   */
    int pathLength = moves.size() + 1;

    for(int i=0; i < pathLength; i++)
    {
        /*   North   */
        if (moves[i] == 'N') {
            if (start->north == nullptr) {
                return escape;
            }else {
                /*  Checks each step in path for magical item   */
                if (needs.contains(start->contents)) {
                    string founditem = start->contents;
                    needs.remove(founditem);
                }else if(start->contents != ""){
                    error("non-magical item found in Labrynth");
                }
                start = start->north;
            }
        }
        /*   South   */
        else if (moves[i] == 'S') {
            if (needs.contains(start->contents)) {
                string founditem = start->contents;
                needs.remove(founditem);
            }else if(start->contents != ""){
                error("non-magical item found in Labrynth");
            }
            if (start->south == nullptr) {
                return escape;
            }else {
                start = start->south;
            }
        }
        /*   West   */
        else if (moves[i] == 'W') {
            if (needs.contains(start->contents)) {
                string founditem = start->contents;
                needs.remove(founditem);
            }else if(start->contents != ""){
                error("non-magical item found in Labrynth");
            }
            if (start->west == nullptr) {
                return escape;
            }else {
                start = start->west;
            }
        }
        /*   East   */
        else if (moves[i] == 'E') {
            if (needs.contains(start->contents)) {
                string founditem = start->contents;
                needs.remove(founditem);
            }else if(start->contents != ""){
                error("non-magical item found in Labrynth");
            }
            if (start->east == nullptr) {
                return escape;
            }else {
                start = start->east;
            }
        }
    }
    if (needs.isEmpty()) {
        //assumes that if no items are needed, exit is permitted
        escape = true;
    }
    return escape;
}





/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "Scott");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

