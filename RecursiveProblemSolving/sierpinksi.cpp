/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This 
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/* order 0 is a filled triangle - order (n > 0) is three sierpinski triangles of order n-1, each half as large as main triangle arranged so they meet corner to corner*/
/* 
 * TODO: Replace this comment with a descriptive function
 * GPoint midpt = {(p1.getX() + p2.getX())/2, (p1.getY() + p2.getY())/2}
 */

GPoint midpt(GPoint p1, GPoint p2) {
    return {(p1.getX() + p2.getX())/2, (p1.getY() + p2.getY())/2};
}

void drawSierpinskiTriangleHelper(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* Negative inputs are not computable */
    if (order < 0) {
       error("order must be non-negative integer");
    }
    /* Base Case: Order 0 triangle is a solid black triangle */
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
    /* RECURSIVE CASE: three triangles of order n - 1 drawn  */
    } else {
        //triangle in bottom left corner onet
        drawSierpinskiTriangleHelper(window, one, midpt(one, two), midpt(one, three), order - 1);
        //triangle twot on the top
        drawSierpinskiTriangleHelper(window, two, midpt(two, one), midpt(two, three), order -1);
        //triangle three in bottom right corner
        drawSierpinskiTriangleHelper(window, three, midpt(three, one), midpt(three, two), order - 1);
    }
}


void drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    //if order is negative throw an error
    /* order 0 is a filled triangle - order (n > 0) is three sierpinski triangles of order n-1, each half as large as main triangle arranged so they meet corner to corner*/

    drawSierpinskiTriangleHelper(window, one, two, three, order);
}






/* * * * * * Test Cases * * * * * */

/* 
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo. 
 */

void runDemos();

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runDemos();
}

